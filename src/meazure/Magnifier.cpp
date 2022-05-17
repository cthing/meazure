/*
 * Copyright 2001 C Thing Software
 *
 * This file is part of Meazure.
 *
 * Meazure is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Meazure is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Meazure.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pch.h"
#include "Magnifier.h"
#include "Layout.h"
#include "Resource.h"
#include "ScreenMgr.h"
#include "Colors.h"
#include <AfxPriv.h>


BEGIN_MESSAGE_MAP(MeaMagnifier, CWnd)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_MEA_RUNSTATE, OnRunState)
    ON_MESSAGE(MeaHPTimerMsg, OnHPTimer)
    ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
END_MESSAGE_MAP()


MeaMagnifier::MeaMagnifier() : CWnd(),
m_curPos(0, 0),
m_enabled(true),
m_runState(kDefRunState),
m_colorFmt(kDefColorFmt),
m_zoomIndex(kDefZoomIndex),
m_showGrid(kDefShowGrid),
m_magHeight(0) {}


MeaMagnifier::~MeaMagnifier() {
    try {
        Disable();
    } catch (...) {
        MeaAssert(false);
    }
}

bool MeaMagnifier::Create(const POINT& topLeft, int width, CWnd* parentWnd) {
    CRect frontRect, rearRect, colorRect;

    if (!CWnd::CreateEx(WS_EX_CONTROLPARENT,
                        AfxRegisterWndClass(0, nullptr, GetSysColorBrush(COLOR_BTNFACE)), _T(""),
                        WS_CHILD | WS_VISIBLE, CRect(topLeft, CSize(width, 5)), parentWnd, 0xFFFF)) {
        return false;
    }

    //
    // Create the color display
    //
    m_magHeight = width + kMargin.cy;
    if (!m_swatchLabel.Create(IDS_MEA_COLOR, SS_RIGHT | WS_VISIBLE | WS_CHILD, CPoint(0, m_magHeight), this)) {
        return false;
    }
    if (!m_swatchField.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_READONLY, CPoint(0, m_magHeight), 12, this)) {
        return false;
    }
    m_swatchField.GetClientRect(colorRect);
    if (!m_swatchWin.Create(AfxRegisterWndClass(0), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
                            CRect(CPoint(0, m_magHeight), CSize(kSwatchWidth, colorRect.Height())), this, 0xFFFF)) {
        return false;
    }

    MeaLayout::PlaceAfter(m_swatchLabel, m_swatchField, kMargin.cx / 2);
    MeaLayout::PlaceAfter(m_swatchField, m_swatchWin, kMargin.cx);

    //
    // Create the pause button
    //
    m_runStateBtn.LoadBitmaps(IDB_MEA_NORMPAUSE, IDB_MEA_SELPAUSE);
    m_runStateBtn.Create(WS_VISIBLE, this, IDC_MEA_RUNSTATE, IDS_MEA_PAUSE);
    m_runStateBtn.SetToggleType(true);
    m_runStateBtn.GetClientRect(colorRect);

    MeaLayout::PlaceAfter(m_swatchWin, m_runStateBtn, 3 * kMargin.cx);
    MeaLayout::AlignCenter(m_magHeight, &m_swatchLabel, &m_swatchField, &m_swatchWin, &m_runStateBtn, nullptr);

    //
    // Create the zoom control
    //
    m_magHeight += colorRect.Height() + kMargin.cy;

    if (!m_zoomLabel.Create(IDS_MEA_ZOOM, WS_CHILD | WS_VISIBLE, CPoint(0, m_magHeight), this)) {
        return false;
    }
    if (!m_factorLabel.Create(IDS_MEA_FACTOR, WS_CHILD | WS_VISIBLE, CPoint(0, m_magHeight), this)) {
        return false;
    }
    m_zoomLabel.GetClientRect(frontRect);
    m_factorLabel.GetClientRect(rearRect);
    if (!m_zoomSlider.Create(TBS_HORZ | TBS_NOTICKS | WS_VISIBLE,
                             CRect(frontRect.right, m_magHeight, width - rearRect.Width() - 2, m_magHeight + kZoomHeight),
                             this, 0xFFFF)) {
        return false;
    }

    MeaLayout::PlaceAfter(m_zoomLabel, m_zoomSlider, 0);
    MeaLayout::PlaceAfter(m_zoomSlider, m_factorLabel, 0);
    MeaLayout::AlignCenter(m_magHeight, &m_zoomLabel, &m_zoomSlider, &m_factorLabel, nullptr);

    m_magHeight += kZoomHeight;

    MeaLayout::SetWindowSize(*this, width, m_magHeight);

    m_zoomSlider.SetRange(kMinZoomIndex, kMaxZoomIndex);
    m_zoomSlider.SetPos(0);

    m_timer.Create(this);
    OnHPTimer(0, 0);

    return true;
}

void MeaMagnifier::SaveProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        profile.WriteInt(_T("ZoomIndex"), m_zoomIndex);
        profile.WriteBool(_T("MagGrid"), m_showGrid);
        profile.WriteInt(_T("ColorFmt"), m_colorFmt);
    }
}

void MeaMagnifier::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        SetZoomIndex(profile.ReadInt(_T("ZoomIndex"), m_zoomIndex));
        SetShowGrid(profile.ReadBool(_T("MagGrid"), m_showGrid));
        SetColorFmt(static_cast<MeaMagnifier::ColorFmt>(profile.ReadInt(_T("ColorFmt"), m_colorFmt)));
    }
}

void MeaMagnifier::MasterReset() {
    SetZoomIndex(kDefZoomIndex);
    SetShowGrid(kDefShowGrid);
    SetColorFmt(kDefColorFmt);
    SetRunState(kDefRunState);
}

void MeaMagnifier::Enable() {
    if (!m_enabled) {
        m_enabled = true;

        if (m_hWnd != nullptr) {
            ShowWindow(SW_SHOW);
            OnHPTimer(0, 0);
        }
    }
}

void MeaMagnifier::Disable() {
    if (m_enabled) {
        m_enabled = false;

        m_timer.Stop();

        if (m_hWnd != nullptr) {
            ShowWindow(SW_HIDE);
        }
    }
}

void MeaMagnifier::SetRunState(RunState runState) {
    if (m_runState != runState) {
        m_runStateBtn.SetState(runState == RunState::Freeze);
        ChangeRunState(runState);
    }
}

void MeaMagnifier::OnRunState() {
    ChangeRunState((GetRunState() == RunState::Run) ? RunState::Freeze : RunState::Run);
}

void MeaMagnifier::ChangeRunState(RunState runState) {
    if (m_runState != runState) {
        m_runState = runState;

        if (m_enabled) {
            if (m_runState == RunState::Run) {
                OnHPTimer(0, 0);
            } else {
                m_timer.Stop();
            }
        }
    }
}

void MeaMagnifier::Update() {
    if (IsEnabled()) {
        CRect rect;
        CString str;

        GetClientRect(rect);
        rect.bottom = rect.right;

        str.Format(_T("%d X"), kZoomFactorArr[m_zoomIndex]);
        m_factorLabel.SetWindowText(str);

        InvalidateRect(rect, FALSE);
        UpdateWindow();
    }
}

LRESULT MeaMagnifier::OnHelpHitTest(WPARAM, LPARAM) {
    return HID_BASE_COMMAND + ID_MEA_MAGNIFIER;
}

void MeaMagnifier::OnHScroll(UINT /* nSBCode */, UINT /* nPos */,
                             CScrollBar* pScrollBar) {
    m_zoomIndex = reinterpret_cast<CSliderCtrl*>(pScrollBar)->GetPos();
    Update();
}

LRESULT MeaMagnifier::OnHPTimer(WPARAM, LPARAM) {
    Update();

    m_timer.Start(kUpdateRate);

    return 0;
}

void MeaMagnifier::OnPaint() {
    CPaintDC dc(this);
    Draw(dc);
}

void MeaMagnifier::Draw(HDC hDC) {
    int srcLen, j;
    int xCoord, yCoord;

    //
    // Center the magnifier around cursor
    //
    if (m_runState == RunState::Run) {
        GetParent()->SendMessage(MeaGetPositionMsg, 0, reinterpret_cast<WPARAM>(&m_curPos));
    }

    //
    // Initialize the destination rectangle
    //
    CRect rect;
    GetClientRect(rect);

    CRect dstRect(0, 0, rect.Width(), rect.Width());
    int dstWidth = dstRect.Width();
    int dstHeight = dstRect.Height();

    //
    // Create a memory context
    //
    HDC memDC = ::CreateCompatibleDC(hDC);
    HBITMAP memBmp = ::CreateCompatibleBitmap(hDC, dstWidth, dstHeight);
    HGDIOBJ memBmpOld = ::SelectObject(memDC, memBmp);

    //
    // Calculate the size of the source rectangle
    //
    srcLen = (dstWidth / kZoomFactorArr[m_zoomIndex]) / 2;
    if (srcLen == 0) {
        srcLen = 1;
    }
    CRect srcRect((m_curPos.x - srcLen) + 1, (m_curPos.y - srcLen) + 1,
                    m_curPos.x + srcLen, m_curPos.y + srcLen);
    int srcWidth = srcRect.Width();
    int srcHeight = srcRect.Height();

    // Get the screen corresponding to the current position.
    //
    MeaScreenMgr& mgr = MeaScreenMgr::Instance();
    CRect screenRect = mgr.GetScreenRect(mgr.GetScreenIter(m_curPos));

    //
    // Determine if the source rectangle extends beyond the screen, and if so,
    // clear the edges to black and set the clip region.
    //
    if (srcRect.left < screenRect.left || srcRect.right > screenRect.right ||
            srcRect.top < screenRect.top || srcRect.bottom > screenRect.bottom) {
        RECT clipRect;

        clipRect.left = (srcRect.left < screenRect.left) ? screenRect.left : srcRect.left;
        clipRect.top = (srcRect.top < screenRect.top) ? screenRect.top : srcRect.top;
        clipRect.right = (srcRect.right > screenRect.right) ? screenRect.right : srcRect.right;
        clipRect.bottom = (srcRect.bottom > screenRect.bottom) ? screenRect.bottom : srcRect.bottom;

        clipRect.left = (clipRect.left - srcRect.left) * dstWidth / srcWidth;
        clipRect.right = (clipRect.right - srcRect.left) * dstWidth / srcWidth;
        clipRect.top = (clipRect.top - srcRect.top) * dstHeight / srcHeight;
        clipRect.bottom = (clipRect.bottom - srcRect.top) * dstHeight / srcHeight;

        ::BitBlt(memDC, dstRect.left, dstRect.top, dstRect.right, dstRect.bottom, 0, 0, 0, BLACKNESS);

        CRgn rgn;
        rgn.CreateRectRgnIndirect(&clipRect);
        ::SelectClipRgn(memDC, rgn);
    }

    //
    // Blt the screen to the magnifier.
    //
    ::SetStretchBltMode(hDC, COLORONCOLOR);
    HDC screenDC = ::GetDC(nullptr);
    COLORREF colorValue = ::GetPixel(screenDC, m_curPos.x, m_curPos.y);
    ::StretchBlt(memDC, 0, 0, dstWidth, dstHeight, screenDC, srcRect.left, srcRect.top, srcWidth, srcHeight, SRCCOPY);
    ::ReleaseDC(nullptr, screenDC);

    ::FrameRect(memDC, dstRect, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

    //
    // Draw the grid on the image.
    //
    if (m_showGrid && (kZoomFactorArr[m_zoomIndex] >= kMinGridFactor)) {
        CPen gridPen(PS_SOLID, 1, RGB(0, 0, 0));
        HGDIOBJ hOld = ::SelectObject(memDC, gridPen);

        for (j = 0; j < srcWidth; j++) {
            xCoord = j * dstWidth / srcWidth;
            ::MoveToEx(memDC, xCoord, 0, nullptr);
            ::LineTo(memDC, xCoord, dstHeight);
        }
        for (j = 0; j < srcHeight; j++) {
            yCoord = j * dstHeight / srcHeight;
            ::MoveToEx(memDC, 0, yCoord, nullptr);
            ::LineTo(memDC, dstWidth, yCoord);
        }
        ::DeleteObject(::SelectObject(memDC, hOld));
    }

    //
    // Draw the center marker on the image.
    //
    xCoord = srcWidth / 2;
    yCoord = srcHeight / 2;

    RECT centerRect = {
        xCoord * dstWidth / srcWidth,
        yCoord * dstHeight / srcHeight,
        (xCoord + 1) * dstWidth / srcWidth + 1,
        (yCoord + 1) * dstHeight / srcHeight + 1
    };

    CBrush brush(RGB(0xFF, 0, 0));
    ::FrameRect(memDC, &centerRect, brush);

    ::BitBlt(hDC, dstRect.left, dstRect.top, dstRect.right, dstRect.bottom, memDC, 0, 0, SRCCOPY);

    //
    // Clean up blt objects
    //
    ::SelectObject(memDC, memBmpOld);
    ::DeleteObject(memBmp);
    ::DeleteDC(memDC);

    //
    // Report the color information
    //
    TCHAR* colorLbl = _T("");
    CString colorStr;

    switch (m_colorFmt) {
    case RGBFmt:
        colorLbl = _T("RGB:");
        colorStr.Format(_T("%03d %03d %03d"), GetRValue(colorValue),
            GetGValue(colorValue), GetBValue(colorValue));
        break;
    case RGBHexFmt:
        colorLbl = _T("RGB:");
        colorStr.Format(_T("#%02X%02X%02X"), GetRValue(colorValue),
            GetGValue(colorValue), GetBValue(colorValue));
        break;
    case CMYFmt:
        colorLbl = _T("CMY:");
        colorStr.Format(_T("%03d %03d %03d"), 255 - GetRValue(colorValue),
            255 - GetGValue(colorValue), 255 - GetBValue(colorValue));
        break;
    case CMYKFmt:
    {
        int c = 255 - GetRValue(colorValue);
        int m = 255 - GetGValue(colorValue);
        int y = 255 - GetBValue(colorValue);
        int k = min(c, min(m, y));
        colorLbl = _T("CMYK:");
        colorStr.Format(_T("%03d %03d %03d %03d"), c - k, m - k, y - k, k);
    }
    break;
    case HSLFmt:
    {
        HSL hsl;
        MeaColors::RGBtoHSL(colorValue, hsl);
        colorLbl = _T("HSL:");
        colorStr.Format(_T("%03.0f %03.0f %03.0f"),
            240.0 * hsl.hue,
            240.0 * hsl.saturation,
            240.0 * hsl.lightness);
    }
    break;
    case YCbCrFmt:
    {
        double r = GetRValue(colorValue);
        double g = GetGValue(colorValue);
        double b = GetBValue(colorValue);
        double y = 0.257 * r + 0.504 * g + 0.098 * b + 16.0;
        double cb = -0.148 * r - 0.291 * g + 0.439 * b + 128.0;
        double cr = 0.439 * r - 0.368 * g - 0.071 * b + 128.0;
        colorLbl = _T("YCbCr:");
        colorStr.Format(_T("%03.0f %03.0f %03.0f"), y, cb, cr);
    }
    break;
    case YIQFmt:
    {
        double r = GetRValue(colorValue);
        double g = GetGValue(colorValue);
        double b = GetBValue(colorValue);
        double y = 0.299 * r + 0.587 * g + 0.114 * b;
        double i = 0.596 * r - 0.275 * g - 0.321 * b;
        double q = 0.212 * r - 0.523 * g + 0.311 * b;
        colorLbl = _T("YIQ:");
        colorStr.Format(_T("%03.0f %03.0f %03.0f"), y, (i < 0.0) ? 0.0 : i, (q < 0.0) ? 0.0 : q);
    }
    break;
    default:
        MeaAssert(false);
        break;
    }

    m_swatchLabel.SetWindowText(colorLbl);

    CString origStr;
    m_swatchField.GetWindowText(origStr);
    if (origStr != colorStr)
        m_swatchField.SetWindowText(colorStr);

    RECT colorRect;
    m_swatchWin.GetClientRect(&colorRect);
    CDC* dc = m_swatchWin.GetDC();
    dc->FillSolidRect(&colorRect, colorValue);
    m_swatchWin.ReleaseDC(dc);
}

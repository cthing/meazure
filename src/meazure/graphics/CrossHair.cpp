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

#include <meazure/pch.h>
#include "CrossHair.h"
#include "Plotter.h"
#include <meazure/resource.h>
#include <meazure/ui/Layout.h>
#define COMPILE_LAYERED_WINDOW_STUBS
#include <meazure/ui/LayeredWindows.h>
#include <cassert>
#include <functional>


CSize MeaCrossHair::m_size;
CSize MeaCrossHair::m_halfSize;
CSize MeaCrossHair::m_spread;
UINT MeaCrossHair::m_flashInterval { 100 };


BEGIN_MESSAGE_MAP(MeaCrossHair, MeaGraphic)
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


MeaCrossHair::MeaCrossHair(const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider) :
    MeaGraphic(),
    m_screenProvider(screenProvider),
    m_unitsProvider(unitsProvider),
    m_callback(nullptr),
    m_mouseCaptured(false),
    m_mouseOver(false),
    m_backBrush(nullptr),
    m_borderBrush(nullptr),
    m_hiliteBrush(nullptr),
    m_drawState(Normal),
    m_flashCount(0),
    m_opacity(255),
    m_origCHBitmap(nullptr),
    m_origBackBitmap(nullptr) {}

MeaCrossHair::~MeaCrossHair() {
    m_callback = nullptr;
    m_backBrush = nullptr;
    m_borderBrush = nullptr;
    m_hiliteBrush = nullptr;
    m_origCHBitmap = nullptr;
    m_origBackBitmap = nullptr;
}

void MeaCrossHairCallback::OnCHSelect(const CHInfo* /* info */) {}

void MeaCrossHairCallback::OnCHDeselect(const CHInfo* /* info */) {}

void MeaCrossHairCallback::OnCHMove(const CHInfo* /* info */) {}

void MeaCrossHairCallback::OnCHEnter(const CHInfo* /* info */) {}

void MeaCrossHairCallback::OnCHLeave(const CHInfo* /* info */) {}

bool MeaCrossHair::Create(COLORREF borderColor, COLORREF backColor,
                          COLORREF hiliteColor, BYTE opacity,
                          MeaCrossHairCallback* callback,
                          const CWnd* parent, UINT toolTipId, UINT id) {
    m_callback = callback;

    SetColors(borderColor, backColor, hiliteColor);

    // Calculate class properties of the crosshair when the first
    // instance of the class is created.
    //
    if (m_size.cx == 0) {
        MeaFSize res = m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter(AfxGetMainWnd()));

        m_size = m_unitsProvider.ConvertToPixels(MeaInchesId, res, 0.25, 25);
        m_size.cx += 1 - (m_size.cx % 2);       // Must be odd
        m_size.cy += 1 - (m_size.cy % 2);

        m_halfSize.cx = m_size.cx / 2;
        m_halfSize.cy = m_size.cy / 2;

        m_spread = m_unitsProvider.ConvertToPixels(MeaInchesId, res, 0.04, 4);
        m_spread.cx += (m_spread.cx % 2);       // Must be even
        m_spread.cy += (m_spread.cy % 2);
    }

    assert(m_backBrush != nullptr);
    CString wndClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
        LoadCursor(nullptr, IDC_ARROW));

    if (!MeaGraphic::Create(wndClass, m_size, parent, id)) {
        return false;
    }

    if (HaveLayeredWindows() && (parent == nullptr)) {
        ModifyStyleEx(0, WS_EX_LAYERED);
        SetOpacity(opacity);
    }

    SetRegion();

    // If layered windows are available (Windows 2000 and beyond),
    // and the crosshair is being used as a child window, create
    // the bitmaps used to show crosshair opacity.
    //
    if (HaveLayeredWindows() && (m_parent != nullptr)) {
        CRect rect;

        CDC* dc = GetDC();

        m_backDC.CreateCompatibleDC(dc);
        m_backBitmap.CreateCompatibleBitmap(dc, m_size.cx, m_size.cy);
        m_origBackBitmap = m_backDC.SelectObject(&m_backBitmap);

        m_chDC.CreateCompatibleDC(dc);
        m_chBitmap.CreateCompatibleBitmap(dc, m_size.cx, m_size.cy);
        m_origCHBitmap = m_chDC.SelectObject(&m_chBitmap);

        MeaLayout::DrawOpacityBackground(*this, m_backDC, m_screenProvider, m_unitsProvider);

        ReleaseDC(dc);
    }

    if (toolTipId != 0xffff) {
        m_toolTip.Create(this, TTS_ALWAYSTIP);
        m_toolTip.AddTool(this, toolTipId, nullptr, 0);
    }

    return true;
}

void MeaCrossHair::OnDestroy() {
    MeaGraphic::OnDestroy();

    DestroyColors();

    if (m_origCHBitmap != nullptr) {
        m_chDC.SelectObject(m_origCHBitmap);
        m_chBitmap.DeleteObject();
        m_chDC.DeleteDC();
        m_origCHBitmap = nullptr;

        m_backDC.SelectObject(m_origBackBitmap);
        m_backBitmap.DeleteObject();
        m_backDC.DeleteDC();
        m_origBackBitmap = nullptr;
    }
}

BOOL MeaCrossHair::PreTranslateMessage(MSG* pMsg) {
    // Need to intercept messages to support tooltips
    // on the crosshair.
    //
    if (m_toolTip.m_hWnd != nullptr) {
        m_toolTip.RelayEvent(pMsg);
    }
    return MeaGraphic::PreTranslateMessage(pMsg);
}

void MeaCrossHair::SetColors(COLORREF borderColor, COLORREF backColor,
                             COLORREF hiliteColor) {
    DestroyColors();
    m_borderBrush = new CBrush(borderColor);
    m_backBrush = new CBrush(backColor);
    m_hiliteBrush = new CBrush(hiliteColor);

    if (m_hWnd != nullptr) {
        Invalidate(FALSE);
        UpdateWindow();
    }
}

void MeaCrossHair::SetOpacity(BYTE opacity) {
    m_opacity = opacity;

    if (m_hWnd != nullptr) {
        if (m_parent == nullptr) {
            SetLayeredWindowAttributes(*this, 0, opacity, LWA_ALPHA);
        } else {
            Invalidate(FALSE);
            UpdateWindow();
        }
    }
}

void MeaCrossHair::DestroyColors() {
    if (m_backBrush != nullptr) {
        delete m_backBrush;
        m_backBrush = nullptr;
    }
    if (m_borderBrush != nullptr) {
        delete m_borderBrush;
        m_borderBrush = nullptr;
    }
    if (m_hiliteBrush != nullptr) {
        delete m_hiliteBrush;
        m_hiliteBrush = nullptr;
    }
}

void MeaCrossHair::SetRegion() {
    POINT coords[4 * kTotalLayers];
    POINT* coord = coords;

    // Each petal of the crosshair is made up of stacked rectangles.
    // Each rectangle is thk high by 2 * spread wide. Each rectangle
    // is called a layer.
    //
    //           |---| spread
    //       ********* ---------
    //       *       * - thk   |
    //        *     *          |
    //        *     *          |
    //         *   *           |
    //         *   *       5 layers
    //          * *            |
    //          * *            |
    //           *             |
    //           * -------------

    std::function<void(int, int)> addPoint = [&](int x, int y) {
        coord->x = x;
        coord->y = y;
        coord++;
    };

    MeaPlotter::PlotCrosshair(m_size, m_spread, kPetalLayers, addPoint);

    HRGN region = ::CreatePolyPolygonRgn(coords, m_numCoords.data(), kTotalLayers, ALTERNATE);
    SetWindowRgn(region, FALSE);
}

void MeaCrossHair::SetPosition(const POINT& center) {
    POINT leftTop = GetLeftTop(center);
    SetWindowPos(nullptr, leftTop.x, leftTop.y, 0, 0,
        SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
}

void MeaCrossHair::Flash(int flashCount) {
    if (m_hWnd != nullptr) {
        m_flashCount = flashCount;
        m_drawState = Inverted;
        Invalidate(FALSE);
        UpdateWindow();
        SetTimer(ID_MEA_CROSSHAIR_TIMER, m_flashInterval, nullptr);
    }
}

void MeaCrossHair::OnTimer(UINT_PTR timerId) {
    KillTimer(timerId);
    m_drawState = (m_drawState == Normal) ? Inverted : Normal;
    Invalidate(FALSE);
    UpdateWindow();
    if (--m_flashCount > 0) {
        SetTimer(timerId, m_flashInterval, nullptr);
    }
}

void MeaCrossHair::OnPaint() {
    CPaintDC dc(this);

    // If the crosshair is a popup, simply draw it normally. If it
    // is a child window, alpha blend it onto its background.
    //
    if (m_origCHBitmap == nullptr) {
        DrawCrossHair(dc);
    } else {
        DrawCrossHair(m_chDC);
        MeaLayout::AlphaBlend(m_chDC, m_backDC, m_size.cx, m_size.cy, m_opacity);
        dc.BitBlt(0, 0, m_size.cx, m_size.cy, &m_chDC, 0, 0, SRCCOPY);
    }
}

void MeaCrossHair::DrawCrossHair(CDC& dc) {
    // Fill the crosshair.
    //
    CRect rect;
    GetClientRect(&rect);

    CBrush* pOldBrush = dc.SelectObject(m_mouseOver ? m_hiliteBrush : m_backBrush);
    dc.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    dc.SelectObject(pOldBrush);

    CRgn rgn;
    rgn.CreateRectRgn(0, 0, 1, 1);
    GetWindowRgn(rgn);

    // Draw a contrasting outline around the crosshair.
    //
    dc.FrameRgn(&rgn, (m_drawState == Normal) ? m_borderBrush : m_hiliteBrush, 1, 1);
}

void MeaCrossHair::FillInfo(MeaCrossHairCallback::CHInfo& chs, UINT flags,
                            const CPoint& point) {
    chs.ch = this;
    chs.centerPoint = point - (m_pointerOffset - m_halfSize);
    ClientToScreen(&chs.centerPoint);
    chs.centerPoint = m_screenProvider.LimitPosition(chs.centerPoint);
    chs.id = m_id;
    chs.flags = flags;
}

void MeaCrossHair::OnLButtonDown(UINT flags, CPoint point) {
    m_pointerOffset = point;

    SetCapture();
    m_mouseCaptured = true;

    MeaCrossHairCallback::CHInfo chs;
    FillInfo(chs, flags, point);

    if (m_callback != nullptr) {
        m_callback->OnCHSelect(&chs);
    }
}

void MeaCrossHair::OnLButtonUp(UINT flags, CPoint point) {
    m_mouseCaptured = false;
    ::ReleaseCapture();

    MeaCrossHairCallback::CHInfo chs;
    FillInfo(chs, flags, point);

    if (m_callback != nullptr) {
        m_callback->OnCHDeselect(&chs);
    }
}

void MeaCrossHair::OnMouseMove(UINT flags, CPoint point) {
    //
    // If crosshair has been selected, send callback.
    //
    if (m_mouseCaptured) {
        MeaCrossHairCallback::CHInfo chs;
        FillInfo(chs, flags, point);

        if (m_callback != nullptr) {
            m_callback->OnCHMove(&chs);
        }
    }

    //
    // If not already highlighted, highlight the crosshair and
    // set up for receiving the mouse leave event.
    //
    if (!m_mouseOver) {
        m_mouseOver = true;

        MeaCrossHairCallback::CHInfo chs;
        FillInfo(chs, flags, point);

        if (m_callback != nullptr) {
            m_callback->OnCHEnter(&chs);
        }

        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        ::_TrackMouseEvent(&tme);

        Invalidate(FALSE);
        UpdateWindow();
    }
}

LRESULT MeaCrossHair::OnMouseLeave(WPARAM /* wParam */, LPARAM /* lParam */) {
    m_mouseOver = false;

    CPoint point(0, 0);
    MeaCrossHairCallback::CHInfo chs;
    FillInfo(chs, 0, point);

    if (m_callback != nullptr) {
        m_callback->OnCHLeave(&chs);
    }

    Invalidate(TRUE);
    UpdateWindow();

    return 0;
}

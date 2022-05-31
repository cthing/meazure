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
#include "DataWin.h"
#include "LayeredWindows.h"
#include <meazure/tools/ToolMgr.h>
#include <meazure/ui/Layout.h>
#include <meazure/utilities/Geometry.h>
#include <meazure/resource.h>


BEGIN_MESSAGE_MAP(MeaDataWin, CWnd)
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_TIMER()
END_MESSAGE_MAP()


bool MeaDataWin::m_armed { kDefArmed };
SIZE MeaDataWin::m_margin { 3, 1 };
UINT MeaDataWin::m_flashInterval { 100 };   // Flash interval in milliseconds


MeaDataWin::MeaDataWin(const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider) :
    CWnd(),
    m_screenProvider(screenProvider),
    m_unitsProvider(unitsProvider),
    m_parent(nullptr),
    m_textHeight(0),
    m_winHeight(0),
    m_dataOffset(0),
    m_drawState(Normal),
    m_flashCount(0),
    m_opacity(255) {}

MeaDataWin::~MeaDataWin() {
    m_parent = nullptr;
}

void MeaDataWin::SaveProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        profile.WriteBool(_T("ShowDataWin"), m_armed);
    }
}

void MeaDataWin::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        m_armed = profile.ReadBool(_T("ShowDataWin"), m_armed);
    }
}

void MeaDataWin::MasterReset() {
    m_armed = kDefArmed;
}

bool MeaDataWin::Create(BYTE opacity,
                        UINT xLabelId, UINT yLabelId,
                        UINT wLabelId, UINT hLabelId,
                        UINT dLabelId, UINT aLabelId,
                        const CWnd* parent) {
    m_parent = parent;

    // Create the window.
    //
    bool ret = CreateEx(
                    (HaveLayeredWindows() && (parent == nullptr)) ? WS_EX_LAYERED : 0,
                    AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0),
                    "",
                    ((parent == nullptr) ? WS_POPUP : WS_CHILD) | WS_BORDER,
                    0, 0, 5, 5,
                    (parent == nullptr) ? *AfxGetMainWnd() : *parent, 0) ? true : false;

    SetOpacity(opacity);

    // Set the labels for the data window.
    //
    if (xLabelId != kNoLabelId) {
        VERIFY(m_xLabel.LoadString(xLabelId));
        VERIFY(m_yLabel.LoadString(yLabelId));
    }
    if (wLabelId != kNoLabelId) {
        VERIFY(m_wLabel.LoadString(wLabelId));
        VERIFY(m_hLabel.LoadString(hLabelId));
    }
    if (dLabelId != kNoLabelId) {
        VERIFY(m_dLabel.LoadString(dLabelId));
    }
    if (aLabelId != kNoLabelId) {
        VERIFY(m_aLabel.LoadString(aLabelId));
    }

    // Get the font use by tooltips (same as that used by the status bar.
    //
    NONCLIENTMETRICS ncMetrics;

    ncMetrics.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS),
                        &ncMetrics, 0);
    m_font.CreateFontIndirect(&ncMetrics.lfStatusFont);
    SetFont(&m_font, FALSE);

    CDC* dc = GetDC();
    if (dc == nullptr) {
        return false;
    }

    // Select the new font into the DC
    //
    CFont* origFont = dc->SelectObject(&m_font);

    // Get the character height for the font.
    //
    TEXTMETRIC metrics;

    if (!dc->GetTextMetrics(&metrics)) {
        ReleaseDC(dc);
        return false;
    }
    m_textHeight = metrics.tmHeight;
    m_winHeight = 2 * m_margin.cy + metrics.tmInternalLeading;
    if (xLabelId != kNoLabelId) {
        m_winHeight += 2 * m_textHeight;
    }
    if (wLabelId != kNoLabelId) {
        m_winHeight += 2 * m_textHeight;
    }
    if (dLabelId != kNoLabelId) {
        m_winHeight += m_textHeight;
    }
    if (aLabelId != kNoLabelId) {
        m_winHeight += m_textHeight;
    }

    // Get the max length of the labels.
    //
    m_dataOffset = 0;

    if (xLabelId != kNoLabelId) {
        CSize xs = dc->GetTextExtent(m_xLabel);
        CSize ys = dc->GetTextExtent(m_yLabel);

        m_dataOffset = ((xs.cx > ys.cx) ? xs.cx : ys.cx);
    }
    if (wLabelId != kNoLabelId) {
        CSize ws = dc->GetTextExtent(m_wLabel);
        CSize hs = dc->GetTextExtent(m_hLabel);

        if (m_dataOffset < ws.cx) {
            m_dataOffset = ws.cx;
        }
        if (m_dataOffset < hs.cx) {
            m_dataOffset = hs.cx;
        }
    }
    if (dLabelId != kNoLabelId) {
        CSize ds = dc->GetTextExtent(m_dLabel);

        if (m_dataOffset < ds.cx) {
            m_dataOffset = ds.cx;
        }
    }
    if (aLabelId != kNoLabelId) {
        CSize as = dc->GetTextExtent(m_aLabel);

        if (m_dataOffset < as.cx) {
            m_dataOffset = as.cx;
        }
    }
    m_dataOffset += 3;

    dc->SelectObject(origFont);
    ReleaseDC(dc);

    return ret;
}

void MeaDataWin::OnDestroy() {
    CWnd::OnDestroy();

    m_font.DeleteObject();
}

void MeaDataWin::Show() {
    if ((m_armed || (m_parent != nullptr)) && (m_hWnd != nullptr)) {
        SetWindowPos(nullptr, 0, 0, CalcWidth(), m_winHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        ShowWindow(SW_SHOWNOACTIVATE);
    }
}

void MeaDataWin::Strobe() {
    if (m_hWnd && IsWindowVisible()) {
        m_flashCount = 1;
        m_drawState = Inverted;
        Invalidate(FALSE);
        UpdateWindow();
        SetTimer(ID_MEA_DATAWIN_TIMER, m_flashInterval, nullptr);
    }
}

void MeaDataWin::SetOpacity(BYTE opacity) {
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

int MeaDataWin::CalcWidth() {
    const CRect& vscreen = m_screenProvider.GetVirtualRect();
    CPoint topLeft(vscreen.TopLeft());
    CPoint bottomRight(vscreen.BottomRight());
    MeaFSize wh = m_unitsProvider.GetWidthHeight(topLeft, bottomRight);

    CString xStr = _T("-") + m_unitsProvider.Format(MeaX, wh.cx);
    CString yStr = _T("-") + m_unitsProvider.Format(MeaY, wh.cy);
    CString wStr = m_unitsProvider.Format(MeaW, wh.cx);
    CString hStr = m_unitsProvider.Format(MeaH, wh.cy);
    CString dStr = m_unitsProvider.Format(MeaD, MeaGeometry::CalcLength(wh.cx, wh.cy));
    CString aStr = m_unitsProvider.FormatConvertAngle(-3.0);

    int maxLen = 0;

    CDC* dc = GetDC();
    CFont* origFont = dc->SelectObject(&m_font);
    CSize size;

    size = dc->GetTextExtent(xStr);
    if (size.cx > maxLen) {
        maxLen = size.cx;
    }
    size = dc->GetTextExtent(yStr);
    if (size.cx > maxLen) {
        maxLen = size.cx;
    }
    size = dc->GetTextExtent(wStr);
    if (size.cx > maxLen) {
        maxLen = size.cx;
    }
    size = dc->GetTextExtent(hStr);
    if (size.cx > maxLen) {
        maxLen = size.cx;
    }
    size = dc->GetTextExtent(dStr);
    if (size.cx > maxLen) {
        maxLen = size.cx;
    }
    if (MeaToolMgr::Instance().IsToolEnabled(MeaAngleTool::kToolName)) {
        size = dc->GetTextExtent(aStr);
        if (size.cx > maxLen) {
            maxLen = size.cx;
        }
    }

    dc->SelectObject(origFont);
    ReleaseDC(dc);

    return m_dataOffset + maxLen + 2 * m_margin.cx;
}

void MeaDataWin::Update(const CRect& rect) {
    if (IsWindowVisible()) {
        CRect testRect(rect);
        CRect winRect;
        GetClientRect(winRect);
        testRect.InflateRect(winRect.Width(), winRect.Height());

        int x, y;
        const CRect& screenRect = m_screenProvider.GetScreenRect(m_screenProvider.GetScreenIter(rect));

        y = (testRect.bottom >= screenRect.bottom) ? testRect.top : rect.bottom;
        x = (testRect.left <= screenRect.left) ? rect.right : testRect.left;

        winRect.OffsetRect(x, y);
        if (winRect.right > screenRect.right) {
            x -= winRect.Width();
        }

        SetWindowPos(nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

        Invalidate(FALSE);
        UpdateWindow();
    }
}

void MeaDataWin::OnPaint() {
    CPaintDC dc(this);

    if (!HaveLayeredWindows() || (m_parent == nullptr)) {
        DrawWin(dc);
    } else {
        CRect rect;
        CDC backDC;
        CDC dataDC;
        CBitmap backBitmap;
        CBitmap dataBitmap;

        GetClientRect(rect);

        backDC.CreateCompatibleDC(&dc);
        backBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
        CBitmap* origBackBitmap = backDC.SelectObject(&backBitmap);

        dataDC.CreateCompatibleDC(&dc);
        dataBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
        CBitmap* origDataBitmap = dataDC.SelectObject(&dataBitmap);

        MeaLayout::DrawOpacityBackground(*this, backDC, m_screenProvider, m_unitsProvider);
        DrawWin(dataDC);

        MeaLayout::AlphaBlend(dataDC, backDC, rect.Width(), rect.Height(), m_opacity);

        dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dataDC, 0, 0, SRCCOPY);

        dataDC.SelectObject(origDataBitmap);
        dataBitmap.DeleteObject();
        dataDC.DeleteDC();

        backDC.SelectObject(origBackBitmap);
        backBitmap.DeleteObject();
        backDC.DeleteDC();
    }
}

void MeaDataWin::DrawWin(CDC& dc) {
    CRect clientRect;
    bool haveLine = false;

    CFont* origFont = dc.SelectObject(&m_font);
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(GetSysColor((m_drawState == Normal) ? COLOR_INFOTEXT : COLOR_INFOBK));

    GetClientRect(clientRect);

    CBrush backBrush;
    backBrush.CreateSysColorBrush(COLOR_INFOBK);
    CBrush* origBrush = dc.SelectObject(&backBrush);
    dc.PatBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), PATCOPY);

    int y = m_margin.cy;
    int x = m_margin.cx;

    if (!m_xLabel.IsEmpty()) {
        haveLine = true;
        dc.TextOut(x, y, m_xLabel);
        dc.TextOut(x + m_dataOffset, y, m_xData);
        y += m_textHeight;
        dc.TextOut(x, y, m_yLabel);
        dc.TextOut(x + m_dataOffset, y, m_yData);
    }

    if (!m_wLabel.IsEmpty()) {
        if (haveLine) {
            y += m_textHeight;
        } else {
            haveLine = true;
        }
        dc.TextOut(x, y, m_wLabel);
        dc.TextOut(x + m_dataOffset, y, m_wData);
        y += m_textHeight;
        dc.TextOut(x, y, m_hLabel);
        dc.TextOut(x + m_dataOffset, y, m_hData);
    }

    if (!m_dLabel.IsEmpty()) {
        if (haveLine) {
            y += m_textHeight;
        } else {
            haveLine = true;
        }
        dc.TextOut(x, y, m_dLabel);
        dc.TextOut(x + m_dataOffset, y, m_dData);
    }

    if (!m_aLabel.IsEmpty()) {
        if (haveLine) {
            y += m_textHeight;
        } else {
            haveLine = true;
        }
        dc.TextOut(x, y, m_aLabel);
        dc.TextOut(x + m_dataOffset, y, m_aData);
    }

    dc.SelectObject(origBrush);
    dc.SelectObject(origFont);
}

void MeaDataWin::OnTimer(UINT_PTR timerId) {
    KillTimer(timerId);
    m_drawState = (m_drawState == Normal) ? Inverted : Normal;
    Invalidate(FALSE);
    UpdateWindow();
    if (--m_flashCount > 0) {
        SetTimer(timerId, m_flashInterval, nullptr);
    }
}

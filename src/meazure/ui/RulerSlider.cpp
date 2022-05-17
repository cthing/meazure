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
#include "RulerSlider.h"
#include <meazure/Messages.h>


IMPLEMENT_DYNAMIC(MeaRulerSlider, CProgressCtrl)


BEGIN_MESSAGE_MAP(MeaRulerSlider, CProgressCtrl)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_ENABLE()
END_MESSAGE_MAP()


MeaRulerSlider::MeaRulerSlider() :
    CProgressCtrl(),
    m_vertical(false),
    m_captured(false),
    m_sliderActive(false),
    m_sliderPos(0) {
    VERIFY(m_posFont.CreatePointFont(80, _T("Arial"), nullptr));

    m_activePen.CreatePen(PS_SOLID, 0, GetSysColor(COLOR_WINDOWTEXT));
    m_inactivePen.CreatePen(PS_SOLID, 0, GetSysColor(COLOR_INACTIVECAPTIONTEXT));

    m_activeBrush.CreateSolidBrush(GetSysColor(COLOR_WINDOWTEXT));
    m_inactiveBrush.CreateSolidBrush(GetSysColor(COLOR_INACTIVECAPTIONTEXT));
}


MeaRulerSlider::~MeaRulerSlider() {}

BOOL MeaRulerSlider::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    Init(rect);

    return CProgressCtrl::Create(dwStyle, rect, pParentWnd, nID);
}

void MeaRulerSlider::Init(const CRect& rect) {
    m_vertical = (GetStyle() & PBS_VERTICAL) != 0;

    if (m_vertical) {
        m_sliderPos = rect.top;
        m_sliderRect.bottom = m_sliderPos;
    } else {
        m_sliderPos = rect.left;
        m_sliderRect.right = m_sliderPos;
    }
}

void MeaRulerSlider::PreSubclassWindow() {
    if (IsWindow(m_hWnd)) {
        CRect rect;
        GetClientRect(rect);
        Init(rect);
    }

    CProgressCtrl::PreSubclassWindow();
}

int MeaRulerSlider::GetSliderPos() {
    return (IsWindow(m_hWnd) ? m_sliderPos : 0);
}

void MeaRulerSlider::SetSliderPos(int pos) {
    m_sliderPos = (pos > 0) ? pos : 1;
    Invalidate(TRUE);
}

bool MeaRulerSlider::IsHot(int pos) const {
    return (pos >= (m_sliderPos - kHotSpotOffset) && pos <= (m_sliderPos + kHotSpotOffset));
}

void MeaRulerSlider::OnLButtonDown(UINT /* nFlags */, CPoint point) {
    SetCapture();
    m_captured = true;

    if (IsHot(m_vertical ? point.y : point.x)) {
        SetCursor(LoadCursor(nullptr, m_vertical ? IDC_SIZENS : IDC_SIZEWE));
        m_sliderActive = true;

        Invalidate(TRUE);
    }
}

void MeaRulerSlider::OnLButtonUp(UINT /* nFlags */, CPoint /* point */) {
    if (m_captured) {
        m_captured = false;
        m_sliderActive = false;
        Invalidate(TRUE);
        ReleaseCapture();
    }
}

void MeaRulerSlider::OnMouseMove(UINT /* nFlags */, CPoint point) {
    if (m_captured) {
        CRect rcClient;
        GetClientRect(&rcClient);

        if (m_sliderActive) {
            if (m_vertical) {
                if (point.y < rcClient.bottom && point.y > rcClient.top) {
                    m_sliderPos = point.y;
                } else if (point.y <= rcClient.top) {
                    m_sliderPos = rcClient.top + 1;
                } else {
                    m_sliderPos = rcClient.bottom;
                }
            } else {
                if (point.x < rcClient.right && point.x > rcClient.left) {
                    m_sliderPos = point.x;
                } else if (point.x <= rcClient.left) {
                    m_sliderPos = rcClient.left + 1;
                } else {
                    m_sliderPos = rcClient.right;
                }
            }

            Invalidate(TRUE);

            GetParent()->PostMessage(MeaCaliperPositionMsg, m_sliderPos, GetDlgCtrlID());
        }

        TRACE("Left: %d\n", GetSliderPos());
    }

    if (IsHot(m_vertical ? point.y : point.x)) {
        SetCursor(LoadCursor(nullptr, m_vertical ? IDC_SIZENS : IDC_SIZEWE));
    }
}

void MeaRulerSlider::OnEnable(BOOL bEnable) {
    CProgressCtrl::OnEnable(bEnable);
    Invalidate();
}

void MeaRulerSlider::OnSize(UINT nType, int cx, int cy) {
    CProgressCtrl::OnSize(nType, cx, cy);

    Init(CRect(0, 0, 0, 0));
}

BOOL MeaRulerSlider::OnEraseBkgnd(CDC* /* pDC */) {
    return TRUE;
}

void MeaRulerSlider::OnPaint() {
    CPaintDC dc(this);
    CRect rcClient;
    GetClientRect(&rcClient);

    CPen* pOldPen = dc.SelectObject(IsWindowEnabled() ? &m_activePen : &m_inactivePen);

    // Slider.
    //
    CRect rcWite;

    if (m_vertical) {
        m_sliderRect.bottom = m_sliderPos;
        m_sliderRect.top = rcClient.top;
        m_sliderRect.left = rcClient.left;
        m_sliderRect.right = rcClient.right;

        rcWite.left = m_sliderRect.left;
        rcWite.top = m_sliderRect.bottom;
    } else {
        m_sliderRect.right = m_sliderPos;
        m_sliderRect.left = rcClient.left;
        m_sliderRect.top = rcClient.top;
        m_sliderRect.bottom = rcClient.bottom;

        rcWite.left = m_sliderRect.right;
        rcWite.top = m_sliderRect.top;
    }
    rcWite.right = rcClient.right;
    rcWite.bottom = rcClient.bottom;

    if (IsWindowEnabled()) {
        dc.FillSolidRect(m_sliderRect, GetSysColor(COLOR_INFOBK));
    } else {
        dc.FillSolidRect(m_sliderRect, GetSysColor(COLOR_WINDOW));
    }
    dc.FillSolidRect(rcWite, GetSysColor(COLOR_WINDOW));

    // Draw ticks.
    //    
    if (m_vertical) {
        int y;
        int tickHeight = rcClient.Width() / kTickFactor;

        for (y = rcClient.top - 1; y <= rcClient.bottom; y += kTickPosition) {
            dc.MoveTo(rcClient.right, y);
            dc.LineTo(rcClient.right - tickHeight, y);
        }
    } else {
        int x;
        int tickHeight = rcClient.Height() / kTickFactor;

        for (x = rcClient.left - 1; x <= rcClient.right; x += kTickPosition) {
            dc.MoveTo(x, rcClient.bottom);
            dc.LineTo(x, rcClient.bottom - tickHeight);
        }
    }

    // Draw the caliper markers on the slider.
    //
    DrawCalipers(&dc);

    // Draw text indicating number of ticks passed by.
    //
    if (m_sliderActive) {
        CString str;
        str.Format(_T(" %d "), m_vertical ? (m_sliderRect.bottom - rcClient.top) : (m_sliderRect.right - rcClient.left));

        CFont* pOldFont = dc.SelectObject(&m_posFont);
        CSize size = dc.GetTextExtent(str);

        int x, y;

        if (m_vertical) {
            x = 0;
            y = rcClient.top;
            if (size.cy < m_sliderRect.Height()) {
                y += (m_sliderRect.Height() - size.cy) / 2;
            }
        } else {
            int tickHeight = rcClient.Height() / kTickFactor;

            y = rcClient.bottom - (tickHeight + rcClient.Height() + size.cy) / 2;
            x = rcClient.left;
            if (size.cx < m_sliderRect.Width()) {
                x += (m_sliderRect.Width() - size.cx) / 2;
            }
        }

        dc.SetBkMode(TRANSPARENT);
        dc.TextOut(x, y, str);

        dc.SelectObject(pOldFont);
    }

    dc.SelectObject(pOldPen);
}

void MeaRulerSlider::DrawCalipers(CDC* pDC) {
    CRect rcClient;
    POINT pts[3], pte[3];

    GetClientRect(&rcClient);

    if (m_vertical) {
        int tickHeight = rcClient.Width() / kTickFactor;

        pts[0].x = rcClient.right - 1;
        pts[0].y = rcClient.top;

        pts[1].x = rcClient.right - rcClient.Width() / 2;
        pts[1].y = rcClient.top;

        pts[2].x = rcClient.right - 1;
        pts[2].y = rcClient.top + tickHeight;

        pte[0].x = rcClient.right - 1;
        pte[0].y = m_sliderRect.bottom - 1;

        pte[1].x = rcClient.right - rcClient.Width() / 2;
        pte[1].y = m_sliderRect.bottom - 1;

        pte[2].x = rcClient.right - 1;
        pte[2].y = (m_sliderRect.bottom - tickHeight) - 1;

        pDC->MoveTo(m_sliderRect.left, pte[0].y);
        pDC->LineTo(m_sliderRect.left + rcClient.Width() / 2 - 2, pte[0].y);
    } else {
        int tickHeight = rcClient.Height() / kTickFactor;

        pts[0].x = rcClient.left;
        pts[0].y = rcClient.bottom - 1;

        pts[1].x = rcClient.left + tickHeight;
        pts[1].y = rcClient.bottom - 1;

        pts[2].x = rcClient.left;
        pts[2].y = rcClient.bottom - rcClient.Height() / 2;

        pte[0].x = m_sliderRect.right - 1;
        pte[0].y = rcClient.bottom - 1;

        pte[1].x = (m_sliderRect.right - tickHeight) - 1;
        pte[1].y = rcClient.bottom - 1;

        pte[2].x = m_sliderRect.right - 1;
        pte[2].y = rcClient.bottom - rcClient.Height() / 2;

        pDC->MoveTo(pte[0].x, m_sliderRect.top);
        pDC->LineTo(pte[0].x, m_sliderRect.top + rcClient.Height() / 2 - 2);
    }

    CBrush* pOldBrush = static_cast<CBrush*>(pDC->SelectObject(IsWindowEnabled() ? &m_activeBrush : &m_inactiveBrush));
    pDC->Polygon(pts, 3);
    pDC->Polygon(pte, 3);
    pDC->SelectObject(pOldBrush);
}

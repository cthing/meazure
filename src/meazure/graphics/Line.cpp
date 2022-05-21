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
#include "Line.h"
#include <meazure/utilities/MeaAssert.h>
#include <meazure/utilities/Geometry.h>
#include "Colors.h"
#include <cstdlib>


const CSize MeaLine::kMargin(5, 5);

int* MeaLine::m_varr = nullptr;


BEGIN_MESSAGE_MAP(MeaLine, MeaGraphic)
    ON_MESSAGE(MeaHPTimerMsg, OnHPTimer)
END_MESSAGE_MAP()


MeaLine::MeaLine() :
    MeaGraphic(),
    m_startPoint(kInitCoord, kInitCoord),
    m_endPoint(kInitCoord, kInitCoord),
    m_wasAngled(true),
    m_foreBrush(new CBrush(MeaColors::Get(MeaColors::LineFore))),
    m_orientation(Vertical),
    m_arr(nullptr), m_count(0), m_shrink(0) {}

MeaLine::~MeaLine() {
    try {
        m_timer.Stop();

        if (m_varr != nullptr) {
            delete[] m_varr;
            m_varr = nullptr;
        }
        if (m_arr != nullptr) {
            delete[] m_arr;
        }

        delete m_foreBrush;
    } catch (...) {
        MeaAssert(false);
    }
}

bool MeaLine::Create(int shrink, const MeaScreenProvider& screenProvider, const CWnd* parent) {
    const CRect& vscreen = screenProvider.GetVirtualRect();
    UINT size = 2 * static_cast<UINT>(MeaGeometry::CalcLength(static_cast<double>(vscreen.Width()),
                                                              static_cast<double>(vscreen.Height())));
    m_shrink = shrink;

    if (m_varr == nullptr) {
        m_varr = new int[size];
        for (UINT i = 0; i < size; i++) {
            m_varr[i] = 4;
        }
    }
    if (m_arr == nullptr) {
        m_arr = new POINT[4 * size];
    }

    m_timer.Create(this);

    CString wndClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, nullptr, *m_foreBrush);

    return MeaGraphic::Create(wndClass, CSize(0, 0), parent);
}

void MeaLine::Hide() {
    m_timer.Stop();
    MeaGraphic::Hide();
}

void MeaLine::SetColor(COLORREF color) {
    CBrush* brush = new CBrush(color);
    if (m_hWnd != nullptr) {
        ::SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND,
                        reinterpret_cast<LONG_PTR>(static_cast<HBRUSH>(*brush)));
    }

    delete m_foreBrush;
    m_foreBrush = brush;

    if (m_hWnd != nullptr) {
        Invalidate();
        UpdateWindow();
    }
}

void MeaLine::PlotLine() {
    POINT startPoint;
    POINT endPoint;

    // The performance of the region creation functions appears to
    // be sensitive to the direction in which the line is drawn.
    //
    if (m_startPoint.y > m_endPoint.y) {
        startPoint = m_startPoint;
        endPoint = m_endPoint;
    } else {
        startPoint = m_endPoint;
        endPoint = m_startPoint;
    }

    // Bresenham algorithm adapted from "Graphics Gems",
    // Academic Press, 1990, p. 685. We need to create the line in
    // this brute force way because relying on the polygon region
    // method produces a horrible looking line.
    //
    int dx = endPoint.x - startPoint.x;
    int dy = endPoint.y - startPoint.y;

    int ax = 2 * abs(dx);
    int ay = 2 * abs(dy);

    int sx = (dx < 0) ? -1 : 1;
    int sy = (dy < 0) ? -1 : 1;

    int x = startPoint.x;
    int y = startPoint.y;

    m_count = 0;

    if (ax > ay) {      /* x dominant */
        int d = ay - (ax / 2);
        while (x != endPoint.x) {
            if (d >= 0) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
            AddPoint(x, y);
        }
    } else {            /* y dominant */
        int d = ax - (ay / 2);
        while (y != endPoint.y) {
            if (d >= 0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
            AddPoint(x, y);
        }
    }
}

void MeaLine::SetPosition(const POINT& startPoint, const POINT& endPoint) {
    if (m_startPoint == startPoint && m_endPoint == endPoint) {
        if (m_visible) {
            Show();
        }
        return;
    }

    m_startPoint = startPoint;
    m_endPoint = endPoint;

    // Determine the line orientation. The orientation is used
    // to optimize creating the line region.
    //
    if (endPoint.x == startPoint.x) {
        m_orientation = Vertical;
    } else if (endPoint.y == startPoint.y) {
        m_orientation = Horizontal;
    } else {
        m_orientation = Angled;
        m_wasAngled = true;
    }

    // An angled window must use the timer to dampen the drawing
    // to avoid visual artifacts. If the line is not angled, the
    // timer is effectively not used.
    //
    if (m_orientation == Angled) {
        ShowWindow(SW_HIDE);
        m_timer.Start(40);
    } else {
        if (m_wasAngled) {
            ShowWindow(SW_HIDE);
        }
        m_timer.Start(0);
    }
}

LRESULT MeaLine::OnHPTimer(WPARAM, LPARAM) {
    CRect rect(m_startPoint, m_endPoint);
    rect.NormalizeRect();

    // If the line is vertical or horizontal, the window has
    // a simple rectangular region. If the line is angled, the
    // window needs to be a polygonal region.
    //
    if (m_orientation == Angled) {
        rect.InflateRect(kMargin);
        PlotLine();

        MeaAssert(m_arr != nullptr);
        HRGN region = ::CreatePolyPolygonRgn(&m_arr[4 * m_shrink], m_varr,
            m_count - (2 * m_shrink), ALTERNATE);
        ::OffsetRgn(region, -rect.left, -rect.top);
        SetWindowRgn(region, TRUE);
    } else {
        if (m_orientation == Vertical) {
            rect.right++;
            rect.top += m_shrink;
            rect.bottom -= m_shrink;
        } else {
            rect.bottom++;
            rect.left += m_shrink;
            rect.right -= m_shrink;
        }

        if (m_wasAngled) {
            SetWindowRgn(nullptr, TRUE);
            m_wasAngled = false;
        }
    }

    SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
            SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSENDCHANGING);
    if (m_visible) {
        Show();
    }

    return 0;
}

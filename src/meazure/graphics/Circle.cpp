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
#include "Circle.h"
#include "Plotter.h"
#include "Colors.h"
#include <cassert>
#include <cstddef>
#include <functional>


 // Padding around circle
const CSize MeaCircle::kMargin(1, 1);

int* MeaCircle::m_varr { nullptr };


BEGIN_MESSAGE_MAP(MeaCircle, MeaGraphic)
    ON_MESSAGE(MeaHPTimerMsg, OnHPTimer)
END_MESSAGE_MAP()


MeaCircle::MeaCircle() :
    MeaGraphic(),
    m_center(kInitCoord, kInitCoord),
    m_perimeter(kInitCoord, kInitCoord),
    m_foreBrush(new CBrush(MeaColors::Get(MeaColors::LineFore))),
    m_arr(nullptr),
    m_count(0) {}

MeaCircle::~MeaCircle() {
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
        assert(false);
    }
}

bool MeaCircle::Create(const MeaScreenProvider& screenProvider, const CWnd* parent) {
    // Determine the size for the region array. A good estimate is two times
    // the circumference of the largest circle one can drawn on the screen. The
    // largest possible circle is defined by the virtual screen rectangle, which
    // is made up of each screen display.
    //
    m_clipRect = screenProvider.GetVirtualRect();
    double radius = MeaGeometry::CalcLength(static_cast<double>(m_clipRect.Width()),
                                            static_cast<double>(m_clipRect.Height()));
    std::size_t size = 2 * static_cast<std::size_t>(MeaGeometry::CalcCircumference(radius));

    // Single the circular region is composed of a series of rectangles, preload
    // the vertex count array with 4, the number of vertices composing a rectangle.
    //
    if (m_varr == nullptr) {
        m_varr = new int[size];
        for (std::size_t i = 0; i < size; i++) {
            m_varr[i] = 4;
        }
    }

    // Allocate space for each vertex.
    //
    if (m_arr == nullptr) {
        m_arr = new POINT[4 * size];
    }

    // Create the drawing timer.
    //
    m_timer.Create(this);

    // Create the window for the circle.
    //
    CString wndClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, nullptr, *m_foreBrush);
    return MeaGraphic::Create(wndClass, CSize(0, 0), parent);
}

void MeaCircle::Hide() {
    // No drawing when the window is hidden, so stop the timer.
    //
    m_timer.Stop();
    MeaGraphic::Hide();
}

void MeaCircle::SetColor(COLORREF color) {
    // Change the window's background brush. This will
    // draw the circle in the new color. Recall, the window
    // is formed in a circle, so it is the window's background
    // that defines the color of the circle.
    //
    CBrush* brush = new CBrush(color);
    if (m_hWnd != nullptr) {
        ::SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(static_cast<HBRUSH>(*brush)));
    }

    delete m_foreBrush;
    m_foreBrush = brush;

    if (m_hWnd != nullptr) {
        Invalidate();
        UpdateWindow();
    }
}

void MeaCircle::PlotCircle(int radius) {
    m_count = 0;

    std::function<void(int, int)> addPoint = std::bind(&MeaCircle::AddPoint, this, std::placeholders::_1,
                                                       std::placeholders::_2);
    MeaPlotter::PlotCircle(m_center, radius, addPoint);
}

void MeaCircle::SetPosition(const POINT& center, const POINT& perimeter) {
    // If the circle has not moved, just make sure it is visible.
    //
    if (m_center == center && m_perimeter == perimeter) {
        if (m_visible) {
            Show();
        }
        return;
    }

    m_center = center;
    m_perimeter = perimeter;

    if (m_center == m_perimeter) {
        return;
    }

    // Hide the circle until the timer expires. At that
    // time the timer callback will change the window's
    // region and redisplay the window.
    //
    ShowWindow(SW_HIDE);
    m_timer.Start(50);
}

LRESULT MeaCircle::OnHPTimer(WPARAM, LPARAM) {
    int radius = GetRadius();

    // Determine the overall size of the circle's window.
    //
    CPoint topLeft(m_center.x - radius, m_center.y - radius);
    CPoint bottomRight(m_center.x + radius, m_center.y + radius);
    CRect rect(topLeft, bottomRight);

    rect.InflateRect(kMargin);

    // Plot the points on the circle and create a rectangle
    // around each one.
    //
    PlotCircle(radius);

    // Create a polygonal window region made up of polygons.
    //
    assert(m_arr != nullptr);
    HRGN region = ::CreatePolyPolygonRgn(m_arr, m_varr, static_cast<int>(m_count), ALTERNATE);
    ::OffsetRgn(region, -rect.left, -rect.top);
    SetWindowRgn(region, TRUE);

    SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
                 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSENDCHANGING);
    if (m_visible) {
        Show();
    }

    return 0;
}

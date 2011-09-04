/*
 * Copyright 2001, 2004, 2011 C Thing Software
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

#include "StdAfx.h"
#include "Rectangle.h"
#include "Colors.h"


BEGIN_MESSAGE_MAP(MeaRectangle, MeaGraphic)
    ON_MESSAGE(MeaHPTimerMsg, OnHPTimer)
END_MESSAGE_MAP()


MeaRectangle::MeaRectangle() :
    MeaGraphic(),
    m_point1(kInitCoord, kInitCoord),
    m_point2(kInitCoord, kInitCoord),
    m_foreBrush(new CBrush(MeaColors::Get(MeaColors::LineFore))),
    m_thk(0), m_shrink(0)
{
}


MeaRectangle::~MeaRectangle()
{
    try {
        m_timer.Stop();

        delete m_foreBrush;
    }
    catch(...) {
        MeaAssert(false);
    }
}


bool MeaRectangle::Create(int thk, int shrink, const CWnd *parent)
{
    m_thk = thk;
    m_shrink = shrink;

    CString wndClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, NULL,
        *m_foreBrush);

    if (!MeaGraphic::Create(wndClass, CSize(0, 0), parent)) {
        return false;
    }

    m_timer.Create(this);

    SetRegion();

    return true;
}


void MeaRectangle::Hide()
{
    m_timer.Stop();
    MeaGraphic::Hide();
}


void MeaRectangle::SetPosition(const POINT& point1, const POINT& point2)
{
    if (m_point1 == point1 && m_point2 == point2) {
        return;
    }
    
    m_point1 = point1;
    m_point2 = point2;

    ShowWindow(SW_HIDE);
    m_timer.Start(5);
}


void MeaRectangle::SetColor(COLORREF color)
{
    CBrush *brush   = new CBrush(color);
    if (m_hWnd != NULL) {
        ::SetClassLong(m_hWnd, GCL_HBRBACKGROUND,
                        reinterpret_cast<LONG>(static_cast<HBRUSH>(*brush)));
    }
    
    delete m_foreBrush;
    m_foreBrush = brush;

    if (m_hWnd != NULL) {
        Invalidate();
        UpdateWindow();
    }
}


LRESULT MeaRectangle::OnHPTimer(WPARAM, LPARAM)
{
    if (!m_visible) {
        return 0;
    }

    CRect rect(m_point1, m_point2);
    rect.NormalizeRect();
    rect.InflateRect(0, 0, 1, 1);

    SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),
            SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSENDCHANGING);
    SetRegion();
    
    Show();

    return 0;
}


void MeaRectangle::SetRegion()
{
    CRect rect;

    GetClientRect(rect);
    HRGN outerRegion = ::CreateRectRgnIndirect(rect);
    rect.DeflateRect(m_thk, m_thk);
    HRGN innerRegion = ::CreateRectRgnIndirect(rect);

    ::CombineRgn(outerRegion, outerRegion, innerRegion, RGN_DIFF);

    ::DeleteObject(innerRegion);

    if (m_shrink > 0) {
        RECT cornerRect;

        // Punch a rectangular hole in the top left corner.
        //
        cornerRect.top      = m_point1.y - m_shrink;
        cornerRect.bottom   = m_point1.y + m_shrink;
        cornerRect.left     = m_point1.x - m_shrink;
        cornerRect.right    = m_point1.x + m_shrink;
        ScreenToClient(&cornerRect);
        HRGN cornerRegion = ::CreateRectRgnIndirect(&cornerRect);
        ::CombineRgn(outerRegion, outerRegion, cornerRegion, RGN_DIFF);
        ::DeleteObject(cornerRegion);

        // Punch a rectangular hole in the bottom right corner.
        //
        cornerRect.top      = m_point2.y - m_shrink;
        cornerRect.bottom   = m_point2.y + m_shrink;
        cornerRect.left     = m_point2.x - m_shrink;
        cornerRect.right    = m_point2.x + m_shrink;
        ScreenToClient(&cornerRect);
        cornerRegion = ::CreateRectRgnIndirect(&cornerRect);
        ::CombineRgn(outerRegion, outerRegion, cornerRegion, RGN_DIFF);
        ::DeleteObject(cornerRegion);
    }

    SetWindowRgn(outerRegion, TRUE);
}

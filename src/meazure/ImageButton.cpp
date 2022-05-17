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
#define COMPILE_THEME_STUBS
#include "Themes.h"
#include "ImageButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(MeaImageButton, CButton)
    ON_WM_DESTROY()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


MeaImageButton::MeaImageButton(UINT up, UINT down, UINT disabled) :
    CButton(), m_theme(nullptr), m_up(up), m_down(down), m_disabled(disabled),
    m_depressed(false), m_toggle(false) {}

MeaImageButton::~MeaImageButton() {
    m_theme = nullptr;
}

bool MeaImageButton::Create(DWORD dwStyle, CWnd* pParentWnd, UINT nID, UINT toolTipID) {
    CRect brect(0, 0, 5, 5);

    if (m_up != 0) {
        BITMAP bmpval;
        HBITMAP bitmap = static_cast<HBITMAP>(::LoadImage(AfxGetInstanceHandle(),
                                                          MAKEINTRESOURCE(m_up),
                                                          IMAGE_BITMAP,
                                                          0, 0,
                                                          LR_LOADMAP3DCOLORS));
        ::GetObject(bitmap, sizeof(BITMAP), &bmpval);

        brect.right = bmpval.bmWidth;
        brect.bottom = bmpval.bmHeight;

        ::DeleteObject(bitmap);

        CSize border(::GetSystemMetrics(SM_CXBORDER), ::GetSystemMetrics(SM_CYBORDER));
        brect.InflateRect(2 * border.cx, 2 * border.cy);
    }

    if (!CButton::Create(nullptr, dwStyle | WS_CHILD | BS_OWNERDRAW,
                         brect, pParentWnd, nID)) {
        return false;
    }

    // XP Theme support, if available
    //
    m_theme = IsThemeActive() ? OpenThemeData(*this, L"BUTTON") : nullptr;

    if (toolTipID != 0xffff) {
        m_toolTip.Create(this, TTS_ALWAYSTIP);
        m_toolTip.AddTool(this, toolTipID, nullptr, 0);
    }

    return true;
}

void MeaImageButton::OnDestroy() {
    CButton::OnDestroy();

    if (m_theme != nullptr) {
        CloseThemeData(m_theme);
        m_theme = nullptr;
    }
}

BOOL MeaImageButton::PreTranslateMessage(MSG* pMsg) {
    if (m_toolTip.m_hWnd != nullptr) {
        m_toolTip.RelayEvent(pMsg);
    }
    return CButton::PreTranslateMessage(pMsg);
}

void MeaImageButton::DrawItem(LPDRAWITEMSTRUCT dis) {
    CDC* dc = CDC::FromHandle(dis->hDC);    // Get a CDC we can use
    CRect r(dis->rcItem);                   // Copy the button rectangle
    HBITMAP bitmap;                         // Handle to the bitmap we are drawing
    BITMAP bmpval;                          // Parameters of the bitmap
    int saved = dc->SaveDC();               // Save the DC for later restoration
    bool grayout = false;                   // Gray out stock image?
    bool isDown = m_toggle ? m_depressed : ((dis->itemState & ODS_SELECTED) != 0);

    // Load the bitmap. The three states are
    //      ODS_SELECTED    ODS_DISABLED 
    //          1               n/a         Button is pressed
    //          0               0           Button is up
    //          0               1           Button is disabled
    //
    // The possible images supplied     The images used
    // ------specified------    ----------use----------
    //  up  down    disabled    up  down    disabled
    //  =====================   ========================
    //  A   B       C           A   B       C
    //  A   0       C           A   A       C
    //  A   B       0           A   B       gray(A)
    //  A   0       0           A   A       gray(A)
    //
    UINT id = 0;

    if (isDown) {       // selected
        //
        // If the down image is given, use that, else use the
        // up image.
        //
        id = m_down != 0 ? m_down : m_up;
    } else {                                // unselected
        if (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) { // grayed
            if (m_disabled == 0) {      // no disabled image
                id = m_up;
                grayout = true;     // gray out manually
            } else {                // use disabled image
                id = m_disabled;
            }
        } else {                                            // enabled
            id = m_up;
        }
    }

    bitmap = static_cast<HBITMAP>(::LoadImage(AfxGetInstanceHandle(),
                                              MAKEINTRESOURCE(id),
                                              IMAGE_BITMAP,
                                              0, 0,
                                              LR_LOADMAP3DCOLORS));

    //
    // Get the bitmap parameters, because we will need width and height
    //
    ::GetObject(bitmap, sizeof(BITMAP), &bmpval);

    //
    // Draw the traditional pushbutton edge using DrawEdge
    //
    if (m_theme != nullptr) {
        if (isDown) {
            DrawThemeEdge(m_theme, dis->hDC, BP_PUSHBUTTON, PBS_PRESSED, &dis->rcItem, EDGE_SUNKEN,
                          BF_RECT | BF_MIDDLE | BF_SOFT, nullptr);
        } else {
            DrawThemeEdge(m_theme, dis->hDC, BP_PUSHBUTTON, PBS_NORMAL, &dis->rcItem, EDGE_RAISED,
                          BF_RECT | BF_MIDDLE | BF_SOFT, nullptr);
        }
    } else {
        if (isDown) {
            dc->DrawEdge(&dis->rcItem, EDGE_SUNKEN, BF_RECT | BF_MIDDLE | BF_SOFT);
        } else {    // up
            dc->DrawEdge(&dis->rcItem, EDGE_RAISED, BF_RECT | BF_MIDDLE | BF_SOFT);
        }
    }

    //
    // Select the bitmap into a DC
    //
    CDC memDC;
    memDC.CreateCompatibleDC(dc);
    memDC.SelectObject(bitmap);

    //
    // If the button was disabled but we don't have a disabled image, gray out
    // the image by replacing every other pixel with a gray pixel
    // This is best done only if the bitmap is small
    //
    if (grayout) {      // gray out
        COLORREF gray = ::GetSysColor(COLOR_3DFACE);
        for (int x = 0; x < bmpval.bmWidth; x += 2) {
            for (int y = 0; y < bmpval.bmHeight; y++) {
                memDC.SetPixelV(x + (y & 1), y, gray);
            }
        }
    }

    dc->BitBlt((r.Width() - bmpval.bmWidth) / 2, (r.Height() - bmpval.bmHeight) / 2,
                bmpval.bmWidth, bmpval.bmHeight,
                &memDC, 0, 0, SRCCOPY);

    dc->RestoreDC(saved);
    ::DeleteObject(bitmap);
}

void MeaImageButton::OnLButtonDown(UINT nFlags, CPoint point) {
    m_depressed = !m_depressed;
    InvalidateRect(nullptr);
    CButton::OnLButtonDown(nFlags, point);
}

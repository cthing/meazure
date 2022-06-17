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
#include "Layout.h"
#include <stdarg.h>


void MeaLayout::AlignLeft(int leftX, ...) {
    va_list args;
    CWnd* wnd;
    RECT rect;

    va_start(args, leftX);
    while ((wnd = va_arg(args, CWnd*)) != nullptr) {
        wnd->GetWindowRect(&rect);
        wnd->GetParent()->ScreenToClient(&rect);
        SetWindowPos(*wnd, leftX, rect.top);
    }
    va_end(args);
}

void MeaLayout::AlignRight(int leftX, ...) {
    va_list args;
    CWnd* wnd;
    CRect rect;
    int maxWidth = 0;

    va_start(args, leftX);
    while ((wnd = va_arg(args, CWnd*)) != nullptr) {
        wnd->GetWindowRect(&rect);
        if (rect.Width() > maxWidth) {
            maxWidth = rect.Width();
        }
    }
    va_end(args);

    va_start(args, leftX);
    while ((wnd = va_arg(args, CWnd*)) != nullptr) {
        wnd->GetWindowRect(&rect);
        wnd->GetParent()->ScreenToClient(&rect);
        SetWindowPos(*wnd, leftX + maxWidth - rect.Width(), rect.top);
    }
    va_end(args);
}

void MeaLayout::AlignRightTo(const CWnd* baseWnd, ...) {
    va_list args;
    CWnd* wnd;
    CRect rect;

    baseWnd->GetWindowRect(&rect);
    baseWnd->GetParent()->ScreenToClient(&rect);
    int rightX = rect.right;

    va_start(args, baseWnd);
    while ((wnd = va_arg(args, CWnd*)) != nullptr) {
        wnd->GetWindowRect(&rect);
        wnd->GetParent()->ScreenToClient(&rect);
        SetWindowPos(*wnd, rightX - rect.Width(), rect.top);
    }
    va_end(args);
}

void MeaLayout::AlignCenter(int topY, ...) {
    va_list args;
    CWnd* wnd;
    CRect rect;
    int maxHeight = 0;

    va_start(args, topY);
    while ((wnd = va_arg(args, CWnd*)) != nullptr) {
        wnd->GetWindowRect(&rect);
        if (rect.Height() > maxHeight) {
            maxHeight = rect.Height();
        }
    }
    va_end(args);

    va_start(args, topY);
    while ((wnd = va_arg(args, CWnd*)) != nullptr) {
        wnd->GetWindowRect(&rect);
        wnd->GetParent()->ScreenToClient(&rect);
        SetWindowPos(*wnd, rect.left, topY + (maxHeight - rect.Height()) / 2);
    }
    va_end(args);
}

void MeaLayout::PlaceAfter(const CWnd& frontWnd, const CWnd& backWnd, int spacing) {
    RECT rect;

    frontWnd.GetWindowRect(&rect);
    frontWnd.GetParent()->ScreenToClient(&rect);
    SetWindowPos(backWnd, rect.right + spacing, rect.top);
}

void MeaLayout::GetBoundingSize(SIZE* sizep, const CWnd* parentWnd) {
    RECT rect;
    CRect boundingRect(0, 0, 0, 0);

    for (CWnd* child = parentWnd->GetWindow(GW_CHILD); child != nullptr; child = child->GetNextWindow()) {
        child->GetWindowRect(&rect);
        boundingRect.UnionRect(boundingRect, &rect);
    }

    sizep->cx = boundingRect.Width();
    sizep->cy = boundingRect.Height();
}

void MeaLayout::AlphaBlend(CDC& dstDC, const CDC& srcDC, int width, int height, BYTE alpha) {
    int x, y;
    double a = alpha / 255.0;
    double a1 = (1 - a);

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            COLORREF dstColor = dstDC.GetPixel(x, y);
            COLORREF srcColor = srcDC.GetPixel(x, y);

            BYTE r = static_cast<BYTE>(a * GetRValue(dstColor) + a1 * GetRValue(srcColor));
            BYTE g = static_cast<BYTE>(a * GetGValue(dstColor) + a1 * GetGValue(srcColor));
            BYTE b = static_cast<BYTE>(a * GetBValue(dstColor) + a1 * GetBValue(srcColor));

            dstDC.SetPixel(x, y, RGB(r, g, b));
        }
    }
}

void MeaLayout::DrawOpacityBackground(const CWnd& wnd, CDC& dc, const MeaScreenProvider& screenProvider,
                                      const MeaUnitsProvider& unitsProvider) {
    CRect clientRect;
    CRect winRect;

    COLORREF foreColor = RGB(0, 0, 0);
    COLORREF backColor = RGB(0xFF, 0xFF, 0xFF);

    wnd.GetClientRect(clientRect);
    wnd.GetWindowRect(winRect);

    dc.FillSolidRect(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), backColor);

    MeaFSize res = screenProvider.GetScreenRes(screenProvider.GetScreenIter(winRect));

    SIZE forePixels = unitsProvider.ConvertToPixels(MeaInchesId, res, 0.02, 3);
    CSize sepPixels(3 * forePixels.cx, 3 * forePixels.cy);

    int x, y;

    for (x = clientRect.left; x < clientRect.right; x += sepPixels.cx) {
        for (y = clientRect.top; y < clientRect.bottom; y += sepPixels.cy) {
            dc.FillSolidRect(x, y, forePixels.cx, forePixels.cy, foreColor);
        }
    }
}

int MeaLayout::GetEffectiveDPI(const CWnd& wnd) {
    int effectiveDpi = GetDpiForWindow(wnd);
    return (effectiveDpi > 0) ? effectiveDpi : USER_DEFAULT_SCREEN_DPI;
}

int MeaLayout::GetDPIScalePercent(const CWnd& wnd) {
    int effectiveDpi = GetDpiForWindow(wnd);
    return (effectiveDpi > 0) ? MulDiv(100, effectiveDpi, USER_DEFAULT_SCREEN_DPI) : 100;
}

double MeaLayout::GetDPIScaleFactor(const CWnd& wnd) {
    double effectiveDpi = GetDpiForWindow(wnd);
    return (effectiveDpi > 0) ? effectiveDpi / static_cast<double>(USER_DEFAULT_SCREEN_DPI) : 1.0;
}

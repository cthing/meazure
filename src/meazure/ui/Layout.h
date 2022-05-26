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

/// @file
/// @brief Header file for window layout utilities.

#pragma once

#include <math.h>
#include <meazure/units/Units.h>
#include "ScreenProvider.h"
#include <meazure/units/UnitsProvider.h>


/// Utility methods for performing various window layout tasks.
///
class MeaLayout {

public:
    MeaLayout() = delete;
    ~MeaLayout() = delete;

    /// Aligns the specified windows such that their left edges all begin
    /// at the specified x location.
    ///
    /// @param leftX        [in] Align each window's left edge to this x position, in pixels.
    /// @param ...          [in] CWnd pointers terminated by nullptr.
    ///
    static void AlignLeft(int leftX, ...);

    /// Aligns the specified windows such that their right edges are all
    /// aligned. The widest window is positioned at the specified x location.
    ///
    /// @param leftX        [in] Widest window starts at this x position, in pixels.
    /// @param ...          [in] CWnd pointers terminated by nullptr.
    ///
    static void AlignRight(int leftX, ...);

    /// Aligns the specified windows such that they all share a common vertical
    /// center. The top of the tallest window begins at the specified y position.
    ///
    /// @param topY         [in] Tallest window begins at this y position, in pixels.
    /// @param ...          [in] CWnd pointers terminated by nullptr.
    ///
    static void AlignCenter(int topY, ...);

    /// Performs a right alignment of the specified windows relative to the right
    /// edge of the specified base window.
    ///
    /// @param baseWnd      [in] Align windows relative to this base window.
    /// @param ...          [in] CWnd pointers terminated by nullptr.
    ///
    static void AlignRightTo(const CWnd* baseWnd, ...);

    /// Places the back window after the specified front window and separates
    /// them by the specified spacing.
    ///
    /// @param frontWnd     [in] Window in front of backWnd.
    /// @param backWnd      [in] Window to place after frontWnd.
    /// @param spacing      [in] Separation between the two windows, in pixels.
    ///
    static void PlaceAfter(const CWnd& frontWnd, const CWnd& backWnd, int spacing);


    /// Sets the position of the specified window.
    ///
    /// @param wnd      [in] Window to position.
    /// @param x        [in] X position for the window, in pixels.
    /// @param y        [in] Y position for the window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool SetWindowPos(const CWnd& wnd, int x, int y) {
        return ::SetWindowPos(wnd.m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? true : false;
    }

    /// Sets the size of the specified window.
    ///
    /// @param wnd      [in] Window to size.
    /// @param cx       [in] Width for the window, in pixels.
    /// @param cy       [in] Height for the window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool SetWindowSize(const CWnd& wnd, int cx, int cy) {
        return ::SetWindowPos(wnd.m_hWnd, nullptr, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER) ? true : false;
    }

    /// Returns the height of the specified window.
    ///
    /// @param wnd      [in] Window whose height is desired.
    ///
    /// @return Height of the window in pixels.
    ///
    static int GetWindowHeight(const CWnd& wnd) {
        CRect rect;
        wnd.GetWindowRect(rect);
        return rect.Height();
    }

    /// Sets the height of the window.
    ///
    /// @param wnd      [in] Window whose height is to be set.
    /// @param cy       [in] Height for the window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool SetWindowHeight(const CWnd& wnd, int cy) {
        CRect rect;
        wnd.GetWindowRect(rect);
        return ::SetWindowPos(wnd.m_hWnd, nullptr, 0, 0, rect.Width(), cy, SWP_NOMOVE | SWP_NOZORDER) ? true : false;
    }

    /// Moves the specified window vertically.
    ///
    /// @param wnd      [in] Window to be moved.
    /// @param cy       [in] Vertical distance to move window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool MoveWindowHeight(const CWnd& wnd, int cy) {
        CRect rect;
        wnd.GetWindowRect(rect);
        CWnd* parentWnd = wnd.GetParent();
        parentWnd->ScreenToClient(rect);
        return ::SetWindowPos(wnd.m_hWnd, nullptr, rect.left, rect.top + cy, 0, 0,
                              SWP_NOSIZE | SWP_NOZORDER) ? true : false;
    }

    /// Calculates the bounding dimensions for all the children of
    /// the specified window.
    ///
    /// @param sizep        [out] Bounding dimensions encompassing all child windows.
    /// @param parentWnd    [in] All child windows of this window form the bounding dimensions.
    ///
    static void GetBoundingSize(SIZE* sizep, const CWnd* parentWnd);

    /// Converts the x position in screen coordinates to
    /// an x position in the specified window's coordinate
    /// system.
    ///
    /// @param wnd      [in] The x coordinate is converts to this
    ///                 window's coordinate system.
    /// @param x        [in, out] The x coordinate to convert.
    ///
    /// @return <b>true</b> if the conversion was successful.
    ///
    static bool ScreenToClientX(const CWnd& wnd, int& x) {
        CPoint pt(x, 0);
        bool ret = ScreenToClient(wnd.m_hWnd, &pt) == TRUE;
        x = pt.x;
        return ret;
    }

    /// Converts the y position in screen coordinates to
    /// a y position in the specified window's coordinate
    /// system.
    ///
    /// @param wnd      [in] The y coordinate is converts to this
    ///                 window's coordinate system.
    /// @param y        [in, out] The y coordinate to convert.
    ///
    /// @return <b>true</b> if the conversion was successful.
    ///
    static bool ScreenToClientY(const CWnd& wnd, int& y) {
        CPoint pt(0, y);
        bool ret = ScreenToClient(wnd.m_hWnd, &pt) == TRUE;
        y = pt.y;
        return ret;
    }

    /// Performs and alpha blend on the two specified device contexts.
    /// The resulting blend is stored in the destination device context.
    /// The device contexts must be the same size.
    ///
    /// @param dstDC    [in] Destination device context.
    /// @param srcDC    [in] Source device context.
    /// @param width    [in] Width of the device contexts, in pixels.
    /// @param height   [in] Height of the device contexts, in pixels.
    /// @param alpha    [in] Opacity value between 0 (transparent) and 255 (opaque).
    ///
    static void AlphaBlend(CDC& dstDC, const CDC& srcDC, int width, int height, BYTE alpha);

    /// Draws an image to use as a background for use in setting
    /// the a control's opacity.
    ///
    /// @param wnd      [in] Window representing the control.
    /// @param dc       [in] Device context into which to draw the background.
    /// @param screenProvider   [in] Screen information provider
    /// @param unitsProvider    [in] Units information provider
    ///
    static void DrawOpacityBackground(const CWnd& wnd, CDC& dc, const MeaScreenProvider& screenProvider,
                                      const MeaUnitsProvider& unitsProvider);
};

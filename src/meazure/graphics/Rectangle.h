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
/// @brief Rectangle graphic header file.

#pragma once

#include "Graphic.h"
#include <meazure/utilities/Timer.h>


/// A rectangle element. The rectangle is used by the Rectangle measurement
/// tool. The rectangle is positioned by specifying the location of its
/// its top left corner (point 1) and its bottom right corner (point 2).
/// The rectangle is formed by creating a rectangular window and punching
/// out a smaller rectangle from the window's interior. The remaining
/// region forms the line of the rectangle. 
///
class MeaRectangle : public MeaGraphic {

public:
    /// Constructs a rectangle. Prior to displaying the rectangle with
    /// the Show() method, the Create() method must be called to create
    /// the rectangle's window.
    ///
    MeaRectangle();

    /// Destroys the rectangle.
    ///
    virtual ~MeaRectangle();

    /// Creates the window for the rectangle. This method must be called
    /// before the rectangle can be displayed and before any operation that
    /// attempts to manipulate the rectangle's window.
    ///
    /// @param thk          [in] Thickness of the rectangle's line, in pixels.
    /// @param shrink       [in] Distance, in pixels, to shrink line of the
    ///                     rectangle from the top left and bottom right
    ///                     corners. The typical use for this parameter is in
    ///                     the Rectangle tool. Shrinking the line allows the
    ///                     contents under the corner points to be clear for
    ///                     viewing and measuring.
    /// @param parent       [in] Parent window or <b>nullptr</b> if the rectangle
    ///                     does not have an immediate parent. If a parent
    ///                     window is specified, the rectangle is created as
    ///                     a child window (<b>WS_CHILD</b>). Otherwise the
    ///                     rectangle is created as a popup window
    ///                     (<b>WS_POPUP</b>) and is assigned the main window
    ///                     as a parent.
    ///
    /// @return <b>true</b> if the window is created successfully.
    ///
    bool Create(int thk, int shrink, const CWnd* parent = nullptr);

    /// Hides the graphic. Call the Create() method prior
    /// to calling this method for the first time.
    ///
    virtual void Hide() override;

    /// Sets the position of the rectangle.
    ///
    /// @param point1   [in] Location of the top left corner of the rectangle, in pixels
    /// @param point2   [in] Location of the bottom right corner of the rectangle, in pixels
    ///
    void SetPosition(const POINT& point1, const POINT& point2);

    /// Sets the color of the rectangle's line.
    ///
    /// @param color    [in] Color value defined by the Windows GDI RGB macro.
    ///
    void SetColor(COLORREF color);

protected:
    DECLARE_MESSAGE_MAP()

    /// Windows GDI was not optimized for doing crazy things like creating
    /// a window that is a thin rectangle. Because of this, drawing the window
    /// is very expensive. To minimize visual artifacts when the rectangle is
    /// repositioned or resized, a timer is used to effectively add damping
    /// to the drawing process. Even if the user may be making quick changes
    /// to the rectangle, the time delay reduces the number of redraws. When the
    /// SetPosition() method is called, it just sets a timer and returns. When
    /// the timer expires, this method is called and actually redraws the
    /// window.
    ///
    /// @param wParam   [in] not used
    /// @param lParam   [in] not used
    ///
    /// @return The return is not used so 0 is returned.
    ///
    afx_msg LRESULT OnHPTimer(WPARAM wParam, LPARAM lParam);

private:
    /// Copy constructor is purposely undefined.
    ///
    MeaRectangle(const MeaRectangle&);

    /// Assignment operator is purposely undefined
    ///
    MeaRectangle& operator=(const MeaRectangle&);

    /// Sets the windows region effectively forming the rectangle. The
    /// rectangle is formed by first creating a normal rectangular window.
    /// A smaller rectangle is formed based on the desired thickness for
    /// the rectangle's line. This smaller rectangle is used to punch out
    /// the window's region, thereby forming the rectangle. The shrinking
    /// of the rectangle lines around the top left and bottom right corners,
    /// if any, is done by using small rectangles the size of the shrink
    /// to punch out the corners of the window.
    ///
    void SetRegion();


    CPoint m_point1;        ///< Location of the top left corner of the rectangle, in pixels.
    CPoint m_point2;        ///< Location of the bottom right corner of the rectangle, in pixels.
    CBrush* m_foreBrush;    ///< Brush to paint the rectangle's line.
    MeaTimer m_timer;       ///< Timer for delayed drawing of the rectangle to reduce visual artifacts.
    int m_thk;              ///< Thickness of the rectangle's line, in pixels.
    int m_shrink;           ///< Number pixels to hang back from the top left and bottom right corners of the rectangle.
};

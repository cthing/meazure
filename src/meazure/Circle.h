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
/// @brief Circle graphic header file.

#pragma once

#include "Graphic.h"
#include "Layout.h"
#include "Timer.h"


/// A circle element. The circle is positioned by specifying the center
/// and it is sized by specifying a point on the perimeter. The circle
/// is formed by using a series of circularly arranged polygonal regions
/// to create a thin circular window.
///
class MeaCircle : public MeaGraphic {

public:
    /// Constructs a circle. Prior to displaying the circle with
    /// the Show() method, the Create() method must be called to
    /// create the circle's window.
    ///
    MeaCircle();

    /// Destroys the circle.
    ///
    virtual ~MeaCircle();

    /// Creates the window for the circle. This method must be called
    /// before the circle can be displayed and before any operation that
    /// attempts to manipulate the circle's window.
    ///
    /// @param parent       [in] Parent window or <b>nullptr</b> if the circle does
    ///                     not have an immediate parent. If a parent window is
    ///                     specified, the circle is created as a child window
    ///                     (<b>WS_CHILD</b>). Otherwise the circle is created
    ///                     as a popup window (<b>WS_POPUP</b>) and is assigned
    ///                     the main window as a parent.
    ///
    /// @return <b>true</b> if the window is created successfully.
    ///
    bool Create(const CWnd* parent = nullptr);

    /// Hides the graphic. Call the Create() method prior
    /// to calling this method for the first time.
    ///
    virtual void Hide() override;

    /// Sets the position and size of the circle.
    ///
    /// @param center       [in] Location of the center of the circle, in pixels
    /// @param perimeter    [in] Location of a point on the perimeter of the circle, in pixels
    ///
    void SetPosition(const POINT& center, const POINT& perimeter);

    /// Obtains the position and size of the circle.
    ///
    /// @param center       [out] Location of the center of the circle, in pixels
    /// @param perimeter    [out] Location of a point on the perimeter of the circle, in pixels
    ///
    void GetPosition(POINT& center, POINT& perimeter) {
        center = m_center;
        perimeter = m_perimeter;
    }

    /// Calculates the radius of the circle based on the center and perimeter
    /// points. The following classic equation is used:
    /// \f[
    ///     radius=\sqrt{(center_x-perimeter_x)^2+(center_y-perimeter_y)^2}
    /// \f]
    /// @return The length of the circle's radius, in pixels.
    ///
    int GetRadius() const {
        return static_cast<int>(MeaLayout::CalcLength(m_center, m_perimeter));
    }

    /// Sets the color of the circle's line.
    ///
    /// @param color    [in] Color value defined by the Windows GDI RGB macro.
    ///
    void SetColor(COLORREF color);

protected:
    DECLARE_MESSAGE_MAP()

    /// Windows GDI was not optimized for doing crazy things like creating
    /// a window that is a thin lined circle. Because of this, drawing the
    /// window is very expensive. To minimize visual artifacts when the circle
    /// is repositioned or resized a timer is used to effectively add damping
    /// to the drawing process. Even if the user may be making quick changes
    /// to the circle, the time delay reduces the number of redraws. When the
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
    static const CSize  kMargin;        ///< Padding around the circle, in pixels


    /// Copy constructor is purposely undefined.
    ///
    MeaCircle(const MeaCircle&);

    /// Assignment operator is purposely undefined
    ///
    MeaCircle& operator=(const MeaCircle&);


    /// The circular window region is composed of single pixel rectangles
    /// arranged in a circle. The location of each rectangle is determined
    /// using an algorithm from the paper "A Fast Bresenham Type Algorithm
    /// for Drawing Circles" by John Kennedy, Mathematics Dept., Santa
    /// Monica College (http://homepage.smc.edu/kennedy_john/BCIRCLE.PDF,
    /// rkennedy@ix.netcom.com).
    ///
    /// @param radius   [in] radius of the circle, in pixels
    ///
    void PlotCircle(int radius);

    /// The circular window is composed of a series of single pixel
    /// rectangles arranged in a circle. At each point on the circle
    /// the coordinates of a rectangle surrounding that point are added
    /// to an array of points. This array of points will eventually be
    /// handed to the GDI function CreatePolyPolygonRgn.
    ///
    /// @param x    [in] X coordinate of the point on the circle
    /// @param y    [in] Y coordinate of the point on the circle
    ///
    void AddPoint(int x, int y) {
        POINT p;

        // Put the coordinates into a point
        //
        p.x = x;
        p.y = y;

        // Make sure the point is somewhere on the virtual rectangle
        // formed by all display monitors.
        //
        if (m_clipRect.PtInRect(p)) {
            // Determine where in the coordinate array
            // we will start adding the rectangle coordinates.
            //
            POINT* arr = &m_arr[m_count << 2];

            // (x,y)   (x+1, y)
            //    ---------
            //   |         |
            //   |         |
            //   |         |
            //    ---------
            // (x,y+1) (x+1,y+1)


            // (x,y)
            //
            * arr++ = p;

            // (x+1,y)
            //
            p.x++;
            *arr++ = p;

            // (x+1,y+1)
            //
            p.y++;
            *arr++ = p;

            // (x, y+1)
            //
            p.x--;
            *arr = p;

            // Keep track of the number of rectangles
            //
            m_count++;
        }
    }


    CPoint m_center;        ///< Location of the center of the circle, in pixels
    CPoint m_perimeter;     ///< Location of a point on the perimeter of the circle, in pixels
    CRect m_clipRect;       ///< Virtual rectangle formed by all display monitors, in pixels
    CBrush* m_foreBrush;    ///< Brush for drawing the circle
    MeaTimer m_timer;       ///< Timer for delayed drawing of circle to reduce visual artifacts
    POINT* m_arr;           ///< Array of coordinates for each rectangle making up the circular region
    unsigned int m_count;   ///< Number of rectangles in the circular region
    static int* m_varr;     ///< Number of coordinates comprising each polygon making up the circular
                            ///< region (always 4 in our case)
};

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

/// @file
/// @brief Line graphic header file.

#pragma once

#include "Graphic.h"
#include "Timer.h"


/// A line element. The line is used by many tools including the Line
/// and Grid tools. The line is positioned by specifying the location
/// of its two end points. The line is formed by arranging adjacent
/// single pixel rectangular regions. 
///
class MeaLine : public MeaGraphic
{
public:
    /// Constructs a line. Prior to displaying the line with the
    /// Show() method, the Create() method must be called to create
    /// the line's window.
    ///
    MeaLine();

    /// Destroys the line.
    ///
    virtual ~MeaLine();


    /// Creates the window for the line. This method must be called
    /// before the line can be displayed and before any operation that
    /// attempts to manipulate the line's window.
    ///
    /// @param shrink       [in] Distance, in pixels, to shrink line from
    ///                     start and end points. The typical use for this
    ///                     parameter is in the Line tool. Shrinking the line
    ///                     allows the contents of the end points to be clear
    ///                     for viewing and measuring.
    /// @param parent       [in] Parent window or <b>NULL</b> if the line does
    ///                     not have an immediate parent. If a parent window is
    ///                     specified, the line is created as a child window
    ///                     (<b>WS_CHILD</b>). Otherwise the line is created
    ///                     as a popup window (<b>WS_POPUP</b>) and is assigned
    ///                     the main window as a parent.
    ///
    /// @return <b>true</b> if the window is created successfully.
    ///
    bool    Create(int shrink, const CWnd *parent = NULL);


    /// Hides the graphic. Call the Create() method prior
    /// to calling this method for the first time.
    ///
    virtual void Hide();


    /// Sets the position of the line.
    ///
    /// @param startPoint   [in] Location of the start of the line, in pixels
    /// @param endPoint     [in] Location of the end of the line, in pixels
    ///
    void    SetPosition(const POINT& startPoint, const POINT& endPoint);

    /// Obtains the position of the line.
    ///
    /// @param startPoint   [out] Location of the start of the line, in pixels
    /// @param endPoint     [out] Location of the end of the line, in pixels
    ///
    void    GetPosition(POINT& startPoint, POINT& endPoint) {
        startPoint  = m_startPoint;
        endPoint    = m_endPoint;
    }

    /// Sets the color of the line.
    ///
    /// @param color    [in] Color value defined by the Windows GDI RGB macro.
    ///
    void    SetColor(COLORREF color);

protected:
    DECLARE_MESSAGE_MAP()

    /// Windows GDI was not optimized for doing crazy things like creating
    /// a window that is a thin line. Because of this, drawing the window
    /// is very expensive. To minimize visual artifacts when the line is
    /// repositioned or resized, a timer is used to effectively add damping
    /// to the drawing process. Even if the user may be making quick changes
    /// to the line, the time delay reduces the number of redraws. When the
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
    /// If the line is vertical or horizontal, the creation of
    /// the window region can be heavily optimized. If the line
    /// is at an arbitrary angle, the optimization cannot be
    /// used.
    ///
    enum Orientation {
        Vertical,       ///< The line is vertical
        Horizontal,     ///< The line is horizontal
        Angled          ///< The line is oriented at an arbitrary angle
    };


    static const CSize  kMargin;    ///< Padding around line bounding rectangle, in pixels


    /// Copy constructor is purposely undefined.
    ///
    MeaLine(const MeaLine&);

    /// Assignment operator is purposely undefined
    ///
    MeaLine& operator=(const MeaLine&);

    /// The window region is composed of single pixel rectangles arranged
    /// in a line from the start point to the end point. The location of
    /// each rectangle is determined using the Bresenham algorithm adapted
    /// from "Graphics Gems", Academic Press, 1990, p. 685. The line needs
    /// to be created in this brute force way because relying on the polygon
    /// region method produces a horrible looking line.
    ///
    void    PlotLine();

    /// The window is composed of a series of single pixel rectangles
    /// arranged in a line. At each point on the line the coordinates of
    /// a rectangle surrounding that point are added to an array of points.
    /// This array of points will eventually be handed to the GDI function
    /// CreatePolyPolygonRgn.
    ///
    /// @param x    [in] X coordinate of the point on the line
    /// @param y    [in] Y coordinate of the point on the line
    ///
    void    AddPoint(int x, int y) {
        POINT p;
        register POINT *arr = &m_arr[m_count << 2];

        // Put the coordinates into a point
        //
        p.x = x;
        p.y = y;

        // (x,y)   (x+1, y)
        //    ---------
        //   |         |
        //   |         |
        //   |         |
        //    ---------
        // (x,y+1) (x+1,y+1)


        // (x,y)
        //
        *arr++ = p;

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

    CPoint          m_startPoint;       ///< Location of the start point of the line, in pixels
    CPoint          m_endPoint;         ///< Location of the end point of the line, in pixels
    bool            m_wasAngled;        ///< Indicates if the line was angled the last time it was drawn
    CBrush          *m_foreBrush;       ///< Brush used to draw the line
    MeaTimer        m_timer;            ///< Timer for delayed drawing of line to reduce visual artifacts
    Orientation     m_orientation;      ///< Current orientation of the line

    POINT           *m_arr;             ///< Array of coordinates for each rectangle making up the window region
    unsigned int    m_count;            ///< Number of rectangles in the window region
    int             m_shrink;           ///< Number of pixels to shrink the length of the line
    static int      *m_varr;            ///< Number of coordinates comprising each polygon making up the window region (always 4 in our case)
};

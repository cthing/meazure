/*
 * Copyright 2022 C Thing Software
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
 /// @brief Interface for classes providing information about display screens. 

#pragma once

#include <map>
#include <meazure/utilities/Geometry.h>


/// Provides information on the display monitor(s).
///
struct MeaScreenProvider {

    class Screen;

    typedef std::map<HMONITOR, Screen*> Screens;    ///< Maps a monitor handle to a screen object.
    typedef Screens::const_iterator ScreenIter;     ///< Constant iterator over the screens.

    /// Used by the STL collections for ordering comparisons.
    ///
    struct less {
        /// Compares two screen iterators.
        /// @param lhs  [in] Left hand side of the comparison.
        /// @param rhs  [in] Right hand side of the comparison.
        /// @return <b>true</b> if lhs < rhs.
        bool operator()(const ScreenIter& lhs, const ScreenIter& rhs) const {
            return (*lhs).first < (*rhs).first;
        }
    };


    /// Returns the number of display screens attached to the system.
    ///
    /// @return Number of display screens attached to the system.
    ///
    virtual int GetNumScreens() const = 0;

    /// Obtains an iterator over each screen attached to the system.
    /// @return Constant iterator over all screens.
    virtual ScreenIter GetScreenIter() const = 0;

    /// Obtains an iterator pointed at the screen that contains the
    /// specified window.
    ///
    /// @param wnd      [in] Window whose screen is desired.
    ///
    /// @return Constant iterator pointed at screen containing the
    ///         specified window.
    ///
    virtual ScreenIter GetScreenIter(const CWnd* wnd) const = 0;

    /// Obtains an iterator pointed at the screen that contains the
    /// specified point.
    ///
    /// @param point    [in] Point whose screen is desired.
    ///
    /// @return Constant iterator pointed at screen containing the
    ///         specified point.
    ///
    virtual ScreenIter GetScreenIter(const POINT& point) const = 0;

    /// Obtains an iterator pointed at the screen that contains the
    /// specified rectangle.
    ///
    /// @param rect     [in] Rectangle whose screen is desired.
    ///
    /// @return Constant iterator pointed at screen containing the
    ///         specified rectangle.
    ///
    virtual ScreenIter GetScreenIter(const RECT& rect) const = 0;

    /// Indicates if the screen iterator is at the end of the
    /// screen collection.
    ///
    /// @param iter     [in] Screen iterator to test.
    ///
    /// @return <b>true</b> if the specified iterator is pointed to
    ///         the end of the collection of screens.
    ///
    virtual bool AtEnd(const ScreenIter& iter) const = 0;

    /// Determines the center of the screen that contains the
    /// app's main window.
    ///
    /// @return Center point of the screen containing the app's
    ///         main window.
    ///
    virtual const CPoint& GetCenter() const = 0;

    /// Returns a point guaranteed to be located off any screen.
    ///
    /// @return Point located off any display screen.
    ///
    virtual CPoint GetOffScreen() const = 0;

    /// Returns the virtual screen rectangle. The virtual rectangle
    /// is the rectangle containing all display screens attached to
    /// the system.
    ///
    /// @return Rectangle encompassing all display screens attached
    ///         to the system, in pixels.
    ///
    virtual const CRect& GetVirtualRect() const = 0;

    /// Returns the screen rectangle for the screen pointed to by
    /// the specified iterator.
    ///
    /// @param iter     [in] Screen iterator pointed at the screen
    ///                 whose rectangle is desired.
    ///
    /// @return Screen rectangle, in pixels.
    ///
    virtual const CRect& GetScreenRect(const ScreenIter& iter) const = 0;

    /// Returns the manually set screen resolution for the screen
    /// pointed to by the specified iterator.
    ///
    /// @param iter         [in] Screen iterator pointed at the screen
    ///                     whose manual resolution is desired.
    /// @param useManualRes [out] <b>true</b> if the resolution was set
    ///                     manually.
    /// @param manualRes    [out] Manually set resolution, in pixels per inch.
    ///
    virtual void GetScreenRes(const ScreenIter& iter, bool& useManualRes, FSIZE& manualRes) const = 0;

    /// Returns the resolution for the screen pointed to by the specified iterator.
    ///
    /// @param iter     [in] Screen iterator pointed at the screen whose
    ///                 resolution is desired.
    ///
    /// @return Screen resolution, in pixels per inch. If the resolution has been
    ///         set manually, it is returned. Otherwise, the operating system
    ///         reported resolution is returned.
    ///
    virtual const FSIZE& GetScreenRes(const ScreenIter& iter) const = 0;

    /// Indicates whether any screen is using the operating system reported
    /// resolution. In other words, are there any screen that require calibration.
    ///
    /// @return <b>true</b> if there are any screens requiring manual calibration.
    ///
    virtual bool AnyOSRes() const = 0;

    /// Returns the name for the specified screen.
    ///
    /// @param iter     [in] Iterator pointing to the screen whose name is desired.
    ///
    /// @return Name of the specified screen.
    ///
    virtual CString GetScreenName(const ScreenIter& iter) const = 0;

    /// Ensures that the specified point is on a screen.
    ///
    /// @param pt       [in] Point to check.
    ///
    /// @return New point based on the specified point that is guaranteed
    ///         to be on a screen.
    ///
    virtual CPoint LimitPosition(const CPoint& pt) const = 0;
};

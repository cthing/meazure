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
/// @brief Graphic element base class header file.

#pragma once

 
/// Base class for all graphic elements. Classes derived from this base
/// class are used by the measurement tools to perform their function.
///
class MeaGraphic: public CWnd
{
public:
    /// Base class constructor. Prior to displaying the graphic with
    /// the Show() method, the Create() method must be called to create
    /// the graphic's window.
    ///
    MeaGraphic();

    /// Base class destructor
    ///
    virtual ~MeaGraphic();


    /// Creates the window for the graphic. This method must be called
    /// before the graphic can be displayed and before any operation that
    /// attempts to manipulate the graphic's window.
    ///
    /// @param classname    [in] Class name for the window. Typically, a derived
    ///                     class obtains a window class name from a call to the
    ///                     AfxRegisterWndClass function.
    /// @param size         [in] Initial size for the window.
    /// @param parent       [in] Parent window or <b>NULL</b> if the graphic does
    ///                     not have an immediate parent. If a parent window is
    ///                     specified, the graphic is created as a child window
    ///                     (<b>WS_CHILD</b>). Otherwise the graphic is created
    ///                     as a popup window (<b>WS_POPUP</b>) and is assigned
    ///                     the main window as a parent.
    /// @param id           Unique ID for this graphic element. Typically, the ID
    ///                     is used to identify the graphic element in a callback.
    ///
    /// @return <b>true</b> if the window is created successfully.
    ///
    bool    Create(LPCTSTR classname, const SIZE& size, const CWnd *parent, UINT id = 0xFFFF);


    /// Displays the graphic. Call the Create() method prior
    /// to calling this method for the first time.
    ///
    virtual void    Show();

    /// Hides the graphic. Call the Create() method prior
    /// to calling this method for the first time.
    ///
    virtual void    Hide();


    /// Changes the z-order of the graphic window so that it
    /// if above the specified window. Call the Create() method
    /// prior to calling this method for the first time.
    ///
    /// @param  winBelow    [in] Window to place below this class's window
    ///
    void    SetAbove(const CWnd *winBelow) { SetWindowPos(winBelow, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); }

protected:
    static const int kInitCoord;    ///< An arbitrary initial coordinate for placing new graphics

    UINT        m_id;               ///< ID for the graphic
    bool        m_visible;          ///< Indicates whether the graphic is shown or hidden
    const CWnd  *m_parent;          ///< Graphic's parent window or NULL if graphic is a popup window
};

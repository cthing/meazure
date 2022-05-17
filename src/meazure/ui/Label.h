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
/// @brief Header file for the label control.

#pragma once


/// Creates a label control whose size is automatically set to the size
/// of its text.
///
class MeaLabel : public CStatic {

public:
    /// Constructs a label object. Before the label can be used, the
    /// Create method must be called.
    ///
    MeaLabel();

    /// Destroys an instance of a label object.
    ///
    virtual ~MeaLabel();

    /// Creates the window for the label. To make the label visible,
    /// add the WS_VISIBLE style or call ShowWindow.
    ///
    /// @param label        [in] Label text.
    /// @param style        [in] Label window styles as defined for CStatic.
    /// @param topLeft      [in] Position of the top, left corner for the label, in pixels.
    /// @param parentWnd    [in] Parent window for the label.
    /// @param id           [in] ID for the label.
    ///
    /// @return <b>true</b> if the label was created successfully.
    ///
    bool Create(LPCTSTR label, DWORD style, const POINT& topLeft, CWnd* parentWnd, UINT id = 0xffff);

    /// Creates the window for the label. To make the label visible,
    /// add the WS_VISIBLE style or call ShowWindow.
    ///
    /// @param labelID      [in] String resource ID for the label text.
    /// @param style        [in] Label window styles as defined for CStatic.
    /// @param topLeft      [in] Position of the top, left corner for the label, in pixels.
    /// @param parentWnd    [in] Parent window for the label.
    /// @param id           [in] ID for the label.
    ///
    /// @return <b>true</b> if the label was created successfully.
    ///
    bool Create(UINT labelID, DWORD style, const POINT& topLeft, CWnd* parentWnd, UINT id = 0xffff);
};

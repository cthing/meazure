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
/// @brief Header file for the text field control.

#pragma once


/// Provides a text field control with appropriate fonts and sizes configured.
///
class MeaTextField : public CEdit {

public:
    /// Creates the window for the field. To make the field visible,
    /// add the WS_VISIBLE style or call ShowWindow.
    ///
    /// @param style        [in] Field styles as defined for CEdit.
    /// @param topLeft      [in] Position of the top, left corner for the field, in pixels.
    /// @param numChars     [in] Number of visible characters. For proportional fonts
    ///                     the number of characters actually visible may vary from the
    ///                     specified value.
    /// @param parentWnd    [in] Parent window for the field.
    /// @param id           [in] ID for the field.
    ///
    /// @return <b>true</b> if the field was created successfully.
    ///
    bool Create(DWORD style, const POINT& topLeft, int numChars, CWnd* parentWnd, UINT id = 0xFFFF);

    /// Indicates if the value in the field is selected.
    ///
    /// @return <b>true</b> if the value in the field is selected.
    ///
    bool IsTextSelected() const {
        int start, end;
        GetSel(start, end);
        return (start != end);
    }

protected:
    DECLARE_MESSAGE_MAP()

    /// Called when the field loses keyboard focus. The parent
    /// window is sent a MeaFieldFocusMsg message so that it can
    /// take actions such as reading the value in the field.
    ///
    /// @param win      [in] Next window to receive focus.
    /// 
    afx_msg void OnKillFocus(CWnd* win);
};

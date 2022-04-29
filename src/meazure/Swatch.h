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
/// @brief Header file for a rectangular color swatch.

#pragma once


/// Rectangular color swatch window.
///
class MeaSwatch : public CWnd {

public:
    /// Constructs a color swatch window. Before the window can
    /// be displayed, the Create method must be called.
    ///
    MeaSwatch();

    /// Destroys an instance of a color swatch window.
    ///
    virtual ~MeaSwatch();

    /// Creates the window for the color swatch and sets its
    /// background to the specified color.
    ///
    /// @param color    [in] Color for the swatch.
    /// @param rect     [in] Dimensions for the swatch window.
    /// @param parent   [in] Parent window for the color swatch.
    ///
    /// @return <b>true</b> if creation was successful.
    ///
    bool Create(COLORREF color, const RECT& rect, CWnd* parent);

    /// Makes the window visible.
    ///
    void Show() { if (m_hWnd != nullptr) ShowWindow(SW_SHOW); }

    /// Hides the window.
    ///
    void Hide() { if (m_hWnd != nullptr) ShowWindow(SW_HIDE); }

    /// Sets the color to be displayed by the swatch.
    ///
    /// @param color    [in] Color to be displayed.
    ///
    void SetColor(COLORREF color);

protected:
    DECLARE_MESSAGE_MAP()

    /// Called when the window is about to be destroyed. The
    /// method cleans up the brush used for painting the color
    /// swatch.
    ///
    afx_msg void OnDestroy();

private:
    /// Deletes the brush used for painting the color swatch.
    ///
    void DestroyColors();


    CBrush* m_backBrush;   ///< Brush used for painting the color swatch.
};



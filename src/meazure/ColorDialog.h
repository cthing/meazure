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
/// @brief Header file for customized color dialog.

#pragma once

#include "Profile.h"


/// Derived class of CColorDialog that manages persisting
/// custom colors and simplifies using the dialog.
///
class MeaColorDialog : public CColorDialog {

public:
    /// Constructs the color dialog.
    ///
    /// @param titleId      [in] String resource ID for the dialog title.
    /// @param clrInit      [in] Initial color to be selected by the dialog.
    /// @param dwFlags      [in] Dialog display flags defined by CColorDialog.
    /// @param pParentWnd   [in] Parent window for the dialog.
    ///
    explicit MeaColorDialog(UINT titleId = 0xFFFF, COLORREF clrInit = 0, DWORD dwFlags = 0, CWnd* pParentWnd = nullptr);

    /// Destroys a color dialog.
    ///
    virtual ~MeaColorDialog();

    /// Modally displays the color dialog.
    ///
    /// @return IDOK or IDCANCEL.
    ///
    virtual INT_PTR DoModal() override;

    /// Called when the dialog window has been created but before
    /// it is visible. This method customizes the dialog for this
    /// class.
    ///
    /// @return Always TRUE.
    ///
    virtual BOOL OnInitDialog() override;

    /// Persists the state of the dialog (e.g. the custom colors)
    /// to the specified profile object.
    ///
    /// @param profile      [in] Profile object into which the state
    ///                     is persisted.
    ///
    static void SaveProfile(MeaProfile& profile);

    /// Restores the state of the dialog from the specified profile object.
    ///
    /// @param profile      [in] Profile object from which the state
    ///                     is persisted.
    ///
    static void LoadProfile(MeaProfile& profile);

    /// Resets the color dialog to its default state.
    ///
    static void MasterReset();

private:
    static constexpr int kNumCustomColors { 16 };   ///< Number of available custom colors.

    static COLORREF m_defColors[];      ///< Default colors.
    static COLORREF m_customColors[];   ///< User definable custom colors.

    CString m_title;        ///< Title for the dialog.
};

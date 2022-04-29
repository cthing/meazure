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
/// @brief Program preferences property sheet header file.

#pragma once


#include "RulerPrefs.h"
#include "CalibrationPrefs.h"
#include "ToolsPrefs.h"
#include "PrecisionPrefs.h"
#include "CustomUnitsPrefs.h"
#include "AdvancedPrefs.h"
#include "Messages.h"


/// Property sheet holding all program preference pages.
///
class MeaPreferences : public CPropertySheet {
    DECLARE_DYNAMIC(MeaPreferences)

public:
    /// Preference page selection indices. With the
    /// exception of kAllPages, these indices must be
    /// consecutive and in the order in which pages are
    /// to be listed on the property sheet.
    ///
    enum PreferencePage {
        kAllPages = -1,         ///< Refers to all property pages.
        kCalibrationPage = 0,   ///< Selects the preference page for screen resolution calibration.
        kRulerPage = 1,         ///< Selects the preference page for ruler appearance.
        kToolsPage = 2,         ///< Selects the preference page for crosshair and tool appearance.
        kPrecisionPage = 3,     ///< Selects the preference page for numerical display precision.
        kCustomPage = 4,        ///< Selects the preference page for defining custom measurement units.
        kAdvancedPage = 5       ///< Selects the preference page for setting a startup profile.
    };


    /// Constructs the property sheet.
    ///
    /// @param nIDCaption       [in] String table ID for the title of the preferences dialog.
    /// @param pParentWnd       [in] Parent window for the preferences dialog. If the value is
    ///                         nullptr, the dialog is a popup.
    /// @param iSelectPage      [in] Preference page to display when the preferences dialog is shown.
    ///                         Default is the first page.
    /// 
    explicit MeaPreferences(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);

    /// Constructs the property sheet.
    ///
    /// @param pszCaption       [in] String for the title of the preferences dialog.
    /// @param pParentWnd       [in] Parent window for the preferences dialog. If the value is
    ///                         nullptr, the dialog is a popup.
    /// @param iSelectPage      [in] Preference page to display when the preferences dialog is shown.
    ///                         Default is the first page.
    /// 
    explicit MeaPreferences(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);

    /// Destroys a property sheet instance.
    ///
    virtual ~MeaPreferences();

    /// Displays the preference page for defining custom measurement units.
    ///
    void ShowCustomPrecision();


    MeaCalibrationPrefs m_calibrationPrefs;     ///< Preference page for screen resolution calibration.
    MeaRulerPrefs m_rulerPrefs;                 ///< Preference page for ruler appearance.
    MeaToolsPrefs m_toolsPrefs;                 ///< Preference page for crosshair and tool appearance.
    MeaPrecisionPrefs m_precisionPrefs;         ///< Preference page for numerical display precision.
    MeaCustomUnitsPrefs m_customPrefs;          ///< Preference page for defining custom measurement units.
    MeaAdvancedPrefs m_advancedPrefs;           ///< Preference page for setting a startup profile.

protected:
    /// Called when the property sheet dialog is dragged. This method calls the
    /// calibration preference page to allow it to update to reflect the current
    /// screen's resolution calibration state.
    ///
    /// @param x    [in] x coordinate of the upper left corner of the dialog window in screen coordinates.
    /// @param y    [in] y coordinate of the upper left corner of the dialog window in screen coordinates.
    /// 
    afx_msg void OnMove(int x, int y);

    /// Called when the property sheet dialog is about to be created. This method
    /// changes the tab style of the dialog from stacked tabs to horizontally
    /// scrolling tabs.
    ///
    /// @param lpCreateStruct   [in] dialog creation information structure.
    /// 
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    /// Calls the parent of the property sheet dialog with the apply preferences
    /// message and the index of the currently active preference page. In addition,
    /// the modified flag is cleared on all preference pages.
    /// 
    afx_msg void OnApply();

    /// Works around a design flaw in the CPropertySheet class. The current
    /// property page is not remembered between invocations of the dialog.
    /// Overriding this method allows catpuring the current page so it can
    /// be restored the next time the dialog is displayed.
    ///
    /// @param wParam   [in] See the CWnd::OnCommand documentation
    /// @param lParam   [in] See the CWnd::OnCommand documentation
    ///
    /// @return TRUE if the message has been processed.
    ///
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;

    DECLARE_MESSAGE_MAP()
};

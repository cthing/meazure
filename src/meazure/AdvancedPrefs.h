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
/// @brief Advanced preferences property page header file.

#pragma once


/// Property page for the advanced preferences. This preference page allows
/// a startup profile to be specified and provides the master reset for all
/// application settings.
///
class MeaAdvancedPrefs : public CPropertyPage {

    DECLARE_DYNCREATE(MeaAdvancedPrefs)

public:
    /// Constructs the advanced preferences property page.
    ///
    MeaAdvancedPrefs();

    /// Destroys the advanced preferences property page.
    ///
    ~MeaAdvancedPrefs();

    enum { IDD = IDD_PREF_ADVANCED };

    CString m_startupProfile;           ///< Profile file to load on application startup.
    CFileDialog* m_startupProfileDlg;   ///< File dialog for selecting the startup profile file.

protected: 
    /// Performs Dynamic Data Exchange (DDX) for the page.
    ///
    /// @param pDX  [in] DDX object for exchanging data between the page UI
    ///             and the member variables.
    ///
    virtual void DoDataExchange(CDataExchange* pDX) override;

    /// When the browse button is pressed, this method is called to
    /// display the profile file selection dialog.
    /// 
    afx_msg void OnProfileBtn();
    
    /// Called when the profile file pathname is changed so that
    /// the modified flag can be set.
    /// 
    afx_msg void OnChangeProfilePathname();
    
    /// Clears the text in the startup profile edit field.
    ///
    afx_msg void OnClearProfile();
    
    /// Called when the mast reset button is pressed. A reset confirmation
    /// dialog is displayed and upon an affirmative response, the master
    /// reset is performed.
    /// 
    afx_msg void OnMasterReset();

    DECLARE_MESSAGE_MAP()
};

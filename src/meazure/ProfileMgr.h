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
/// @brief Header for the profile file manager.

#pragma once

#include "Profile.h"
#include "Singleton.h"


/// Manages the saving and loading of profile files. A profile file
/// records the state of all tools and other aspects of the measurement
/// environment.
///
class MeaProfileMgr : public MeaSingleton_T<MeaProfileMgr>
{
public:
    /// Creates the profile file save dialog.
    ///
    /// @return File save dialog.
    ///
    CFileDialog* CreateSaveDialog();
    
    /// Creates the profile file open dialog.
    ///
    /// @return File open dialog.
    ///
    CFileDialog* CreateLoadDialog();


    /// Displays the file open dialog and loads the
    /// selected profile file.
    ///
    void Load();
    
    /// Loads the specified profile file.
    ///
    /// @param pathname     [in] Profile pathname.
    ///
    void Load(LPCTSTR pathname) const;
    
    /// Displays the file save dialog and saves the
    /// selected profile file.
    ///
    void Save();

    
    /// Persists the state of the profile file manager.
    ///
    /// @param profile      [in] Profile object into which the manager
    ///                     should persist its state.
    ///
    void SaveProfile(MeaProfile& profile) const;

    /// Restores the state of the profile file manager.
    ///
    /// @param profile      [in] Profile object from which the manager
    ///                     should restore its state.
    ///
    void LoadProfile(MeaProfile& profile);


    /// Resets the profile manager to its default state.
    ///
    void MasterReset();


    /// Tests whether the specified filename represents a profile file.
    ///
    /// @param filename     [in] File to test
    ///
    /// @return <b>true</b> if the specified file is a profile file.
    ///
    static bool IsProfileFile(LPCTSTR filename);

private:
    MEA_SINGLETON_DECL(MeaProfileMgr);      ///< Managers are singletons.

    /// Purposely undefined.
    MeaProfileMgr(const MeaProfileMgr&);

    /// Purposely undefined.
    MeaProfileMgr& operator=(const MeaProfileMgr&);


    static LPCTSTR  m_ext;          ///< Profile file suffix.
    static LPCTSTR  m_filter;       ///< File dialog filter string.

    CFileDialog *m_saveDialog;      ///< Profile file save dialog.
    CFileDialog *m_loadDialog;      ///< Profile file open dialog.
    CString     m_saveDlgTitle;     ///< File save dialog title.
    CString     m_loadDlgTitle;     ///< File open dialog title.
    CString     m_initialDir;       ///< Initial directory for file dialogs.
};

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
/// @brief Header file for the application class.

#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"


/// The application class. Instantiating this class starts the application.
///
class App : public CWinApp {

public:
    /// Starts the application.
    ///
    App();

    /// Called by the framework to see if each modified doc wants
    /// to save before the application goes away.
    ///
    /// @return TRUE if safe to exit the application, FALSE otherwise.
    ///
    virtual BOOL SaveAllModified() override;

    /// Central interface to the windows help system. In this case
    /// we override the base class calls to the old WinHelp system and
    /// instead call the new Windows HTML Help system.
    ///
    /// @param dwData   [in] ID for context sensitive help.
    /// @param nCmd     [in] Type of help requested.
    ///
    virtual void WinHelp(DWORD dwData, UINT nCmd);

    /// Performs all initialization for the application including
    /// processing of the command line arguments, if any.
    /// @return TRUE if initialization successful.
    /// 
    virtual BOOL InitInstance() override;

    /// Displays the application About dialog.
    /// 
    afx_msg void OnAppAbout();

    /// Called by the Mouse Hook DLL to route pointer movement
    /// messages to the View.
    ///
    /// @param wParam   [in] Message ID.
    /// @param lParam   [in] MOUSEHOOKSTRUCT
    ///
    afx_msg void OnMouseHook(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};

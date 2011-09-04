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
/// @brief Header file for a status bar that allows the overriding of the
///     idle message.

#pragma once


/// CStatusBar derived class that allows the overriding of the message
/// displayed when the application is idle.
///
class MeaStatusBar : public CStatusBar
{
public:
    /// Constructs the status bar. To use the status bar, its Create
    /// method must be called.
    ///
    MeaStatusBar();
    
    /// Destroys an instance of the status bar.
    ///
    virtual ~MeaStatusBar();


    /// Sets the message to be displayed in the status bar when the
    /// application is idle.
    ///
    /// @param msg      [in] Message to be displayed during idle.
    ///
    void SetIdleMessage(LPCTSTR msg);

protected:
    /// Called when the status bar is created. This method sets
    /// the default idle message.
    ///
    /// @param lpCreateStruct       [in] Window creation structure
    ///
    /// @return Zero to indicate that window creation should continue.
    ///
    afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);

    /// Called when the status bar wants to display a new message.
    /// When the status bar wants to display the idle message, the
    /// idle message set in this class is substituted for the system
    /// default message.
    ///
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Idle message ID or pointer.
    ///
    /// @return Nonzero if successful.
    ///
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    CString m_idleMsg;  ///< Message to display when app is idle.
};

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
/// @brief Header file for the position recording and management dialog.

#pragma once

#include "PositionLogObserver.h"


/// A non-modal dialog for recording new positions and managing positions
/// that have already been recorded. Besides recording positions, the
/// dialog allows individual positions to be deleted, replaced, and
/// annotated.
///
/// This class is an observer of the position log manager. This allows
/// the dialog to be called when positions are recorded, deleted,
/// loaded or saved.
///
class MeaPositionLogDlg : public CDialog, public MeaPositionLogObserver {

public:
    enum { IDD = IDD_POSITION_MGR };

    /// Constructs the position manager dialog. Since this is
    /// a non-modal dialog, its Create method is called followed
    /// by a call to ShowWindow.
    ///
    MeaPositionLogDlg();

    /// Destroys the dialog.
    ///
    virtual ~MeaPositionLogDlg();

    /// Creates the window for the dialog.
    ///
    /// @return TRUE if the window was created successfully.
    ///
    BOOL Create() {
        return CDialog::Create(IDD_POSITION_MGR, nullptr);
    }

    // Position log observer support

    /// Called when a position log file is loaded.
    ///
    virtual void LogLoaded() override;

    /// Called when a position log file is saved.
    ///
    virtual void LogSaved() override;

    /// Called when a new position is recorded.
    /// @param posIndex     [in] Index of the new position.
    virtual void PositionAdded(int posIndex) override;

    /// Called when an existing position is replaced with a new position.
    /// @param posIndex     [in] Index of the replaced position.
    virtual void PositionReplaced(int posIndex) override;

    /// Called when a position is deleted.
    /// @param posIndex     [in] Index of the deleted position.
    virtual void PositionDeleted(int posIndex) override;

    /// Called when all recorded positions have been deleted.
    ///
    virtual void PositionsDeleted() override;

    /// Called right before the dialog is displayed. Sets the
    /// state of all UI controls.
    /// @return TRUE to continue the display of the dialog.
    /// 
    virtual BOOL OnInitDialog() override;

protected:
    /// Called when the Add position button is pressed. Records
    /// the current tool positions.
    /// 
    afx_msg void OnAddPosition();

    /// Called when the Delete All positions button is pressed.
    /// Deletes all positions.
    /// 
    afx_msg void OnDeletePositions();

    /// Called when the Delete position button is pressed. Deletes
    /// the position corresponding to the position scrollbar.
    /// 
    afx_msg void OnDeletePosition();

    /// Called when the Replace button is pressed. The current
    /// scrollbar position is replaced with the current radio tool
    /// position.
    /// 
    afx_msg void OnReplacePosition();

    /// Called when the position scrollbar is moved.
    /// @param nSBCode      [in] Trackbar code.
    /// @param nPos         [in] Position value.
    /// @param pScrollBar   [in] Scrollbar control object.
    /// 
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    /// Called when the position description field loses the keyboard
    /// focus. If the text in the field differs from that in the
    /// current position, it is entered into the position object.
    /// 
    afx_msg void OnKillfocusPositionDesc();

    /// Called when the Load button is pressed. Queries the user
    /// for the position log file to load.
    /// 
    afx_msg void OnLoadPositions();

    /// Called when the Save button is pressed. Saves the position
    /// log file to the current pathname or queries for a pathname.
    /// 
    afx_msg void OnSavePositions();

    /// Called when the Save As button is pressed. Queries for a
    /// pathname and saves the position log file to that pathname.
    /// 
    afx_msg void OnSavePositionsAs();

    DECLARE_MESSAGE_MAP()


    /// Called when the dialog Close button is pressed. This method
    /// destroys the dialog's window.
    ///
    virtual void OnCancel() override;

    /// Called after the non-client area of the dialog is destroyed.
    /// This is the last step in dialog destruction and is the place
    /// where a non-modal dialog class instance can be deleted.
    ///
    virtual void PostNcDestroy() override;

    /// Sets the dialog title with the currently opened position filename.
    ///
    void SetDlgTitle();

    /// Returns the scrollbar value which is the position list index.
    /// @return Scrollbar position.
    int GetScrollPos() const;

    /// Sets the scrollbar value which is the position list index.
    /// @param pos      [in] Position index.
    void SetScrollPos(int pos) const;

    /// Sets the range for the position list scrollbar.
    void SetScrollRange() const;

    /// Update the position number field, the recorded date field
    /// and the description field.
    ///
    /// @param posIndex     [in] Position index or -1 if the scrollbar
    ///                     should be read to determine the current position.
    ///
    void UpdatePositionInfo(int posIndex = -1) const;

    /// Enables/disables the dialog controls based on the current
    /// state of the position manager.
    ///
    void UpdateEnable() const;
};

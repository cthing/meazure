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
 /// @brief Header file for the Position Log file save dialog. 

#pragma once


/// The Save dialog for Position Log files. The dialog is a standard CFileDialog with the addition of a text field
/// for specifying the Log file title and a text field for specifying the Log file description. Unfortunately, the
/// old style MFC file dialog must be used so that the text fields can be appropriately sized and placed. The
/// CFileDialog.AddEditBox only creates short single line text fields. This is inappropriate for the position log
/// description.
///
class MeaPositionSaveDlg : public CFileDialog {
    DECLARE_DYNAMIC(MeaPositionSaveDlg)

public:
    /// Constructs a position log file dialog. Call the dialog's DoModal method to display the dialog.
    ///
    /// @param lpszDefExt       [in] Default file extension.
    /// @param lpszFileName     [in] Initial filename appearing in filename box.
    /// @param lpszFilter       [in] A series of string pairs that specify filters you can apply to the file.
    /// @param pParentWnd       [in] A pointer to the file dialog-box object’s parent or owner window.
    ///
    explicit MeaPositionSaveDlg(PCTSTR lpszDefExt = nullptr, PCTSTR lpszFileName = nullptr,
                                PCTSTR lpszFilter = nullptr, CWnd* pParentWnd = nullptr);

    /// Destroys a position log file dialog.
    ///
    virtual ~MeaPositionSaveDlg();

    enum { IDD = IDD_POSITION_SAVE };

    /// Position Log file description.
    /// 
    CString m_desc;

    /// Title for the dialog.
    ///
    CString m_title;

protected:
    DECLARE_MESSAGE_MAP()

    static constexpr int kFilenameComboId { 1148 };     ///< Identifier of CFildDialog filename combo control.
    static constexpr int kFilenameLabelId { 1090 };     ///< Identifier of CFileDialog filename label control.


    /// Called to set the size and position of dialog items. We are using the method to properly position
    /// the items added in the dialog template (i.e. title and description controls).
    ///
    afx_msg void OnSize(UINT nType, int cx, int cy);

    /// Performs dynamic data exchange (DDX) for the dialog.
    ///
    /// @param pDX      [in] DDX operation object.
    ///
    virtual void DoDataExchange(CDataExchange* pDX) override;

    /// Called to validate the filename when the Save button is pressed. We are using this method to
    /// manually trigger DDX.
    /// 
    /// @return TRUE if the filename is valid.
    /// 
    virtual BOOL OnFileNameOK() override;
};

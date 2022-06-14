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

#include <meazure/pch.h>
#include <meazure/resource.h>
#include "PositionSaveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(MeaPositionSaveDlg, CFileDialog)


BEGIN_MESSAGE_MAP(MeaPositionSaveDlg, CFileDialog)
    ON_WM_SIZE()
END_MESSAGE_MAP()


MeaPositionSaveDlg::MeaPositionSaveDlg(PCTSTR lpszDefExt, PCTSTR lpszFileName, PCTSTR lpszFilter, CWnd* pParentWnd) :
    CFileDialog(FALSE, lpszDefExt, lpszFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLETEMPLATE,
                lpszFilter, pParentWnd, 0, FALSE) {
    m_ofn.hInstance = AfxGetInstanceHandle();
    m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_POSITION_SAVE);
}

MeaPositionSaveDlg::~MeaPositionSaveDlg() {}

void MeaPositionSaveDlg::DoDataExchange(CDataExchange* pDX) {
    CFileDialog::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_LOG_DESC, m_desc);
    DDX_Text(pDX, IDC_LOG_TITLE, m_title);
}

BOOL MeaPositionSaveDlg::OnFileNameOK() {
    // DDX does not appear to be invoked when the Save button is pressed on the file dialog.
    // So we need to do it manually here.
    UpdateData(TRUE);

    return CFileDialog::OnFileNameOK();
}

void MeaPositionSaveDlg::OnSize(UINT nType, int cx, int cy) {
    CFileDialog::OnSize(nType, cx, cy);

    // Obtain the dimensions and placement of the CFileDialog's filename combo box.
    CWnd* filenameCombo = GetParent()->GetDlgItem(kFilenameComboId);
    if (filenameCombo == nullptr) {
        return;
    }
    RECT filenameComboRect;
    filenameCombo->GetWindowRect(&filenameComboRect);
    GetParent()->ScreenToClient(&filenameComboRect);

    // Obtain the dimensions and placement of the CFileDialog's filename combo box label.
    CWnd* filenameLabel = GetParent()->GetDlgItem(kFilenameLabelId);
    if (filenameLabel == nullptr) {
        return;
    }
    RECT filenameLabelRect;
    filenameLabel->GetWindowRect(&filenameLabelRect);
    GetParent()->ScreenToClient(&filenameLabelRect);

    // Move and size the position log title field to align with the filename combo.
    CWnd* titleField = GetDlgItem(IDC_LOG_TITLE);
    RECT titleFieldRect;
    titleField->GetWindowRect(&titleFieldRect);
    GetParent()->ScreenToClient(&titleFieldRect);
    titleFieldRect.left = filenameComboRect.left;
    titleFieldRect.right = filenameComboRect.right;
    titleField->MoveWindow(&titleFieldRect);

    // Move and size the position log description field to align with the filename combo.
    CWnd* descField = GetDlgItem(IDC_LOG_DESC);
    RECT descFieldRect;
    descField->GetWindowRect(&descFieldRect);
    GetParent()->ScreenToClient(&descFieldRect);
    descFieldRect.left = filenameComboRect.left;;
    descFieldRect.right = filenameComboRect.right;
    descField->MoveWindow(&descFieldRect);

    // Move the position log title label to left align with the filename combo label.
    CWnd* titleLabel = GetDlgItem(IDC_LOG_TITLE_LABEL);
    RECT titleLabelRect;
    titleLabel->GetWindowRect(&titleLabelRect);
    GetParent()->ScreenToClient(&titleLabelRect);
    int delta = filenameLabelRect.left - titleLabelRect.left;
    titleLabelRect.left += delta;
    titleLabelRect.right += delta;
    titleLabel->MoveWindow(&titleLabelRect);

    // Move the position log description label to left align with the filename combo label.
    CWnd* descLabel = GetDlgItem(IDC_LOG_DESC_LABEL);
    RECT descLabelRect;
    descLabel->GetWindowRect(&descLabelRect);
    GetParent()->ScreenToClient(&descLabelRect);
    descLabelRect.left += delta;
    descLabelRect.right += delta;
    descLabel->MoveWindow(&descLabelRect);
}

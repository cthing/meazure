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

#include "StdAfx.h"
#include "Resource.h"
#include "PositionSaveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(MeaPositionSaveDlg, CFileDialog)


BEGIN_MESSAGE_MAP(MeaPositionSaveDlg, CFileDialog)
    //{{AFX_MSG_MAP(MeaPositionSaveDlg)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


MeaPositionSaveDlg::MeaPositionSaveDlg(LPCTSTR lpszDefExt, 
                                       LPCTSTR lpszFileName,
                                       LPCTSTR lpszFilter,
                                       CWnd* pParentWnd) : CFileDialog(FALSE,
                                       lpszDefExt, lpszFileName,
                                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
                                       OFN_ENABLETEMPLATE | OFN_EXPLORER,
                                       lpszFilter, pParentWnd)
{
    m_ofn.hInstance = AfxGetInstanceHandle();
    m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_POSITION_SAVE);

    //{{AFX_DATA_INIT(MeaPositionSaveDlg)
    m_desc = _T("");
    m_title = _T("");
    //}}AFX_DATA_INIT
}


MeaPositionSaveDlg::~MeaPositionSaveDlg()
{
}


void MeaPositionSaveDlg::DoDataExchange(CDataExchange* pDX)
{
    CFileDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(MeaPositionSaveDlg)
    DDX_Text(pDX, IDC_LOG_DESC, m_desc);
    DDX_Text(pDX, IDC_LOG_TITLE, m_title);
    //}}AFX_DATA_MAP
}

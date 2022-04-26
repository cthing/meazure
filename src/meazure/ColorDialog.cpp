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
#include "ColorDialog.h"


COLORREF MeaColorDialog::m_defColors[] {
    RGB(0x50, 0x50, 0x50),      // 0 - Default crosshair border color
    RGB(0xFF, 0xFF, 0xFF),      // 1
    RGB(0xFF, 0xFF, 0xFF),      // 2
    RGB(0xFF, 0xFF, 0xFF),      // 3
    RGB(0xFF, 0xFF, 0xFF),      // 4
    RGB(0xFF, 0xFF, 0xFF),      // 5
    RGB(0xFF, 0xFF, 0xFF),      // 6
    RGB(0xFF, 0xFF, 0xFF),      // 7
    RGB(0xFF, 0xFF, 0xFF),      // 8
    RGB(0xFF, 0xFF, 0xFF),      // 9
    RGB(0xFF, 0xFF, 0xFF),      // 10
    RGB(0xFF, 0xFF, 0xFF),      // 11
    RGB(0xFF, 0xFF, 0xFF),      // 12
    RGB(0xFF, 0xFF, 0xFF),      // 13
    RGB(0xFF, 0xFF, 0xFF),      // 14
    RGB(0xFF, 0xFF, 0xFF),      // 15
};
COLORREF MeaColorDialog::m_customColors[] {
    m_defColors[0],
    m_defColors[1],
    m_defColors[2],
    m_defColors[3],
    m_defColors[4],
    m_defColors[5],
    m_defColors[6],
    m_defColors[7],
    m_defColors[8],
    m_defColors[9],
    m_defColors[10],
    m_defColors[11],
    m_defColors[12],
    m_defColors[13],
    m_defColors[14],
    m_defColors[15],
};


MeaColorDialog::MeaColorDialog(UINT titleId, COLORREF clrInit, DWORD dwFlags,
                               CWnd* pParentWnd) :
    CColorDialog(clrInit, dwFlags, pParentWnd)
{
    if (titleId != 0xFFFF) {
        VERIFY(m_title.LoadString(titleId));
    }
}


MeaColorDialog::~MeaColorDialog()
{
}


INT_PTR MeaColorDialog::DoModal()
{
    m_cc.lpCustColors = m_customColors;

    return CColorDialog::DoModal();
}


BOOL MeaColorDialog::OnInitDialog()
{
    CColorDialog::OnInitDialog();

    if (!m_title.IsEmpty()) {
        SetWindowText(m_title);
    }
    return TRUE;
}


void MeaColorDialog::SaveProfile(MeaProfile& profile)
{
    if (!profile.UserInitiated()) {
        for (int i = 0; i < kNumCustomColors; i++) {
            CString str;

            str.Format(_T("CustomColor%X"), i);
            profile.WriteInt(str, m_customColors[i]);
        }
    }
}


void MeaColorDialog::LoadProfile(MeaProfile& profile)
{
    if (!profile.UserInitiated()) {
        for (int i = 0; i < kNumCustomColors; i++) {
            CString str;

            str.Format(_T("CustomColor%X"), i);
            m_customColors[i] = static_cast<COLORREF>(profile.ReadInt(str, m_customColors[i]));
        }
    }
}


void MeaColorDialog::MasterReset()
{
    for (int i = 0; i < kNumCustomColors; i++) {
        m_customColors[i] = m_defColors[i];
    }
}

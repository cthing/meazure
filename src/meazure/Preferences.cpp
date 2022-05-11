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
#include "Preferences.h"
#include "ScreenMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(MeaPreferences, CPropertySheet)


BEGIN_MESSAGE_MAP(MeaPreferences, CPropertySheet)
    ON_WM_MOVE()
    ON_WM_CREATE()
    ON_BN_CLICKED(ID_APPLY_NOW, OnApply)
END_MESSAGE_MAP()


MeaPreferences::MeaPreferences(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) :
    CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
    m_calibrationPrefs(MeaScreenMgr::Instance()),
    m_rulerPrefs(MeaScreenMgr::Instance()),
    m_toolsPrefs(MeaScreenMgr::Instance()) {

    // Remove the help button from the dialog.
    //
    m_psh.dwFlags &= ~(PSH_HASHELP);

    // Add the preference pages to the dialog.
    //
    AddPage(&m_calibrationPrefs);
    AddPage(&m_rulerPrefs);
    AddPage(&m_toolsPrefs);
    AddPage(&m_precisionPrefs);
    AddPage(&m_customPrefs);
    AddPage(&m_advancedPrefs);
}

MeaPreferences::MeaPreferences(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(pszCaption, pParentWnd, iSelectPage),
    m_calibrationPrefs(MeaScreenMgr::Instance()),
    m_rulerPrefs(MeaScreenMgr::Instance()),
    m_toolsPrefs(MeaScreenMgr::Instance()) {
    // Remove the help button from the dialog.
    //
    m_psh.dwFlags &= ~(PSH_HASHELP);

    // Add the preference pages to the dialog.
    //
    AddPage(&m_calibrationPrefs);
    AddPage(&m_rulerPrefs);
    AddPage(&m_toolsPrefs);
    AddPage(&m_precisionPrefs);
    AddPage(&m_customPrefs);
    AddPage(&m_advancedPrefs);
}

MeaPreferences::~MeaPreferences() {}

void MeaPreferences::OnApply() {
    if (GetActivePage()->OnKillActive()) {
        GetActivePage()->UpdateData(TRUE);
        GetParent()->SendMessage(MeaPrefsApplyMsg, 0, GetActiveIndex());
        m_calibrationPrefs.SetModified(FALSE);
        m_rulerPrefs.SetModified(FALSE);
        m_toolsPrefs.SetModified(FALSE);
        m_precisionPrefs.SetModified(FALSE);
        m_customPrefs.SetModified(FALSE);
        m_advancedPrefs.SetModified(FALSE);
    }
}

void MeaPreferences::ShowCustomPrecision() {
    SetActivePage(&m_precisionPrefs);
    m_precisionPrefs.SelectCustomUnits();
}

void MeaPreferences::OnMove(int x, int y) {
    CPropertySheet::OnMove(x, y);
    m_calibrationPrefs.OnMove(x, y);
}

int MeaPreferences::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    EnableStackedTabs(FALSE);

    return CPropertySheet::OnCreate(lpCreateStruct);
}

BOOL MeaPreferences::OnCommand(WPARAM wParam, LPARAM lParam) {
    UINT const id = LOWORD(wParam);

    if (IDOK == id || IDCANCEL == id) {
        // HACK: CPropertySheet::GetActiveIndex() behaves differently depending
        // on whether the window exists [e.g. whether inside DoModal()].
        // If window exists, it actually asks the tab control for the active
        // page. Otherwise it merely returns m_psh.nStartPage -- meaning that
        // after DoModal() returns, the value is simply what we startedwith.
        // This makes it impossible to bring up the sheet next time with
        // whatever page the user last had active -- a desirable behavior.
        //
        // To make GetActivePage() return what page the user actually had
        // active, stuff the value in m_psh.nStartPage here. Note that this
        // makes any static CGenPropertySheet instances retain the active page
        // automatically. Non-static instances will need to save the
        // GetActiveIndex() value in a static variable and call SetActivePage()
        // on the next creation/invocation.
        //
        m_psh.nStartPage = GetActiveIndex();
    }

    return CPropertySheet::OnCommand(wParam, lParam);
}

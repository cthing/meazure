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

#include "pch.h"
#include "Resource.h"
#include "ProfileMgr.h"
#include "FileProfile.h"
#include "MainFrm.h"
#include <stdlib.h>


LPCTSTR MeaProfileMgr::m_ext { _T("mea") };
LPCTSTR MeaProfileMgr::m_filter { _T("Meazure Profiles (*.mea)|*.mea|All Files (*.*)|*.*||") };


MeaProfileMgr::MeaProfileMgr(token) :
    MeaSingleton_T<MeaProfileMgr>(),
    m_saveDialog(nullptr),
    m_loadDialog(nullptr),
    m_saveDlgTitle(reinterpret_cast<LPCSTR>(IDS_MEA_SAVE_PROFILE_DLG)),
    m_loadDlgTitle(reinterpret_cast<LPCSTR>(IDS_MEA_LOAD_PROFILE_DLG)) {
    //
    // Set the initial directory for the save/load dialogs to the Profiles
    // subfolder in the application's folder. This can be changed based on
    // the last directory visited from the registry.
    //
    TCHAR pathname[MAX_PATH], drive[_MAX_DRIVE], path[_MAX_DIR];

    if (::GetModuleFileName(nullptr, pathname, MAX_PATH) > 0) {
        _tsplitpath_s(pathname, drive, _MAX_DRIVE, path, _MAX_DIR, nullptr, 0, nullptr, 0);
        m_initialDir = drive;
        m_initialDir += path;
        m_initialDir += _T("Profiles");
    }
}

MeaProfileMgr::~MeaProfileMgr() {
    try {
        delete m_saveDialog;
        delete m_loadDialog;
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaProfileMgr::SaveProfile(MeaProfile& profile) const {
    if (!profile.UserInitiated()) {
        profile.WriteStr(_T("LastProfileDir"), static_cast<LPCTSTR>(m_initialDir));
    }
}

void MeaProfileMgr::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        m_initialDir = profile.ReadStr(_T("LastProfileDir"), static_cast<LPCTSTR>(m_initialDir));
    }
}

void MeaProfileMgr::MasterReset() {
    m_initialDir.Empty();
}

CFileDialog* MeaProfileMgr::CreateSaveDialog() {
    if (m_saveDialog == nullptr) {
        m_saveDialog = new CFileDialog(FALSE, m_ext, nullptr,
            OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, m_filter);
        m_saveDialog->m_ofn.lpstrTitle = m_saveDlgTitle;
        m_saveDialog->m_ofn.lpstrInitialDir = m_initialDir;
    }
    return m_saveDialog;
}

CFileDialog* MeaProfileMgr::CreateLoadDialog() {
    if (m_loadDialog == nullptr) {
        m_loadDialog = new CFileDialog(TRUE, m_ext, nullptr,
            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, m_filter);
        m_loadDialog->m_ofn.lpstrTitle = m_loadDlgTitle;
        m_loadDialog->m_ofn.lpstrInitialDir = m_initialDir;
    }
    return m_loadDialog;
}

void MeaProfileMgr::Load() {
    CFileDialog* dlg = CreateLoadDialog();

    if (dlg->DoModal() == IDOK) {
        CString pathname = dlg->GetPathName();
        TCHAR drive[_MAX_DRIVE];
        TCHAR dir[_MAX_DIR];

        //
        // Remember the directory for persisting.
        //
        _tsplitpath_s(pathname, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
        m_initialDir = drive;
        m_initialDir += dir;

        Load(pathname);
    }
}

void MeaProfileMgr::Load(LPCTSTR pathname) const {
    try {
        MeaFileProfile profile(pathname, MeaFileProfile::ProfRead);

        static_cast<CMainFrame*>(AfxGetMainWnd())->LoadProfile(profile);
    } catch (CFileException* fe) {
        TCHAR errStr[256];
        CString msg;
        fe->GetErrorMessage(errStr, 256);
        msg.Format(IDS_MEA_NO_LOAD_PROFILE, errStr);
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
        fe->Delete();
    } catch (MeaXMLParserException&) {
        // Handled by the parser.
    } catch (...) {
        CString msg(reinterpret_cast<LPCSTR>(IDS_MEA_NO_PROFILE));
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
    }
}

void MeaProfileMgr::Save() {
    CFileDialog* dlg = CreateSaveDialog();

    if (dlg->DoModal() == IDOK) {
        CString pathname = dlg->GetPathName();
        TCHAR drive[_MAX_DRIVE];
        TCHAR dir[_MAX_DIR];

        //
        // Remember the directory for persisting.
        //
        _tsplitpath_s(pathname, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
        m_initialDir = drive;
        m_initialDir += dir;

        //
        // Save the profile
        //
        try {
            MeaFileProfile profile(pathname, MeaFileProfile::ProfWrite);

            static_cast<CMainFrame*>(AfxGetMainWnd())->SaveProfile(profile);
        } catch (CFileException* fe) {
            TCHAR errStr[256];
            CString msg;
            fe->GetErrorMessage(errStr, 256);
            msg.Format(IDS_MEA_NO_SAVE_PROFILE, errStr);
            MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
            fe->Delete();
        }
    }
}

bool MeaProfileMgr::IsProfileFile(LPCTSTR filename) {
    TCHAR ext[_MAX_EXT];
    int i = 0;

    _tsplitpath_s(filename, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
    if (ext[i] == _T('.')) {
        i++;
    }
    return (_tcsicmp(&ext[i], m_ext) == 0);
}

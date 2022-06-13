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
#include "RegistryProfile.h"
#include <meazure/VersionInfo.h>


MeaRegistryProfile::MeaRegistryProfile(MeaRegistryProvider& registry) : MeaProfile(), m_registry(registry) {
    // Determine which version of the registry profile is available
    // by testing for a known key.
    //
    CString curVer;
    curVer.Format(_T("%d.0"), MeaVersionInfo::Instance().GetProfileFileMajor());

    m_loadVersion = (!HaveVersionKey(curVer) && HaveVersionKey(_T("1.0"))) ? _T("1.0") : curVer;
    m_saveVersion = curVer;
}

MeaRegistryProfile::~MeaRegistryProfile() {}

bool MeaRegistryProfile::WriteBool(PCTSTR key, bool value) {
    return m_registry.WriteInt(m_saveVersion, key, value) == TRUE;
}

bool MeaRegistryProfile::WriteInt(PCTSTR key, int value) {
    return m_registry.WriteInt(m_saveVersion, key, value) == TRUE;
}

bool MeaRegistryProfile::WriteDbl(PCTSTR key, double value) {
    CString vstr;

    vstr.Format(_T("%f"), value);
    return m_registry.WriteString(m_saveVersion, key, vstr) == TRUE;
}

bool MeaRegistryProfile::WriteStr(PCTSTR key, PCTSTR value) {
    return m_registry.WriteString(m_saveVersion, key, value) == TRUE;
}

bool MeaRegistryProfile::ReadBool(PCTSTR key, bool defaultValue) {
    return m_registry.GetInt(m_loadVersion, key, defaultValue) != 0;
}

UINT MeaRegistryProfile::ReadInt(PCTSTR key, int defaultValue) {
    return m_registry.GetInt(m_loadVersion, key, defaultValue);
}


double MeaRegistryProfile::ReadDbl(PCTSTR key, double defaultValue) {
    CString str = m_registry.GetString(m_loadVersion, key, _T(""));
    if (str.IsEmpty()) {
        return defaultValue;
    }
    return atof(str);
}

CString MeaRegistryProfile::ReadStr(PCTSTR key, PCTSTR defaultValue) {
    return m_registry.GetString(m_loadVersion, key, defaultValue);
}

bool MeaRegistryProfile::UserInitiated() {
    return false;
}

int MeaRegistryProfile::GetVersion() {
    return atoi(m_loadVersion);
}

bool MeaRegistryProfile::HaveVersionKey(PCTSTR version) const {
    CString key;
    HKEY hKey;

    key.Format(_T("Software\\%s\\%s\\%s"), m_registry.GetKeyName(), AfxGetAppName(), version);

    if (m_registry.OpenKey(HKEY_CURRENT_USER, key, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }
    m_registry.CloseKey(hKey);
    return true;
}

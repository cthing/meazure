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

#include "StdAfx.h"
#include "RegistryProfile.h"
#include "VersionInfo.h"


MeaRegistryProfile::MeaRegistryProfile() : MeaProfile()
{
    // Determine which version of the registry profile is available
    // by testing for a known key.
    //
    CString curVer;
    curVer.Format(_T("%d.0"), g_versionInfo.GetProfileFileMajor());

    m_loadVersion = (!HaveVersionKey(curVer) && HaveVersionKey(_T("1.0"))) ? _T("1.0") : curVer;
    m_saveVersion = curVer;
}


MeaRegistryProfile::~MeaRegistryProfile()
{
}


bool MeaRegistryProfile::WriteBool(LPCTSTR key, bool value)
{
    return AfxGetApp()->WriteProfileInt(m_saveVersion, key, value) == TRUE;
}


bool MeaRegistryProfile::WriteInt(LPCTSTR key, int value)
{
    return AfxGetApp()->WriteProfileInt(m_saveVersion, key, value) == TRUE;
}


bool MeaRegistryProfile::WriteDbl(LPCTSTR key, double value)
{
    CString vstr;

    vstr.Format(_T("%f"), value);
    return AfxGetApp()->WriteProfileString(m_saveVersion, key, vstr) == TRUE;
}


bool MeaRegistryProfile::WriteStr(LPCTSTR key, LPCTSTR value)
{
    return AfxGetApp()->WriteProfileString(m_saveVersion, key, value) == TRUE;
}


bool MeaRegistryProfile::ReadBool(LPCTSTR key, bool defaultValue)
{
    return AfxGetApp()->GetProfileInt(m_loadVersion, key, defaultValue) != 0;
}


UINT MeaRegistryProfile::ReadInt(LPCTSTR key, int defaultValue)
{
    return AfxGetApp()->GetProfileInt(m_loadVersion, key, defaultValue);
}


double MeaRegistryProfile::ReadDbl(LPCTSTR key, double defaultValue)
{
    CString str = AfxGetApp()->GetProfileString(m_loadVersion, key, _T(""));
    if (str.IsEmpty()) {
        return defaultValue;
    }
    return atof(str);
}


CString MeaRegistryProfile::ReadStr(LPCTSTR key, LPCTSTR defaultValue)
{
    return AfxGetApp()->GetProfileString(m_loadVersion, key, defaultValue);
}


int MeaRegistryProfile::GetVersion()
{
    return atoi(m_loadVersion);
}


bool MeaRegistryProfile::HaveVersionKey(LPCTSTR version) const
{
    CString key;
    HKEY hKey;

    key.Format(_T("Software\\%s\\%s\\%s"), AfxGetApp()->m_pszRegistryKey,
                AfxGetAppName(), version);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }
    RegCloseKey(hKey);
    return true;
}


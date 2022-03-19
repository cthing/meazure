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
#include "VersionInfo.h"


MeaVersionInfo g_versionInfo;


MeaVersionInfo::MeaVersionInfo()
{
    TCHAR pathname[_MAX_PATH];
    DWORD size;
    DWORD handle;
    UINT len;

    GetModuleFileName(NULL, pathname, _MAX_PATH);
    size = GetFileVersionInfoSize(pathname, &handle);
    m_data = new char[size];
    GetFileVersionInfo(pathname, 0, size, m_data);
    VerQueryValue(m_data, _T("\\"), reinterpret_cast<void**>(&m_info), &len);
}


MeaVersionInfo::~MeaVersionInfo()
{
    delete []m_data;
}


CString MeaVersionInfo::GetProductVersion() const
{
    CString str;
    if (GetProductFix() == 0) {
        str.Format(_T("%d.%d"), GetProductMajor(), GetProductMinor());
    } else {
        str.Format(_T("%d.%d.%d"), GetProductMajor(), GetProductMinor(), GetProductFix());
    }
    return str;
}

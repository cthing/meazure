/*
 * Copyright 2022 C Thing Software
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
#include "Registry.h"


BOOL MeaRegistry::WriteInt(LPCTSTR section, LPCTSTR entry, int value) {
    return AfxGetApp()->WriteProfileInt(section, entry, value);
}

BOOL MeaRegistry::WriteString(LPCTSTR section, LPCTSTR entry, LPCTSTR value) {
    return AfxGetApp()->WriteProfileString(section, entry, value);
}

UINT MeaRegistry::GetInt(LPCTSTR section, LPCTSTR entry, int defaultValue) {
    return AfxGetApp()->GetProfileInt(section, entry, defaultValue);
}

CString MeaRegistry::GetString(LPCTSTR section, LPCTSTR entry, LPCTSTR defaultValue) {
    return AfxGetApp()->GetProfileString(section, entry, defaultValue);
}

LPCTSTR MeaRegistry::GetKeyName() {
    return AfxGetApp()->m_pszRegistryKey;
}

LSTATUS MeaRegistry::OpenKey(HKEY key, LPCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) {
    return ::RegOpenKeyEx(key, subKey, options, samDesired, result);
}

LSTATUS MeaRegistry::CloseKey(HKEY key) {
    return ::RegCloseKey(key);
}

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


BOOL MeaRegistry::WriteInt(PCTSTR section, PCTSTR entry, int value) {
    return AfxGetApp()->WriteProfileInt(section, entry, value);
}

BOOL MeaRegistry::WriteString(PCTSTR section, PCTSTR entry, PCTSTR value) {
    return AfxGetApp()->WriteProfileString(section, entry, value);
}

UINT MeaRegistry::GetInt(PCTSTR section, PCTSTR entry, int defaultValue) {
    return AfxGetApp()->GetProfileInt(section, entry, defaultValue);
}

CString MeaRegistry::GetString(PCTSTR section, PCTSTR entry, PCTSTR defaultValue) {
    return AfxGetApp()->GetProfileString(section, entry, defaultValue);
}

PCTSTR MeaRegistry::GetKeyName() {
    return AfxGetApp()->m_pszRegistryKey;
}

LSTATUS MeaRegistry::OpenKey(HKEY key, PCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) {
    return ::RegOpenKeyEx(key, subKey, options, samDesired, result);
}

LSTATUS MeaRegistry::CloseKey(HKEY key) {
    return ::RegCloseKey(key);
}

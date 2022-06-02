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

 /// @file
 /// @brief Provides access to the Windows Registry.

#pragma once

#include "RegistryProvider.h"


class MeaRegistry : public MeaRegistryProvider {

public:
    BOOL WriteInt(LPCTSTR section, LPCTSTR entry, int value) override;

    BOOL WriteString(LPCTSTR section, LPCTSTR entry, LPCTSTR value) override;

    UINT GetInt(LPCTSTR section, LPCTSTR entry, int defaultValue) override;

    CString GetString(LPCTSTR section, LPCTSTR entry, LPCTSTR defaultValue) override;

    LPCTSTR GetKeyName() override;

    LSTATUS OpenKey(HKEY key, LPCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) override;

    LSTATUS CloseKey(HKEY key) override;
};
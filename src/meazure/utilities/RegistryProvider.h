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
/// @brief Interface representing the Windows Registry.

#pragma once


/// Interface for Windows Registry interaction.
///
struct MeaRegistryProvider {

    /// Writes the specified value into the specified section of the application's registry.
    /// 
    /// @param section  [in] Points to a null-terminated string that specifies the section containing the entry.
    ///     If the section does not exist, it is created. The name of the section is case independent.
    /// @param entry    [in] Points to a null-terminated string that contains the entry into which the value is to
    ///     be written. If the entry does not exist in the specified section, it is created.
    /// @param value    [in] Contains the value to be written.
    /// @return TRUE if successful; otherwise FALSE.
    /// 
    virtual BOOL WriteInt(LPCTSTR section, LPCTSTR entry, int value) = 0;

    /// Write the specified string into the specified section of the application's registry.
    /// 
    /// @param section  [in] Points to a null-terminated string that specifies the section containing the entry.
    ///     If the section does not exist, it is created. The name of the section is case independent.
    /// @param entry    [in] Points to a null-terminated string that contains the entry into which the value is to
    ///     be written. If the entry does not exist in the specified section, it is created. If this parameter is
    ///     <b>NULL</b>, the section specified by <b>section</b> is deleted.
    /// @param value    [in] Points to the string to be written. If this parameter is NULL, the entry specified by
    ///     the <b>entry</b> parameter is deleted.
    /// @return TRUE if successful; otherwise FALSE.
    /// 
    virtual BOOL WriteString(LPCTSTR section, LPCTSTR entry, LPCTSTR value) = 0;

    /// Retrieves the value of an integer from an entry within a specified section of the application's registry.
    /// 
    /// @param section       [in] Points to a null-terminated string that specifies the section containing the entry.
    /// @param entry         [in] Points to a null-terminated string that contains the entry whose value is to be retrieved.
    /// @param defaultValue  [in] Specifies the default value to return if the framework cannot find the entry.
    /// @return The integer value of the string that follows the specified entry if the function is successful. The
    ///     return value is the value of the <b>defaultValue</b> parameter if the function does not find the entry.
    ///     The return value is 0 if the value that corresponds to the specified entry is not an integer.
    /// 
    virtual UINT GetInt(LPCTSTR section, LPCTSTR entry, int defaultValue) = 0;

    /// Retrieves the string associated with an entry within the specified section in the application's registry.
    /// 
    /// @param section       [in] Points to a null-terminated string that specifies the section containing the entry.
    /// @param entry         [in] Points to a null-terminated string that contains the entry whose string is to be
    ///     retrieved. This value must not be <b>NULL</b>.
    /// @param defaultValue  [in] Points to the default string value for the given entry if the entry cannot be found
    /// @return The return value is the string from the application's registry entry or <b>defaultValue</b> if the
    ///     string cannot be found. The maximum string length supported by the framework is <b>_MAX_PATH</b>. If
    ///     <b>defaultValue</b> is NULL, the return value is an empty string.
    /// 
    virtual CString GetString(LPCTSTR section, LPCTSTR entry, LPCTSTR defaultValue) = 0;

    /// Determines where, in the registry or INI file, application profile settings are stored
    ///
    /// @return Application registry key.
    /// 
    virtual LPCTSTR GetKeyName() = 0;

    /// Opens the specified registry key. Note that key names are not case sensitive.
    /// 
    /// @param key         [in] A handle to an open registry key
    /// @param subKey      [in] Name of the registry sub-key
    /// @param options     [in] Specifies the option to apply when opening the key
    /// @param samDesired  [in] A mask that specifies the desired access rights to the key to be opened
    /// @param result      [in] A pointer to a variable that receives a handle to the opened key
    /// @return If the function succeeds, the return value is <b>ERROR_SUCCESS</b>.
    /// 
    virtual LSTATUS OpenKey(HKEY key, LPCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) = 0;

    /// Closes a handle to the specified registry key.
    /// 
    /// @param key  [in] A handle to the open key to be closed
    /// @return If the function succeeds, the return value is <b>ERROR_SUCCESS</b>.
    /// 
    virtual LSTATUS CloseKey(HKEY key) = 0;
};

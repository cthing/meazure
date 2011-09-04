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

/// @file
/// @brief Profile base class header file. 

#pragma once


/// Base class for the persisting the application state to
/// either the Registry or a file.
///
class MeaProfile {
public:
    /// Constructs a profile instance.
    ///
    MeaProfile();
    
    /// Destroys a profile instance.
    ///
    virtual ~MeaProfile();


    /// Writes a boolean value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] Boolean value for the key
    ///
    virtual bool    WriteBool(LPCTSTR key, bool value);

    /// Writes an integer value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] Integer value for the key
    ///
    virtual bool    WriteInt(LPCTSTR key, int value);

    /// Writes a double value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] Double value for the key
    ///
    virtual bool    WriteDbl(LPCTSTR key, double value);

    /// Writes a string value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] String value for the key
    ///
    virtual bool    WriteStr(LPCTSTR key, LPCTSTR value);


    /// Reads a boolean value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    virtual bool    ReadBool(LPCTSTR key, bool defaultValue);

    /// Reads an unsigned integer value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    virtual UINT    ReadInt(LPCTSTR key, int defaultValue);

    /// Reads a double value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    virtual double  ReadDbl(LPCTSTR key, double defaultValue);

    /// Reads a string value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    virtual CString ReadStr(LPCTSTR key, LPCTSTR defaultValue);

    
    /// Indicates whether the profile is being written at
    /// the user's request (i.e. a file profile).
    ///
    /// @return <b>true</b> if the user initiated the profile writing.
    ///         The base class implementation of this method always
    ///         returns <b>false</b>.
    ///
    virtual bool    UserInitiated();
    
    /// Returns the profile format version number.
    ///
    /// @return Profile format version number.
    ///
    virtual int     GetVersion();
};

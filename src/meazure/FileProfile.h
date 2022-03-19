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

/// @file
/// @brief File-based profile header file.

#pragma once

#include "Profile.h"
#include "XMLParser.h"
#include <map>


/// Persists the application state to an XML file.
///
class MeaFileProfile : public MeaProfile, public MeaXMLParserHandler {
public:
    /// File access mode.
    enum Mode {
        ProfRead,       ///< Open profile file for reading.
        ProfWrite       ///< Open profile file for writing.
    };


    /// Creates an instance of a profile file object. The specified
    /// profile file is opened for either reading or writing.
    ///
    /// @param pathname     [in] Pathname for the profile file. If the
    ///                     file is opened for writing, it is created if
    ///                     it does not exist.
    /// @param mode         [in] Opening mode for the profile file (i.e. read or write).
    ///
    MeaFileProfile(LPCTSTR pathname, Mode mode);
    
    /// Closes the profile file and destroys the object instance.
    ///
    virtual ~MeaFileProfile();


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
    /// @return Always <b>true</b> because a file profile
    ///         is only written at the user's request.
    ///
    virtual bool    UserInitiated();
    
    /// Returns the profile format version number.
    ///
    /// @return Profile format version number.
    ///
    virtual int     GetVersion();


    /// Called when the XML parser encounters the start of an element.
    ///
    /// @param container    [in] Name of the element containing this element.
    /// @param elementName  [in] Name of the element.
    /// @param attrs        [in] Attributes for the element.
    ///
    virtual void    StartElementHandler(const CString& container,
                                        const CString& elementName,
                                        const MeaXMLAttributes& attrs);

    /// Called when the XML parser encounters character data.
    ///
    /// @param container    [in] Name of the element containing the character data.
    /// @param data         [in] Character data.
    ///
    virtual void    CharacterDataHandler(const CString& container,
                                         const CString& data);

    /// Called when the XML parser needs to parse an external entity.
    ///
    /// @param parser   [in] XML parser.
    /// @param pathname [in] Pathname of the external entity to open and parse.
    ///
    virtual void    ParseEntity(MeaXMLParser& parser,
                                const CString& pathname);

    /// Obtains the pathname of the profile file represented by an instance
    /// of this class.
    ///
    /// @return Pathname of the profile file.
    ///
    virtual CString GetFilePathname();

private:
    /// Printf style method that writes the specified values using the
    /// specified format and indentation level.
    ///
    /// @param indentLevel      [in] Indentation level for the output.
    /// @param format           [in] Printf-style format string for the output.
    /// @param ...              [in] Items to write to the file.
    ///
    void    Write(int indentLevel, LPCTSTR format, ...);

    /// Writes the XML boilerplate at the start of the XML profile file.
    ///
    void    WriteFileStart();
    
    /// Write the XML boilerplate at the end of the XML profile file.
    ///
    void    WriteFileEnd();

    /// Supervises the parsing of the XML profile file.
    ///
    void    ParseFile();

    CStdioFile  m_stdioFile;        ///< File object representing the profile.
    Mode        m_mode;             ///< Opening mode for the profile file.
    int         m_readVersion;      ///< Profile format version number read from the profile file.
    CString     m_title;            ///< Title for the profile file.

    std::map<CString, CString>  m_valueMap;     ///< Maps profile keys to values.
};

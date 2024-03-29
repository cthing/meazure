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
#include <meazure/xml/XMLParser.h>
#include <meazure/xml/XMLWriter.h>
#include <map>
#include <memory>
#include <fstream>


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
    MeaFileProfile(PCTSTR pathname, Mode mode);

    /// Closes the profile file and destroys the object instance.
    ///
    virtual ~MeaFileProfile();

    /// Writes a boolean value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] Boolean value for the key
    /// @return true if successful.
    ///
    virtual bool WriteBool(PCTSTR key, bool value) override;

    /// Writes an integer value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] Integer value for the key
    /// @return true if successful.
    ///
    virtual bool WriteInt(PCTSTR key, int value) override;

    /// Writes a double value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] Double value for the key
    /// @return true if successful.
    ///
    virtual bool WriteDbl(PCTSTR key, double value) override;

    /// Writes a string value to the specified key.
    ///
    /// @param key      [in] Profile key to write
    /// @param value    [in] String value for the key
    /// @return true if successful.
    ///
    virtual bool WriteStr(PCTSTR key, PCTSTR value) override;

    /// Reads a boolean value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    /// @return Boolean value for the key or the default value if the key is not found.
    /// 
    virtual bool ReadBool(PCTSTR key, bool defaultValue) override;

    /// Reads an unsigned integer value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    /// @return Integer value for the key or the default value if the key is not found.
    /// 
    virtual UINT ReadInt(PCTSTR key, int defaultValue) override;

    /// Reads a double value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    /// @return Double value for the key or the default value if the key is not found.
    /// 
    virtual double ReadDbl(PCTSTR key, double defaultValue) override;

    /// Reads a string value from the specified key.
    ///
    /// @param key              [in] Profile key to read
    /// @param defaultValue     [in] Default value to use if the key is not found in the profile.
    /// @return String value for the key or the default value if the key is not found.
    /// 
    virtual CString ReadStr(PCTSTR key, PCTSTR defaultValue) override;

    /// Indicates whether the profile is being written at
    /// the user's request (i.e. a file profile).
    ///
    /// @return Always <b>true</b> because a file profile
    ///         is only written at the user's request.
    ///
    virtual bool UserInitiated() override;

    /// Returns the profile format version number.
    ///
    /// @return Profile format version number.
    ///
    virtual int GetVersion() override;

    /// Called when the XML parser encounters the start of an element.
    ///
    /// @param container    [in] Name of the element containing this element.
    /// @param elementName  [in] Name of the element.
    /// @param attrs        [in] Attributes for the element.
    ///
    virtual void StartElement(const CString& container, const CString& elementName,
                              const MeaXMLAttributes& attrs) override;

    /// Called when the XML parser encounters character data.
    ///
    /// @param container    [in] Name of the element containing the character data.
    /// @param data         [in] Character data.
    ///
    virtual void CharacterData(const CString& container, const CString& data) override;

    /// Obtains the pathname of the profile file represented by an instance
    /// of this class.
    ///
    /// @return Pathname of the profile file.
    ///
    virtual CString GetFilePathname() override;

private:
    /// Writes the XML boilerplate at the start of the XML profile file.
    ///
    void WriteFileStart();

    /// Write the XML boilerplate at the end of the XML profile file.
    ///
    void WriteFileEnd();

    /// Supervises the parsing of the XML profile file.
    /// 
    /// @param pathname  [in] Profile file to parse
    ///
    void ParseFile(PCTSTR pathname);


    typedef std::unique_ptr<MeaXMLWriter> MeaXMLWriterPtr;


    CString m_pathname;             ///< Pathname of the file.
    std::ofstream m_writeStream;    ///< Output stream for the profile.
    MeaXMLWriterPtr m_writer;       ///< Writer for the profile.
    Mode m_mode;                    ///< Opening mode for the profile file.
    int m_readVersion;              ///< Profile format version number read from the profile file.
    CString m_title;                ///< Title for the profile file.
    std::map<CString, CString>  m_valueMap;     ///< Maps profile keys to values.
};

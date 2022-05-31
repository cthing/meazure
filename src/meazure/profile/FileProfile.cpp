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
#include "FileProfile.h"
#include <meazure/VersionInfo.h>
#include <meazure/utilities/TimeStamp.h>
#include <cassert>


MeaFileProfile::MeaFileProfile(LPCTSTR pathname, Mode mode) :
    MeaProfile(),
    MeaXMLParserHandler(),
    m_mode(mode),
    m_readVersion(1) {
    CFileException fe;

    UINT flags = (m_mode == ProfRead) ? CFile::modeRead : (CFile::modeCreate | CFile::modeWrite);

    if (m_stdioFile.Open(pathname, flags, &fe) == FALSE) {
        AfxThrowFileException(fe.m_cause, fe.m_lOsError, pathname);
    }

    m_title.Format(_T("%s Profile File"), static_cast<LPCTSTR>(AfxGetAppName()));

    if (m_mode == ProfWrite) {
        WriteFileStart();
    } else {
        ParseFile();
    }
}

MeaFileProfile::~MeaFileProfile() {
    try {
        if (m_mode == ProfWrite) {
            WriteFileEnd();
        }

        m_stdioFile.Close();
    } catch (...) {
        assert(false);
    }
}

bool MeaFileProfile::WriteBool(LPCTSTR key, bool value) {
    Write(2, _T("<%s value=\"%s\"/>\n"), key, (value ? _T("true") : _T("false")));
    return true;
}

bool MeaFileProfile::WriteInt(LPCTSTR key, int value) {
    Write(2, _T("<%s value=\"%d\"/>\n"), key, value);
    return true;
}

bool MeaFileProfile::WriteDbl(LPCTSTR key, double value) {
    Write(2, _T("<%s value=\"%f\"/>\n"), key, value);
    return true;
}

bool MeaFileProfile::WriteStr(LPCTSTR key, LPCTSTR value) {
    Write(2, _T("<%s value=\"%s\"/>\n"), key, value);
    return true;
}

void MeaFileProfile::Write(int indentLevel, LPCTSTR format, ...) {
    CString indent(_T(' '), indentLevel * 4);
    CString str;
    va_list args;

    va_start(args, format);
    str.FormatV(format, args);
    va_end(args);

    m_stdioFile.WriteString(indent + MeaXMLParser::ToUTF8(str));
}

bool MeaFileProfile::ReadBool(LPCTSTR key, bool defaultValue) {
    std::map<CString, CString>::const_iterator iter;

    iter = m_valueMap.find(key);
    if (iter != m_valueMap.end()) {
        CString val = (*iter).second;
        val.MakeLower();
        return ((val == _T("true")) || (val == _T("1")) || (val == _T("yes")));
    }
    return defaultValue;
}

UINT MeaFileProfile::ReadInt(LPCTSTR key, int defaultValue) {
    std::map<CString, CString>::const_iterator iter;

    iter = m_valueMap.find(key);
    if (iter != m_valueMap.end()) {
        return _ttoi((*iter).second);
    }
    return defaultValue;
}

double MeaFileProfile::ReadDbl(LPCTSTR key, double defaultValue) {
    std::map<CString, CString>::const_iterator iter;

    iter = m_valueMap.find(key);
    if (iter != m_valueMap.end()) {
        return _tcstod((*iter).second, nullptr);
    }
    return defaultValue;
}

CString MeaFileProfile::ReadStr(LPCTSTR key, LPCTSTR defaultValue) {
    std::map<CString, CString>::const_iterator iter;

    iter = m_valueMap.find(key);
    if (iter != m_valueMap.end()) {
        return (*iter).second;
    }
    return defaultValue;
}

bool MeaFileProfile::UserInitiated() {
    return true;
}

int MeaFileProfile::GetVersion() {
    return m_readVersion;
}

void MeaFileProfile::WriteFileStart() {
    Write(0, _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"));
    Write(0, _T("<profile version=\"%d\">\n"), MeaVersionInfo::Instance().GetProfileFileMajor());

    TCHAR nameBuffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(nameBuffer, &size);

    Write(1, _T("<info>\n"));
    Write(2, _T("<title>%s</title>\n"), static_cast<LPCTSTR>(MeaXMLParser::Encode(m_title)));
    Write(2, _T("<created date=\"%s\"/>\n"), static_cast<LPCTSTR>(MeaTimeStamp::Make(time(nullptr))));
    Write(2, _T("<generator name=\"%s\" version=\"%s\" build=\"%d\"/>\n"),
          static_cast<LPCTSTR>(AfxGetAppName()),
          static_cast<LPCTSTR>(MeaVersionInfo::Instance().GetProductVersion()),
          MeaVersionInfo::Instance().GetProductBuild());
    Write(2, _T("<machine name=\"%s\"/>\n"), nameBuffer);
    Write(1, _T("</info>\n"));
    Write(1, _T("<data>\n"));
}

void MeaFileProfile::WriteFileEnd() {
    Write(1, _T("</data>\n"));
    Write(0, _T("</profile>\n"));
}

void MeaFileProfile::ParseFile() {
    MeaXMLParser parser(this);

    // Set the base path on the parser.
    //
    TCHAR drive[_MAX_PATH], path[_MAX_DIR];

    _tsplitpath_s(m_stdioFile.GetFilePath(), drive, _MAX_PATH, path, _MAX_DIR, nullptr, 0, nullptr, 0);
    _tcscat_s(drive, _MAX_PATH, path);
    parser.SetBasePath(drive);

    // Read the contents of the file into a parsing buffer.
    //
    int size = static_cast<int>(m_stdioFile.GetLength());
    void* buf = parser.GetBuffer(size);
    UINT count = m_stdioFile.Read(buf, size);

    // Parse the file
    //
    parser.ParseBuffer(count, true);
}

void MeaFileProfile::StartElementHandler(const CString& container,
                                         const CString& elementName,
                                         const MeaXMLAttributes& attrs) {
    bool isDefault;

    if (elementName == _T("profile")) {
        int value;
        attrs.GetValueInt(_T("version"), value, isDefault);
        m_readVersion = value;
    } else if ((container == _T("data")) || (m_readVersion == 1)) {
        CString value;
        attrs.GetValueStr(_T("value"), value, isDefault);
        m_valueMap[elementName] = value;
    }
}

void MeaFileProfile::CharacterDataHandler(const CString& container,
                                          const CString& data) {
    if (container == _T("title")) {
        m_title += data;
    }
}

void MeaFileProfile::ParseEntity(MeaXMLParser& parser,
                                 const CString& pathname) {
    CFile entityFile;
    CFileException fe;

    if (!entityFile.Open(pathname, CFile::modeRead, &fe)) {
        AfxThrowFileException(fe.m_cause, fe.m_lOsError, pathname);
    }

    MeaXMLParser entityParser(parser);

    // Read the contents of the entity file into a parsing buffer.
    //
    int size = static_cast<int>(entityFile.GetLength());
    void* buf = entityParser.GetBuffer(size);
    UINT count = entityFile.Read(buf, size);

    // Parse the entity file
    //
    entityParser.ParseBuffer(count, true);

    entityFile.Close();
}

CString MeaFileProfile::GetFilePathname() {
    return m_stdioFile.GetFilePath();
}

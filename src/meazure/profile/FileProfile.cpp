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
#include <meazure/utilities/StringUtils.h>
#include <meazure/xml/XMLWriter.h>
#include <cassert>
#include <cstring>


MeaFileProfile::MeaFileProfile(PCTSTR pathname, Mode mode) :
    m_pathname(pathname),
    m_mode(mode),
    m_readVersion(1) {

    m_title.Format(_T("%s Profile File"), static_cast<PCTSTR>(AfxGetAppName()));

    if (m_mode == ProfWrite) {
        m_writeStream.exceptions(std::ios::failbit | std::ios::badbit);
        m_writeStream.open(MeaStringUtils::ACPtoUTF8(m_pathname), std::ios::out | std::ios::trunc);
        m_writer = std::make_unique<MeaXMLWriter>(m_writeStream);

        WriteFileStart();
    } else {
        ParseFile(pathname);
    }
}

MeaFileProfile::~MeaFileProfile() {
    if (m_mode == ProfWrite) {
        try {
            WriteFileEnd();
        } catch (...) {
            assert(false);
        }
    }
}

bool MeaFileProfile::WriteBool(PCTSTR key, bool value) {
    m_writer->StartElement(key)
        .AddAttribute(_T("value"), (value ? _T("true") : _T("false")))
        .EndElement();
    return true;
}

bool MeaFileProfile::WriteInt(PCTSTR key, int value) {
    m_writer->StartElement(key)
        .AddAttribute(_T("value"), value)
        .EndElement();
    return true;
}

bool MeaFileProfile::WriteDbl(PCTSTR key, double value) {
    m_writer->StartElement(key)
        .AddAttribute(_T("value"), value)
        .EndElement();
    return true;
}

bool MeaFileProfile::WriteStr(PCTSTR key, PCTSTR value) {
    m_writer->StartElement(key)
        .AddAttribute(_T("value"), value)
        .EndElement();
    return true;
}

bool MeaFileProfile::ReadBool(PCTSTR key, bool defaultValue) {
    std::map<CString, CString>::const_iterator iter;

    iter = m_valueMap.find(key);
    if (iter != m_valueMap.end()) {
        CString val = (*iter).second;
        val.MakeLower();
        return ((val == _T("true")) || (val == _T("1")) || (val == _T("yes")));
    }
    return defaultValue;
}

UINT MeaFileProfile::ReadInt(PCTSTR key, int defaultValue) {
    std::map<CString, CString>::const_iterator iter;

    iter = m_valueMap.find(key);
    if (iter != m_valueMap.end()) {
        return _ttoi((*iter).second);
    }
    return defaultValue;
}

double MeaFileProfile::ReadDbl(PCTSTR key, double defaultValue) {
    std::map<CString, CString>::const_iterator iter;

    iter = m_valueMap.find(key);
    if (iter != m_valueMap.end()) {
        return _tcstod((*iter).second, nullptr);
    }
    return defaultValue;
}

CString MeaFileProfile::ReadStr(PCTSTR key, PCTSTR defaultValue) {
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
    m_writer->StartDocument();

    m_writer->StartElement(_T("profile"))
        .AddAttribute(_T("version"), MeaVersionInfo::Instance().GetProfileFileMajor());

    m_writer->StartElement(_T("info"));

    m_writer->StartElement(_T("title"))
        .Characters(m_title)
        .EndElement();
    
    m_writer->StartElement(_T("created"))
        .AddAttribute(_T("date"), MeaTimeStamp::Make(time(nullptr)))
        .EndElement();

    m_writer->StartElement(_T("generator"))
        .AddAttribute(_T("name"), AfxGetAppName())
        .AddAttribute(_T("version"), MeaVersionInfo::Instance().GetProductVersion())
        .AddAttribute(_T("build"), MeaVersionInfo::Instance().GetProductBuild())
        .EndElement();

    TCHAR nameBuffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(nameBuffer, &size);

    m_writer->StartElement(_T("machine"))
        .AddAttribute(_T("name"), nameBuffer)
        .EndElement();

    m_writer->EndElement();         // info

    m_writer->StartElement(_T("data"));
}

void MeaFileProfile::WriteFileEnd() {
    m_writer->EndElement();         // data
    m_writer->EndElement();         // profile
    m_writer->EndDocument();
}

void MeaFileProfile::ParseFile(PCTSTR pathname) {
    MeaXMLParser parser(this);
    parser.ParseFile(pathname);
}

void MeaFileProfile::StartElement(const CString& container, const CString& elementName,
                                  const MeaXMLAttributes& attrs) {
    if (elementName == _T("profile")) {
        int value;
        attrs.GetValueInt(_T("version"), value);
        m_readVersion = value;
    } else if ((container == _T("data")) || (m_readVersion == 1)) {
        CString value;
        attrs.GetValueStr(_T("value"), value);
        m_valueMap[elementName] = value;
    }
}

void MeaFileProfile::CharacterData(const CString& container,
                                          const CString& data) {
    if (container == _T("title")) {
        m_title += data;
    }
}

CString MeaFileProfile::GetFilePathname() {
    return m_pathname;
}

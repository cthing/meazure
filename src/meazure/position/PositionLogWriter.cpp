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
#include <meazure/resource.h>
#include <meazure/VersionInfo.h>
#include <meazure/utilities/TimeStamp.h>
#include "PositionLogWriter.h"


void MeaPositionLogWriter::Save() {
    m_writer.StartDocument();

    m_writer.Doctype(_T("positionLog"), m_provider.GetCurrentDtdUrl());

    m_writer.StartElement(_T("positionLog"))
        .AddAttribute(_T("version"), MeaVersionInfo::Instance().GetLogFileMajor());

    WriteInfoSection();
    WriteDesktopsSection();
    WritePositionsSection();

    m_writer.EndElement();        // positionLog
    m_writer.EndDocument();
}

void MeaPositionLogWriter::WriteInfoSection() {
    TCHAR nameBuffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    GetComputerName(nameBuffer, &size);

    m_writer.StartElement(_T("info"));

    m_writer.StartElement(_T("title"))
        .Characters(m_provider.GetTitle())
        .EndElement();
    m_writer.StartElement(_T("created"))
        .AddAttribute(_T("date"), MeaTimeStamp::Make(time(nullptr)))
        .EndElement();
    m_writer.StartElement(_T("generator"))
        .AddAttribute(_T("name"), AfxGetAppName())
        .AddAttribute(_T("version"), MeaVersionInfo::Instance().GetProductVersion())
        .AddAttribute(_T("build"), MeaVersionInfo::Instance().GetProductBuild())
        .EndElement();
    m_writer.StartElement(_T("machine"))
        .AddAttribute(_T("name"), nameBuffer)
        .EndElement();
    if (!m_provider.GetDescription().IsEmpty()) {
        m_writer.StartElement(_T("desc"))
            .Characters(m_provider.GetDescription())
            .EndElement();
    }

    m_writer.EndElement();    // info
}

void MeaPositionLogWriter::WriteDesktopsSection() {
    m_writer.StartElement(_T("desktops"));
    MeaPositionProvider::PositionDesktops desktops = m_provider.GetReferencedDesktops();
    for (const MeaPositionDesktop& desktop : desktops) {
        desktop.Save(m_writer);
    }
    m_writer.EndElement();    // desktops
}

void MeaPositionLogWriter::WritePositionsSection() {
    m_writer.StartElement(_T("positions"));
    m_provider.GetPositions().Save(m_writer);
    m_writer.EndElement();    // positions
}

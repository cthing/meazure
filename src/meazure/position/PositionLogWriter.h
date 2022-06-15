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
 /// @brief Responsible for writing the position log file.

#pragma once

#include <meazure/xml/XMLWriter.h>
#include "PositionProvider.h"


/// Responsible to writing the position log file.
///
class MeaPositionLogWriter {

public:
    MeaPositionLogWriter(MeaXMLWriter& writer, const MeaPositionProvider& provider) :
        m_writer(writer), m_provider(provider) {}

    /// Performs the writing of the position log file.
    ///
    void Save();

private:
    /// Writes the general information section of the position log file.
    /// 
    void WriteInfoSection();

    /// Writes the desktop information section of the position log file.
    /// 
    void WriteDesktopsSection();

    /// Writes the positions section of the position log file.
    /// 
    void WritePositionsSection();


    MeaXMLWriter& m_writer;
    const MeaPositionProvider& m_provider;
};

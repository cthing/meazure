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
/// @brief Interface for classes that provide the ability to write to the position log file.

#pragma once


///  Interface for classes that provide the ability to write to the position log file.
///
struct MeaPositionLogWriter {
    /// Printf-like method for writing to a position log file.
    ///
    /// @param indentLevel      [in] Output indentation level.
    /// @param format           [in] Printf-style format string.
    /// @param ...              [in] Items to write to the file.
    /// @throws CFileException if there was a problem writing the information
    ///
    virtual void Write(int indentLevel, LPCTSTR format, ...) = 0;
};

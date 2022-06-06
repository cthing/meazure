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
/// @brief Header file for writing XML files.

#pragma once


namespace MeaXMLWriter {

    /// Replaces all occurrences of '&', '<', '>', '"', and "'" with the corresponding character entities. Attribute
    /// values and character data (i.e. text between start/end elements) must be encoded. This method should be called
    /// <b>before</b> converting the string to UTF-8.
    ///
    /// @param str      [in] String to encode.
    /// @return XML encoded string.
    ///
    CString Encode(const CString& str);
}

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
 /// @brief String manipulation utility functions.

#pragma once


namespace MeaStringUtils {

    /// Converts the specified double to a string with the
    /// minimum number of decimal places.
    ///
    /// @param value    [in] Numerical value to convert to a string.
    ///
    /// @return String corresponding to the double value.
    ///
    CString DblToStr(double value);

    /// Tests whether the specified string is a number. For the
    /// purposes of this method, a number is a base 10 double
    /// precision floating point value.
    ///
    /// @param str      [in] String to test
    /// @param valuep   [out] Value parsed from the string. nullptr
    ///                 if not interested. If the return value is
    ///                 <b>false</b>, valuep's value is undefined.
    ///
    /// @return <b>true</b> if string represents a number.
    ///
    bool IsNumber(LPCTSTR str, double* valuep = nullptr);

    /// Tests whether the specified string is a boolean value. For
    /// the purpose of this method, the strings "1", "TRUE", "true"
    /// are boolean <b>true</b> values, while "0", "FALSE", "false"
    /// are boolean <b>false</b>.
    ///
    /// @param str      [in] String to test
    /// @param valuep   [out] Value parsed from the string. nullptr
    ///                 if not interested. If the return value is
    ///                 <b>false</b>, valuep's value is undefined.
    ///
    /// @return <b>true</b> if string represents a boolean value.
    ///
    bool IsBoolean(LPCTSTR str, bool* valuep = nullptr);

    /// Converts standalone LF characters into a CR+LF pair.
    /// Existing CR+LF pairs in the string are not effected.
    ///
    /// @param str      [in] String to process.
    ///
    /// @return Converted string.
    ///
    CString LFtoCRLF(CString str);

    /// Converts CR+LF pairs to a single LF character.
    ///
    /// @param str      [in] String to process.
    ///
    /// @return Converted string.
    ///
    CString CRLFtoLF(CString str);
};

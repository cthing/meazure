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
 /// @brief Header file for a class representing a globally unique identifier.

#pragma once

#include <iostream>


/// Represents a globally unique identifier (GUID) and common operations
/// on a GUID.
///
class MeaGUID {

public:
    /// Constructs a GUID with either the specified string representation of a GUID or a newly generated GUID
    /// using CoCreateGuid if no string is specified.
    /// 
    /// @param guidStr  [in] String representing a GUID (e.g. "00000000-0000-0000-0000-000000000000").
    ///     If <b>nullptr</b> is specified, a GUID is generated internally.
    ///
    explicit MeaGUID(PCTSTR guidStr = nullptr);

    /// Constructs a GUID based on the specified GUID structure.
    ///
    /// @param guid     [in] Operating system defined GUID structure.
    ///
    explicit MeaGUID(const GUID& guid) { Assign(guid); }

    /// Copy constructor.
    ///
    /// @param guid     [in] MeaGUID object to be copied.
    ///
    MeaGUID(const MeaGUID& guid) { Assign(guid); }

    /// Destroys a GUID object.
    ///
    ~MeaGUID() {}

    /// Assignment operator.
    /// @param guid     [in] Operating system defined GUID structure.
    /// @return This object.
    MeaGUID& operator=(const GUID& guid) { return Assign(guid); }

    /// Assignment operator.
    /// @param guidStr  [in] String representing a GUID.
    /// @return This object.
    MeaGUID& operator=(const PCTSTR guidStr) { return Assign(guidStr); }

    /// Assignment operator.
    /// @param guid     [in] MeaGUID object.
    /// @return This object.
    MeaGUID& operator=(const MeaGUID& guid) { return Assign(guid); }

    /// Tests for equality between two MeaGUID objects.
    /// @param guid     [in] MeaGUID object to test against this object.
    /// @return <b>true</b> if the specified object equals this object.
    bool operator==(const MeaGUID& guid) const { return this->m_guid == guid.m_guid; }

    /// Tests for inequality between two MeaGUID objects.
    /// @param guid     [in] MeaGUID object to test against this object.
    /// @return <b>true</b> if the specified object does not equal this object.
    bool operator!=(const MeaGUID& guid) const { return !(*this == guid); }

    /// Tests for equality between two this object and a GUID structure.
    /// @param guid     [in] Operating system defined GUID structure to test against this object.
    /// @return <b>true</b> if the specified GUID equals this object.
    bool operator==(const GUID& guid) const {
        return ((m_guid.Data1 == guid.Data1) && 
                (m_guid.Data2 == guid.Data2) &&
                (m_guid.Data3 == guid.Data3) &&
                (m_guid.Data4[0] == guid.Data4[0]) &&
                (m_guid.Data4[1] == guid.Data4[1]) &&
                (m_guid.Data4[2] == guid.Data4[2]) &&
                (m_guid.Data4[3] == guid.Data4[3]) &&
                (m_guid.Data4[4] == guid.Data4[4]) &&
                (m_guid.Data4[5] == guid.Data4[5]) &&
                (m_guid.Data4[6] == guid.Data4[6]) &&
                (m_guid.Data4[7] == guid.Data4[7]));
    }

    /// Tests for inequality between two this object and a GUID structure.
    /// @param guid     [in] Operating system defined GUID structure to test against this object.
    /// @return <b>true</b> if the specified GUID does not equal this object.
    bool operator!=(const GUID& guid) const { return !(*this == guid); }

    /// Casts to an operating system defined GUID structure.
    /// @return Operating system defined GUID structure.
    operator GUID() const { return m_guid; }


    /// Used by the STL to perform ordering of MeaGUID objects in collections.
    ///
    struct less {
        /// Compares two MeaGUID objects.
        ///
        /// @param lhs      [in] Left hand side of the comparison.
        /// @param rhs      [in] Right hand side of the comparison.
        ///
        /// @return <b>true</b> if lhs < rhs lexically.
        ///
        bool operator()(const MeaGUID& lhs, const MeaGUID& rhs) const {
            return lhs.ToString() < rhs.ToString();
        }
    };


    /// Assign the specified GUID structure to this.
    /// @param guid     [in] Operating system defined GUID structure.
    /// @return This object.
    MeaGUID& Assign(const GUID& guid) {
        m_guid = guid;
        return *this;
    }

    /// Assign the specified GUID string to this.
    /// @param guidStr  [in] String representing a GUID.
    /// @return This object.
    MeaGUID& Assign(PCTSTR guidStr);

    /// Assign the specified MeaGUID object to this.
    /// @param guid     [in] MeaGUID object to assign.
    /// @return This object.
    MeaGUID& Assign(const MeaGUID& guid) {
        if (this != &guid) {
            Assign(guid.m_guid);
        }
        return *this;
    }

    /// Returns a string representation of the GUID. Note that the
    /// string returned is not preserved across calls to this method.
    /// Copy the returned string to preserve it.
    ///
    /// @return String representation of the GUID.
    ///
    CString ToString() const;

private:
    static constexpr PCTSTR kGuidFormat = _T("%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX");

    GUID m_guid;                ///< Underlying GUID for the object.
};


/// Writes the GUID to the specified output stream. The output format is the same as ToString().
/// 
/// @param os    [in] Output stream to which the GUID should be written
/// @param guid  [in] The GUID to be written 
/// @return The specified output stream
/// 
std::ostream& operator<<(std::ostream& os, const MeaGUID& guid);

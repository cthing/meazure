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


/// Represents a globally unique identifier (GUID) and common operations
/// on a GUID.
///
class MeaGUID
{
public:
    /// Constructs a GUID with a newly generated GUID using CoCreateGuid.
    ///
    MeaGUID();

    /// Constructs a GUID based on the specified GUID structure.
    ///
    /// @param guid     [in] Operating system defined GUID structure.
    ///
    explicit MeaGUID(const GUID& guid) { Assign(guid); }
    
    /// Creates a GUID object initialized with by the string representing
    /// a GUID (e.g. "00000000-0000-0000-0000-000000000000")
    ///
    /// @param guidStr  [in] String representing a GUID.
    ///
    explicit MeaGUID(LPCTSTR guidStr) { Assign(guidStr); }

    /// Copy constructor.
    ///
    /// @param guid     [in] MeaGUID object to be copied.
    ///
    MeaGUID(const MeaGUID& guid) { Assign(guid); }

    /// Destroys a GUID object.
    ///
    ~MeaGUID() { }


    /// Assignment operator.
    /// @param guid     [in] Operating system defined GUID structure.
    /// @return This object.
    MeaGUID& operator=(const GUID& guid) { return Assign(guid); }

    /// Assignment operator.
    /// @param guidStr  [in] String representing a GUID.
    /// @return This object.
    MeaGUID& operator=(const LPCTSTR guidStr) { return Assign(guidStr); }

    /// Assignment operator.
    /// @param guid     [in] MeaGUID object.
    /// @return This object.
    MeaGUID& operator=(const MeaGUID& guid) { return Assign(guid); }


    /// Tests for equality between two MeaGUID objects.
    /// @param guid     [in] MeaGUID object to test against this object.
    /// @return <b>true</b> if the specified object equals this object.
    bool operator==(const MeaGUID& guid) const { return IsEqual(guid); }

    /// Tests for inequality between two MeaGUID objects.
    /// @param guid     [in] MeaGUID object to test against this object.
    /// @return <b>true</b> if the specified object does not equal this object.
    bool operator!=(const MeaGUID& guid) const { return !IsEqual(guid); }


    /// Tests for equality between two this object and a GUID structure.
    /// @param guid     [in] Operating system defined GUID structure to test against this object.
    /// @return <b>true</b> if the specified GUID equals this object.
    bool operator==(const GUID& guid) const { return IsEqual(guid); }

    /// Tests for inequality between two this object and a GUID structure.
    /// @param guid     [in] Operating system defined GUID structure to test against this object.
    /// @return <b>true</b> if the specified GUID does not equal this object.
    bool operator!=(const GUID& guid) const { return !IsEqual(guid); }


    /// Casts to an operating system defined GUID structure.
    /// @return Operating system defined GUID structure.
    operator GUID() const { return m_guid; }

    /// Casts to a string.
    /// @return String representation of the GUID.
    operator LPCTSTR() const { return static_cast<LPCTSTR>(ToString()); }


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
        bool operator()(const MeaGUID& lhs, const MeaGUID& rhs) const
        {
            return lhs.ToString() < rhs.ToString();
        }
    };


    /// Assign the specified GUID structure to this.
    /// @param guid     [in] Operating system defined GUID structure.
    /// @return This object.
    MeaGUID&    Assign(const GUID& guid) {
        m_guid = guid;
        return *this;
    }

    /// Assign the specified GUID string to this.
    /// @param guidStr  [in] String representing a GUID.
    /// @return This object.
    MeaGUID&    Assign(LPCTSTR guidStr);

    /// Assign the specified MeaGUID object to this.
    /// @param guid     [in] MeaGUID object to assign.
    /// @return This object.
    MeaGUID&    Assign(const MeaGUID& guid) {
        if (this != &guid) {
            Assign(guid.m_guid);
        }
        return *this;
    }


    /// Tests the specified MeaGUID object for equality with this.
    /// @param guid     [in] MeaGUID object to test.
    /// @return <b>true</b> if the specified GUID object equals this object.
    bool    IsEqual(const MeaGUID& guid) const { return IsEqual(guid.m_guid); }

    /// Tests the specified GUID structure for equality with this.
    /// @param guid     [in] Operating system defined GUID structure.
    /// @return <b>true</b> if the specified GUID structure equals this object.
    bool    IsEqual(const GUID& guid) const {
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


    /// Returns a string representation of the GUID. Note that the
    /// string returned is not preserved across calls to this method.
    /// Copy the returned string to preserve it.
    ///
    /// @return String representation of the GUID.
    ///
    CString ToString() const;

private:
    GUID    m_guid;             ///< Underlying GUID for the object.

    mutable CString m_buffer;   ///< Buffer holding the string representation of the GUID.
};

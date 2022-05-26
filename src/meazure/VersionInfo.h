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
/// @brief Header file for application version number information.

#pragma once

#include "utilities/Singleton.h"


/// Convenience class for obtaining version number information for the
/// applications.
///
class MeaVersionInfo : public MeaSingleton_T<MeaVersionInfo> {

public:
    /// Constructs an instance of the version number information class.
    ///
    MeaVersionInfo(token);

    /// Destroys an instance of the class.
    ///
    virtual ~MeaVersionInfo();

    /// Retrieves the product version number.
    /// @return Product version number expressed as string of the form
    ///         major.minor or major.minor.fix (if fix > 0).
    CString GetProductVersion() const;

    /// Returns the product major version number.
    /// @return Major version number.
    int GetProductMajor() const { return HIWORD(m_info->dwProductVersionMS); }

    /// Returns the product minor version number.
    /// @return Minor version number.
    int GetProductMinor() const { return LOWORD(m_info->dwProductVersionMS); }

    /// Returns the product fix number.
    /// @return Fix number.
    int GetProductFix() const { return HIWORD(m_info->dwProductVersionLS); }

    /// Returns the product build number.
    /// @return Build number.
    int GetProductBuild() const { return LOWORD(m_info->dwProductVersionLS); }


    /// Returns the version number of the profile file format.
    /// @return Version number of the profile file format.
    int GetProfileFileMajor() const { return 2; }

    /// Returns the version number of the position log file format.
    /// @return Version number of the position log file format.
    int GetLogFileMajor() const { return 1; }

private:
    /// The class does not have copy semantics so this method is
    /// purposely undefined.
    MeaVersionInfo(const MeaVersionInfo&);

    /// The class does not have assignment semantics so this method is
    /// purposely undefined.
    MeaVersionInfo& operator=(const MeaVersionInfo&);

    void* m_data;               ///< Version information buffer.
    VS_FIXEDFILEINFO* m_info;   ///< Version information.
};

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
/// @brief Header file for the command-line processing class.

#pragma once


/// Handles command-line parameters specific to Meazure. Parameters are:
///
/// nl - Disable layered windows
///
class MeaCommandLineInfo : public CCommandLineInfo
{
public:
    /// Constructs the command-line parser.
    ///
    MeaCommandLineInfo();
    
    /// Destroys the command-line parser.
    ///
    virtual ~MeaCommandLineInfo();

    /// Performs the actual work of parsing the command-line
    /// parameters.
    ///
    /// @param param    [in] Command-line switch or argument.
    /// @param flag     [in] TRUE if param is a switch, FALSE if it is a parameter.
    /// @param last     [in] TRUE if last item on the command line.
    ///
    virtual void ParseParam(LPCTSTR param, BOOL flag, BOOL last) override;
};

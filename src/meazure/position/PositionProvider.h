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
 /// @brief Provides position information for writing a position log file.

#pragma once

#include "PositionCollection.h"
#include "PositionDesktop.h"
#include <list>
#include <functional>


/// Interface for providing position information.
///
struct MeaPositionProvider {
    typedef std::list<std::reference_wrapper<const MeaPositionDesktop>> PositionDesktops;

    /// Returns the title for the position log.
    /// 
    /// @return Title for the position log.
    ///  
    virtual const CString& GetTitle() const = 0;

    /// Returns the description for the position log.
    /// 
    /// @return Description for the position log.
    ///  
    virtual const CString& GetDescription() const = 0;

    /// Returns the URL for the current position log file DTD.
    /// 
    /// @return Position log DTD URL.
    /// 
    virtual PCTSTR GetCurrentDtdUrl() const = 0;

    /// Returns a list of desktop information objects that are currently referenced by recorded positions.
    /// 
    /// @return List of desktop information objects.
    virtual PositionDesktops GetReferencedDesktops() const = 0;

    /// Returns the recorded positions.
    /// 
    /// @return Recorded positions.
    ///  
    virtual const MeaPositionCollection& GetPositions() const = 0;
};

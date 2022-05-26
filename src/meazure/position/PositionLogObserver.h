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
/// @brief Header file declaring an interface for observers of the position log.

#pragma once


/// Interface for objects that need to be informed about changes to the
/// position log. Classes that wish to be informed about changes to the
/// position log file inherit from this interface as a mix-in. The class
/// then implements the methods declared by this interface.
///
class MeaPositionLogObserver {

public:
    /// Called when a position log file is loaded.
    ///
    virtual void LogLoaded() = 0;

    /// Called when a position log file is saved.
    ///
    virtual void LogSaved() = 0;

    /// Called when a new position is recorded.
    /// @param posIndex     [in] Index of the new position.
    virtual void PositionAdded(int posIndex) = 0;

    /// Called when an existing position is replaced with a new position.
    /// @param posIndex     [in] Index of the replaced position.
    virtual void PositionReplaced(int posIndex) = 0;

    /// Called when a position is deleted.
    /// @param posIndex     [in] Index of the deleted position.
    virtual void PositionDeleted(int posIndex) = 0;

    /// Called when all recorded positions have been deleted.
    ///
    virtual void PositionsDeleted() = 0;
};

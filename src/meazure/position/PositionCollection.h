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
/// @brief Represents a collection of logged positions.

#pragma once

#include "Position.h"
#include "PositionLogWriter.h"
#include <map>


/// Represents a collection of positions. A position log consists
/// of a collection of positions. In turn, a position consists of
/// on or more points depending on the measurement tool.
///
class MeaPositionCollection {

public:
    /// Destroys a position collection object and the positions objects it contains.
    ///
    ~MeaPositionCollection();

    /// Indicates if there are any positions stored in the object.
    ///
    /// @return <b>true</b> if there are positions.
    ///
    bool Empty() const { return m_posMap.empty(); }

    /// Returns the number of positions stored in the object.
    ///
    /// @return Number of positions.
    ///
    unsigned int Size() const { return static_cast<unsigned int>(m_posMap.size()); }

    /// Adds the specified position to the collection of positions.
    ///
    /// @param position     [in] Position to add to the collection.
    ///
    void Add(MeaPosition* position);

    /// Replaces the specified position at the specified location in the collection.
    ///
    /// @param posIndex     [in] Zero based index indicating where in
    ///                     the collection to place the position.
    /// @param position     [in] Position object to place in the collection.
    /// @throws std::out_of_range if the specified position is out of bounds
    ///
    void Set(int posIndex, MeaPosition* position);

    /// Returns the position object at the specified location in the collection.
    ///
    /// @param posIndex     [in] Zero based index into the collection.
    ///
    /// @return Position object located at the specified location in the
    ///         collection.
    /// @throws std::out_of_range if the specified position is out of bounds
    ///
    MeaPosition& Get(int posIndex);

    /// Removes the position object from the specified location in the
    /// collection and destroys the object.
    ///
    /// @param posIndex     [in] Zero based index indicating where in
    ///                     the collection to delete a position.
    /// @throws std::out_of_range if the specified position is out of bounds
    ///
    void Delete(int posIndex);

    /// Removes all positions from the collection and destroys the
    /// position objects.
    ///
    void DeleteAll();

    /// Saves all positions in the collection to the log file.
    ///
    /// @param writer       [in] Provides ability to write a position to the log.
    /// @param indent       [in] Output indentation level.
    /// @throws CFileException if there was a problem saving the position
    ///
    void Save(MeaPositionLogWriter& writer, int indent) const;

private:
    typedef std::map<int, MeaPosition*> PositionMap;       ///< Maps indices to position objects.

    PositionMap m_posMap;       ///< Collection of positions.
};

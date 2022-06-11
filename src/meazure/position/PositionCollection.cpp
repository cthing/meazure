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

#include <meazure/pch.h>
#include "PositionCollection.h"


MeaPositionCollection::~MeaPositionCollection() {
    try {
        DeleteAll();
    } catch (...) {
        assert(false);
    }
}

void MeaPositionCollection::Add(MeaPosition* position) {
    int posIndex = Size();
    m_posMap[posIndex] = position;
}

void MeaPositionCollection::Set(int posIndex, MeaPosition* position) {
    PositionMap::iterator iter = m_posMap.find(posIndex);
    if (iter == m_posMap.end()) {
        throw std::out_of_range("Positions::Set posIndex out of range");
    }

    delete (*iter).second;
    (*iter).second = position;
}

MeaPosition& MeaPositionCollection::Get(int posIndex) {
    PositionMap::const_iterator iter = m_posMap.find(posIndex);
    if (iter == m_posMap.end()) {
        throw std::out_of_range("Positions::Get posIndex out of range");
    }

    return *(*iter).second;
}

void MeaPositionCollection::Delete(int posIndex) {
    // Find the position entry corresponding to the specified
    // position index.
    //
    PositionMap::iterator iter = m_posMap.find(posIndex);
    if (iter == m_posMap.end()) {
        throw std::out_of_range("Positions::Delete posIndex out of range");
    }

    // Delete the position object associated with the index.
    //
    delete (*iter).second;

    // Move each position object "down" one index.
    //
    for (iter++; iter != m_posMap.end(); ++iter, posIndex++) {
        m_posMap[posIndex] = (*iter).second;
    }

    // Delete the last entry in the map since it has been
    // moved "down" one index.
    //
    m_posMap.erase(static_cast<int>(m_posMap.size()) - 1);
}

void MeaPositionCollection::DeleteAll() {
    for (const auto& posEntry : m_posMap) {
        delete posEntry.second;
    }
    m_posMap.clear();
}

void MeaPositionCollection::Save(MeaXMLWriter& writer) const {
    for (const auto& posEntry : m_posMap) {
        posEntry.second->Save(writer);
    }
}

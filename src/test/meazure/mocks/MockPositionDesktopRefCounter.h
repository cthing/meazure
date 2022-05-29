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

#pragma once

#include <meazure/position/PositionDesktop.h>
#include <meazure/utilities/GUID.h>
#include <boost/test/unit_test.hpp>
#include <map>

class MockPositionDesktopRefCounter : public MeaPositionDesktopRefCounter {
public:
    std::map<MeaGUID, int, MeaGUID::less> m_refCounts;

    MockPositionDesktopRefCounter() {}

    void AddDesktopRef(const MeaGUID& id) override {
        auto iter = m_refCounts.find(id);
        if (iter == m_refCounts.end()) {
            m_refCounts[id] = 1;
        } else {
            (*iter).second++;
        }
    }

    void ReleaseDesktopRef(const MeaGUID& id) override {
        auto iter = m_refCounts.find(id);
        if (iter == m_refCounts.end()) {
            BOOST_FAIL("Could not find position desktop ID to release");
        } else {
            (*iter).second--;
        }
    }
};

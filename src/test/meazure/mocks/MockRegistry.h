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

#pragma once

#include <meazure/utilities/RegistryProvider.h>
#include <boost/test/unit_test.hpp>
#include <functional>


class MockRegistry : public MeaRegistryProvider {

public:
    MockRegistry() {
        reset();
    }

    BOOL WriteInt(PCTSTR section, PCTSTR entry, int value) override {
        m_writeIntCount++;
        return m_writeInt(section, entry, value);
    }

    BOOL WriteString(PCTSTR section, PCTSTR entry, PCTSTR value) override {
        m_writeStringCount++;
        return m_writeString(section, entry, value);
    }

    UINT GetInt(PCTSTR section, PCTSTR entry, int defaultValue) override {
        m_getIntCount++;
        return m_getInt(section, entry, defaultValue);
    }

    CString GetString(PCTSTR section, PCTSTR entry, PCTSTR defaultValue) override {
        m_getStringCount++;
        return m_getString(section, entry, defaultValue);
    }

    PCTSTR GetKeyName() override {
        return _T("cthing");
    }

    LSTATUS OpenKey(HKEY key, PCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) override {
        m_openKeyCount++;
        return m_openKey(key, subKey, options, samDesired, result);
    }

    LSTATUS CloseKey(HKEY key) override {
        m_closeKeyCount++;
        return m_closeKey(key);
    }

    void reset() {
        m_writeIntCount = 0;
        m_writeStringCount = 0;
        m_getIntCount = 0;
        m_getStringCount = 0;
        m_openKeyCount = 0;
        m_closeKeyCount = 0;

        m_writeInt = [](PCTSTR, PCTSTR, int) {
            BOOST_FAIL("WriteInt callback not provided");
            return TRUE;
        };
        m_writeString = [](PCTSTR, PCTSTR, PCTSTR) {
            BOOST_FAIL("WriteString callback not provided");
            return TRUE;
        };
        m_getInt = [](PCTSTR, PCTSTR, int) {
            BOOST_FAIL("GetInt callback not provided");
            return 0;
        };
        m_getString = [](PCTSTR, PCTSTR, PCTSTR) {
            BOOST_FAIL("GetString callback not provided");
            return _T("");
        };
        m_openKey = [](HKEY, PCTSTR, DWORD, REGSAM, PHKEY) {
            BOOST_FAIL("OpenKey callback not provided");
            return ERROR_SUCCESS;
        };
        m_closeKey = [](HKEY) {
            BOOST_FAIL("CloseKey callback not provided");
            return ERROR_SUCCESS;
        };

        m_writeIntNoop = [](PCTSTR, PCTSTR, int) {
            return TRUE;
        };
        m_writeStringNoop = [](PCTSTR, PCTSTR, PCTSTR) {
            return TRUE;
        };
        m_getIntNoop = [](PCTSTR, PCTSTR, int) {
            return 0;
        };
        m_getStringNoop = [](PCTSTR, PCTSTR, PCTSTR) {
            return _T("");
        };
        m_openKeyNoop = [](HKEY, PCTSTR, DWORD, REGSAM, PHKEY) {
            return ERROR_SUCCESS;
        };
        m_closeKeyNoop = [](HKEY) {
            return ERROR_SUCCESS;
        };
    }


    int m_writeIntCount;
    int m_writeStringCount;
    int m_getIntCount;
    int m_getStringCount;
    int m_openKeyCount;
    int m_closeKeyCount;

    std::function<BOOL(PCTSTR, PCTSTR, int)> m_writeInt;
    std::function<BOOL(PCTSTR, PCTSTR, PCTSTR)> m_writeString;
    std::function<UINT(PCTSTR, PCTSTR, int)> m_getInt;
    std::function<CString(PCTSTR, PCTSTR, PCTSTR)> m_getString;
    std::function<LSTATUS(HKEY, PCTSTR, DWORD, REGSAM, PHKEY)> m_openKey;
    std::function<LSTATUS(HKEY)> m_closeKey;

    std::function<BOOL(PCTSTR, PCTSTR, int)> m_writeIntNoop;
    std::function<BOOL(PCTSTR, PCTSTR, PCTSTR)> m_writeStringNoop;
    std::function<UINT(PCTSTR, PCTSTR, int)> m_getIntNoop;
    std::function<CString(PCTSTR, PCTSTR, PCTSTR)> m_getStringNoop;
    std::function<LSTATUS(HKEY, PCTSTR, DWORD, REGSAM, PHKEY)> m_openKeyNoop;
    std::function<LSTATUS(HKEY)> m_closeKeyNoop;
};

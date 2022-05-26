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

    BOOL WriteInt(LPCTSTR section, LPCTSTR entry, int value) override {
        m_writeIntCount++;
        return m_writeInt(section, entry, value);
    }

    BOOL WriteString(LPCTSTR section, LPCTSTR entry, LPCTSTR value) override {
        m_writeStringCount++;
        return m_writeString(section, entry, value);
    }

    UINT GetInt(LPCTSTR section, LPCTSTR entry, int defaultValue) override {
        m_getIntCount++;
        return m_getInt(section, entry, defaultValue);
    }

    CString GetString(LPCTSTR section, LPCTSTR entry, LPCTSTR defaultValue) override {
        m_getStringCount++;
        return m_getString(section, entry, defaultValue);
    }

    LPCTSTR GetKeyName() override {
        return _T("cthing");
    }

    LSTATUS OpenKey(HKEY key, LPCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) override {
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

        m_writeInt = [](LPCTSTR, LPCTSTR, int) {
            BOOST_FAIL("WriteInt callback not provided");
            return TRUE;
        };
        m_writeString = [](LPCTSTR, LPCTSTR, LPCTSTR) {
            BOOST_FAIL("WriteString callback not provided");
            return TRUE;
        };
        m_getInt = [](LPCTSTR, LPCTSTR, int) {
            BOOST_FAIL("GetInt callback not provided");
            return 0;
        };
        m_getString = [](LPCTSTR, LPCTSTR, LPCTSTR) {
            BOOST_FAIL("GetString callback not provided");
            return _T("");
        };
        m_openKey = [](HKEY, LPCTSTR, DWORD, REGSAM, PHKEY) {
            BOOST_FAIL("OpenKey callback not provided");
            return ERROR_SUCCESS;
        };
        m_closeKey = [](HKEY) {
            BOOST_FAIL("CloseKey callback not provided");
            return ERROR_SUCCESS;
        };

        m_writeIntNoop = [](LPCTSTR, LPCTSTR, int) {
            return TRUE;
        };
        m_writeStringNoop = [](LPCTSTR, LPCTSTR, LPCTSTR) {
            return TRUE;
        };
        m_getIntNoop = [](LPCTSTR, LPCTSTR, int) {
            return 0;
        };
        m_getStringNoop = [](LPCTSTR, LPCTSTR, LPCTSTR) {
            return _T("");
        };
        m_openKeyNoop = [](HKEY, LPCTSTR, DWORD, REGSAM, PHKEY) {
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

    std::function<BOOL(LPCTSTR, LPCTSTR, int)> m_writeInt;
    std::function<BOOL(LPCTSTR, LPCTSTR, LPCTSTR)> m_writeString;
    std::function<UINT(LPCTSTR, LPCTSTR, int)> m_getInt;
    std::function<CString(LPCTSTR, LPCTSTR, LPCTSTR)> m_getString;
    std::function<LSTATUS(HKEY, LPCTSTR, DWORD, REGSAM, PHKEY)> m_openKey;
    std::function<LSTATUS(HKEY)> m_closeKey;

    std::function<BOOL(LPCTSTR, LPCTSTR, int)> m_writeIntNoop;
    std::function<BOOL(LPCTSTR, LPCTSTR, LPCTSTR)> m_writeStringNoop;
    std::function<UINT(LPCTSTR, LPCTSTR, int)> m_getIntNoop;
    std::function<CString(LPCTSTR, LPCTSTR, LPCTSTR)> m_getStringNoop;
    std::function<LSTATUS(HKEY, LPCTSTR, DWORD, REGSAM, PHKEY)> m_openKeyNoop;
    std::function<LSTATUS(HKEY)> m_closeKeyNoop;
};

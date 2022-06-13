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

#include "pch.h"
#define BOOST_TEST_MODULE RegistryProfileTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include "mocks/MockRegistry.h"
#include <meazure/profile/RegistryProfile.h>
#include <float.h>

namespace tt = boost::test_tools;


struct TestFixture {
    TestFixture() {
        mock.m_openKey = mock.m_openKeyNoop;
        mock.m_closeKey = mock.m_closeKeyNoop;
    }

    MockRegistry mock;
};


BOOST_FIXTURE_TEST_CASE(TestCtorNoKey, TestFixture) {
    mock.m_openKey = [](HKEY key, PCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) {
        CString subKey1(_T("Software\\cthing\\RegistryProfileTest\\1.0"));
        CString subKey2(_T("Software\\cthing\\RegistryProfileTest\\2.0"));

        BOOST_TEST(key == HKEY_CURRENT_USER);
        BOOST_TEST(((subKey == subKey1) || (subKey == subKey2)));
        BOOST_TEST(options == static_cast<DWORD>(0));
        BOOST_TEST(samDesired == static_cast<REGSAM>(KEY_READ));
        result = &key;
        return ERROR_BADKEY;
    };

    MeaRegistryProfile profile(mock);

    BOOST_TEST(mock.m_openKeyCount == 2);
    BOOST_TEST(mock.m_closeKeyCount == 0);
}

BOOST_FIXTURE_TEST_CASE(TestCtorExistingKey, TestFixture) {
    HKEY akey = HKEY_LOCAL_MACHINE;

    mock.m_openKey = [&](HKEY key, PCTSTR subKey, DWORD options, REGSAM samDesired, PHKEY result) {
        BOOST_TEST(key == HKEY_CURRENT_USER);
        BOOST_TEST(subKey == CString(_T("Software\\cthing\\RegistryProfileTest\\2.0")));
        BOOST_TEST(options == static_cast<DWORD>(0));
        BOOST_TEST(samDesired == static_cast<REGSAM>(KEY_READ));
        *result = akey;
        return ERROR_SUCCESS;
    };

    mock.m_closeKey = [&](HKEY key) {
        BOOST_TEST(key == akey);
        return ERROR_SUCCESS;
    };

    MeaRegistryProfile profile(mock);

    BOOST_TEST(mock.m_openKeyCount == 1);
    BOOST_TEST(mock.m_closeKeyCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestWriteBool, TestFixture) {
    CString testKey(_T("test"));
    mock.m_writeInt = [&](PCTSTR section, PCTSTR key, int value) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(value);
        return TRUE;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.WriteBool(testKey, true));
    BOOST_TEST(mock.m_writeIntCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestWriteInt, TestFixture) {
    CString testKey(_T("test"));
    int testValue = 17;
    mock.m_writeInt = [&](PCTSTR section, PCTSTR key, int value) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(value == testValue);
        return TRUE;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.WriteInt(testKey, testValue));
    BOOST_TEST(mock.m_writeIntCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestWriteDbl, TestFixture) {
    CString testKey(_T("test"));
    double testValue = 17.5;
    CString testValueStr;
    testValueStr.Format(_T("%f"), testValue);
    mock.m_writeString = [&](PCTSTR section, PCTSTR key, PCTSTR value) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(value == testValueStr);
        return TRUE;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.WriteDbl(testKey, testValue));
    BOOST_TEST(mock.m_writeStringCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestWriteStr, TestFixture) {
    CString testKey(_T("test"));
    CString testValue(_T("a value"));
    mock.m_writeString = [&](PCTSTR section, PCTSTR key, PCTSTR value) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(value == testValue);
        return TRUE;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.WriteStr(testKey, testValue));
    BOOST_TEST(mock.m_writeStringCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestReadBool, TestFixture) {
    CString testKey(_T("test"));
    mock.m_getInt = [&](PCTSTR section, PCTSTR key, int defaultValue) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(!defaultValue);
        return 1;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.ReadBool(testKey, false));
    BOOST_TEST(mock.m_getIntCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestReadInt, TestFixture) {
    CString testKey(_T("test"));
    int testDefault = 17;
    UINT testValue = 13;
    mock.m_getInt = [&](PCTSTR section, PCTSTR key, int defaultValue) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(defaultValue == testDefault);
        return testValue;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.ReadInt(testKey, testDefault) == testValue);
    BOOST_TEST(mock.m_getIntCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestReadDbl, TestFixture) {
    CString testKey(_T("test"));
    double testDefault = 17.5;
    double testValue = 13.7;
    CString testValueStr;
    testValueStr.Format(_T("%f"), testValue);
    mock.m_getString = [&](PCTSTR section, PCTSTR key, PCTSTR defaultValue) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(defaultValue == CString(_T("")));
        return testValueStr;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.ReadDbl(testKey, testDefault) == testValue, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(mock.m_getStringCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestReadDblDefault, TestFixture) {
    CString testKey(_T("test"));
    double testDefault = 17.5;
    double testValue = 13.7;
    CString testValueStr;
    testValueStr.Format(_T("%f"), testValue);
    mock.m_getString = [&](PCTSTR section, PCTSTR key, PCTSTR defaultValue) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(defaultValue == CString(_T("")));
        return CString(_T(""));
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.ReadDbl(testKey, testDefault) == testDefault, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(mock.m_getStringCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestReeadStr, TestFixture) {
    CString testKey(_T("test"));
    CString testDefault(_T("test def"));
    CString testValue(_T("something"));
    mock.m_getString = [&](PCTSTR section, PCTSTR key, PCTSTR defaultValue) {
        BOOST_TEST(section == CString(_T("2.0")));
        BOOST_TEST(key == testKey);
        BOOST_TEST(defaultValue == testDefault);
        return testValue;
    };

    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.ReadStr(testKey, testDefault) == testValue);
    BOOST_TEST(mock.m_getStringCount == 1);
}

BOOST_FIXTURE_TEST_CASE(TestUserInitiated, TestFixture) {
    MeaRegistryProfile profile(mock);
    BOOST_TEST(!profile.UserInitiated());
}

BOOST_FIXTURE_TEST_CASE(TestGetVersion, TestFixture) {
    MeaRegistryProfile profile(mock);
    BOOST_TEST(profile.GetVersion() == 2);
}

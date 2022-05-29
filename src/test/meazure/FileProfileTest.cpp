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
#define BOOST_TEST_MODULE FileProfileTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/profile/FileProfile.h>
#include <filesystem>
#include <float.h>

namespace tt = boost::test_tools;


struct TestFixture {
    TestFixture() {
        GetTempPath(MAX_PATH, tempPathBuffer);
        GetTempFileName(tempPathBuffer, _T("MeaTest"), 0, tempFileName);
    }

    ~TestFixture() {
       DeleteFile(tempFileName);
    }

    TCHAR tempPathBuffer[MAX_PATH];
    TCHAR tempFileName[MAX_PATH];
};


BOOST_FIXTURE_TEST_CASE(TestReadWrite, TestFixture) {
    {
        MeaFileProfile profile(tempFileName, MeaFileProfile::ProfWrite);
        BOOST_TEST(profile.WriteBool(_T("key1"), true));
        BOOST_TEST(profile.WriteInt(_T("key2"), 17));
        BOOST_TEST(profile.WriteDbl(_T("key3"), 3.14159));
        BOOST_TEST(profile.WriteStr(_T("key4"), _T("abcd")));
    }

    BOOST_TEST(std::filesystem::exists(tempFileName));

    {
        MeaFileProfile profile(tempFileName, MeaFileProfile::ProfRead);

        BOOST_TEST(profile.ReadBool(_T("key1"), false));
        BOOST_TEST(profile.ReadInt(_T("key2"), 10) == static_cast<UINT>(17));
        BOOST_TEST(profile.ReadDbl(_T("key3"), 1.5) == 3.14159, tt::tolerance(FLT_EPSILON));
        BOOST_TEST(profile.ReadStr(_T("key4"), _T("foo")) == _T("abcd"));

        BOOST_TEST(!profile.ReadBool(_T("key15"), false));
        BOOST_TEST(profile.ReadInt(_T("key16"), 10) == static_cast<UINT>(10));
        BOOST_TEST(profile.ReadDbl(_T("key17"), 1.5) == 1.5, tt::tolerance(FLT_EPSILON));
        BOOST_TEST(profile.ReadStr(_T("key18"), _T("foo")) == _T("foo"));
    }
}

BOOST_FIXTURE_TEST_CASE(TestUserInitiated, TestFixture) {
    MeaFileProfile profile(tempFileName, MeaFileProfile::ProfWrite);
    BOOST_TEST(profile.UserInitiated());
}

BOOST_FIXTURE_TEST_CASE(TestGetVersion, TestFixture) {
    MeaFileProfile profile(tempFileName, MeaFileProfile::ProfWrite);
    BOOST_TEST(profile.GetVersion() == 1);
}

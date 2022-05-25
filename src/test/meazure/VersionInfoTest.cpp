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
#define BOOST_TEST_MODULE VersionInfoTest
#include <boost/test/unit_test.hpp>
#include <meazure/VersionInfo.h>
#include <meazure/VersionNumbers.h>
#include <regex>


struct GlobalFixture {
    GlobalFixture() {
        if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
            BOOST_FAIL("Fatal Error: MFC initialization failed");
        }
    }

    CWinApp theApp;
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);

BOOST_AUTO_TEST_CASE(TestVersionNumbers) {
    std::regex versionRegex("[\\d]+,[\\s]*[\\d]+,[\\s]*[\\d]+,[\\s]*[\\d]+");
    BOOST_TEST(std::regex_match(STR_FILE_VERSION, versionRegex));
    BOOST_TEST(std::regex_match(STR_PRODUCT_VERSION, versionRegex));
}

BOOST_AUTO_TEST_CASE(TestGetProductVersion) {
    MeaVersionInfo versionInfo;
    std::regex productRegex("[\\d]+\\.[\\d]+(\\.[\\d]+)?");
    LPCTSTR productVersion = versionInfo.GetProductVersion();
    BOOST_TEST(std::regex_match(productVersion, productRegex));
}

BOOST_AUTO_TEST_CASE(TestVersionComponents) {
    MeaVersionInfo versionInfo;
    BOOST_TEST(versionInfo.GetProductMajor() >= 0);
    BOOST_TEST(versionInfo.GetProductMinor() >= 0);
    BOOST_TEST(versionInfo.GetProductFix() >= 0);
    BOOST_TEST(versionInfo.GetProductBuild() >= 0);
}

BOOST_AUTO_TEST_CASE(TestProfileVersion) {
    MeaVersionInfo versionInfo;
    BOOST_TEST(versionInfo.GetProfileFileMajor() == 2);
}

BOOST_AUTO_TEST_CASE(TestLogVersion) {
    MeaVersionInfo versionInfo;
    BOOST_TEST(versionInfo.GetLogFileMajor() == 1);
}

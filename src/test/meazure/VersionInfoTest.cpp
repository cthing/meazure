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
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/VersionInfo.h>
#include <meazure/VersionNumbers.h>
#include <regex>


BOOST_AUTO_TEST_CASE(TestVersionNumbers) {
    std::regex versionRegex("[\\d]+,[\\s]*[\\d]+,[\\s]*[\\d]+,[\\s]*[\\d]+");
    BOOST_TEST(std::regex_match(STR_FILE_VERSION, versionRegex));
    BOOST_TEST(std::regex_match(STR_PRODUCT_VERSION, versionRegex));
}

BOOST_AUTO_TEST_CASE(TestGetProductVersion) {
    MeaVersionInfo& versionInfo = MeaVersionInfo::Instance();
    BOOST_TEST(versionInfo.GetProductVersion() == _T("1.2.3"));
}

BOOST_AUTO_TEST_CASE(TestVersionComponents) {
    MeaVersionInfo& versionInfo = MeaVersionInfo::Instance();
    BOOST_TEST(versionInfo.GetProductMajor() == 1);
    BOOST_TEST(versionInfo.GetProductMinor() == 2);
    BOOST_TEST(versionInfo.GetProductFix() == 3);
    BOOST_TEST(versionInfo.GetProductBuild() == 4);
}

BOOST_AUTO_TEST_CASE(TestProfileVersion) {
    MeaVersionInfo& versionInfo = MeaVersionInfo::Instance();
    BOOST_TEST(versionInfo.GetProfileFileMajor() == 2);
}

BOOST_AUTO_TEST_CASE(TestLogVersion) {
    MeaVersionInfo& versionInfo = MeaVersionInfo::Instance();
    BOOST_TEST(versionInfo.GetLogFileMajor() == 1);
}

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
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/NumericUtils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


namespace {
    void TestIsFloatingEqual() {
        BOOST_CHECK(MeaNumericUtils::IsFloatingEqual(0.0, 0.0));
        BOOST_CHECK(MeaNumericUtils::IsFloatingEqual(1.0, 1.0));
        BOOST_CHECK(MeaNumericUtils::IsFloatingEqual(-1.0, -1.0));
        BOOST_CHECK(MeaNumericUtils::IsFloatingEqual(3.141596, 3.141596));
        BOOST_CHECK(MeaNumericUtils::IsFloatingEqual(1.0 / 17.0, 1 / 17.0));
        BOOST_CHECK(MeaNumericUtils::IsFloatingEqual(std::numeric_limits<double>::epsilon(),
                                                     std::numeric_limits<double>::epsilon()));
        BOOST_CHECK(MeaNumericUtils::IsFloatingEqual(1.000000001f, 1.000000002f));

        BOOST_CHECK(!MeaNumericUtils::IsFloatingEqual(0.0, 1.5));
        BOOST_CHECK(!MeaNumericUtils::IsFloatingEqual(-10.0, 3.141596));
        BOOST_CHECK(!MeaNumericUtils::IsFloatingEqual(1.000001f, 1.000002f));
    }

    void TestIsFloatingZero() {
        BOOST_CHECK(MeaNumericUtils::IsFloatingZero(0.0));
        BOOST_CHECK(MeaNumericUtils::IsFloatingZero(-0.0));
        BOOST_CHECK(MeaNumericUtils::IsFloatingZero(std::numeric_limits<double>::epsilon()));

        BOOST_CHECK(!MeaNumericUtils::IsFloatingZero(1.0));
        BOOST_CHECK(!MeaNumericUtils::IsFloatingZero(-1.0));
        BOOST_CHECK(!MeaNumericUtils::IsFloatingZero(2.0 * std::numeric_limits<double>::epsilon()));
        BOOST_CHECK(!MeaNumericUtils::IsFloatingZero(-2.0 * std::numeric_limits<double>::epsilon()));
    }
}

boost::unit_test_framework::test_suite* init_unit_test_suite(int, char* []) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        std::cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    boost::unit_test_framework::test_suite* numericSuite = BOOST_TEST_SUITE("Numeric Tests");
    numericSuite->add(BOOST_TEST_CASE(&TestIsFloatingEqual));
    numericSuite->add(BOOST_TEST_CASE(&TestIsFloatingZero));
    boost::unit_test_framework::framework::master_test_suite().add(numericSuite);

    return nullptr;
}

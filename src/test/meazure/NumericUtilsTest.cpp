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
#define BOOST_TEST_MODULE NumericUtilsTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/NumericUtils.h>
#include <float.h>

namespace bt = boost::unit_test;


BOOST_AUTO_TEST_CASE(TestIsEqualF) {
    BOOST_TEST(MeaNumericUtils::IsEqualF(0.0, 0.0));
    BOOST_TEST(MeaNumericUtils::IsEqualF(1.0, 1.0));
    BOOST_TEST(MeaNumericUtils::IsEqualF(-1.0, -1.0));
    BOOST_TEST(MeaNumericUtils::IsEqualF(3.141596, 3.141596));
    BOOST_TEST(MeaNumericUtils::IsEqualF(1.0 / 17.0, 1 / 17.0));
    BOOST_TEST(MeaNumericUtils::IsEqualF(std::numeric_limits<double>::epsilon(),
                                         std::numeric_limits<double>::epsilon()));
    BOOST_TEST(MeaNumericUtils::IsEqualF(1.000000001f, 1.000000002f));

    BOOST_TEST(!MeaNumericUtils::IsEqualF(0.0, 1.5));
    BOOST_TEST(!MeaNumericUtils::IsEqualF(-10.0, 3.141596));
    BOOST_TEST(!MeaNumericUtils::IsEqualF(1.000001f, 1.000002f));
}

BOOST_AUTO_TEST_CASE(TestIsZeroF) {
    BOOST_TEST(MeaNumericUtils::IsZeroF(0.0));
    BOOST_TEST(MeaNumericUtils::IsZeroF(-0.0));
    BOOST_TEST(MeaNumericUtils::IsZeroF(std::numeric_limits<double>::epsilon()));

    BOOST_TEST(!MeaNumericUtils::IsZeroF(1.0));
    BOOST_TEST(!MeaNumericUtils::IsZeroF(-1.0));
    BOOST_TEST(!MeaNumericUtils::IsZeroF(2.0 * std::numeric_limits<double>::epsilon()));
    BOOST_TEST(!MeaNumericUtils::IsZeroF(-2.0 * std::numeric_limits<double>::epsilon()));
}

BOOST_AUTO_TEST_CASE(TestDegToRad, *bt::tolerance(DBL_EPSILON)) {
    BOOST_TEST(MeaNumericUtils::DegToRad(0.0) == 0.0);
    BOOST_TEST(MeaNumericUtils::DegToRad(180.0) == MeaNumericUtils::PI);
    BOOST_TEST(MeaNumericUtils::DegToRad(360.0) == 2.0 * MeaNumericUtils::PI);
    BOOST_TEST(MeaNumericUtils::DegToRad(125.0) == 2.1816615649929116);
}

BOOST_AUTO_TEST_CASE(TestRadToDeg, *bt::tolerance(DBL_EPSILON)) {
    BOOST_TEST(MeaNumericUtils::RadToDeg(0.0) == 0.0);
    BOOST_TEST(MeaNumericUtils::RadToDeg(MeaNumericUtils::PI) == 180.0);
    BOOST_TEST(MeaNumericUtils::RadToDeg(2.0 * MeaNumericUtils::PI) == 360.0);
    BOOST_TEST(MeaNumericUtils::RadToDeg(2.1816615649929116) == 125.0);
}

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
#define BOOST_TEST_MODULE PositionScreenTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/position/PositionScreen.h>
#include <meazure/xml/XMLParser.h>
#include <meazure/xml/XMLWriter.h>
#include "mocks/MockScreenProvider.h"
#include "mocks/MockUnitsProvider.h"
#include <fstream>


BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaPositionScreen)


struct TestFixture {
    TestFixture(): unitsProvider(screenProvider) {}

    MockScreenProvider screenProvider;
    MockUnitsProvider unitsProvider;
};


BOOST_AUTO_TEST_CASE(TestDefaultCtor) {
    MeaPositionScreen screen;

    BOOST_TEST(!screen.IsPrimary());
    BOOST_TEST(screen.GetRect() == MeaFRect());
    BOOST_TEST(screen.GetRes() == MeaFSize());
    BOOST_TEST(!screen.IsManualRes());
    BOOST_TEST(screen.GetDesc() == _T(""));
}

BOOST_FIXTURE_TEST_CASE(TestIterCtor, TestFixture) {
    MeaPositionScreen screen(screenProvider.GetScreenIter(), unitsProvider, screenProvider);

    BOOST_TEST(screen.IsPrimary());
    BOOST_TEST(screen.GetRect() == MeaFRect(0.0, 1024.0, 0.0, 1280.0));
    BOOST_TEST(screen.GetRes() == MeaFSize(96.0, 96.0));
    BOOST_TEST(!screen.IsManualRes());
    BOOST_TEST(screen.GetDesc() == _T("MockScreen"));
}

BOOST_FIXTURE_TEST_CASE(TestCopy, TestFixture) {
    MeaPositionScreen screen1(screenProvider.GetScreenIter(), unitsProvider, screenProvider);
    MeaPositionScreen screen2(screen1);
    MeaPositionScreen screen3;

    screen3 = screen1;

    BOOST_TEST(screen2.IsPrimary());
    BOOST_TEST(screen2.GetRect() == MeaFRect(0.0, 1024.0, 0.0, 1280.0));
    BOOST_TEST(screen2.GetRes() == MeaFSize(96.0, 96.0));
    BOOST_TEST(!screen2.IsManualRes());
    BOOST_TEST(screen2.GetDesc() == _T("MockScreen"));

    BOOST_TEST(screen3.IsPrimary());
    BOOST_TEST(screen3.GetRect() == MeaFRect(0.0, 1024.0, 0.0, 1280.0));
    BOOST_TEST(screen3.GetRes() == MeaFSize(96.0, 96.0));
    BOOST_TEST(!screen3.IsManualRes());
    BOOST_TEST(screen3.GetDesc() == _T("MockScreen"));
}

BOOST_FIXTURE_TEST_CASE(TestEquality, TestFixture) {
    MeaPositionScreen screen1(screenProvider.GetScreenIter(), unitsProvider, screenProvider);
    MeaPositionScreen screen2(screenProvider.GetScreenIter(), unitsProvider, screenProvider);
    MeaPositionScreen screen3;

    BOOST_TEST(screen1 == screen1);
    BOOST_TEST(screen1 == screen2);
    BOOST_TEST(screen1 != screen3);
}

BOOST_FIXTURE_TEST_CASE(TestSaveLoad, TestFixture) {
    MeaPositionScreen screen1(screenProvider.GetScreenIter(), unitsProvider, screenProvider);

    std::ostringstream stream;
    MeaXMLWriter writer(stream);
    writer.StartDocument();
    screen1.Save(writer);
    writer.EndDocument();

    MeaXMLParser parser;
    parser.ParseString(stream.str().c_str());

    const MeaXMLNode* screenNode = parser.GetDOM();
    MeaPositionScreen screen2(screenProvider.GetScreenIter(), unitsProvider, screenProvider);
    screen2.Load(screenNode);

    BOOST_TEST(screen2.IsPrimary());
    BOOST_TEST(screen2.GetRect() == MeaFRect(0.0, 1024.0, 0.0, 1280.0));
    BOOST_TEST(screen2.GetRes() == MeaFSize(96.0, 96.0));
    BOOST_TEST(!screen2.IsManualRes());
    BOOST_TEST(screen2.GetDesc() == _T("MockScreen"));
}

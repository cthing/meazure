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
#define BOOST_TEST_MODULE PositionCollectionTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/position/PositionCollection.h>
#include <meazure/xml/XMLWriter.h>
#include "mocks/MockScreenProvider.h"
#include "mocks/MockUnitsProvider.h"
#include "mocks/MockPositionDesktopRefCounter.h"
#include <stdexcept>
#include <fstream>


BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaPosition)


struct TestFixture {
    TestFixture() : unitsProvider(screenProvider), desktop(unitsProvider, screenProvider), ref(&counter, desktop) {}

    MockScreenProvider screenProvider;
    MockUnitsProvider unitsProvider;
    MeaPositionDesktop desktop;
    MockPositionDesktopRefCounter counter;
    MeaPositionDesktopRef ref;
};


BOOST_AUTO_TEST_CASE(TestDefault) {
    MeaPositionCollection positions;

    BOOST_TEST(positions.Empty());
    BOOST_TEST(positions.Size() == static_cast<unsigned int>(0));
}

BOOST_FIXTURE_TEST_CASE(TestAdd, TestFixture) {
    MeaPosition* position1 = new MeaPosition(ref);
    position1->SetDesc(_T("Position 1"));

    MeaPosition* position2 = new MeaPosition(ref);
    position2->SetDesc(_T("Position 2"));

    MeaPosition* position3 = new MeaPosition(ref);
    position3->SetDesc(_T("Position 3"));

    MeaPositionCollection positions;
    positions.Add(position1);
    positions.Add(position2);
    positions.Add(position3);

    BOOST_TEST(!positions.Empty());
    BOOST_TEST(positions.Size() == static_cast<unsigned int>(3));
    BOOST_TEST(positions.Get(0) == *position1);
    BOOST_TEST(positions.Get(1) == *position2);
    BOOST_TEST(positions.Get(2) == *position3);
}

BOOST_FIXTURE_TEST_CASE(TestSet, TestFixture) {
    MeaPosition* position1 = new MeaPosition(ref);
    position1->SetDesc(_T("Position 1"));

    MeaPosition* position2 = new MeaPosition(ref);
    position2->SetDesc(_T("Position 2"));

    MeaPosition* position3 = new MeaPosition(ref);
    position3->SetDesc(_T("Position 3"));

    MeaPosition* position4 = new MeaPosition(ref);
    position4->SetDesc(_T("Position 4"));

    MeaPosition* position5 = new MeaPosition(ref);
    position5->SetDesc(_T("Position 5"));

    MeaPosition* position6 = new MeaPosition(ref);
    position6->SetDesc(_T("Position 6"));

    MeaPositionCollection positions;
    positions.Add(position1);
    positions.Add(position2);
    positions.Add(position3);

    positions.Set(2, position4);
    positions.Set(1, position5);
    positions.Set(0, position6);

    BOOST_TEST(!positions.Empty());
    BOOST_TEST(positions.Size() == static_cast<unsigned int>(3));
    BOOST_TEST(positions.Get(0) == *position6);
    BOOST_TEST(positions.Get(1) == *position5);
    BOOST_TEST(positions.Get(2) == *position4);
}

BOOST_FIXTURE_TEST_CASE(TestDelete, TestFixture) {
    MeaPosition* position1 = new MeaPosition(ref);
    position1->SetDesc(_T("Position 1"));

    MeaPosition* position2 = new MeaPosition(ref);
    position2->SetDesc(_T("Position 2"));

    MeaPosition* position3 = new MeaPosition(ref);
    position3->SetDesc(_T("Position 3"));

    MeaPositionCollection positions;
    positions.Add(position1);
    positions.Add(position2);
    positions.Add(position3);

    positions.Delete(1);

    BOOST_TEST(!positions.Empty());
    BOOST_TEST(positions.Size() == static_cast<unsigned int>(2));
    BOOST_TEST(positions.Get(0) == *position1);
    BOOST_TEST(positions.Get(1) == *position3);
}

BOOST_FIXTURE_TEST_CASE(TestDeleteAll, TestFixture) {
    MeaPosition* position1 = new MeaPosition(ref);
    position1->SetDesc(_T("Position 1"));

    MeaPosition* position2 = new MeaPosition(ref);
    position2->SetDesc(_T("Position 2"));

    MeaPosition* position3 = new MeaPosition(ref);
    position3->SetDesc(_T("Position 3"));

    MeaPositionCollection positions;
    positions.Add(position1);
    positions.Add(position2);
    positions.Add(position3);

    positions.DeleteAll();

    BOOST_TEST(positions.Empty());
    BOOST_TEST(positions.Size() == static_cast<unsigned int>(0));
}

BOOST_FIXTURE_TEST_CASE(TestExceptions, TestFixture) {
    MeaPosition* position1 = new MeaPosition(ref);
    position1->SetDesc(_T("Position 1"));

    MeaPosition* position2 = new MeaPosition(ref);
    position2->SetDesc(_T("Position 2"));

    MeaPosition* position3 = new MeaPosition(ref);
    position3->SetDesc(_T("Position 3"));

    MeaPositionCollection positions;
    positions.Add(position1);
    positions.Add(position2);

    BOOST_CHECK_THROW(positions.Set(10, position3), std::out_of_range);
    BOOST_CHECK_THROW(positions.Get(10), std::out_of_range);
    BOOST_CHECK_THROW(positions.Delete(10), std::out_of_range);
}

BOOST_FIXTURE_TEST_CASE(TestSave, TestFixture) {
    MeaPositionCollection positions;
    MeaPosition* position1 = new MeaPosition(ref);
    MeaPosition* position2 = new MeaPosition(ref);
    positions.Add(position1);
    positions.Add(position2);

    std::ostringstream stream;
    MeaXMLWriter writer(stream);
    writer.StartDocument();
    writer.StartElement(_T("test"));
    positions.Save(writer);
    writer.EndElement();
    writer.EndDocument();

    BOOST_TEST(CString(stream.str().c_str()).Find(_T("<position ")) >= 0);
}

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
#define BOOST_TEST_MODULE PositionDesktopTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/position/PositionDesktop.h>
#include <meazure/units/Units.h>
#include <meazure/utilities/GUID.h>
#include <meazure/xml/XMLParser.h>
#include <meazure/xml/XMLWriter.h>
#include "mocks/MockScreenProvider.h"
#include "mocks/MockUnitsProvider.h"
#include "mocks/MockPositionDesktopRefCounter.h"
#include <regex>
#include <float.h>
#include <fstream>

namespace tt = boost::test_tools;


BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaPositionDesktop)


struct TestFixture {
    TestFixture() : unitsProvider(screenProvider) {}

    MockScreenProvider screenProvider;
    MockUnitsProvider unitsProvider;
};


BOOST_FIXTURE_TEST_CASE(TestNoGuidCtor, TestFixture) {
    MeaPositionDesktop desktop(unitsProvider, screenProvider);

    BOOST_TEST(desktop.GetLinearUnits()->GetUnitsId() == MeaPixelsId);
    BOOST_TEST(desktop.GetAngularUnits()->GetUnitsId() == MeaDegreesId);
    BOOST_TEST(!desktop.IsInvertY());
    BOOST_TEST(desktop.GetOrigin() == MeaFPoint(0.0, 0.0));
    std::regex guidRegex("[A-Fa-f\\d]{8}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{12}");
    BOOST_TEST(std::regex_match(static_cast<PCTSTR>(desktop.GetId().ToString()), guidRegex));
    BOOST_TEST(desktop.GetCustomName() == _T(""));
    BOOST_TEST(desktop.GetCustomAbbrev() == _T(""));
    BOOST_TEST(desktop.GetCustomBasisStr() == _T(""));
    BOOST_TEST(desktop.GetCustomFactor() == 0.0, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestGuidCtor, TestFixture) {
    MeaGUID guid;
    MeaPositionDesktop desktop(guid.ToString(), unitsProvider, screenProvider);

    BOOST_TEST(desktop.GetLinearUnits()->GetUnitsId() == MeaPixelsId);
    BOOST_TEST(desktop.GetAngularUnits()->GetUnitsId() == MeaDegreesId);
    BOOST_TEST(!desktop.IsInvertY());
    BOOST_TEST(desktop.GetOrigin() == MeaFPoint(0.0, 0.0));
    BOOST_TEST(desktop.GetId() == guid);
    BOOST_TEST(desktop.GetCustomName() == _T(""));
    BOOST_TEST(desktop.GetCustomAbbrev() == _T(""));
    BOOST_TEST(desktop.GetCustomBasisStr() == _T(""));
    BOOST_TEST(desktop.GetCustomFactor() == 0.0, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestIdProperty, TestFixture) {
    MeaGUID guid1;
    MeaGUID guid2;
    MeaPositionDesktop desktop(guid1.ToString(), unitsProvider, screenProvider);

    BOOST_TEST(desktop.GetId() == guid1);
    desktop.SetId(guid2.ToString());
    BOOST_TEST(desktop.GetId() == guid2);
}

BOOST_FIXTURE_TEST_CASE(TestCopy, TestFixture) {
    MeaPositionDesktop desktop1(unitsProvider, screenProvider);
    MeaPositionDesktop desktop2(desktop1);
    MeaPositionDesktop desktop3 = desktop1;

    BOOST_TEST(desktop1.GetId() == desktop2.GetId());
    BOOST_TEST(desktop1.GetId() == desktop3.GetId());
}

BOOST_FIXTURE_TEST_CASE(TestDesktopEquality, TestFixture) {
    MockUnitsProvider unitsProvider1(screenProvider);
    MockUnitsProvider unitsProvider2(screenProvider);
    unitsProvider2.SetOrigin(MeaFPoint(2.0, 3.0));
    MeaPositionDesktop desktop1(unitsProvider1, screenProvider);
    MeaPositionDesktop desktop2(desktop1);
    MeaPositionDesktop desktop3(unitsProvider2, screenProvider);

    BOOST_TEST(desktop1 == desktop1);
    BOOST_TEST(desktop1 == desktop2);
    BOOST_TEST(desktop1 != desktop3);
}

BOOST_FIXTURE_TEST_CASE(TestSaveLoad, TestFixture) {
    unitsProvider.SetOrigin(MeaFPoint(2.0, 3.0));
    MeaPositionDesktop desktop1(unitsProvider, screenProvider);

    std::ostringstream stream;
    MeaXMLWriter writer(stream);
    writer.StartDocument();
    desktop1.Save(writer);
    writer.EndDocument();

    MeaXMLParser parser;
    parser.ParseString(stream.str().c_str());

    const MeaXMLNode* desktopNode = parser.GetDOM();
    MeaPositionDesktop desktop2(unitsProvider, screenProvider);
    desktop2.Load(desktopNode);

    BOOST_TEST(desktop2.GetLinearUnits()->GetUnitsId() == MeaPixelsId);
    BOOST_TEST(desktop2.GetAngularUnits()->GetUnitsId() == MeaDegreesId);
    BOOST_TEST(!desktop2.IsInvertY());
    BOOST_TEST(desktop2.GetOrigin() == MeaFPoint(2.0, 3.0));
    std::regex guidRegex("[A-Fa-f\\d]{8}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{12}");
    BOOST_TEST(std::regex_match(static_cast<PCTSTR>(desktop2.GetId().ToString()), guidRegex));
    BOOST_TEST(desktop2.GetCustomName() == _T(""));
    BOOST_TEST(desktop2.GetCustomAbbrev() == _T(""));
    BOOST_TEST(desktop2.GetCustomBasisStr() == _T(""));
    BOOST_TEST(desktop2.GetCustomFactor() == 0.0, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestDesktopRefCtor1, TestFixture) {
    MeaPositionDesktop desktop1(unitsProvider, screenProvider);
    MockPositionDesktopRefCounter counter;
    MeaPositionDesktopRef* ref1 = new MeaPositionDesktopRef(&counter, desktop1);

    BOOST_TEST(counter.m_refCounts.size() == 1);
    BOOST_TEST(counter.m_refCounts.count(desktop1.GetId()) == 1);
    BOOST_TEST(counter.m_refCounts.at(desktop1.GetId()) == 1);
    BOOST_TEST(ref1->GetId() == desktop1.GetId());
    BOOST_TEST(ref1->ToString() == desktop1.GetId().ToString());

    MeaPositionDesktop desktop2(unitsProvider, screenProvider);
    MeaPositionDesktopRef* ref2 = new MeaPositionDesktopRef(&counter, desktop2);

    BOOST_TEST(counter.m_refCounts.size() == 2);
    BOOST_TEST(counter.m_refCounts.count(desktop2.GetId()) == 1);
    BOOST_TEST(counter.m_refCounts.at(desktop2.GetId()) == 1);
    BOOST_TEST(ref2->GetId() == desktop2.GetId());
    BOOST_TEST(ref2->ToString() == desktop2.GetId().ToString());

    delete ref1;
    BOOST_TEST(counter.m_refCounts.at(desktop1.GetId()) == 0);
    BOOST_TEST(counter.m_refCounts.at(desktop2.GetId()) == 1);

    delete ref2;
    BOOST_TEST(counter.m_refCounts.at(desktop1.GetId()) == 0);
    BOOST_TEST(counter.m_refCounts.at(desktop2.GetId()) == 0);
}

BOOST_FIXTURE_TEST_CASE(TestDesktopRefCtor2, TestFixture) {
    MeaPositionDesktop desktop(unitsProvider, screenProvider);
    MockPositionDesktopRefCounter counter;
    MeaPositionDesktopRef* ref = new MeaPositionDesktopRef(&counter, desktop.GetId().ToString());

    BOOST_TEST(counter.m_refCounts.size() == 1);
    BOOST_TEST(counter.m_refCounts.count(desktop.GetId()) == 1);
    BOOST_TEST(counter.m_refCounts.at(desktop.GetId()) == 1);
    BOOST_TEST(ref->GetId() == desktop.GetId());
    BOOST_TEST(ref->ToString() == desktop.GetId().ToString());

    delete ref;
    BOOST_TEST(counter.m_refCounts.at(desktop.GetId()) == 0);
}

BOOST_FIXTURE_TEST_CASE(TestDesktopRefCopy, TestFixture) {
    MeaPositionDesktop desktop(unitsProvider, screenProvider);
    MockPositionDesktopRefCounter counter;
    MeaPositionDesktopRef ref1(&counter, desktop);
    MeaPositionDesktopRef ref2(ref1);
    MeaPositionDesktopRef ref3 = ref1;

    BOOST_TEST(counter.m_refCounts.size() == 1);
    BOOST_TEST(counter.m_refCounts.count(desktop.GetId()) == 1);
    BOOST_TEST(counter.m_refCounts.at(desktop.GetId()) == 3);
    BOOST_TEST(ref1.GetId() == desktop.GetId());
    BOOST_TEST(ref2.GetId() == desktop.GetId());
    BOOST_TEST(ref3.GetId() == desktop.GetId());
}

BOOST_FIXTURE_TEST_CASE(TestDesktopRefEquality, TestFixture) {
    MeaPositionDesktop desktop1(unitsProvider, screenProvider);
    MeaPositionDesktop desktop2(unitsProvider, screenProvider);
    MockPositionDesktopRefCounter counter;
    MeaPositionDesktopRef ref1(&counter, desktop1);
    MeaPositionDesktopRef ref2(ref1);
    MeaPositionDesktopRef ref3(&counter, desktop2);

    BOOST_TEST(ref1 == ref1);
    BOOST_TEST(ref1 == ref2);
    BOOST_TEST(ref1 != ref3);
}

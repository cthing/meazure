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
#define BOOST_TEST_MODULE PositionTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/position/Position.h>
#include <meazure/xml/XMLParser.h>
#include <meazure/xml/XMLWriter.h>
#include "mocks/MockScreenProvider.h"
#include "mocks/MockUnitsProvider.h"
#include "mocks/MockPositionDesktopRefCounter.h"
#include <regex>
#include <float.h>
#include <fstream>

namespace tt = boost::test_tools;


BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaPosition)


struct TestFixture {
    TestFixture() : unitsProvider(screenProvider), desktop(unitsProvider, screenProvider), ref(&counter, desktop) {}

    MockScreenProvider screenProvider;
    MockUnitsProvider unitsProvider;
    MeaPositionDesktop desktop;
    MockPositionDesktopRefCounter counter;
    MeaPositionDesktopRef ref;
};


BOOST_FIXTURE_TEST_CASE(TestCtor1, TestFixture) {
    MeaPosition position(ref);

    BOOST_TEST(position.GetToolName() == _T(""));
    BOOST_TEST(position.GetDesc() == _T(""));
    std::regex timestampRegex("[\\d]{4}-[\\d]{2}-[\\d]{2}T[\\d]{2}:[\\d]{2}:[\\d]{2}Z");
    BOOST_TEST(std::regex_match(position.GetTimeStamp().GetString(), timestampRegex));
    BOOST_TEST(position.GetDesktopRef() == ref);
    BOOST_TEST(position.GetPoints().empty());
    BOOST_TEST(position.GetWidth() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetHeight() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetDistance() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetArea() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetAngle() == 0.0, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestCtor2, TestFixture) {
    PCTSTR timestamp = _T("2022 - 05 - 02T05:20 : 12Z");
    PCTSTR tool = _T("Tool1");
    MeaPosition position(ref, tool, timestamp);

    BOOST_TEST(position.GetToolName() == tool);
    BOOST_TEST(position.GetDesc() == _T(""));
    BOOST_TEST(position.GetTimeStamp() == timestamp);
    BOOST_TEST(position.GetDesktopRef() == ref);
    BOOST_TEST(position.GetPoints().empty());
    BOOST_TEST(position.GetWidth() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetHeight() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetDistance() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetArea() == 0.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetAngle() == 0.0, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestProperties, TestFixture) {
    MeaPosition position(ref);

    PCTSTR tool = _T("Tool2");
    position.SetToolName(tool);
    BOOST_TEST(position.GetToolName() == tool);

    PCTSTR desc = _T("Interesting position");
    position.SetDesc(desc);
    BOOST_TEST(position.GetDesc() == desc);
}

BOOST_FIXTURE_TEST_CASE(TestAddPoint, TestFixture) {
    MeaPosition position(ref);
    MeaFPoint pt1(1.0, 2.0);
    MeaFPoint pt2(5.0, 7.0);
    MeaFPoint pt3(17.0, 10.0);

    position.AddPoint(_T("a"), pt1);
    position.AddPoint(_T("b"), pt2);
    position.AddPoint(_T("c"), pt3);

    const MeaPosition::PointMap& points = position.GetPoints();
    BOOST_TEST(points.size() == 3);
    BOOST_TEST(points.at(_T("a")) == pt1);
    BOOST_TEST(points.at(_T("b")) == pt2);
    BOOST_TEST(points.at(_T("c")) == pt3);
}

BOOST_FIXTURE_TEST_CASE(TestRecordPoints, TestFixture) {
    MeaPosition position(ref);
    MeaFPoint pt1(1.0, 2.0);
    MeaFPoint pt2(5.0, 7.0);
    MeaFPoint pt3(17.0, 10.0);

    position.RecordXY1(pt1);
    position.RecordXY2(pt2);
    position.RecordXYV(pt3);

    const MeaPosition::PointMap& points = position.GetPoints();
    BOOST_TEST(points.size() == 3);
    BOOST_TEST(points.at(_T("1")) == pt1);
    BOOST_TEST(points.at(_T("2")) == pt2);
    BOOST_TEST(points.at(_T("v")) == pt3);
}

BOOST_FIXTURE_TEST_CASE(TestRecordWH, TestFixture) {
    MeaPosition position(ref);
    MeaFSize size(10.0, 20.0);

    position.RecordWH(size);
    BOOST_TEST(position.GetWidth() == 10.0, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(position.GetHeight() == 20.0, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestRecordDisatnce, TestFixture) {
    MeaPosition position(ref);

    MeaFSize size(10.0, 20.0);
    position.RecordDistance(size);
    BOOST_TEST(position.GetDistance() == 22.360679774997898, tt::tolerance(FLT_EPSILON));

    position.RecordDistance(30.0);
    BOOST_TEST(position.GetDistance() == 30.0, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestRecordAngle, TestFixture) {
    MeaPosition position(ref);

    double angle = 24.6;
    position.RecordAngle(angle);
    BOOST_TEST(position.GetAngle() == angle, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestRecordArea, TestFixture) {
    MeaPosition position(ref);

    MeaFSize size(10.0, 20.0);
    position.RecordRectArea(size);
    BOOST_TEST(position.GetArea() == 200.0, tt::tolerance(FLT_EPSILON));

    position.RecordCircleArea(10.0);
    BOOST_TEST(position.GetArea() == 314.15926535897933, tt::tolerance(FLT_EPSILON));
}

BOOST_FIXTURE_TEST_CASE(TestCopy, TestFixture) {
    MeaPosition position1(ref);
    MeaPosition position2(position1);
    MeaPosition position3 = position1;

    BOOST_TEST(position1 == position2);
    BOOST_TEST(position1 == position3);
}

BOOST_FIXTURE_TEST_CASE(TestEquality, TestFixture) {
    MeaPosition position1(ref);
    MeaPosition position2(position1);
    MeaPosition position3(ref);
    position3.SetDesc(_T("Hello"));

    BOOST_TEST(position1 == position1);
    BOOST_TEST(position1 == position2);
    BOOST_TEST(position1 != position3);
}

BOOST_FIXTURE_TEST_CASE(TestSaveLoad, TestFixture) {
    MeaPosition position1(ref);

    position1.SetToolName(_T("ToolX"));
    position1.SetDesc(_T("Some point"));
    position1.AddPoint(_T("a"), MeaFPoint(17.5, 83.1));
    position1.RecordXY1(MeaFPoint(10.0, 20.0));
    position1.RecordXY2(MeaFPoint(13.45, 12.4));
    position1.RecordXYV(MeaFPoint(18.56, 98.3));
    position1.RecordWH(MeaFSize(10.2, 34.7));
    position1.RecordDistance(4.321);
    position1.RecordAngle(12.0);
    position1.RecordCircleArea(3.2);

    std::ostringstream stream;
    MeaXMLWriter writer(stream);
    writer.StartDocument();
    position1.Save(writer);
    writer.EndDocument();

    MeaXMLParser parser;
    parser.ParseString(stream.str().c_str());

    const MeaXMLNode* positionNode = parser.GetDOM();

    CString idStr;
    CString toolStr;
    CString dateStr;

    positionNode->GetAttributes().GetValueStr(_T("desktopRef"), idStr);
    positionNode->GetAttributes().GetValueStr(_T("tool"), toolStr);
    positionNode->GetAttributes().GetValueStr(_T("date"), dateStr);

    MeaPositionDesktopRef ref2(&counter, idStr);
    MeaPosition position2(ref2, toolStr, dateStr);

    BOOST_TEST(position2 != position1);

    position2.Load(positionNode);
    
    BOOST_TEST(position1 == position2);
}

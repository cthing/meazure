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
#define BOOST_TEST_MODULE PositionLogWriterTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/position/PositionLogWriter.h>
#include <meazure/position/PositionDesktop.h>
#include <meazure/xml/XMLParser.h>
#include <meazure/xml/XMLWriter.h>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include "mocks/MockScreenProvider.h"
#include "mocks/MockUnitsProvider.h"
#include "mocks/MockPositionDesktopRefCounter.h"
#include "mocks/MockPositionProvider.h"
#include <float.h>
#include <regex>
#include <list>

namespace tt = boost::test_tools;


struct TestFixture {
    TestFixture() : unitsProvider(screenProvider), desktop(unitsProvider, screenProvider), ref(&counter, desktop) {}

    MockScreenProvider screenProvider;
    MockUnitsProvider unitsProvider;
    MockPositionProvider positionProvider;
    MeaPositionDesktop desktop;
    MockPositionDesktopRefCounter counter;
    MeaPositionDesktopRef ref;
};


static void verifyElement(const MeaXMLNode* elem, PCTSTR elemName) {
    BOOST_TEST(elem);
    BOOST_TEST(elem->GetType() == MeaXMLNode::Type::Element);
    BOOST_TEST(elem->GetData() == elemName);
}

static void verifyAttr(const MeaXMLNode* elem, PCTSTR attrName, PCTSTR expectedValue) {
    const MeaXMLAttributes& attrs = elem->GetAttributes();

    CString value;
    BOOST_TEST(attrs.GetValueStr(attrName, value));
    BOOST_TEST(value == expectedValue);
}

static void verifyAttr(const MeaXMLNode* elem, PCTSTR attrName, int expectedValue) {
    const MeaXMLAttributes& attrs = elem->GetAttributes();

    int value;
    BOOST_TEST(attrs.GetValueInt(attrName, value));
    BOOST_TEST(value == expectedValue);
}

static void verifyAttr(const MeaXMLNode* elem, PCTSTR attrName, double expectedValue) {
    const MeaXMLAttributes& attrs = elem->GetAttributes();

    double value;
    BOOST_TEST(attrs.GetValueDbl(attrName, value));
    BOOST_TEST(value == expectedValue, tt::tolerance(FLT_EPSILON));
}

static void verifyAttrMatches(const MeaXMLNode* elem, PCTSTR attrName, PCSTR valueRegex) {
    const MeaXMLAttributes& attrs = elem->GetAttributes();

    CString value;
    BOOST_TEST(attrs.GetValueStr(attrName, value));
    std::regex compiledRegex(valueRegex);
    BOOST_TEST(std::regex_match(static_cast<PCTSTR>(value), compiledRegex));
}


BOOST_FIXTURE_TEST_CASE(TestSave, TestFixture) {
    struct TestHandler : public MeaXMLParserHandler {
        bool hadEntity = false;

        xercesc::InputSource* ResolveEntity(const CString& pathname) override {
            hadEntity = true;

            TCHAR fname[_MAX_FNAME], ext[_MAX_EXT];
            _tsplitpath_s(pathname, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, ext, _MAX_EXT);

            BOOST_TEST(CString(fname) == _T("PositionLog1"));
            BOOST_TEST(CString(ext) == _T(".dtd"));

            CStringW widePathname(pathname);
            return new xercesc::LocalFileInputSource(reinterpret_cast<const XMLCh* const>(static_cast<PCWSTR>(widePathname)));
        }

        void ParsingError(const CString& error, const CString&, int line, int col) override {
            std::cerr << "Line: " << line << " Col: " << col << '\n';
            BOOST_FAIL(error);
        }

        void ValidationError(const CString& error, const CString&, int line, int col) override {
            std::cerr << "Line: " << line << " Col: " << col << '\n';
            BOOST_FAIL(error);
        }
    } testHandler;


    PCSTR timestampRegex = u8"[\\d]{4}-[\\d]{2}-[\\d]{2}T[\\d]{2}:[\\d]{2}:[\\d]{2}Z";
    PCSTR guidRegex = u8"[A-Fa-f\\d]{8}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{4}-[A-Fa-f\\d]{12}";

    positionProvider.AddReferencedDesktop(desktop);

    MeaPosition* position1 = new MeaPosition(ref);
    position1->SetToolName(_T("LineTool"));
    position1->AddPoint("1", MeaFPoint(1.0, 2.0));
    position1->AddPoint("2", MeaFPoint(3.0, 7.0));
    position1->RecordWH(MeaFSize(2.0, 5.0));
    position1->SetDesc(_T("Position 1"));
    positionProvider.AddPosition(position1);

    MeaPosition* position2 = new MeaPosition(ref);
    position2->SetToolName(_T("CircleTool"));
    position2->AddPoint("1", MeaFPoint(4.0, 5.0));
    position2->AddPoint("2", MeaFPoint(7.0, 10.0));
    position2->SetDesc(_T("Position 2"));
    positionProvider.AddPosition(position2);

    MeaPosition* position3 = new MeaPosition(ref);
    position3->SetToolName(_T("AngleTool"));
    position3->AddPoint("1", MeaFPoint(1.0, 2.0));
    position3->AddPoint("2", MeaFPoint(3.0, 7.5));
    position3->AddPoint("v", MeaFPoint(6.0, 9.0));
    position3->RecordAngle(20.0);
    positionProvider.AddPosition(position3);

    std::ostringstream stream;
    MeaXMLWriter writer(stream);
    MeaPositionLogWriter logWriter(writer, positionProvider);
    BOOST_CHECK_NO_THROW(logWriter.Save());

    //std::cerr << stream.str();

    MeaXMLParser parser(&testHandler, true);
    BOOST_CHECK_NO_THROW(parser.ParseString(stream.str().c_str()));
    BOOST_TEST(testHandler.hadEntity);

    const MeaXMLNode* positionLogElem = parser.GetDOM();
    verifyElement(positionLogElem, _T("positionLog"));
    verifyAttr(positionLogElem, _T("version"), 1);

    const MeaXMLNode* infoElem = positionLogElem->FindChildElement(_T("info"));
    verifyElement(infoElem, _T("info"));

    const MeaXMLNode* titleElem = infoElem->FindChildElement(_T("title"));
    verifyElement(titleElem, _T("title"));
    BOOST_TEST(titleElem->GetChildData() == _T("Test Title"));

    const MeaXMLNode* descElem = infoElem->FindChildElement(_T("desc"));
    verifyElement(descElem, _T("desc"));
    BOOST_TEST(descElem->GetChildData() == _T("This is a test"));

    const MeaXMLNode* createdElem = infoElem->FindChildElement(_T("created"));
    verifyElement(createdElem, _T("created"));
    verifyAttrMatches(createdElem, _T("date"), timestampRegex);

    const MeaXMLNode* generatorElem = infoElem->FindChildElement(_T("generator"));
    verifyElement(generatorElem, _T("generator"));
    verifyAttr(generatorElem, _T("name"), _T("PositionLogWriterTest"));
    verifyAttr(generatorElem, _T("version"), _T("1.2.3"));
    verifyAttr(generatorElem, _T("build"), 4);

    const MeaXMLNode* machineElem = infoElem->FindChildElement(_T("machine"));
    verifyElement(machineElem, _T("machine"));
    verifyAttrMatches(generatorElem, _T("name"), _T("[\\S]+"));

    const MeaXMLNode* desktopsElem = positionLogElem->FindChildElement(_T("desktops"));
    verifyElement(desktopsElem, _T("desktops"));

    const MeaXMLNode* desktopElem = desktopsElem->FindChildElement(_T("desktop"));
    verifyElement(desktopElem, _T("desktop"));
    verifyAttrMatches(desktopElem, _T("id"), guidRegex);

    const MeaXMLNode* unitsElem = desktopElem->FindChildElement(_T("units"));
    verifyElement(unitsElem, _T("units"));
    verifyAttr(unitsElem, _T("length"), _T("px"));
    verifyAttr(unitsElem, _T("angle"), _T("deg"));

    const MeaXMLNode* originElem = desktopElem->FindChildElement(_T("origin"));
    verifyElement(originElem, _T("origin"));
    verifyAttr(originElem, _T("xoffset"), 0.0);
    verifyAttr(originElem, _T("yoffset"), 0.0);
    verifyAttr(originElem, _T("invertY"), _T("false"));

    const MeaXMLNode* sizeElem = desktopElem->FindChildElement(_T("size"));
    verifyElement(sizeElem, _T("size"));
    verifyAttr(sizeElem, _T("x"), 1280.0);
    verifyAttr(sizeElem, _T("y"), 1024.0);

    const MeaXMLNode* screensElem = desktopElem->FindChildElement(_T("screens"));
    verifyElement(screensElem, _T("screens"));

    const MeaXMLNode* screenElem = screensElem->FindChildElement(_T("screen"));
    verifyElement(screenElem, _T("screen"));
    verifyAttr(screenElem, _T("desc"), _T("MockScreen"));
    verifyAttr(screenElem, _T("primary"), _T("true"));

    const MeaXMLNode* rectElem = screenElem->FindChildElement(_T("rect"));
    verifyElement(rectElem, _T("rect"));
    verifyAttr(rectElem, _T("top"), 0.0);
    verifyAttr(rectElem, _T("bottom"), 1024.0);
    verifyAttr(rectElem, _T("left"), 0.0);
    verifyAttr(rectElem, _T("right"), 1280.0);

    const MeaXMLNode* resolutionElem = screenElem->FindChildElement(_T("resolution"));
    verifyElement(resolutionElem, _T("resolution"));
    verifyAttr(resolutionElem, _T("x"), 96.0);
    verifyAttr(resolutionElem, _T("y"), 96.0);
    verifyAttr(resolutionElem, _T("manual"), _T("false"));

    const MeaXMLNode* positionsElem = positionLogElem->FindChildElement(_T("positions"));
    verifyElement(positionsElem, _T("positions"));

    MeaXMLNode::NodeList_c positionElems = positionsElem->FindChildElements(_T("position"));
    BOOST_TEST(positionElems.size() == 3);

    MeaXMLNode::NodeList_c::const_iterator positionIter = positionElems.begin();

    // Position 1

    const MeaXMLNode* position1Elem = *positionIter++;
    verifyElement(position1Elem, _T("position"));
    verifyAttrMatches(position1Elem, _T("desktopRef"), guidRegex);
    verifyAttr(position1Elem, _T("tool"), _T("LineTool"));
    verifyAttrMatches(position1Elem, _T("date"), timestampRegex);

    const MeaXMLNode* position1DescElem = position1Elem->FindChildElement(_T("desc"));
    verifyElement(position1DescElem, _T("desc"));
    BOOST_TEST(position1DescElem->GetChildData() == _T("Position 1"));

    const MeaXMLNode* position1PointsElem = position1Elem->FindChildElement(_T("points"));
    verifyElement(position1PointsElem, "points");

    MeaXMLNode::NodeList_c position1PointElems = position1PointsElem->FindChildElements(_T("point"));
    BOOST_TEST(position1PointElems.size() == 2);

    MeaXMLNode::NodeList_c::const_iterator position1PointIter = position1PointElems.begin();

    const MeaXMLNode* position1Point1Elem = *position1PointIter++;
    verifyElement(position1Point1Elem, _T("point"));
    verifyAttr(position1Point1Elem, _T("name"), _T("1"));
    verifyAttr(position1Point1Elem, _T("x"), 1.0);
    verifyAttr(position1Point1Elem, _T("y"), 2.0);

    const MeaXMLNode* position1Point2Elem = *position1PointIter++;
    verifyElement(position1Point2Elem, _T("point"));
    verifyAttr(position1Point2Elem, _T("name"), _T("2"));
    verifyAttr(position1Point2Elem, _T("x"), 3.0);
    verifyAttr(position1Point2Elem, _T("y"), 7.0);

    const MeaXMLNode* position1PropertiesElem = position1Elem->FindChildElement(_T("properties"));
    verifyElement(position1PropertiesElem, "properties");

    const MeaXMLNode* position1WidthElem = position1PropertiesElem->FindChildElement(_T("width"));
    verifyElement(position1WidthElem, _T("width"));
    verifyAttr(position1WidthElem, _T("value"), 2.0);

    const MeaXMLNode* position1HeightElem = position1PropertiesElem->FindChildElement(_T("height"));
    verifyElement(position1HeightElem, _T("height"));
    verifyAttr(position1HeightElem, _T("value"), 5.0);

    // Position 2

    const MeaXMLNode* position2Elem = *positionIter++;
    verifyElement(position2Elem, _T("position"));
    verifyAttrMatches(position2Elem, _T("desktopRef"), guidRegex);
    verifyAttr(position2Elem, _T("tool"), _T("CircleTool"));
    verifyAttrMatches(position2Elem, _T("date"), timestampRegex);

    const MeaXMLNode* position2DescElem = position2Elem->FindChildElement(_T("desc"));
    verifyElement(position2DescElem, _T("desc"));
    BOOST_TEST(position2DescElem->GetChildData() == _T("Position 2"));

    const MeaXMLNode* position2PointsElem = position2Elem->FindChildElement(_T("points"));
    verifyElement(position2PointsElem, "points");

    MeaXMLNode::NodeList_c position2PointElems = position2PointsElem->FindChildElements(_T("point"));
    BOOST_TEST(position2PointElems.size() == 2);

    MeaXMLNode::NodeList_c::const_iterator position2PointIter = position2PointElems.begin();

    const MeaXMLNode* position2Point1Elem = *position2PointIter++;
    verifyElement(position2Point1Elem, _T("point"));
    verifyAttr(position2Point1Elem, _T("name"), _T("1"));
    verifyAttr(position2Point1Elem, _T("x"), 4.0);
    verifyAttr(position2Point1Elem, _T("y"), 5.0);

    const MeaXMLNode* position2Point2Elem = *position2PointIter++;
    verifyElement(position2Point2Elem, _T("point"));
    verifyAttr(position2Point2Elem, _T("name"), _T("2"));
    verifyAttr(position2Point2Elem, _T("x"), 7.0);
    verifyAttr(position2Point2Elem, _T("y"), 10.0);

    // Position 3

    const MeaXMLNode* position3Elem = *positionIter++;
    verifyElement(position3Elem, _T("position"));
    verifyAttrMatches(position3Elem, _T("desktopRef"), guidRegex);
    verifyAttr(position3Elem, _T("tool"), _T("AngleTool"));
    verifyAttrMatches(position3Elem, _T("date"), timestampRegex);

    const MeaXMLNode* position3DescElem = position3Elem->FindChildElement(_T("desc"));
    BOOST_TEST(position3DescElem == nullptr);

    const MeaXMLNode* position3PointsElem = position3Elem->FindChildElement(_T("points"));
    verifyElement(position3PointsElem, "points");

    MeaXMLNode::NodeList_c position3PointElems = position3PointsElem->FindChildElements(_T("point"));
    BOOST_TEST(position3PointElems.size() == 3);

    MeaXMLNode::NodeList_c::const_iterator position3PointIter = position3PointElems.begin();

    const MeaXMLNode* position3Point1Elem = *position3PointIter++;
    verifyElement(position3Point1Elem, _T("point"));
    verifyAttr(position3Point1Elem, _T("name"), _T("1"));
    verifyAttr(position3Point1Elem, _T("x"), 1.0);
    verifyAttr(position3Point1Elem, _T("y"), 2.0);

    const MeaXMLNode* position3Point2Elem = *position3PointIter++;
    verifyElement(position3Point2Elem, _T("point"));
    verifyAttr(position3Point2Elem, _T("name"), _T("2"));
    verifyAttr(position3Point2Elem, _T("x"), 3.0);
    verifyAttr(position3Point2Elem, _T("y"), 7.5);

    const MeaXMLNode* position3Point3Elem = *position3PointIter++;
    verifyElement(position3Point3Elem, _T("point"));
    verifyAttr(position3Point3Elem, _T("name"), _T("v"));
    verifyAttr(position3Point3Elem, _T("x"), 6.0);
    verifyAttr(position3Point3Elem, _T("y"), 9.0);

    const MeaXMLNode* position3PropertiesElem = position3Elem->FindChildElement(_T("properties"));
    verifyElement(position3PropertiesElem, "properties");

    const MeaXMLNode* position3AngleElem = position3PropertiesElem->FindChildElement(_T("angle"));
    verifyElement(position3AngleElem, _T("angle"));
    verifyAttr(position3AngleElem, _T("value"), 20.0);
}

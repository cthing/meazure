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
#include "mocks/MockScreenProvider.h"
#include <Units.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;
using namespace boost::unit_test;
using boost::unit_test_framework::test_suite;


BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaUnits::DisplayPrecisions)
BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaUnits::DisplayPrecisionNames)
BOOST_TEST_DONT_PRINT_LOG_VALUE(POINT)


bool operator==(const POINT& lhs, const POINT& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}


namespace {
    const MeaUnits::DisplayPrecisionNames linearPrecisionNames { "x", "y", "w", "h", "d", "area", "rx", "ry" };

    void VerifyUnitsId(MeaAngularUnits& units, MeaAngularUnitsId unitsId, LPCTSTR unitsStr) {
        BOOST_CHECK_EQUAL(units.GetUnitsStr(), unitsStr);
        BOOST_CHECK_EQUAL(units.GetUnitsId(), unitsId);
    }

    void VerifyUnitsId(MeaLinearUnits& units, MeaLinearUnitsId unitsId, LPCTSTR unitsStr) {
        BOOST_CHECK_EQUAL(units.GetUnitsStr(), unitsStr);
        BOOST_CHECK_EQUAL(units.GetUnitsId(), unitsId);
    }

    void VerifyPrecisions(MeaUnits& units, const MeaUnits::DisplayPrecisions& defaultPrecisions,
                          const MeaUnits::DisplayPrecisionNames names) {
        BOOST_CHECK_EQUAL(units.GetDisplayPrecisions(), defaultPrecisions);
        BOOST_CHECK_EQUAL(units.GetDefaultPrecisions(), defaultPrecisions);

        MeaUnits::DisplayPrecisions precisions { 10, 20, 30 };
        units.SetDisplayPrecisions(precisions);
        BOOST_CHECK_EQUAL(units.GetDisplayPrecisions(), precisions);

        units.RestoreDefaultPrecisions();
        BOOST_CHECK_EQUAL(units.GetDisplayPrecisions(), defaultPrecisions);

        BOOST_CHECK_EQUAL(units.GetDisplayPrecisionNames(), names);
    }

    void VerifyFormat(MeaAngularUnits& units, double value, LPCTSTR valueStr) {
        BOOST_CHECK_EQUAL(units.Format(MeaA, value), valueStr);
    }

    void VerifyFormat(MeaLinearUnits& units, MeaLinearMeasurementId measurementId, double value, LPCTSTR valueStr) {
        BOOST_CHECK_EQUAL(units.Format(measurementId, value), valueStr);
    }

    void VerifyConvertAngle(MeaAngularUnits& units, double radians, double expected) {
        BOOST_CHECK_CLOSE(units.ConvertAngle(radians), expected, 0.0001);
    }

    void VerifyInvertY(MeaLinearUnits& units) {
        BOOST_CHECK(units.GetInvertY() == false);
        units.SetInvertY(true);
        BOOST_CHECK(units.GetInvertY());
        units.SetInvertY(false);
    }

    void VerifyOrigin(MeaLinearUnits& units) {
        POINT origin = { 0, 0 };
        BOOST_CHECK_EQUAL(units.GetOrigin(), origin);

        POINT newOrigin = { 100, 200 };
        units.SetOrigin(newOrigin);
        BOOST_CHECK_EQUAL(units.GetOrigin(), newOrigin);
        
        units.SetOrigin(origin);
    }

    void VerifyRequiresRes(const MeaLinearUnits& units, bool requiresRes) {
        BOOST_CHECK(units.RequiresRes() == requiresRes);
    }

    void VerifyConvertCoords(MeaLinearUnits& units, const FPOINT& corrected, const FPOINT& uncorrected) {
        POINT newOrigin = { 100, 200 };
        units.SetOrigin(newOrigin);
        units.SetInvertY(true);
        POINT pos = { 110, 220 };
        FPOINT result = units.ConvertCoord(pos);
        BOOST_CHECK_CLOSE(result.x, corrected.x, 0.0001);
        BOOST_CHECK_CLOSE(result.y, corrected.y, 0.0001);
        result = units.ConvertPos(pos);
        BOOST_CHECK_CLOSE(result.x, uncorrected.x, 0.0001);
        BOOST_CHECK_CLOSE(result.y, uncorrected.y, 0.0001);

        BOOST_CHECK_CLOSE(units.UnconvertCoord(MeaConvertX, nullptr, corrected.x), 110.0, 0.0001);
        BOOST_CHECK_CLOSE(units.UnconvertCoord(MeaConvertY, nullptr, corrected.y), 220.0, 0.0001);

        int c1, c2;
        BOOST_CHECK(units.UnconvertCoord(MeaConvertX, nullptr, corrected.x, c1, c2));
        BOOST_CHECK_EQUAL(c1, 110);
        BOOST_CHECK_EQUAL(c2, 111);
        BOOST_CHECK(units.UnconvertCoord(MeaConvertY, nullptr, corrected.y, c1, c2));
        BOOST_CHECK_EQUAL(c1, 220);
        BOOST_CHECK_EQUAL(c2, 221);

        POINT convertedPos = { 110, 220 };
        BOOST_CHECK_EQUAL(units.UnconvertCoord(corrected), convertedPos);
        BOOST_CHECK_EQUAL(units.UnconvertPos(uncorrected), convertedPos);

        newOrigin = { 0, 0 };
        units.SetOrigin(newOrigin);
        units.SetInvertY(false);
    }

    void VerifyConvertRes(MeaLinearUnits& units, const FSIZE& convertedRes) {
        FSIZE res = { 100.0, 90.0 };
        FSIZE result = units.ConvertRes(res);
        BOOST_CHECK_CLOSE(result.cx, convertedRes.cx, 0.0001);
        BOOST_CHECK_CLOSE(result.cy, convertedRes.cy, 0.0001);
    }

    void VerifyToPixels(MeaLinearUnits& units, double x, double y, int convertedX, int convertedY) {
        FSIZE res = { 100.0, 90.0 };
        BOOST_CHECK_EQUAL(units.ConvertToPixels(res, x, 1).cx, convertedX);
        BOOST_CHECK_EQUAL(units.ConvertToPixels(res, 0.0, 100).cx, 100);
        BOOST_CHECK_EQUAL(units.ConvertToPixels(res, y, 1).cy, convertedY);
    }

    void VerifyFromPixels(MeaLinearUnits& units, double xfactor, double yfactor) {
        FSIZE res = { 100.0, 90.0 };
        FSIZE result = units.FromPixels(res);
        BOOST_CHECK_CLOSE(result.cx, xfactor, 0.0001);
        BOOST_CHECK_CLOSE(result.cy, yfactor, 0.0001);
    }


    //=========================================================================


    void TestDegreeUnits() {
        MeaDegreeUnits units;
        
        VerifyUnitsId(units, MeaDegreesId, "deg");

        MeaUnits::DisplayPrecisions defaultPrecisions { 1 };
        MeaUnits::DisplayPrecisionNames names { "angle" };
        VerifyPrecisions(units, defaultPrecisions, names);

        VerifyFormat(units, 90.12345, "90.1");

        VerifyConvertAngle(units, 1.0, 57.2958);
        VerifyConvertAngle(units, 0.0, 0.0);
        VerifyConvertAngle(units, 3.14159265359, 180.0);
    }

    void TestRadianUnits() {
        MeaRadianUnits units;
        
        VerifyUnitsId(units, MeaRadiansId, "rad");

        MeaUnits::DisplayPrecisions defaultPrecisions { 3 };
        MeaUnits::DisplayPrecisionNames names { "angle" };
        VerifyPrecisions(units, defaultPrecisions, names);

        VerifyFormat(units, 1.12345, "1.123");

        VerifyConvertAngle(units, 1.0, 1.0);
        VerifyConvertAngle(units, 0.0, 0.0);
        VerifyConvertAngle(units, 3.14159265359, 3.141593);
    }

    void TestPixelUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaPixelUnits units(*screenProvider);
        
        VerifyUnitsId(units, MeaPixelsId, "px");

        MeaUnits::DisplayPrecisions defaultPrecisions { 0, 0, 0, 0, 1, 0, 1, 1 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);
        VerifyRequiresRes(units, false);

        VerifyFormat(units, MeaX, 90.12345, "90");
        VerifyFormat(units, MeaD, 90.12345, "90.1");

        FPOINT correctedPos = { 10.0, -20.0 };
        FPOINT uncorrectedPos = { 110.0, 220.0 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE res = { 100.0, 90.0 };
        VerifyConvertRes(units, res);

        VerifyToPixels(units, 10.0, 20.0, 10, 20);
        VerifyFromPixels(units, 1.0, 1.0);
    }

    void TestPointUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaPointUnits units(*screenProvider);
        
        VerifyUnitsId(units, MeaPointsId, "pt");

        MeaUnits::DisplayPrecisions defaultPrecisions { 1, 1, 1, 1, 1, 1, 2, 2 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);
        VerifyRequiresRes(units, true);

        VerifyFormat(units, MeaX, 90.12345, "90.1");
        VerifyFormat(units, MeaRx, 90.12345, "90.12");

        FPOINT correctedPos = { 7.5, -15.0 };
        FPOINT uncorrectedPos = { 82.5, 165.0 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE convertedRes = { 1.3888888889, 1.25 };
        VerifyConvertRes(units, convertedRes);

        VerifyToPixels(units, 15.0, 15.0, 20, 18);
        VerifyFromPixels(units, 0.72, 0.8);
    }

    void TestPicaUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaPicaUnits units(*screenProvider);

        VerifyUnitsId(units, MeaPicasId, "pc");

        MeaUnits::DisplayPrecisions defaultPrecisions { 2, 2, 2, 2, 2, 2, 1, 1 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);
        VerifyRequiresRes(units, true);

        VerifyFormat(units, MeaX, 90.12345, "90.12");
        VerifyFormat(units, MeaRx, 90.12345, "90.1");

        FPOINT correctedPos = { 0.625, -1.25 };
        FPOINT uncorrectedPos = { 6.875, 13.75 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE convertedRes = { 16.66666667, 15.0 };
        VerifyConvertRes(units, convertedRes);

        VerifyToPixels(units, 1.25, 1.25, 20, 18);
        VerifyFromPixels(units, 0.06, 0.066666667);
    }

    void TestTwipUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaTwipUnits units(*screenProvider);

        VerifyUnitsId(units, MeaTwipsId, "tp");

        MeaUnits::DisplayPrecisions defaultPrecisions { 0, 0, 0, 0, 0, 0, 4, 4 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);
        VerifyRequiresRes(units, true);

        VerifyFormat(units, MeaX, 90.12345, "90");
        VerifyFormat(units, MeaRx, 90.123456, "90.1235");

        FPOINT correctedPos = { 150.0, -300.0 };
        FPOINT uncorrectedPos = { 1650.0, 3300.0 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE convertedRes = { 0.0694444444, 0.0625 };
        VerifyConvertRes(units, convertedRes);

        VerifyToPixels(units, 5000, 2000, 347, 125);
        VerifyFromPixels(units, 14.4, 16.0);
    }

    void TestInchUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaInchUnits units(*screenProvider);

        VerifyUnitsId(units, MeaInchesId, "in");

        MeaUnits::DisplayPrecisions defaultPrecisions { 3, 3, 3, 3, 3, 3, 1, 1 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);
        VerifyRequiresRes(units, true);

        VerifyFormat(units, MeaX, 90.12345, "90.123");
        VerifyFormat(units, MeaRx, 90.123456, "90.1");

        FPOINT correctedPos = { 0.10416666666666666, -0.20833333333333331 };
        FPOINT uncorrectedPos = { 1.1458333333333333, 2.2916666666666665 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE convertedRes = { 100.0, 90.0 };
        VerifyConvertRes(units, convertedRes);

        VerifyToPixels(units, 50, 20, 5000, 1800);
        VerifyFromPixels(units, 0.01, 0.011111111111111112);
    }

    void TestCentimeterUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaCentimeterUnits units(*screenProvider);

        VerifyUnitsId(units, MeaCentimetersId, "cm");

        MeaUnits::DisplayPrecisions defaultPrecisions { 2, 2, 2, 2, 2, 2, 1, 1 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);
        VerifyRequiresRes(units, true);

        VerifyFormat(units, MeaX, 90.12345, "90.12");
        VerifyFormat(units, MeaRx, 90.123456, "90.1");

        FPOINT correctedPos = { 0.26458333333333334, -0.52916666666666667 };
        FPOINT uncorrectedPos = { 2.9104166666666669, 5.8208333333333337 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE convertedRes = { 39.370078740157481, 35.433070866141733 };
        VerifyConvertRes(units, convertedRes);

        VerifyToPixels(units, 50, 20, 1968, 708);
        VerifyFromPixels(units, 0.025399999999999999, 0.028222222222222221);
    }

    void TestMillimeterUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaMillimeterUnits units(*screenProvider);

        VerifyUnitsId(units, MeaMillimetersId, "mm");

        MeaUnits::DisplayPrecisions defaultPrecisions { 1, 1, 1, 1, 1, 1, 2, 2 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);
        VerifyRequiresRes(units, true);

        VerifyFormat(units, MeaX, 90.12345, "90.1");
        VerifyFormat(units, MeaRx, 90.123456, "90.12");

        FPOINT correctedPos = { 2.6458333333333334, -5.2916666666666667 };
        FPOINT uncorrectedPos = { 29.104166666666669, 58.208333333333337 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE convertedRes = { 3.9370078740157481, 3.5433070866141733 };
        VerifyConvertRes(units, convertedRes);

        VerifyToPixels(units, 50, 20, 196, 70);
        VerifyFromPixels(units, 0.254, 0.28222222222222221);
    }

    void TestCustomUnits() {
        MockScreenProvider* screenProvider = new MockScreenProvider();
        MeaCustomUnits units(*screenProvider, &MockScreenProvider::ChangeLabel);

        MockScreenProvider::changeLabelCalled = false;
        BOOST_CHECK(units.HaveCustomUnits() == false);
        units.SetName("Abcd");
        units.SetAbbrev("ab");
        BOOST_CHECK(MockScreenProvider::changeLabelCalled);
        BOOST_CHECK_EQUAL(MockScreenProvider::changeLabelUnits, MeaCustomId);
        BOOST_CHECK_EQUAL(MockScreenProvider::changeLabel, "ab");
        BOOST_CHECK(units.HaveCustomUnits());
        BOOST_CHECK_EQUAL(units.GetName(), "Abcd");
        BOOST_CHECK_EQUAL(units.GetAbbrev(), "ab");

        BOOST_CHECK_EQUAL(units.GetScaleBasis(), MeaCustomUnits::PixelBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasisStr(), "px");
        BOOST_CHECK(units.RequiresRes() == false);
        units.SetScaleBasis(MeaCustomUnits::CentimeterBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasis(), MeaCustomUnits::CentimeterBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasisStr(), "cm");
        BOOST_CHECK(units.RequiresRes());
        units.SetScaleBasis(MeaCustomUnits::InchBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasis(), MeaCustomUnits::InchBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasisStr(), "in");
        BOOST_CHECK(units.RequiresRes());

        units.SetScaleBasis("px");
        BOOST_CHECK_EQUAL(units.GetScaleBasis(), MeaCustomUnits::PixelBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasisStr(), "px");
        BOOST_CHECK(units.RequiresRes() == false);
        units.SetScaleBasis("cm");
        BOOST_CHECK_EQUAL(units.GetScaleBasis(), MeaCustomUnits::CentimeterBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasisStr(), "cm");
        BOOST_CHECK(units.RequiresRes());
        units.SetScaleBasis("in");
        BOOST_CHECK_EQUAL(units.GetScaleBasis(), MeaCustomUnits::InchBasis);
        BOOST_CHECK_EQUAL(units.GetScaleBasisStr(), "in");
        BOOST_CHECK(units.RequiresRes());

        BOOST_CHECK_CLOSE(units.GetScaleFactor(), 1.0, 0.0001);
        units.SetScaleFactor(2.0);
        BOOST_CHECK_CLOSE(units.GetScaleFactor(), 2.0, 0.0001);

        VerifyUnitsId(units, MeaCustomId, "custom");

        MeaUnits::DisplayPrecisions defaultPrecisions { 0, 0, 0, 0, 1, 0, 1, 1 };
        VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

        VerifyInvertY(units);
        VerifyOrigin(units);

        VerifyFormat(units, MeaX, 90.12345, "90");
        VerifyFormat(units, MeaRx, 90.123456, "90.1");

        FPOINT correctedPos = { 0.0520833333333333294, -0.10416666666666666 };
        FPOINT uncorrectedPos = { 0.57291666666666663, 1.1458333333333333 };
        VerifyConvertCoords(units, correctedPos, uncorrectedPos);

        FSIZE convertedRes = { 200.0, 180.0 };
        VerifyConvertRes(units, convertedRes);

        VerifyToPixels(units, 50, 20, 10000, 3600);
        VerifyFromPixels(units, 0.0050000000000000001, 0.0055555555555555558);
    }
}

test_suite* init_unit_test_suite(int, char* []) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    test_suite* angularUnitsTestSuite = BOOST_TEST_SUITE("Angular Units Tests");
    angularUnitsTestSuite->add(BOOST_TEST_CASE(&TestDegreeUnits));
    angularUnitsTestSuite->add(BOOST_TEST_CASE(&TestRadianUnits));
    framework::master_test_suite().add(angularUnitsTestSuite);

    test_suite* linearUnitsTestSuite = BOOST_TEST_SUITE("Linear Units Tests");
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestPixelUnits));
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestPointUnits));
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestPicaUnits));
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestTwipUnits));
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestInchUnits));
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestCentimeterUnits));
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestMillimeterUnits));
    linearUnitsTestSuite->add(BOOST_TEST_CASE(&TestCustomUnits));
    framework::master_test_suite().add(linearUnitsTestSuite);

    return nullptr;
}

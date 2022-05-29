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
#define BOOST_TEST_MODULE UnitsTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include "mocks/MockScreenProvider.h"
#include <meazure/units/Units.h>

namespace bt = boost::unit_test;
namespace tt = boost::test_tools;

BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaUnits::DisplayPrecisions)
BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaUnits::DisplayPrecisionNames)
BOOST_TEST_DONT_PRINT_LOG_VALUE(POINT)


bool operator==(const POINT& lhs, const POINT& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}


const MeaUnits::DisplayPrecisionNames linearPrecisionNames { "x", "y", "w", "h", "d", "area", "rx", "ry" };

void VerifyUnitsId(MeaAngularUnits& units, MeaAngularUnitsId unitsId, LPCTSTR unitsStr) {
    BOOST_TEST(units.GetUnitsStr() == unitsStr);
    BOOST_TEST(units.GetUnitsId() == unitsId);
}

void VerifyUnitsId(MeaLinearUnits& units, MeaLinearUnitsId unitsId, LPCTSTR unitsStr) {
    BOOST_TEST(units.GetUnitsStr() == unitsStr);
    BOOST_TEST(units.GetUnitsId() == unitsId);
}

void VerifyPrecisions(MeaUnits& units, const MeaUnits::DisplayPrecisions& defaultPrecisions,
                        const MeaUnits::DisplayPrecisionNames names) {
    BOOST_TEST(units.GetDisplayPrecisions() == defaultPrecisions);
    BOOST_TEST(units.GetDefaultPrecisions() == defaultPrecisions);

    MeaUnits::DisplayPrecisions precisions { 10, 20, 30 };
    units.SetDisplayPrecisions(precisions);
    BOOST_TEST(units.GetDisplayPrecisions() == precisions);

    units.RestoreDefaultPrecisions();
    BOOST_TEST(units.GetDisplayPrecisions() == defaultPrecisions);

    BOOST_TEST(units.GetDisplayPrecisionNames() == names);
}

void VerifyFormat(MeaAngularUnits& units, double value, LPCTSTR valueStr) {
    BOOST_TEST(units.Format(MeaA, value) == valueStr);
}

void VerifyFormat(MeaLinearUnits& units, MeaLinearMeasurementId measurementId, double value, LPCTSTR valueStr) {
    BOOST_TEST(units.Format(measurementId, value) == valueStr);
}

void VerifyConvertAngle(MeaAngularUnits& units, double radians, double expected) {
    BOOST_TEST(units.ConvertAngle(radians) == expected, tt::tolerance(0.000001));
}

void VerifyInvertY(MeaLinearUnits& units) {
    BOOST_TEST(!units.GetInvertY());
    units.SetInvertY(true);
    BOOST_TEST(units.GetInvertY());
    units.SetInvertY(false);
}

void VerifyOrigin(MeaLinearUnits& units) {
    POINT origin = { 0, 0 };
    BOOST_TEST(units.GetOrigin() == origin);

    POINT newOrigin = { 100, 200 };
    units.SetOrigin(newOrigin);
    BOOST_TEST(units.GetOrigin() == newOrigin);
        
    units.SetOrigin(origin);
}

void VerifyRequiresRes(const MeaLinearUnits& units, bool requiresRes) {
    BOOST_TEST(units.RequiresRes() == requiresRes);
}

void VerifyConvertCoords(MeaLinearUnits& units, const FPOINT& corrected, const FPOINT& uncorrected) {
    POINT newOrigin = { 100, 200 };
    units.SetOrigin(newOrigin);
    units.SetInvertY(true);
    POINT pos = { 110, 220 };
    FPOINT result = units.ConvertCoord(pos);
    BOOST_TEST(result.x == corrected.x, tt::tolerance(0.0001));
    BOOST_TEST(result.y == corrected.y, tt::tolerance(0.0001));
    result = units.ConvertPos(pos);
    BOOST_TEST(result.x == uncorrected.x, tt::tolerance(0.0001));
    BOOST_TEST(result.y == uncorrected.y, tt::tolerance(0.0001));

    BOOST_TEST(units.UnconvertCoord(MeaConvertX, nullptr, corrected.x) == 110.0, tt::tolerance(0.0001));
    BOOST_TEST(units.UnconvertCoord(MeaConvertY, nullptr, corrected.y) == 220.0, tt::tolerance(0.0001));

    int c1, c2;
    BOOST_TEST(units.UnconvertCoord(MeaConvertX, nullptr, corrected.x, c1, c2));
    BOOST_TEST(c1 == 110);
    BOOST_TEST(c2 == 111);
    BOOST_TEST(units.UnconvertCoord(MeaConvertY, nullptr, corrected.y, c1, c2));
    BOOST_TEST(c1 == 220);
    BOOST_TEST(c2 == 221);

    POINT convertedPos = { 110, 220 };
    BOOST_TEST(units.UnconvertCoord(corrected) == convertedPos);
    BOOST_TEST(units.UnconvertPos(uncorrected) == convertedPos);

    newOrigin = { 0, 0 };
    units.SetOrigin(newOrigin);
    units.SetInvertY(false);
}

void VerifyConvertRes(MeaLinearUnits& units, const FSIZE& convertedRes) {
    FSIZE res(100.0, 90.0);
    FSIZE result = units.ConvertRes(res);
    BOOST_TEST(result.cx == convertedRes.cx, tt::tolerance(0.0001));
    BOOST_TEST(result.cy == convertedRes.cy, tt::tolerance(0.0001));
}

void VerifyToPixels(MeaLinearUnits& units, double x, double y, int convertedX, int convertedY) {
    FSIZE res(100.0, 90.0);
    BOOST_TEST(units.ConvertToPixels(res, x, 1).cx == convertedX);
    BOOST_TEST(units.ConvertToPixels(res, 0.0, 100).cx == 100);
    BOOST_TEST(units.ConvertToPixels(res, y, 1).cy == convertedY);
}

void VerifyFromPixels(MeaLinearUnits& units, double xfactor, double yfactor) {
    FSIZE res(100.0, 90.0);
    FSIZE result = units.FromPixels(res);
    BOOST_TEST(result.cx == xfactor, tt::tolerance(0.0001));
    BOOST_TEST(result.cy == yfactor, tt::tolerance(0.0001));
}


//=========================================================================


BOOST_AUTO_TEST_CASE(TestDegreeUnits) {
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

BOOST_AUTO_TEST_CASE(TestRadianUnits) {
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

BOOST_AUTO_TEST_CASE(TestPixelUnits) {
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

    FPOINT correctedPos(10.0, -20.0);
    FPOINT uncorrectedPos(110.0, 220.0);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE res(100.0, 90.0);
    VerifyConvertRes(units, res);

    VerifyToPixels(units, 10.0, 20.0, 10, 20);
    VerifyFromPixels(units, 1.0, 1.0);
}

BOOST_AUTO_TEST_CASE(TestPointUnits) {
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

    FPOINT correctedPos(7.5, -15.0);
    FPOINT uncorrectedPos(82.5, 165.0);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE convertedRes(1.3888888889, 1.25);
    VerifyConvertRes(units, convertedRes);

    VerifyToPixels(units, 15.0, 15.0, 20, 18);
    VerifyFromPixels(units, 0.72, 0.8);
}

BOOST_AUTO_TEST_CASE(TestPicaUnits) {
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

    FPOINT correctedPos(0.625, -1.25);
    FPOINT uncorrectedPos(6.875, 13.75);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE convertedRes(16.66666667, 15.0);
    VerifyConvertRes(units, convertedRes);

    VerifyToPixels(units, 1.25, 1.25, 20, 18);
    VerifyFromPixels(units, 0.06, 0.066666667);
}

BOOST_AUTO_TEST_CASE(TestTwipUnits) {
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

    FPOINT correctedPos(150.0, -300.0);
    FPOINT uncorrectedPos(1650.0, 3300.0);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE convertedRes(0.0694444444, 0.0625);
    VerifyConvertRes(units, convertedRes);

    VerifyToPixels(units, 5000, 2000, 347, 125);
    VerifyFromPixels(units, 14.4, 16.0);
}

BOOST_AUTO_TEST_CASE(TestInchUnits) {
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

    FPOINT correctedPos(0.10416666666666666, -0.20833333333333331);
    FPOINT uncorrectedPos(1.1458333333333333, 2.2916666666666665);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE convertedRes(100.0, 90.0);
    VerifyConvertRes(units, convertedRes);

    VerifyToPixels(units, 50, 20, 5000, 1800);
    VerifyFromPixels(units, 0.01, 0.011111111111111112);
}

BOOST_AUTO_TEST_CASE(TestCentimeterUnits) {
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

    FPOINT correctedPos(0.26458333333333334, -0.52916666666666667);
    FPOINT uncorrectedPos(2.9104166666666669, 5.8208333333333337);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE convertedRes = { 39.370078740157481, 35.433070866141733 };
    VerifyConvertRes(units, convertedRes);

    VerifyToPixels(units, 50, 20, 1968, 708);
    VerifyFromPixels(units, 0.025399999999999999, 0.028222222222222221);
}

BOOST_AUTO_TEST_CASE(TestMillimeterUnits) {
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

    FPOINT correctedPos(2.6458333333333334, -5.2916666666666667);
    FPOINT uncorrectedPos(29.104166666666669, 58.208333333333337);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE convertedRes(3.9370078740157481, 3.5433070866141733);
    VerifyConvertRes(units, convertedRes);

    VerifyToPixels(units, 50, 20, 196, 70);
    VerifyFromPixels(units, 0.254, 0.28222222222222221);
}

BOOST_AUTO_TEST_CASE(TestCustomUnits, *bt::tolerance(0.0001)) {
    MockScreenProvider* screenProvider = new MockScreenProvider();
    MeaCustomUnits units(*screenProvider, &MockScreenProvider::ChangeLabel);

    MockScreenProvider::changeLabelCalled = false;
    BOOST_TEST(!units.HaveCustomUnits());
    units.SetName(_T("Abcd"));
    units.SetAbbrev(_T("ab"));
    BOOST_TEST(MockScreenProvider::changeLabelCalled);
    BOOST_TEST(MockScreenProvider::changeLabelUnits == MeaCustomId);
    BOOST_TEST(MockScreenProvider::changeLabel == _T("ab"));
    BOOST_TEST(units.HaveCustomUnits());
    BOOST_TEST(units.GetName() == _T("Abcd"));
    BOOST_TEST(units.GetAbbrev() == _T("ab"));

    BOOST_TEST(units.GetScaleBasis() == MeaCustomUnits::PixelBasis);
    BOOST_TEST(units.GetScaleBasisStr() == _T("px"));
    BOOST_TEST(!units.RequiresRes());
    units.SetScaleBasis(MeaCustomUnits::CentimeterBasis);
    BOOST_TEST(units.GetScaleBasis() == MeaCustomUnits::CentimeterBasis);
    BOOST_TEST(units.GetScaleBasisStr() == _T("cm"));
    BOOST_TEST(units.RequiresRes());
    units.SetScaleBasis(MeaCustomUnits::InchBasis);
    BOOST_TEST(units.GetScaleBasis() == MeaCustomUnits::InchBasis);
    BOOST_TEST(units.GetScaleBasisStr() == _T("in"));
    BOOST_TEST(units.RequiresRes());

    units.SetScaleBasis("px");
    BOOST_TEST(units.GetScaleBasis() == MeaCustomUnits::PixelBasis);
    BOOST_TEST(units.GetScaleBasisStr() == _T("px"));
    BOOST_TEST(!units.RequiresRes());
    units.SetScaleBasis("cm");
    BOOST_TEST(units.GetScaleBasis() == MeaCustomUnits::CentimeterBasis);
    BOOST_TEST(units.GetScaleBasisStr() == _T("cm"));
    BOOST_TEST(units.RequiresRes());
    units.SetScaleBasis("in");
    BOOST_TEST(units.GetScaleBasis() == MeaCustomUnits::InchBasis);
    BOOST_TEST(units.GetScaleBasisStr() == _T("in"));
    BOOST_TEST(units.RequiresRes());

    BOOST_TEST(units.GetScaleFactor() == 1.0);
    units.SetScaleFactor(2.0);
    BOOST_TEST(units.GetScaleFactor() == 2.0);

    VerifyUnitsId(units, MeaCustomId, "custom");

    MeaUnits::DisplayPrecisions defaultPrecisions { 0, 0, 0, 0, 1, 0, 1, 1 };
    VerifyPrecisions(units, defaultPrecisions, linearPrecisionNames);

    VerifyInvertY(units);
    VerifyOrigin(units);

    VerifyFormat(units, MeaX, 90.12345, "90");
    VerifyFormat(units, MeaRx, 90.123456, "90.1");

    FPOINT correctedPos(0.0520833333333333294, -0.10416666666666666);
    FPOINT uncorrectedPos(0.57291666666666663, 1.1458333333333333);
    VerifyConvertCoords(units, correctedPos, uncorrectedPos);

    FSIZE convertedRes(200.0, 180.0);
    VerifyConvertRes(units, convertedRes);

    VerifyToPixels(units, 50, 20, 10000, 3600);
    VerifyFromPixels(units, 0.0050000000000000001, 0.0055555555555555558);
}

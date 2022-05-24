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
#define BOOST_TEST_MODULE GeometryTest
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <meazure/utilities/Geometry.h>
#include <float.h>

namespace bt = boost::unit_test;
namespace tt = boost::test_tools;
namespace bdata = boost::unit_test::data;

BOOST_TEST_DONT_PRINT_LOG_VALUE(POINT)


struct GlobalFixture {
    GlobalFixture() {
        if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
            BOOST_FAIL("Fatal Error: MFC initialization failed");
        }
    }

    CWinApp theApp;
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);


BOOST_AUTO_TEST_SUITE(FSIZETests, *bt::tolerance(FLT_EPSILON))

BOOST_AUTO_TEST_CASE(TestInitialSize) {
    FSIZE fs1;
    BOOST_TEST(fs1.cx == 0.0);
    BOOST_TEST(fs1.cy == 0.0);

    FSIZE fs2(1.0, 2.0);
    BOOST_TEST(fs2.cx == 1.0);
    BOOST_TEST(fs2.cy == 2.0);
}

BOOST_AUTO_TEST_CASE(TestAddFSize) {
    FSIZE fs1(10.0, 11.0);
    FSIZE fs2(5.0, 6.0);

    FSIZE fs3 = fs1 + fs2;
    BOOST_TEST(fs3.cx == 15.0);
    BOOST_TEST(fs3.cy == 17.0);
}

BOOST_AUTO_TEST_CASE(TestAddValue) {
    FSIZE fs1(10.0, 11.0);

    FSIZE fs2 = fs1 + 13.0;
    BOOST_TEST(fs2.cx == 23.0);
    BOOST_TEST(fs2.cy == 24.0);
}

BOOST_AUTO_TEST_CASE(TestMultFSize) {
    FSIZE fs1(10.0, 11.0);
    FSIZE fs2(2.0, 3.0);

    FSIZE fs3 = fs1 * fs2;
    BOOST_TEST(fs3.cx == 20.0);
    BOOST_TEST(fs3.cy == 33.0);
}

BOOST_AUTO_TEST_CASE(TestMultSize) {
    FSIZE fs1(10.0, 11.0);
    SIZE s { 2, 3 };

    FSIZE fs2 = fs1 * s;
    BOOST_TEST(fs2.cx == 20.0);
    BOOST_TEST(fs2.cy == 33.0);
}

BOOST_AUTO_TEST_CASE(TestMultValue) {
    FSIZE fs1(10.0, 11.0);

    FSIZE fs2 = fs1 * 2.0;
    BOOST_TEST(fs2.cx == 20.0);
    BOOST_TEST(fs2.cy == 22.0);
}

BOOST_AUTO_TEST_CASE(TestAddAssign) {
    FSIZE fs1(10.0, 11.0);
    FSIZE fs2(5.0, 6.0);

    fs1 += fs2;
    BOOST_TEST(fs1.cx == 15.0);
    BOOST_TEST(fs1.cy == 17.0);
}

BOOST_AUTO_TEST_CASE(TestSubAssign) {
    FSIZE fs1(10.0, 11.0);
    FSIZE fs2(3.0, 6.0);

    fs1 -= fs2;
    BOOST_TEST(fs1.cx == 7.0);
    BOOST_TEST(fs1.cy == 5.0);
}

BOOST_AUTO_TEST_CASE(TestMultAssign) {
    FSIZE fs1(10.0, 11.0);
    FSIZE fs2(3.0, 6.0);

    fs1 *= fs2;
    BOOST_TEST(fs1.cx == 30.0);
    BOOST_TEST(fs1.cy == 66.0);
}

BOOST_AUTO_TEST_CASE(TestLog10) {
    FSIZE fs1(1000.0, 100.0);

    FSIZE fs2 = log10(fs1);
    BOOST_TEST(fs2.cx == 3.0);
    BOOST_TEST(fs2.cy == 2.0);
}

BOOST_AUTO_TEST_CASE(TestFloor) {
    FSIZE fs1(12.98, 4.01);

    FSIZE fs2 = floor(fs1);
    BOOST_TEST(fs2.cx == 12.0);
    BOOST_TEST(fs2.cy == 4.0);
}

BOOST_AUTO_TEST_CASE(TestValueSub) {
    FSIZE fs1(11.0, 45.0);

    FSIZE fs2 = 200.0 - fs1;
    BOOST_TEST(fs2.cx == 189.0);
    BOOST_TEST(fs2.cy == 155.0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(FRECTTests, *bt::tolerance(FLT_EPSILON))

BOOST_AUTO_TEST_CASE(TestInitialRectangle) {
    FRECT fr1;
    BOOST_TEST(fr1.top == 0.0);
    BOOST_TEST(fr1.bottom == 0.0);
    BOOST_TEST(fr1.left == 0.0);
    BOOST_TEST(fr1.right == 0.0);

    FRECT fr2(1.0, 2.0, 3.0, 4.0);
    BOOST_TEST(fr2.top == 1.0);
    BOOST_TEST(fr2.bottom == 2.0);
    BOOST_TEST(fr2.left == 3.0);
    BOOST_TEST(fr2.right == 4.0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(FPOINTTests, *bt::tolerance(FLT_EPSILON))

BOOST_AUTO_TEST_CASE(TestInitialPoint) {
    FPOINT fp1;
    BOOST_TEST(fp1.x == 0.0);
    BOOST_TEST(fp1.y == 0.0);

    FPOINT fp2(1.0, 2.0);
    BOOST_TEST(fp2.x == 1.0);
    BOOST_TEST(fp2.y == 2.0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(GeometryTests, *bt::tolerance(FLT_EPSILON))

BOOST_DATA_TEST_CASE(TestCalcLengthCoords,
                     bdata::make({ 0.0, 1.0,                0.5,                 -1.0 }) ^
                     bdata::make({ 0.0, 2.0,                0.7,                 -2.0 }) ^
                     bdata::make({ 0.0, 2.2360679774997898, 0.86023252670426265, 2.2360679774997898 }),
                     x, y, length) {
    BOOST_TEST(MeaGeometry::CalcLength(x, y) == length);
}

BOOST_DATA_TEST_CASE(TestCalcLengthPoints,
                     bdata::make({ POINT{0, 0}, POINT{1, 2} }) ^
                     bdata::make({ POINT{0, 0}, POINT{2, 4} }) ^
                     bdata::make({ 0.0,         2.2360679774997898 }),
                     start, end, length) {
    BOOST_TEST(MeaGeometry::CalcLength(start, end) == length);
}

BOOST_DATA_TEST_CASE(TestCalcCircumference,
                     bdata::make({ 0.0, 1.0,                15.5 }) ^
                     bdata::make({ 0.0, 6.2831853071795862, 97.389372261283583 }),
                     radius, circumference) {
    BOOST_TEST(MeaGeometry::CalcCircumference(radius) == circumference);
}

struct SectorTestData {
    int x;
    int y;
    int sector;
    bool isHorizontal;
    bool isVertical;
};
std::ostream& operator<<(std::ostream& os, const SectorTestData& sd) {
    os << sd.x << ',' << sd.y << ',' << sd.sector << ',' << sd.isHorizontal << ',' << sd.isVertical;
    return os;
}

BOOST_DATA_TEST_CASE(TestCalcSector,
                     bdata::make({
                                    SectorTestData{  0,  0,  0, false, false },     // Zero vector
                                    SectorTestData{  1,  0,  1, true,  false },     // 0 degrees
                                    SectorTestData{  2,  1,  1, true,  false },     // 27 degrees
                                    SectorTestData{  1,  1,  2, false, true  },     // 45 degrees
                                    SectorTestData{  1,  2,  2, false, true  },     // 63 degrees
                                    SectorTestData{  0,  1,  3, false, true  },     // 90 degrees
                                    SectorTestData{ -1,  2,  3, false, true  },     // 117 degrees
                                    SectorTestData{ -1,  1,  4, true,  false },     // 135 degrees
                                    SectorTestData{ -2,  1,  4, true,  false },     // 153 degrees
                                    SectorTestData{ -1,  0, -4, true,  false },     // 180 degrees
                                    SectorTestData{ -2, -1, -4, true,  false },     // 207 degrees
                                    SectorTestData{ -1, -1, -4, true,  false },     // 225 degrees
                                    SectorTestData{ -1, -2, -3, false, true  },     // 243 degrees
                                    SectorTestData{  0, -1, -3, false, true  },     // 270 degrees
                                    SectorTestData{  1, -2, -2, false, true  },     // 297 degrees
                                    SectorTestData{  1, -1, -2, false, true  },     // 315 degrees
                                    SectorTestData{  2, -1, -1, true,  false }      // 333 degrees
                                 }),
                     sectorData) {
    POINT p0 { 1, 2 };
    POINT p1 { p0.x + sectorData.x, p0.y + sectorData.y };
    BOOST_TEST(MeaGeometry::CalcSector(p0, p1) == sectorData.sector);
    BOOST_TEST(MeaGeometry::IsHorizontallyOriented(p0, p1) == sectorData.isHorizontal);
    BOOST_TEST(MeaGeometry::IsVerticallyOriented(p0, p1) == sectorData.isVertical);
}


struct AngleTestData {
    double x;
    double y;
    double angle;
};
std::ostream& operator<<(std::ostream& os, const AngleTestData& ad) {
    os << ad.x << ',' << ad.y << ',' << ad.angle;
    return os;
}

BOOST_DATA_TEST_CASE(TestCalcAngle1,
                     bdata::make({
                                    AngleTestData{  0.0,  0.0,  0.0 },          // Zero vector
                                    AngleTestData{  1.0,  0.0,  0.0 },          // 0 degrees
                                    AngleTestData{  2.0,  1.0,  0.46364761 },   // 27 degrees
                                    AngleTestData{  1.0,  1.0,  0.78539816 },   // 45 degrees
                                    AngleTestData{  1.0,  2.0,  1.10714872 },   // 63 degrees
                                    AngleTestData{  0.0,  1.0,  1.57079633 },   // 90 degrees
                                    AngleTestData{ -1.0,  2.0,  2.03444393 },   // 117 degrees
                                    AngleTestData{ -1.0,  1.0,  2.35619449 },   // 135 degrees
                                    AngleTestData{ -2.0,  1.0,  2.67794504 },   // 153 degrees
                                    AngleTestData{ -1.0,  0.0,  3.14159265 },   // 180 degrees
                                    AngleTestData{ -2.0, -1.0, -2.67794504 },   // 207 degrees
                                    AngleTestData{ -1.0, -1.0, -2.35619449 },   // 225 degrees
                                    AngleTestData{ -1.0, -2.0, -2.03444393 },   // 243 degrees
                                    AngleTestData{  0.0, -1.0, -1.57079633 },   // 270 degrees
                                    AngleTestData{  1.0, -2.0, -1.10714872 },   // 297 degrees
                                    AngleTestData{  1.0, -1.0, -0.78539816 },   // 315 degrees
                                    AngleTestData{  2.0, -1.0, -0.46364761 },   // 333 degrees
                                    AngleTestData{  1.0, -1.0, -0.78539816 }    // 315 degrees
                                 }),
                     angleData) {
    FPOINT p0(1.0, 2.0);
    FPOINT p1(p0.x + angleData.x, p0.y + angleData.y);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p1) == angleData.angle, tt::tolerance(0.00000001));
}

BOOST_AUTO_TEST_CASE(TestCalcAngle2, *bt::tolerance(0.00000001)) {
    FPOINT p0(1.0, 2.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p0, p0) == 0.0);

    FPOINT p1(5.0, 6.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p1, p1) == 0.0);

    FPOINT p2(4.0, -1.0);
    FPOINT p3(5.0, 6.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p2, p3) == 1.57079633);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p3, p2) == -1.57079633);

    FPOINT p4(1.0, 5.0);
    FPOINT p5(5.0, 2.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p4, p5) == -1.57079633);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p5, p4) == 1.57079633);

    FPOINT p6(5.0, -2.0);
    FPOINT p7(-4.0, 4.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p6, p7) == -2.73670086);
}

BOOST_AUTO_TEST_SUITE_END()

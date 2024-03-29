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
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <meazure/utilities/Geometry.h>
#include <float.h>
#include <sstream>

namespace bt = boost::unit_test;
namespace tt = boost::test_tools;
namespace bdata = boost::unit_test::data;


bool operator==(const POINT& lhs, const POINT& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const SIZE& lhs, const SIZE& rhs) {
    return lhs.cx == rhs.cx && lhs.cy == rhs.cy;
}

bool operator==(const RECT& lhs, const RECT& rhs) {
    return lhs.top == rhs.top && lhs.bottom == rhs.bottom && lhs.left == rhs.left && lhs.right == rhs.right;
}

std::ostream& operator<<(std::ostream& os, const POINT& point) {
    os << '(' << point.x << ',' << point.y << ')';
    return os;
}

std::ostream& operator<<(std::ostream& os, const SIZE& size) {
    os << '(' << size.cx << ',' << size.cy << ')';
    return os;
}

std::ostream& operator<<(std::ostream& os, const RECT& rect) {
    os << '(' << rect.left << ',' << rect.top << ',' << rect.right << ',' << rect.bottom << ')';
    return os;
}



BOOST_AUTO_TEST_SUITE(MeaFSizeTests, *bt::tolerance(FLT_EPSILON))

BOOST_AUTO_TEST_CASE(TestInitialSize) {
    MeaFSize fs1;
    BOOST_TEST(fs1.cx == 0.0);
    BOOST_TEST(fs1.cy == 0.0);

    MeaFSize fs2(1.0, 2.0);
    BOOST_TEST(fs2.cx == 1.0);
    BOOST_TEST(fs2.cy == 2.0);
}

BOOST_AUTO_TEST_CASE(TestSizeEquality) {
    MeaFSize fs1(10.0, 11.0);
    MeaFSize fs2(10.0, 11.0);
    MeaFSize fs3(5.0, 6.0);

    BOOST_TEST(fs1 == fs1);
    BOOST_TEST(fs1 == fs2);
    BOOST_TEST(fs1 != fs3);
}

BOOST_AUTO_TEST_CASE(TestAddFSize) {
    MeaFSize fs1(10.0, 11.0);
    MeaFSize fs2(5.0, 6.0);

    MeaFSize fs3 = fs1 + fs2;
    BOOST_TEST(fs3.cx == 15.0);
    BOOST_TEST(fs3.cy == 17.0);
}

BOOST_AUTO_TEST_CASE(TestAddValue) {
    MeaFSize fs1(10.0, 11.0);

    MeaFSize fs2 = fs1 + 13.0;
    BOOST_TEST(fs2.cx == 23.0);
    BOOST_TEST(fs2.cy == 24.0);
}

BOOST_AUTO_TEST_CASE(TestMultFSize) {
    MeaFSize fs1(10.0, 11.0);
    MeaFSize fs2(2.0, 3.0);

    MeaFSize fs3 = fs1 * fs2;
    BOOST_TEST(fs3.cx == 20.0);
    BOOST_TEST(fs3.cy == 33.0);
}

BOOST_AUTO_TEST_CASE(TestMultSize) {
    MeaFSize fs1(10.0, 11.0);
    SIZE s { 2, 3 };

    MeaFSize fs2 = fs1 * s;
    BOOST_TEST(fs2.cx == 20.0);
    BOOST_TEST(fs2.cy == 33.0);
}

BOOST_AUTO_TEST_CASE(TestMultValue) {
    MeaFSize fs1(10.0, 11.0);

    MeaFSize fs2 = fs1 * 2.0;
    BOOST_TEST(fs2.cx == 20.0);
    BOOST_TEST(fs2.cy == 22.0);
}

BOOST_AUTO_TEST_CASE(TestDivValue) {
    MeaFSize fs1(10.0, 12.0);

    MeaFSize fs2 = fs1 / 2.0;
    BOOST_TEST(fs2.cx == 5.0);
    BOOST_TEST(fs2.cy == 6.0);
}

BOOST_AUTO_TEST_CASE(TestAddAssign) {
    MeaFSize fs1(10.0, 11.0);
    MeaFSize fs2(5.0, 6.0);

    fs1 += fs2;
    BOOST_TEST(fs1.cx == 15.0);
    BOOST_TEST(fs1.cy == 17.0);
}

BOOST_AUTO_TEST_CASE(TestSubAssign) {
    MeaFSize fs1(10.0, 11.0);
    MeaFSize fs2(3.0, 6.0);

    fs1 -= fs2;
    BOOST_TEST(fs1.cx == 7.0);
    BOOST_TEST(fs1.cy == 5.0);
}

BOOST_AUTO_TEST_CASE(TestMultAssign) {
    MeaFSize fs1(10.0, 11.0);
    MeaFSize fs2(3.0, 6.0);

    fs1 *= fs2;
    BOOST_TEST(fs1.cx == 30.0);
    BOOST_TEST(fs1.cy == 66.0);
}

BOOST_AUTO_TEST_CASE(TestLog10) {
    MeaFSize fs1(1000.0, 100.0);

    MeaFSize fs2 = log10(fs1);
    BOOST_TEST(fs2.cx == 3.0);
    BOOST_TEST(fs2.cy == 2.0);
}

BOOST_AUTO_TEST_CASE(TestFloor) {
    MeaFSize fs1(12.98, 4.01);

    MeaFSize fs2 = floor(fs1);
    BOOST_TEST(fs2.cx == 12.0);
    BOOST_TEST(fs2.cy == 4.0);
}

BOOST_AUTO_TEST_CASE(TestValueSub) {
    MeaFSize fs1(11.0, 45.0);

    MeaFSize fs2 = 200.0 - fs1;
    BOOST_TEST(fs2.cx == 189.0);
    BOOST_TEST(fs2.cy == 155.0);
}

BOOST_AUTO_TEST_CASE(TestSizeOstream) {
    MeaFSize fs(10.1, 20.1);

    std::ostringstream ss;
    ss << fs;
    BOOST_TEST(ss.str().c_str() == CString("(10.1,20.1)"));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(MeaFRectTests, *bt::tolerance(FLT_EPSILON))

BOOST_AUTO_TEST_CASE(TestInitialRectangle) {
    MeaFRect fr1;
    BOOST_TEST(fr1.top == 0.0);
    BOOST_TEST(fr1.bottom == 0.0);
    BOOST_TEST(fr1.left == 0.0);
    BOOST_TEST(fr1.right == 0.0);

    MeaFRect fr2(1.0, 2.0, 3.0, 4.0);
    BOOST_TEST(fr2.top == 1.0);
    BOOST_TEST(fr2.bottom == 2.0);
    BOOST_TEST(fr2.left == 3.0);
    BOOST_TEST(fr2.right == 4.0);
}

BOOST_AUTO_TEST_CASE(TestRectEquality) {
    MeaFRect fr1(10.0, 11.0, 12.0, 13.0);
    MeaFRect fr2(10.0, 11.0, 12.0, 13.0);
    MeaFRect fr3(5.0, 6.0, 7.0, 8.0);

    BOOST_TEST(fr1 == fr1);
    BOOST_TEST(fr1 == fr2);
    BOOST_TEST(fr1 != fr3);
}

BOOST_AUTO_TEST_CASE(TestRectOstream) {
    MeaFRect fr(10.1, 11.1, 12.1, 13.1);

    std::ostringstream ss;
    ss << fr;
    BOOST_TEST(ss.str().c_str() == CString("[10.1 13.1 11.1 12.1]"));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(MeaFPointTests, *bt::tolerance(FLT_EPSILON))

BOOST_AUTO_TEST_CASE(TestInitialPoint) {
    MeaFPoint fp1;
    BOOST_TEST(fp1.x == 0.0);
    BOOST_TEST(fp1.y == 0.0);

    MeaFPoint fp2(1.0, 2.0);
    BOOST_TEST(fp2.x == 1.0);
    BOOST_TEST(fp2.y == 2.0);
}

BOOST_AUTO_TEST_CASE(TestPointEquality) {
    MeaFPoint fp1(10.0, 11.0);
    MeaFPoint fp2(10.0, 11.0);
    MeaFPoint fp3(5.0, 6.0);

    BOOST_TEST(fp1 == fp1);
    BOOST_TEST(fp1 == fp2);
    BOOST_TEST(fp1 != fp3);
}

BOOST_AUTO_TEST_CASE(TestPointOstream) {
    MeaFSize fp(10.1, 20.1);

    std::ostringstream ss;
    ss << fp;
    BOOST_TEST(ss.str().c_str() == CString("(10.1,20.1)"));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(GeometryTests, *bt::tolerance(FLT_EPSILON))

BOOST_DATA_TEST_CASE(TestScaleRect,
                     bdata::make({ RECT{1, 2, 3, 4},  RECT{1, 2, 3, 4}, RECT{0, 0, 0, 0},  RECT{1, 2, 3, 4} }) ^
                     bdata::make({ RECT{3, 5, 8, 10}, RECT{1, 2, 3, 4}, RECT{0, 0, 0, 0 }, RECT{0, 0, 0, 0} }) ^
                     bdata::make({ 2.5,               1.0,              7.0,               0.0 }),
                     original, scaled, scaleFactor) {
    BOOST_TEST(MeaGeometry::Scale(original, scaleFactor) == scaled);
}

BOOST_DATA_TEST_CASE(TestScaleSize,
                     bdata::make({ SIZE{1, 3}, SIZE { 1, 3 }, SIZE { 1, 3 }, SIZE { 0, 0 }, SIZE { 1, 3 } }) ^
                     bdata::make({ SIZE{2, 6}, SIZE{3, 8}, SIZE{1, 3}, SIZE{0, 0}, SIZE{0, 0} }) ^
                     bdata::make({ 2.0,        2.5,        1.0,        7.0,        0.0 }),
                     original, scaled, scaleFactor) {
    BOOST_TEST(MeaGeometry::Scale(original, scaleFactor) == scaled);
}

BOOST_DATA_TEST_CASE(TestScaleInt,
                     bdata::make({ 1,   1,   1,   0,   1 }) ^
                     bdata::make({ 2,   3,   1,   0,   0 }) ^
                     bdata::make({ 2.0, 2.5, 1.0, 7.0, 0.0 }),
                     original, scaled, scaleFactor) {
    BOOST_TEST(MeaGeometry::Scale(original, scaleFactor) == scaled);
}

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
    MeaFPoint p0(1.0, 2.0);
    MeaFPoint p1(p0.x + angleData.x, p0.y + angleData.y);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p1) == angleData.angle, tt::tolerance(0.00000001));
}

BOOST_AUTO_TEST_CASE(TestCalcAngle2, *bt::tolerance(0.00000001)) {
    MeaFPoint p0(1.0, 2.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p0, p0) == 0.0);

    MeaFPoint p1(5.0, 6.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p1, p1) == 0.0);

    MeaFPoint p2(4.0, -1.0);
    MeaFPoint p3(5.0, 6.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p2, p3) == 1.57079633);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p3, p2) == -1.57079633);

    MeaFPoint p4(1.0, 5.0);
    MeaFPoint p5(5.0, 2.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p4, p5) == -1.57079633);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p5, p4) == 1.57079633);

    MeaFPoint p6(5.0, -2.0);
    MeaFPoint p7(-4.0, 4.0);
    BOOST_TEST(MeaGeometry::CalcAngle(p0, p6, p7) == -2.73670086);
}

BOOST_AUTO_TEST_SUITE_END()

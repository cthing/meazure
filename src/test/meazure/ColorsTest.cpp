/*
 * Copyright 2011 C Thing Software
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
#define BOOST_TEST_MODULE ColorsTest
#include "GlobalFixture.h"
#define COMPILE_LAYERED_WINDOW_STUBS
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <meazure/ui/LayeredWindows.h>
#include <meazure/graphics/Colors.h>
#include <float.h>

namespace bdata = boost::unit_test::data;
namespace bt = boost::unit_test;
namespace tt = boost::test_tools;


struct CMYTestData {
    int cyan;
    int magenta;
    int yellow;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const CMYTestData& cd) {
    os << '(' << cd.cyan << ',' << cd.magenta << ',' << cd.yellow << '):('
        << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}

struct CMYKTestData {
    int cyan;
    int magenta;
    int yellow;
    int black;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const CMYKTestData& cd) {
    os << '(' << cd.cyan << ',' << cd.magenta << ',' << cd.yellow << ',' << cd.black << '):('
        << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}

struct HSLTestData {
    int hue;
    int saturation;
    int lightness;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const HSLTestData& cd) {
    os << '(' << cd.hue << ',' << cd.saturation << ',' << cd.lightness << '):('
        << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}

struct YCbCrTestData {
    int y;
    int cb;
    int cr;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const YCbCrTestData& cd) {
    os << '(' << cd.y << ',' << cd.cb << ',' << cd.cr << '):('
        << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}

struct YIQTestData {
    int y;
    int i;
    int q;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const YIQTestData& cd) {
    os << '(' << cd.y << ',' << cd.i << ',' << cd.q << '):('
        << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}

struct XYZTestData {
    double x;
    double y;
    double z;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const XYZTestData& cd) {
    os << '(' << cd.x << ',' << cd.y << ',' << cd.z << '):('
        << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}

struct LabTestData {
    double l;
    double a;
    double b;
    double x;
    double y;
    double z;
};
std::ostream& operator<<(std::ostream& os, const LabTestData& cd) {
    os << '(' << cd.l << ',' << cd.a << ',' << cd.b << '):('
        << cd.x << ',' << cd.y << ',' << cd.z << ')';
    return os;
}

struct LabRGBTestData {
    double l;
    double a;
    double b;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const LabRGBTestData& cd) {
    os << '(' << cd.l << ',' << cd.a << ',' << cd.b << '):('
        << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}

struct DiffTestData {
    double l1;
    double a1;
    double b1;
    double l2;
    double a2;
    double b2;
    double diff;
};
std::ostream& operator<<(std::ostream& os, const DiffTestData& cd) {
    os << '(' << cd.l1 << ',' << cd.a1 << ',' << cd.b1 << ") - ("
        << cd.l2 << ',' << cd.a2 << ',' << cd.b2 << ") = " << cd.diff;
    return os;
}


BOOST_AUTO_TEST_CASE(TestCMY) {
    MeaColors::CMY cmy1;
    BOOST_TEST(cmy1.cyan == 0);
    BOOST_TEST(cmy1.magenta == 0);
    BOOST_TEST(cmy1.yellow == 0);

    MeaColors::CMY cmy2(10, 255, 0);
    BOOST_TEST(cmy2.cyan == 10);
    BOOST_TEST(cmy2.magenta == 255);
    BOOST_TEST(cmy2.yellow == 0);
}

BOOST_AUTO_TEST_CASE(TestCMYK) {
    MeaColors::CMYK cmyk1;
    BOOST_TEST(cmyk1.cyan == 0);
    BOOST_TEST(cmyk1.magenta == 0);
    BOOST_TEST(cmyk1.yellow == 0);
    BOOST_TEST(cmyk1.black == 0);

    MeaColors::CMYK cmyk2(10, 255, 0, 100);
    BOOST_TEST(cmyk2.cyan == 10);
    BOOST_TEST(cmyk2.magenta == 255);
    BOOST_TEST(cmyk2.yellow == 0);
    BOOST_TEST(cmyk2.black == 100);
}

BOOST_AUTO_TEST_CASE(TestHSL) {
    MeaColors::HSL hsl1;
    BOOST_TEST(hsl1.hue == 0);
    BOOST_TEST(hsl1.saturation == 0);
    BOOST_TEST(hsl1.lightness == 0);

    MeaColors::HSL hsl2(1, 2, 3);
    BOOST_TEST(hsl2.hue == 1);
    BOOST_TEST(hsl2.saturation == 2);
    BOOST_TEST(hsl2.lightness == 3);
}

BOOST_AUTO_TEST_CASE(TestYCbCr) {
    MeaColors::YCbCr ycbcr1;
    BOOST_TEST(ycbcr1.y == 0);
    BOOST_TEST(ycbcr1.cb == 0);
    BOOST_TEST(ycbcr1.cr == 0);

    MeaColors::YCbCr ycbcr2(20, 30, 40);
    BOOST_TEST(ycbcr2.y == 20);
    BOOST_TEST(ycbcr2.cb == 30);
    BOOST_TEST(ycbcr2.cr == 40);
}

BOOST_AUTO_TEST_CASE(TestYIQ) {
    MeaColors::YIQ yiq1;
    BOOST_TEST(yiq1.y == 0);
    BOOST_TEST(yiq1.i == 0);
    BOOST_TEST(yiq1.q == 0);

    MeaColors::YIQ yiq2(20, 30, 40);
    BOOST_TEST(yiq2.y == 20);
    BOOST_TEST(yiq2.i == 30);
    BOOST_TEST(yiq2.q == 40);
}

BOOST_AUTO_TEST_CASE(TestXYZ, *bt::tolerance(DBL_EPSILON)) {
    MeaColors::XYZ xyz1;
    BOOST_TEST(xyz1.x == 0.0);
    BOOST_TEST(xyz1.y == 0.0);
    BOOST_TEST(xyz1.z == 0.0);

    MeaColors::XYZ xyz2(25.0, 0.50, 7.5);
    BOOST_TEST(xyz2.x == 25.0);
    BOOST_TEST(xyz2.y == 0.50);
    BOOST_TEST(xyz2.z == 7.5);
}

BOOST_AUTO_TEST_CASE(TestLab, *bt::tolerance(DBL_EPSILON)) {
    MeaColors::Lab lab1;
    BOOST_TEST(lab1.l == 0.0);
    BOOST_TEST(lab1.a == 0.0);
    BOOST_TEST(lab1.b == 0.0);

    MeaColors::Lab lab2(25.0, 0.50, 7.5);
    BOOST_TEST(lab2.l == 25.0);
    BOOST_TEST(lab2.a == 0.50);
    BOOST_TEST(lab2.b == 7.5);
}

BOOST_DATA_TEST_CASE(TestRGBtoCMY,
                     bdata::make({
                         CMYTestData{   0,   0,   0, 255, 255, 255 },
                         CMYTestData{ 255, 255, 255,   0,   0,   0 },
                         CMYTestData{ 128,  10,   0, 127, 245, 255 },
                         CMYTestData{   1,   2,   3, 254, 253, 252 },
                     }),
                     colorData) {
    MeaColors::CMY cmy = MeaColors::RGBtoCMY(RGB(colorData.red, colorData.green, colorData.blue));
    BOOST_TEST(cmy.cyan == colorData.cyan);
    BOOST_TEST(cmy.magenta == colorData.magenta);
    BOOST_TEST(cmy.yellow == colorData.yellow);
}

BOOST_DATA_TEST_CASE(TestRGBtoCMYK,
                     bdata::make({
                         CMYKTestData{   0,   0,   0,   0, 255, 255, 255 },
                         CMYKTestData{   0,   0,   0, 255,   0,   0,   0 },
                         CMYKTestData{ 128,  10,   0,   0, 127, 245, 255 },
                         CMYKTestData{  95,  54,   0,  75, 113, 142, 180 },
                         CMYKTestData{ 214,   0, 131,  20,  38, 235, 114 },
                     }),
                     colorData) {
    MeaColors::CMYK cmyk = MeaColors::RGBtoCMYK(RGB(colorData.red, colorData.green, colorData.blue));
    BOOST_TEST(cmyk.cyan == colorData.cyan);
    BOOST_TEST(cmyk.magenta == colorData.magenta);
    BOOST_TEST(cmyk.yellow == colorData.yellow);
    BOOST_TEST(cmyk.black == colorData.black);
}

BOOST_DATA_TEST_CASE(TestRGBtoHSL,
                     bdata::make({
                         HSLTestData{   0,   0,   0,   0,   0,   0 },
                         HSLTestData{   0, 100,  50, 255,   0,   0 },
                         HSLTestData{ 120, 100,  50,   0, 255,   0 },
                         HSLTestData{ 240, 100,  50,   0,   0, 255 },
                         HSLTestData{   0,   0, 100, 255, 255, 255 },
                         HSLTestData{   0, 100,  25, 128,   0,   0 },
                         HSLTestData{ 120, 100,  25,   0, 128,   0 },
                         HSLTestData{ 240, 100,  25,   0,   0, 128 },
                         HSLTestData{ 200,  60,  49,  50, 150, 200 },
                     }),
                     colorData) {
    MeaColors::HSL hsl = MeaColors::RGBtoHSL(RGB(colorData.red, colorData.green, colorData.blue));
    BOOST_TEST(hsl.hue == colorData.hue);
    BOOST_TEST(hsl.saturation == colorData.saturation);
    BOOST_TEST(hsl.lightness == colorData.lightness);
}

BOOST_DATA_TEST_CASE(TestHSLtoRGB,
                     bdata::make({
                         HSLTestData{   0,   0,   0,   0,   0,   0 },
                         HSLTestData{   0, 100,  50, 255,   0,   0 },
                         HSLTestData{ 120, 100,  50,   0, 255,   0 },
                         HSLTestData{ 240, 100,  50,   0,   0, 255 },
                         HSLTestData{   0,   0, 100, 255, 255, 255 },
                         HSLTestData{   0, 100,  25, 128,   0,   0 },
                         HSLTestData{ 120, 100,  25,   0, 128,   0 },
                         HSLTestData{ 240, 100,  25,   0,   0, 128 },
                         HSLTestData{ 200,  60,  49,  50, 150, 200 }
                     }),
                     colorData) {
    MeaColors::HSL hsl(colorData.hue, colorData.saturation, colorData.lightness);
    COLORREF rgb = MeaColors::HSLtoRGB(hsl);
    BOOST_TEST(rgb == RGB(colorData.red, colorData.green, colorData.blue));
}

BOOST_DATA_TEST_CASE(TestRGBtoYCbCr,
                     bdata::make({
                         YCbCrTestData{  16, 128, 128,   0,   0,   0 },
                         YCbCrTestData{ 235, 128, 128, 255, 255, 255 },
                         YCbCrTestData{ 126, 128, 128, 128, 128, 128 },
                         YCbCrTestData{  82,  90, 240, 255,   0,   0 },
                         YCbCrTestData{ 145,  54,  34,   0, 255,   0 },
                         YCbCrTestData{  41, 240, 110,   0,   0, 255 },
                         YCbCrTestData{ 210,  16, 146, 255, 255,   0 },
                         YCbCrTestData{ 170, 166,  16,   0, 255, 255 },
                         YCbCrTestData{ 107, 202, 222, 255,   0, 255 },
                         YCbCrTestData{  32, 134, 123,  10,  20,  30 }
                     }),
                     colorData) {
    MeaColors::YCbCr ycbcr = MeaColors::RGBtoYCbCr(RGB(colorData.red, colorData.green, colorData.blue));
    BOOST_TEST(ycbcr.y == colorData.y);
    BOOST_TEST(ycbcr.cb == colorData.cb);
    BOOST_TEST(ycbcr.cr == colorData.cr);
}

BOOST_DATA_TEST_CASE(TestRGBtoYIQ,
                     bdata::make({
                         YIQTestData{   0,    0,    0,   0,   0,   0 },
                         YIQTestData{ 255,    0,    0, 255, 255, 255 },
                         YIQTestData{ 128,    0,    0, 128, 128, 128 },
                         YIQTestData{  76,  152,   54, 255,   0,   0 },
                         YIQTestData{ 150,  -70, -133,   0, 255,   0 },
                         YIQTestData{  29,  -82,   79,   0,   0, 255 },
                         YIQTestData{ 226,   82,  -79, 255, 255,   0 },
                         YIQTestData{ 179, -152,  -54,   0, 255, 255 },
                         YIQTestData{ 105,   70,  133, 255,   0, 255 },
                         YIQTestData{  18,   -9,    1,  10,  20,  30 }
                     }),
                     colorData) {
    MeaColors::YIQ yiq = MeaColors::RGBtoYIQ(RGB(colorData.red, colorData.green, colorData.blue));
    BOOST_TEST(yiq.y == colorData.y);
    BOOST_TEST(yiq.i == colorData.i);
    BOOST_TEST(yiq.q == colorData.q);
}

BOOST_DATA_TEST_CASE(TestRGBtoXYZ,
                     bdata::make({
                         XYZTestData{  0.0000,   0.0000,   0.0000,   0,   0,   0 },
                         XYZTestData{ 95.0470, 100.0000, 108.8830, 255, 255, 255 },
                         XYZTestData{ 20.5169,  21.5861,  23.5035, 128, 128, 128 },
                         XYZTestData{ 41.2456,  21.2673,   1.9334, 255,   0,   0 },
                         XYZTestData{ 35.7576,  71.5152,  11.9192,   0, 255,   0 },
                         XYZTestData{ 18.0438,   7.2175,  95.0304,   0,   0, 255 },
                         XYZTestData{ 77.0033,  92.7825,  13.8526, 255, 255,   0 },
                         XYZTestData{ 53.8014,  78.7327, 106.9496,   0, 255, 255 },
                         XYZTestData{ 59.2894,  28.4848,  96.9638, 255,   0, 255 },
                         XYZTestData{  0.6096,   0.6585,   1.3230,  10,  20,  30 },
                         XYZTestData{ 25.5444,  28.1543,  58.7206,  90, 150, 200 },
                         XYZTestData{ 31.2431,  24.1726,  12.6913, 200, 110,  90 },
                         XYZTestData{ 25.6330,  47.1587,  19.8057,  20, 210, 100 },
                     }),
                     colorData) {
    MeaColors::XYZ xyz = MeaColors::RGBtoXYZ(RGB(colorData.red, colorData.green, colorData.blue));
    BOOST_TEST(std::round(10000.0 * xyz.x) / 10000.0 == colorData.x, tt::tolerance(DBL_EPSILON));
    BOOST_TEST(std::round(10000.0 * xyz.y) / 10000.0 == colorData.y, tt::tolerance(DBL_EPSILON));
    BOOST_TEST(std::round(10000.0 * xyz.z) / 10000.0 == colorData.z, tt::tolerance(DBL_EPSILON));
}

BOOST_DATA_TEST_CASE(TestXYZtoLab,
                     bdata::make({
                         LabTestData{   0.0000,   0.0000,    0.0000,  0.0000,   0.0000,   0.0000 },
                         LabTestData{ 100.0000,   0.0000,    0.0000, 95.0470, 100.0000, 108.8830 },
                         LabTestData{  53.5851,  -0.0002,    0.0002, 20.5169,  21.5861,  23.5035 },
                         LabTestData{  53.2408,  80.0923,   67.2031, 41.2456,  21.2673,   1.9334 },
                         LabTestData{  87.7347, -86.1827,   83.1793, 35.7576,  71.5152,  11.9192 },
                         LabTestData{  32.2970,  79.1878, -107.8602, 18.0438,   7.2175,  95.0304 },
                         LabTestData{  97.1393, -21.5536,   94.4779, 77.0033,  92.7825,  13.8526 },
                         LabTestData{  91.1132, -48.0874,  -14.1312, 53.8014,  78.7327, 106.9496 },
                         LabTestData{  60.3242,  98.2343,  -60.8249, 59.2894,  28.4848,  96.9638 },
                         LabTestData{   5.9482,  -0.6671,   -8.1376,  0.6096,   0.6585,   1.3230 },
                         LabTestData{  60.0279,  -5.0385,  -31.7121, 25.5444,  28.1543,  58.7206 },
                         LabTestData{  56.2602,  33.6047,   26.8901, 31.2431,  24.1726,  12.6913 },
                         LabTestData{  74.2912, -66.1455,   42.3544, 25.6330,  47.1587,  19.8057 },
                     }),
                     colorData) {
    MeaColors::Lab lab = MeaColors::XYZtoLab(MeaColors::XYZ(colorData.x, colorData.y, colorData.z));
    BOOST_TEST(std::round(10000.0 * lab.l) / 10000.0 == colorData.l, tt::tolerance(DBL_EPSILON));
    BOOST_TEST(std::round(10000.0 * lab.a) / 10000.0 == colorData.a, tt::tolerance(DBL_EPSILON));
    BOOST_TEST(std::round(10000.0 * lab.b) / 10000.0 == colorData.b, tt::tolerance(DBL_EPSILON));
}

BOOST_DATA_TEST_CASE(TestRGBtoLab,
                     bdata::make({
                         LabRGBTestData{   0.0000,   0.0000,    0.0000,   0,   0,   0 },
                         LabRGBTestData{ 100.0000,   0.0000,    0.0000, 255, 255, 255 },
                         LabRGBTestData{  53.5850,   0.0000,    0.0000, 128, 128, 128 },
                         LabRGBTestData{  53.2408,  80.0925,   67.2032, 255,   0,   0 },
                         LabRGBTestData{  87.7347, -86.1827,   83.1793,   0, 255,   0 },
                         LabRGBTestData{  32.2970,  79.1875, -107.8602,   0,   0, 255 },
                         LabRGBTestData{  97.1393, -21.5537,   94.4780, 255, 255,   0 },
                         LabRGBTestData{  91.1132, -48.0875,  -14.1312,   0, 255, 255 },
                         LabRGBTestData{  60.3242,  98.2343,  -60.8249, 255,   0, 255 },
                         LabRGBTestData{   5.9485,  -0.6687,   -8.1374,  10,  20,  30 },
                         LabRGBTestData{  60.0279,  -5.0388,  -31.7120,  90, 150, 200 },
                         LabRGBTestData{  56.2602,  33.6045,   26.8902, 200, 110,  90 },
                         LabRGBTestData{  74.2911, -66.1453,   42.3544,  20, 210, 100 },
                     }),
                     colorData) {
    MeaColors::Lab lab = MeaColors::RGBtoLab(RGB(colorData.red, colorData.green, colorData.blue));
    BOOST_TEST(std::round(10000.0 * lab.l) / 10000.0 == colorData.l, tt::tolerance(DBL_EPSILON));
    BOOST_TEST(std::round(10000.0 * lab.a) / 10000.0 == colorData.a, tt::tolerance(DBL_EPSILON));
    BOOST_TEST(std::round(10000.0 * lab.b) / 10000.0 == colorData.b, tt::tolerance(DBL_EPSILON));
}

BOOST_AUTO_TEST_CASE(TestInterpolateColor) {
    COLORREF color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 50);
    BOOST_TEST(color == RGB(128, 128, 128));

    color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 0);
    BOOST_TEST(color == RGB(0, 0, 0));

    color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 100);
    BOOST_TEST(color == RGB(255, 255, 255));

    color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 10);
    BOOST_TEST(color ==  RGB(26, 26, 26));

    color = MeaColors::InterpolateColor(RGB(10, 20, 30), RGB(200, 150, 100), 50);
    BOOST_TEST(color == RGB(44, 129, 44));
}

BOOST_DATA_TEST_CASE(TestColorDifferenceLab,
                     // Test data from https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/dataNprograms/ciede2000testdata.txt
                     bdata::make({
                         DiffTestData{ 50.0000,   2.6772, -79.7751, 50.0000,   0.0000, -82.7485,  2.0425 },
                         DiffTestData{ 50.0000,   3.1571, -77.2803, 50.0000,   0.0000, -82.7485,  2.8615 },
                         DiffTestData{ 50.0000,   2.8361, -74.0200, 50.0000,   0.0000, -82.7485,  3.4412 },
                         DiffTestData{ 50.0000,  -1.3802, -84.2814, 50.0000,   0.0000, -82.7485,  1.0000 },
                         DiffTestData{ 50.0000,  -1.1848, -84.8006, 50.0000,   0.0000, -82.7485,  1.0000 },
                         DiffTestData{ 50.0000,  -0.9009, -85.5211, 50.0000,   0.0000, -82.7485,  1.0000 },
                         DiffTestData{ 50.0000,   0.0000,   0.0000, 50.0000,  -1.0000,   2.0000,  2.3669 },
                         DiffTestData{ 50.0000,  -1.0000,   2.0000, 50.0000,   0.0000,   0.0000,  2.3669 },
                         DiffTestData{ 50.0000,   2.4900,  -0.0010, 50.0000,  -2.4900,   0.0009,  7.1792 },
                         DiffTestData{ 50.0000,   2.4900,  -0.0010, 50.0000,  -2.4900,   0.0010,  7.1792 },
                         DiffTestData{ 50.0000,   2.4900,  -0.0010, 50.0000,  -2.4900,   0.0011,  7.2195 },
                         DiffTestData{ 50.0000,   2.4900,  -0.0010, 50.0000,  -2.4900,   0.0012,  7.2195 },
                         DiffTestData{ 50.0000,  -0.0010,   2.4900, 50.0000,   0.0009,  -2.4900,  4.8045 },
                         DiffTestData{ 50.0000,  -0.0010,   2.4900, 50.0000,   0.0010,  -2.4900,  4.8045 },
                         DiffTestData{ 50.0000,  -0.0010,   2.4900, 50.0000,   0.0011,  -2.4900,  4.7461 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 50.0000,   0.0000,  -2.5000,  4.3065 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 73.0000,  25.0000, -18.0000, 27.1492 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 61.0000,  -5.0000,  29.0000, 22.8977 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 56.0000, -27.0000,  -3.0000, 31.9030 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 58.0000,  24.0000,  15.0000, 19.4535 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 50.0000,   3.1736,   0.5854,  1.0000 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 50.0000,   3.2972,   0.0000,  1.0000 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 50.0000,   1.8634,   0.5757,  1.0000 },
                         DiffTestData{ 50.0000,   2.5000,   0.0000, 50.0000,   3.2592,   0.3350,  1.0000 },
                         DiffTestData{ 60.2574, -34.0099,  36.2677, 60.4626, -34.1751,  39.4387,  1.2644 },
                         DiffTestData{ 63.0109, -31.0961,  -5.8663, 62.8187, -29.7946,  -4.0864,  1.2630 },
                         DiffTestData{ 61.2901,   3.7196,  -5.3901, 61.4292,   2.2480,  -4.9620,  1.8731 },
                         DiffTestData{ 35.0831, -44.1164,   3.7933, 35.0232, -40.0716,   1.5901,  1.8645 },
                         DiffTestData{ 22.7233,  20.0904, -46.6940, 23.0331,  14.9730, -42.5619,  2.0373 },
                         DiffTestData{ 36.4612,  47.8580,  18.3852, 36.2715,  50.5065,  21.2231,  1.4146 },
                         DiffTestData{ 90.8027,  -2.0831,   1.4410, 91.1528,  -1.6435,   0.0447,  1.4441 },
                         DiffTestData{ 90.9257,  -0.5406,  -0.9208, 88.6381,  -0.8985,  -0.7239,  1.5381 },
                         DiffTestData{ 6.7747,   -0.2908,  -2.4247,  5.8714,  -0.0985,  -2.2286,  0.6377 },
                         DiffTestData{ 2.0776,    0.0795,  -1.1350,  0.9033,  -0.0636,  -0.5514,  0.9082 },
                     }),
                     colorData) {
    MeaColors::Lab lab1(colorData.l1, colorData.a1, colorData.b1);
    MeaColors::Lab lab2(colorData.l2, colorData.a2, colorData.b2);
    double diff = MeaColors::ColorDifference(lab1, lab2);
    BOOST_TEST(std::round(10000.0 * diff) / 10000.0 == colorData.diff, tt::tolerance(DBL_EPSILON));
}

BOOST_AUTO_TEST_CASE(TestMatchBasicColor) {
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(0, 0, 0))->name == _T("black"));
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(255, 0, 0))->name == _T("red"));
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(200, 0, 0))->name == _T("red"));
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(90, 90, 10))->name == _T("olive"));
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(5, 90, 10))->name == _T("green"));
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(195, 17, 210))->name == _T("fuchsia"));
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(195, 200, 230))->name == _T("silver"));
    BOOST_TEST(MeaColors::MatchBasicColor(RGB(240, 215, 230))->name == _T("white"));
}

BOOST_AUTO_TEST_CASE(TestMatchExtendedColor) {
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(0, 0, 0))->name == _T("black"));
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(255, 0, 0))->name == _T("red"));
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(135, 40, 230))->name == _T("blueviolet"));
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(253, 155, 5))->name == _T("orange"));
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(253, 240, 5))->name == _T("yellow"));
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(221, 224, 215))->name == _T("gainsboro"));
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(32, 178, 170))->name == _T("lightseagreen"));
    BOOST_TEST(MeaColors::MatchExtendedColor(RGB(70, 210, 200))->name == _T("mediumturquoise"));
}

BOOST_AUTO_TEST_CASE(TestColorItem) {
    MeaColors::Set(MeaColors::LineFore, RGB(10, 20, 30));
    BOOST_TEST(MeaColors::Get(MeaColors::LineFore) == RGB(10, 20, 30));
    BOOST_TEST(MeaColors::GetR(MeaColors::LineFore) == 10);
    BOOST_TEST(MeaColors::GetG(MeaColors::LineFore) == 20);
    BOOST_TEST(MeaColors::GetB(MeaColors::LineFore) == 30);
}

BOOST_AUTO_TEST_CASE(TestOpacity) {
    MeaColors::SetA(MeaColors::LineFore, 25);
    BOOST_TEST(MeaColors::GetA(MeaColors::LineFore) == 25);
}

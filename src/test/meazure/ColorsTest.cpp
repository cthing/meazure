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

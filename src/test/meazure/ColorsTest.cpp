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
#define COMPILE_LAYERED_WINDOW_STUBS
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <meazure/ui/LayeredWindows.h>
#include <meazure/graphics/Colors.h>
#include <float.h>

namespace bt = boost::unit_test;
namespace tt = boost::test_tools;
namespace bdata = boost::unit_test::data;


struct GlobalFixture {
    GlobalFixture() {
        if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
            BOOST_FAIL("Fatal Error: MFC initialization failed");
        }
    }

    CWinApp theApp;
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);

struct ColorTestData {
    double hue;
    double saturation;
    double lightness;
    int red;
    int green;
    int blue;
};
std::ostream& operator<<(std::ostream& os, const ColorTestData& cd) {
    os << '(' << cd.hue << ',' << cd.saturation << ',' << cd.lightness << '):(' << cd.red << ',' << cd.green << ',' << cd.blue << ')';
    return os;
}


BOOST_DATA_TEST_CASE(TestRGBtoHSL,
                     bdata::make({
                         ColorTestData{     0.0, 0.0, 0.0,   0,   0,   0 },
                         ColorTestData{     0.0, 1.0, 0.5, 255,   0,   0 },
                         ColorTestData{ 1.0/3.0, 1.0, 0.5,   0, 255,   0 },
                         ColorTestData{ 2.0/3.0, 1.0, 0.5,   0,   0, 255 },
                         ColorTestData{     0.0, 0.0, 1.0, 255, 255, 255 },
                         ColorTestData{     0.0, 1.0, 0.25098039215686274, 128,   0,   0 },
                         ColorTestData{ 1.0/3.0, 1.0, 0.25098039215686274,   0, 128,   0 },
                         ColorTestData{ 2.0/3.0, 1.0, 0.25098039215686274,   0,   0, 128 },
                         ColorTestData{ 0.55555555555555547, 0.60000000000000009, 0.49019607843137253, 50, 150, 200 },
                     }),
                     colorData) {
    HSL hsl;

    MeaColors::RGBtoHSL(RGB(colorData.red, colorData.green, colorData.blue), hsl);
    BOOST_TEST(hsl.hue == colorData.hue, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(hsl.saturation == colorData.saturation, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(hsl.lightness == colorData.lightness, tt::tolerance(FLT_EPSILON));
}

BOOST_DATA_TEST_CASE(TestHSLtoRGB,
                     bdata::make({
                         ColorTestData{     0.0, 0.0,  0.0,   0,   0,   0 },
                         ColorTestData{     0.0, 1.0,  0.5, 255,   0,   0 },
                         ColorTestData{ 1.0/3.0, 1.0,  0.5,   0, 255,   0 },
                         ColorTestData{ 2.0/3.0, 1.0,  0.5,   0,   0, 255 },
                         ColorTestData{     0.0, 0.0,  1.0, 255, 255, 255 },
                         ColorTestData{     0.0, 1.0, 0.25, 127,   0,   0 },
                         ColorTestData{ 1.0/3.0, 1.0, 0.25,   0, 127,   0 },
                         ColorTestData{ 2.0/3.0, 1.0, 0.25,   0,   0, 127 },
                         ColorTestData{  0.5556, 0.6, 0.49,  49, 149, 199 }
                     }),
                     colorData) {
    HSL hsl{ colorData.hue, colorData.saturation, colorData.lightness };
    COLORREF rgb;
    MeaColors::HSLtoRGB(hsl, rgb);
    BOOST_TEST(rgb == RGB(colorData.red, colorData.green, colorData.blue));
}

BOOST_AUTO_TEST_CASE(TestInterpolateColor) {
    COLORREF color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 50);
    BOOST_TEST(color == RGB(127, 127, 127));

    color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 0);
    BOOST_TEST(color == RGB(0, 0, 0));

    color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 100);
    BOOST_TEST(color == RGB(255, 255, 255));

    color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 10);
    BOOST_TEST(color ==  RGB(25, 25, 25));

    color = MeaColors::InterpolateColor(RGB(10, 20, 30), RGB(200, 150, 100), 50);
    BOOST_TEST(color == RGB(43, 126, 43));
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

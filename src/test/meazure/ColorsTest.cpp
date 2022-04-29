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
#include "StdAfx.h"
#define COMPILE_LAYERED_WINDOW_STUBS
#include "LayeredWindows.h"
#include <Colors.h>
#include <boost/test/included/unit_test.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


using namespace std;
using namespace boost::unit_test;
using boost::unit_test_framework::test_suite;


namespace {
    void TestRGBtoHSL() {
        HSL hsl;

        MeaColors::RGBtoHSL(RGB(0, 0, 0), hsl);
        BOOST_CHECK_CLOSE(0.0, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(0.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(0.0, hsl.lightness, 0.0001);

        MeaColors::RGBtoHSL(RGB(255, 0, 0), hsl);
        BOOST_CHECK_CLOSE(0.0, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(1.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(0.5, hsl.lightness, 0.0001);

        MeaColors::RGBtoHSL(RGB(0, 255, 0), hsl);
        BOOST_CHECK_CLOSE(0.3333333, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(1.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(0.5, hsl.lightness, 0.0001);

        MeaColors::RGBtoHSL(RGB(0, 0, 255), hsl);
        BOOST_CHECK_CLOSE(0.6666666, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(1.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(0.5, hsl.lightness, 0.0001);

        MeaColors::RGBtoHSL(RGB(255, 255, 255), hsl);
        BOOST_CHECK_CLOSE(0.0, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(0.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(1.0, hsl.lightness, 0.0001);

        MeaColors::RGBtoHSL(RGB(128, 0, 0), hsl);
        BOOST_CHECK_CLOSE(0.0, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(1.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(0.25, hsl.lightness, 0.5);

        MeaColors::RGBtoHSL(RGB(0, 128, 0), hsl);
        BOOST_CHECK_CLOSE(0.3333333, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(1.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(0.25, hsl.lightness, 0.5);

        MeaColors::RGBtoHSL(RGB(0, 0, 128), hsl);
        BOOST_CHECK_CLOSE(0.6666666, hsl.hue, 0.0001);
        BOOST_CHECK_CLOSE(1.0, hsl.saturation, 0.0001);
        BOOST_CHECK_CLOSE(0.25, hsl.lightness, 0.5);

        MeaColors::RGBtoHSL(RGB(50, 150, 200), hsl);
        BOOST_CHECK_CLOSE(0.5556, hsl.hue, 0.1);
        BOOST_CHECK_CLOSE(0.6, hsl.saturation, 0.1);
        BOOST_CHECK_CLOSE(0.49, hsl.lightness, 0.1);
    }

    void TestHSLtoRGB() {
        COLORREF rgb;
        HSL hsl;

        hsl.hue = 0.0;
        hsl.saturation = 0.0;
        hsl.lightness = 0.0;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(0, 0, 0), rgb);

        hsl.hue = 0.0;
        hsl.saturation = 1.0;
        hsl.lightness = 0.5;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(255, 0, 0), rgb);

        hsl.hue = 0.3333333;
        hsl.saturation = 1.0;
        hsl.lightness = 0.5;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(0, 255, 0), rgb);

        hsl.hue = 0.6666666;
        hsl.saturation = 1.0;
        hsl.lightness = 0.5;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(0, 0, 255), rgb);

        hsl.hue = 0.0;
        hsl.saturation = 0.0;
        hsl.lightness = 1.0;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(255, 255, 255), rgb);

        hsl.hue = 0.0;
        hsl.saturation = 1.0;
        hsl.lightness = 0.25;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(127, 0, 0), rgb);

        hsl.hue = 0.3333333;
        hsl.saturation = 1.0;
        hsl.lightness = 0.25;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(0, 127, 0), rgb);

        hsl.hue = 0.6666666;
        hsl.saturation = 1.0;
        hsl.lightness = 0.25;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(0, 0, 127), rgb);

        hsl.hue = 0.5556;
        hsl.saturation = 0.6;
        hsl.lightness = 0.49;
        MeaColors::HSLtoRGB(hsl, rgb);
        BOOST_CHECK_EQUAL(RGB(49, 149, 199), rgb);
    }

    void TestInterpolateColor() {
        COLORREF color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 50);
        BOOST_CHECK_EQUAL(RGB(127, 127, 127), color);

        color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 0);
        BOOST_CHECK_EQUAL(RGB(0, 0, 0), color);

        color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 100);
        BOOST_CHECK_EQUAL(RGB(255, 255, 255), color);

        color = MeaColors::InterpolateColor(RGB(0, 0, 0), RGB(255, 255, 255), 10);
        BOOST_CHECK_EQUAL(RGB(25, 25, 25), color);

        color = MeaColors::InterpolateColor(RGB(10, 20, 30), RGB(200, 150, 100), 50);
        BOOST_CHECK_EQUAL(RGB(43, 126, 43), color);
    }

    void TestColorItem() {
        MeaColors::Set(MeaColors::LineFore, RGB(10, 20, 30));
        BOOST_CHECK_EQUAL(RGB(10, 20, 30), MeaColors::Get(MeaColors::LineFore));
        BOOST_CHECK_EQUAL(10, MeaColors::GetR(MeaColors::LineFore));
        BOOST_CHECK_EQUAL(20, MeaColors::GetG(MeaColors::LineFore));
        BOOST_CHECK_EQUAL(30, MeaColors::GetB(MeaColors::LineFore));
    }

    void TestOpacity() {
        MeaColors::SetA(MeaColors::LineFore, 25);
        BOOST_CHECK_EQUAL(25, MeaColors::GetA(MeaColors::LineFore));
    }
}

test_suite* init_unit_test_suite(int argc, char* argv[]) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    test_suite* suite = BOOST_TEST_SUITE("Colors Tests");
    suite->add(BOOST_TEST_CASE(&TestRGBtoHSL));
    suite->add(BOOST_TEST_CASE(&TestHSLtoRGB));
    suite->add(BOOST_TEST_CASE(&TestInterpolateColor));
    suite->add(BOOST_TEST_CASE(&TestColorItem));
    suite->add(BOOST_TEST_CASE(*TestOpacity));
    return suite;
}

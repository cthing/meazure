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
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/Utils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


namespace {
    void TestInitialSize() {
        FSIZE fs1;
        BOOST_CHECK_CLOSE(0.0, fs1.cx, 0.0001);
        BOOST_CHECK_CLOSE(0.0, fs1.cy, 0.0001);

        FSIZE fs2(1.0, 2.0);
        BOOST_CHECK_CLOSE(1.0, fs2.cx, 0.0001);
        BOOST_CHECK_CLOSE(2.0, fs2.cy, 0.0001);
    }

    void TestAddFSize() {
        FSIZE fs1;
        FSIZE fs2;

        fs1.cx = 10.0;
        fs1.cy = 11.0;

        fs2.cx = 5.0;
        fs2.cy = 6.0;

        FSIZE fs3 = fs1 + fs2;
        BOOST_CHECK_CLOSE(15.0, fs3.cx, 0.0001);
        BOOST_CHECK_CLOSE(17.0, fs3.cy, 0.0001);
    }

    void TestAddValue() {
        FSIZE fs1;
        fs1.cx = 10.0;
        fs1.cy = 11.0;

        FSIZE fs2 = fs1 + 13.0;
        BOOST_CHECK_CLOSE(23.0, fs2.cx, 0.0001);
        BOOST_CHECK_CLOSE(24.0, fs2.cy, 0.0001);
    }

    void TestMultFSize() {
        FSIZE fs1;
        FSIZE fs2;

        fs1.cx = 10.0;
        fs1.cy = 11.0;

        fs2.cx = 2.0;
        fs2.cy = 3.0;

        FSIZE fs3 = fs1 * fs2;
        BOOST_CHECK_CLOSE(20.0, fs3.cx, 0.0001);
        BOOST_CHECK_CLOSE(33.0, fs3.cy, 0.0001);
    }

    void TestMultSize() {
        FSIZE fs1;
        SIZE s;

        fs1.cx = 10.0;
        fs1.cy = 11.0;

        s.cx = 2;
        s.cy = 3;

        FSIZE fs2 = fs1 * s;
        BOOST_CHECK_CLOSE(20.0, fs2.cx, 0.0001);
        BOOST_CHECK_CLOSE(33.0, fs2.cy, 0.0001);
    }

    void TestMultValue() {
        FSIZE fs1;
        fs1.cx = 10.0;
        fs1.cy = 11.0;

        FSIZE fs2 = fs1 * 2.0;
        BOOST_CHECK_CLOSE(20.0, fs2.cx, 0.0001);
        BOOST_CHECK_CLOSE(22.0, fs2.cy, 0.0001);
    }

    void TestAddAssign() {
        FSIZE fs1;
        FSIZE fs2;

        fs1.cx = 10.0;
        fs1.cy = 11.0;

        fs2.cx = 5.0;
        fs2.cy = 6.0;

        fs1 += fs2;
        BOOST_CHECK_CLOSE(15.0, fs1.cx, 0.0001);
        BOOST_CHECK_CLOSE(17.0, fs1.cy, 0.0001);
    }

    void TestSubAssign() {
        FSIZE fs1;
        FSIZE fs2;

        fs1.cx = 10.0;
        fs1.cy = 11.0;

        fs2.cx = 3.0;
        fs2.cy = 6.0;

        fs1 -= fs2;
        BOOST_CHECK_CLOSE(7.0, fs1.cx, 0.0001);
        BOOST_CHECK_CLOSE(5.0, fs1.cy, 0.0001);
    }

    void TestMultAssign() {
        FSIZE fs1;
        FSIZE fs2;

        fs1.cx = 10.0;
        fs1.cy = 11.0;

        fs2.cx = 3.0;
        fs2.cy = 6.0;

        fs1 *= fs2;
        BOOST_CHECK_CLOSE(30.0, fs1.cx, 0.0001);
        BOOST_CHECK_CLOSE(66.0, fs1.cy, 0.0001);
    }

    void TestLog10() {
        FSIZE fs1;
        fs1.cx = 1000.0;
        fs1.cy = 100.0;

        FSIZE fs2 = log10(fs1);
        BOOST_CHECK_CLOSE(3.0, fs2.cx, 0.0001);
        BOOST_CHECK_CLOSE(2.0, fs2.cy, 0.0001);
    }

    void TestFloor() {
        FSIZE fs1;
        fs1.cx = 12.98;
        fs1.cy = 4.01;

        FSIZE fs2 = floor(fs1);
        BOOST_CHECK_CLOSE(12.0, fs2.cx, 0.0001);
        BOOST_CHECK_CLOSE(4.0, fs2.cy, 0.0001);
    }

    void TestValueSub() {
        FSIZE fs1;
        fs1.cx = 11.0;
        fs1.cy = 45.0;

        FSIZE fs2 = 200.0 - fs1;
        BOOST_CHECK_CLOSE(189.0, fs2.cx, 0.0001);
        BOOST_CHECK_CLOSE(155.0, fs2.cy, 0.0001);
    }

    void TestInitialRectangle() {
        FRECT fr1;
        BOOST_CHECK_CLOSE(0.0, fr1.top, 0.0001);
        BOOST_CHECK_CLOSE(0.0, fr1.bottom, 0.0001);
        BOOST_CHECK_CLOSE(0.0, fr1.left, 0.0001);
        BOOST_CHECK_CLOSE(0.0, fr1.right, 0.0001);

        FRECT fr2(1.0, 2.0, 3.0, 4.0);
        BOOST_CHECK_CLOSE(1.0, fr2.top, 0.0001);
        BOOST_CHECK_CLOSE(2.0, fr2.bottom, 0.0001);
        BOOST_CHECK_CLOSE(3.0, fr2.left, 0.0001);
        BOOST_CHECK_CLOSE(4.0, fr2.right, 0.0001);
    }

    void TestInitialPoint() {
        FPOINT fp1;
        BOOST_CHECK_CLOSE(0.0, fp1.x, 0.0001);
        BOOST_CHECK_CLOSE(0.0, fp1.y, 0.0001);

        FPOINT fp2(1.0, 2.0);
        BOOST_CHECK_CLOSE(1.0, fp2.x, 0.0001);
        BOOST_CHECK_CLOSE(2.0, fp2.y, 0.0001);
    }
}

boost::unit_test_framework::test_suite* init_unit_test_suite(int, char*[]) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        std::cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    boost::unit_test_framework::test_suite* fsizeTestSuite = BOOST_TEST_SUITE("FSIZE Tests");
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestInitialSize));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestAddFSize));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestAddValue));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestMultFSize));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestMultSize));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestMultValue));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestAddAssign));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestSubAssign));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestMultAssign));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestLog10));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestFloor));
    fsizeTestSuite->add(BOOST_TEST_CASE(&TestValueSub));
    boost::unit_test_framework::framework::master_test_suite().add(fsizeTestSuite);

    boost::unit_test_framework::test_suite* frectTestSuite = BOOST_TEST_SUITE("FRECT Tests");
    frectTestSuite->add(BOOST_TEST_CASE(&TestInitialRectangle));
    boost::unit_test_framework::framework::master_test_suite().add(frectTestSuite);

    boost::unit_test_framework::test_suite* fpointTestSuite = BOOST_TEST_SUITE("FPOINT Tests");
    fpointTestSuite->add(BOOST_TEST_CASE(&TestInitialPoint));
    boost::unit_test_framework::framework::master_test_suite().add(fpointTestSuite);

    return nullptr;
}

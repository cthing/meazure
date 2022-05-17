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
#include <Utils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


using namespace std;
using namespace boost::unit_test;
using boost::unit_test_framework::test_suite;


namespace {
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

    void TestDblToStr() {
        CString str = MeaUtils::DblToStr(123.456);
        BOOST_CHECK_EQUAL(CString(_T("123.456")), str);

        str = MeaUtils::DblToStr(-123.456);
        BOOST_CHECK_EQUAL(CString(_T("-123.456")), str);

        str = MeaUtils::DblToStr(+123.456);
        BOOST_CHECK_EQUAL(CString(_T("123.456")), str);

        str = MeaUtils::DblToStr(0);
        BOOST_CHECK_EQUAL(CString(_T("0.0")), str);

        str = MeaUtils::DblToStr(0.0);
        BOOST_CHECK_EQUAL(CString(_T("0.0")), str);
    }

    void TestIsNumber() {
        BOOST_CHECK(MeaUtils::IsNumber(_T("123")));
        BOOST_CHECK(MeaUtils::IsNumber(_T("+123")));
        BOOST_CHECK(MeaUtils::IsNumber(_T("-123")));
        BOOST_CHECK(MeaUtils::IsNumber(_T("1.30")));
        BOOST_CHECK(!MeaUtils::IsNumber(_T("a123")));
        BOOST_CHECK(!MeaUtils::IsNumber(_T("")));

        double value;
        MeaUtils::IsNumber(_T("123"), &value);
        BOOST_CHECK_CLOSE(123.0, value, 0.0001);
        MeaUtils::IsNumber(_T("+123"), &value);
        BOOST_CHECK_CLOSE(123.0, value, 0.0001);
        MeaUtils::IsNumber(_T("-123"), &value);
        BOOST_CHECK_CLOSE(-123.0, value, 0.0001);
        MeaUtils::IsNumber(_T("1.30"), &value);
        BOOST_CHECK_CLOSE(1.3, value, 0.0001);
    }

    void TestIsBoolean() {
        BOOST_CHECK(MeaUtils::IsBoolean(_T("1")));
        BOOST_CHECK(MeaUtils::IsBoolean(_T("true")));
        BOOST_CHECK(MeaUtils::IsBoolean(_T("TRUE")));
        BOOST_CHECK(MeaUtils::IsBoolean(_T("0")));
        BOOST_CHECK(MeaUtils::IsBoolean(_T("false")));
        BOOST_CHECK(MeaUtils::IsBoolean(_T("FALSE")));
        BOOST_CHECK(!MeaUtils::IsBoolean(_T("10")));
        BOOST_CHECK(!MeaUtils::IsBoolean(_T("-1")));
        BOOST_CHECK(!MeaUtils::IsBoolean(_T("ON")));
        BOOST_CHECK(!MeaUtils::IsBoolean(_T("OFF")));
        BOOST_CHECK(!MeaUtils::IsBoolean(_T("")));

        bool value;
        MeaUtils::IsBoolean(_T("1"), &value);
        BOOST_CHECK(value);
        MeaUtils::IsBoolean(_T("true"), &value);
        BOOST_CHECK(value);
        MeaUtils::IsBoolean(_T("TRUE"), &value);
        BOOST_CHECK(value);
        MeaUtils::IsBoolean(_T("0"), &value);
        BOOST_CHECK(!value);
        MeaUtils::IsBoolean(_T("false"), &value);
        BOOST_CHECK(!value);
        MeaUtils::IsBoolean(_T("FALSE"), &value);
        BOOST_CHECK(!value);
    }

    void TestLFtoCRLF() {
        BOOST_CHECK_EQUAL(CString(_T("\r\n")), MeaUtils::LFtoCRLF(_T("\n")));
        BOOST_CHECK_EQUAL(CString(_T("\r\n")), MeaUtils::LFtoCRLF(_T("\r\n")));
        BOOST_CHECK_EQUAL(CString(_T("\r\nHello\r\n\r\nWorld\r\n")), MeaUtils::LFtoCRLF(_T("\nHello\n\nWorld\n")));
        BOOST_CHECK_EQUAL(CString(_T("\r\nHello\r\n\r\nWorld\r\n")), MeaUtils::LFtoCRLF(_T("\r\nHello\n\r\nWorld\n")));
        BOOST_CHECK_EQUAL(CString(_T("")), MeaUtils::LFtoCRLF(_T("")));
    }

    void TestCRLFtoLF() {
        BOOST_CHECK_EQUAL(CString(_T("\n")), MeaUtils::CRLFtoLF(_T("\r\n")));
        BOOST_CHECK_EQUAL(CString(_T("\n")), MeaUtils::CRLFtoLF(_T("\n")));
        BOOST_CHECK_EQUAL(CString(_T("\nHello\n\nWorld\n")), MeaUtils::CRLFtoLF(_T("\r\nHello\r\n\r\nWorld\r\n")));
        BOOST_CHECK_EQUAL(CString(_T("\nHello\n\nWorld\n")), MeaUtils::CRLFtoLF(_T("\r\nHello\n\r\nWorld\n")));
        BOOST_CHECK_EQUAL(CString(_T("")), MeaUtils::LFtoCRLF(_T("")));
    }
}

test_suite* init_unit_test_suite(int, char*[]) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    test_suite* fsizeTestSuite = BOOST_TEST_SUITE("FSIZE Tests");
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
    framework::master_test_suite().add(fsizeTestSuite);

    test_suite* utilsTestSuite = BOOST_TEST_SUITE("Utils Tests");
    utilsTestSuite->add(BOOST_TEST_CASE(&TestDblToStr));
    utilsTestSuite->add(BOOST_TEST_CASE(&TestIsNumber));
    utilsTestSuite->add(BOOST_TEST_CASE(&TestIsBoolean));
    utilsTestSuite->add(BOOST_TEST_CASE(&TestLFtoCRLF));
    utilsTestSuite->add(BOOST_TEST_CASE(&TestCRLFtoLF));
    framework::master_test_suite().add(utilsTestSuite);

    return nullptr;
}

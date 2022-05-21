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
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/StringUtils.h>

using namespace boost::unit_test;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


namespace {
    void TestDblToStr() {
        CString str = MeaStringUtils::DblToStr(123.456);
        BOOST_CHECK_EQUAL(CString(_T("123.456")), str);

        str = MeaStringUtils::DblToStr(-123.456);
        BOOST_CHECK_EQUAL(CString(_T("-123.456")), str);

        str = MeaStringUtils::DblToStr(+123.456);
        BOOST_CHECK_EQUAL(CString(_T("123.456")), str);

        str = MeaStringUtils::DblToStr(0);
        BOOST_CHECK_EQUAL(CString(_T("0.0")), str);

        str = MeaStringUtils::DblToStr(0.0);
        BOOST_CHECK_EQUAL(CString(_T("0.0")), str);
    }

    void TestIsNumber() {
        BOOST_CHECK(MeaStringUtils::IsNumber(_T("123")));
        BOOST_CHECK(MeaStringUtils::IsNumber(_T("+123")));
        BOOST_CHECK(MeaStringUtils::IsNumber(_T("-123")));
        BOOST_CHECK(MeaStringUtils::IsNumber(_T("1.30")));
        BOOST_CHECK(!MeaStringUtils::IsNumber(_T("a123")));
        BOOST_CHECK(!MeaStringUtils::IsNumber(_T("")));

        double value;
        MeaStringUtils::IsNumber(_T("123"), &value);
        BOOST_CHECK_CLOSE(123.0, value, 0.0001);
        MeaStringUtils::IsNumber(_T("+123"), &value);
        BOOST_CHECK_CLOSE(123.0, value, 0.0001);
        MeaStringUtils::IsNumber(_T("-123"), &value);
        BOOST_CHECK_CLOSE(-123.0, value, 0.0001);
        MeaStringUtils::IsNumber(_T("1.30"), &value);
        BOOST_CHECK_CLOSE(1.3, value, 0.0001);
    }

    void TestIsBoolean() {
        BOOST_CHECK(MeaStringUtils::IsBoolean(_T("1")));
        BOOST_CHECK(MeaStringUtils::IsBoolean(_T("true")));
        BOOST_CHECK(MeaStringUtils::IsBoolean(_T("TRUE")));
        BOOST_CHECK(MeaStringUtils::IsBoolean(_T("0")));
        BOOST_CHECK(MeaStringUtils::IsBoolean(_T("false")));
        BOOST_CHECK(MeaStringUtils::IsBoolean(_T("FALSE")));
        BOOST_CHECK(!MeaStringUtils::IsBoolean(_T("10")));
        BOOST_CHECK(!MeaStringUtils::IsBoolean(_T("-1")));
        BOOST_CHECK(!MeaStringUtils::IsBoolean(_T("ON")));
        BOOST_CHECK(!MeaStringUtils::IsBoolean(_T("OFF")));
        BOOST_CHECK(!MeaStringUtils::IsBoolean(_T("")));

        bool value;
        MeaStringUtils::IsBoolean(_T("1"), &value);
        BOOST_CHECK(value);
        MeaStringUtils::IsBoolean(_T("true"), &value);
        BOOST_CHECK(value);
        MeaStringUtils::IsBoolean(_T("TRUE"), &value);
        BOOST_CHECK(value);
        MeaStringUtils::IsBoolean(_T("0"), &value);
        BOOST_CHECK(!value);
        MeaStringUtils::IsBoolean(_T("false"), &value);
        BOOST_CHECK(!value);
        MeaStringUtils::IsBoolean(_T("FALSE"), &value);
        BOOST_CHECK(!value);
    }

    void TestLFtoCRLF() {
        BOOST_CHECK_EQUAL(CString(_T("\r\n")), MeaStringUtils::LFtoCRLF(_T("\n")));
        BOOST_CHECK_EQUAL(CString(_T("\r\n")), MeaStringUtils::LFtoCRLF(_T("\r\n")));
        BOOST_CHECK_EQUAL(CString(_T("\r\nHello\r\n\r\nWorld\r\n")), MeaStringUtils::LFtoCRLF(_T("\nHello\n\nWorld\n")));
        BOOST_CHECK_EQUAL(CString(_T("\r\nHello\r\n\r\nWorld\r\n")), MeaStringUtils::LFtoCRLF(_T("\r\nHello\n\r\nWorld\n")));
        BOOST_CHECK_EQUAL(CString(_T("")), MeaStringUtils::LFtoCRLF(_T("")));
    }

    void TestCRLFtoLF() {
        BOOST_CHECK_EQUAL(CString(_T("\n")), MeaStringUtils::CRLFtoLF(_T("\r\n")));
        BOOST_CHECK_EQUAL(CString(_T("\n")), MeaStringUtils::CRLFtoLF(_T("\n")));
        BOOST_CHECK_EQUAL(CString(_T("\nHello\n\nWorld\n")), MeaStringUtils::CRLFtoLF(_T("\r\nHello\r\n\r\nWorld\r\n")));
        BOOST_CHECK_EQUAL(CString(_T("\nHello\n\nWorld\n")), MeaStringUtils::CRLFtoLF(_T("\r\nHello\n\r\nWorld\n")));
        BOOST_CHECK_EQUAL(CString(_T("")), MeaStringUtils::LFtoCRLF(_T("")));
    }
}

test_suite* init_unit_test_suite(int, char* []) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        std::cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    framework::master_test_suite().add(BOOST_TEST_CASE(&TestDblToStr));
    framework::master_test_suite().add(BOOST_TEST_CASE(&TestIsNumber));
    framework::master_test_suite().add(BOOST_TEST_CASE(&TestIsBoolean));
    framework::master_test_suite().add(BOOST_TEST_CASE(&TestLFtoCRLF));
    framework::master_test_suite().add(BOOST_TEST_CASE(&TestCRLFtoLF));

    return nullptr;
}

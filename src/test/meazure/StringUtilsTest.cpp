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
#define BOOST_TEST_MODULE StringUtilsTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/StringUtils.h>
#include <float.h>

namespace bt = boost::unit_test;


BOOST_AUTO_TEST_CASE(TestIntToStr) {
    BOOST_TEST(MeaStringUtils::IntToStr(0) == _T("0"));
    BOOST_TEST(MeaStringUtils::IntToStr(10) == _T("10"));
    BOOST_TEST(MeaStringUtils::IntToStr(+10) == _T("10"));
    BOOST_TEST(MeaStringUtils::IntToStr(-10) == _T("-10"));
}

BOOST_AUTO_TEST_CASE(TestDblToStr) {
    BOOST_TEST(MeaStringUtils::DblToStr(123.456) == _T("123.456"));
    BOOST_TEST(MeaStringUtils::DblToStr(-123.456) == _T("-123.456"));
    BOOST_TEST(MeaStringUtils::DblToStr(+123.456) == _T("123.456"));
    BOOST_TEST(MeaStringUtils::DblToStr(0) == _T("0.0"));
    BOOST_TEST(MeaStringUtils::DblToStr(0.0) == _T("0.0"));
}

BOOST_AUTO_TEST_CASE(TestIsNumber) {
    BOOST_TEST(MeaStringUtils::IsNumber(_T("123")));
    BOOST_TEST(MeaStringUtils::IsNumber(_T("+123")));
    BOOST_TEST(MeaStringUtils::IsNumber(_T("-123")));
    BOOST_TEST(MeaStringUtils::IsNumber(_T("1.30")));
    BOOST_TEST(!MeaStringUtils::IsNumber(_T("a123")));
    BOOST_TEST(!MeaStringUtils::IsNumber(_T("")));
}

BOOST_AUTO_TEST_CASE(TestIsNumberValue, *bt::tolerance(FLT_EPSILON)) {
    double value;
    MeaStringUtils::IsNumber(_T("123"), &value);
    BOOST_TEST(value == 123.0);
    MeaStringUtils::IsNumber(_T("+123"), &value);
    BOOST_TEST(value == 123.0);
    MeaStringUtils::IsNumber(_T("-123"), &value);
    BOOST_TEST(value = -123.0);
    MeaStringUtils::IsNumber(_T("1.30"), &value);
    BOOST_TEST(value == 1.3);
}

BOOST_AUTO_TEST_CASE(TestIsBoolean) {
    BOOST_TEST(MeaStringUtils::IsBoolean(_T("1")));
    BOOST_TEST(MeaStringUtils::IsBoolean(_T("true")));
    BOOST_TEST(MeaStringUtils::IsBoolean(_T("TRUE")));
    BOOST_TEST(MeaStringUtils::IsBoolean(_T("0")));
    BOOST_TEST(MeaStringUtils::IsBoolean(_T("false")));
    BOOST_TEST(MeaStringUtils::IsBoolean(_T("FALSE")));
    BOOST_TEST(!MeaStringUtils::IsBoolean(_T("10")));
    BOOST_TEST(!MeaStringUtils::IsBoolean(_T("-1")));
    BOOST_TEST(!MeaStringUtils::IsBoolean(_T("ON")));
    BOOST_TEST(!MeaStringUtils::IsBoolean(_T("OFF")));
    BOOST_TEST(!MeaStringUtils::IsBoolean(_T("")));
}

BOOST_AUTO_TEST_CASE(TestIsBooleanValue) {
    bool value;
    MeaStringUtils::IsBoolean(_T("1"), &value);
    BOOST_TEST(value);
    MeaStringUtils::IsBoolean(_T("true"), &value);
    BOOST_TEST(value);
    MeaStringUtils::IsBoolean(_T("TRUE"), &value);
    BOOST_TEST(value);
    MeaStringUtils::IsBoolean(_T("0"), &value);
    BOOST_TEST(!value);
    MeaStringUtils::IsBoolean(_T("false"), &value);
    BOOST_TEST(!value);
    MeaStringUtils::IsBoolean(_T("FALSE"), &value);
    BOOST_TEST(!value);
}

BOOST_AUTO_TEST_CASE(TestLFtoCRLF) {
    BOOST_TEST(MeaStringUtils::LFtoCRLF(_T("\n")) == _T("\r\n"));
    BOOST_TEST(MeaStringUtils::LFtoCRLF(_T("\r\n")) == _T("\r\n"));
    BOOST_TEST(MeaStringUtils::LFtoCRLF(_T("\nHello\n\nWorld\n")) == _T("\r\nHello\r\n\r\nWorld\r\n"));
    BOOST_TEST(MeaStringUtils::LFtoCRLF(_T("\r\nHello\n\r\nWorld\n")) == _T("\r\nHello\r\n\r\nWorld\r\n"));
    BOOST_TEST(MeaStringUtils::LFtoCRLF(_T("")) == _T(""));
}

BOOST_AUTO_TEST_CASE(TestCRLFtoLF) {
    BOOST_TEST(MeaStringUtils::CRLFtoLF(_T("\r\n")) == _T("\n"));
    BOOST_TEST(MeaStringUtils::CRLFtoLF(_T("\n")) == _T("\n"));
    BOOST_TEST(MeaStringUtils::CRLFtoLF(_T("\r\nHello\r\n\r\nWorld\r\n")) == _T("\nHello\n\nWorld\n"));
    BOOST_TEST(MeaStringUtils::CRLFtoLF(_T("\r\nHello\n\r\nWorld\n")) == _T("\nHello\n\nWorld\n"));
    BOOST_TEST(MeaStringUtils::LFtoCRLF(_T("")) == _T(""));
}

boost::test_tools::assertion_result IsANSI(boost::unit_test::test_unit_id) {
    return GetACP() == 1252;
}

BOOST_AUTO_TEST_CASE(TestACPtoUTF8String, *boost::unit_test::precondition(IsANSI)) {
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(CString()) == u8"");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(CString(_T("\x00"))) == u8"\u0000");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(CString(_T(" "))) == u8" ");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(CString(_T("\n"))) == u8"\n");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(CString(_T("Hello world"))) == u8"Hello world");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(CString(_T("\x99\x85"))) == u8"\u2122\u2026");

    BOOST_TEST(MeaStringUtils::ACPtoUTF8(nullptr) == u8"");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T("")) == u8"");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T("\x00")) == u8"\u0000");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T(" ")) == u8" ");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T("Hello world")) == u8"Hello world");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T("\x99\x85")) == u8"\u2122\u2026");
}

BOOST_AUTO_TEST_CASE(TestACPtoUTF8Char, *boost::unit_test::precondition(IsANSI)) {
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T('\0')) == u8"\u0000");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T(' ')) == u8" ");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T('\n')) == u8"\n");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T('H')) == u8"H");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T('\x99')) == u8"\u2122");
    BOOST_TEST(MeaStringUtils::ACPtoUTF8(_T('\x85')) == u8"\u2026");
}

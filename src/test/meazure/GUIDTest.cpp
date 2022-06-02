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
#define BOOST_TEST_MODULE GUIDTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/GUID.h>
#include <set>


BOOST_TEST_DONT_PRINT_LOG_VALUE(GUID)


void validateGUID(const MeaGUID& guid) {
    GUID value = guid;
    BOOST_TEST(((value.Data1 > (DWORD)0) ||
                (value.Data2 > (WORD)0) ||
                (value.Data3 > (WORD)0) ||
                (value.Data4[0] > (BYTE)0) ||
                (value.Data4[1] > (BYTE)0) ||
                (value.Data4[2] > (BYTE)0) ||
                (value.Data4[3] > (BYTE)0) ||
                (value.Data4[4] > (BYTE)0) ||
                (value.Data4[5] > (BYTE)0) ||
                (value.Data4[6] > (BYTE)0) ||
                (value.Data4[7] > (BYTE)0)));
}

void validateEqual(const GUID& guid1, const GUID& guid2) {
    BOOST_TEST(guid1.Data1 == guid2.Data1);
    BOOST_TEST(guid1.Data2 == guid2.Data2);
    BOOST_TEST(guid1.Data3 == guid2.Data3);
    BOOST_CHECK_EQUAL_COLLECTIONS(guid1.Data4, guid1.Data4 + 7, guid2.Data4, guid2.Data4 + 7);
}

BOOST_AUTO_TEST_CASE(TestDefaultCtor) {
    MeaGUID guid;
    validateGUID(guid);
}

BOOST_AUTO_TEST_CASE(TestGUIDCtor) {
    GUID guid1;
    HRESULT hr = CoCreateGuid(&guid1);
    if (FAILED(hr)) {
        BOOST_FAIL(hr);
    }
    MeaGUID guid2(guid1);
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestCopyCtor) {
    MeaGUID guid1;
    MeaGUID guid2(guid1);
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestStringCtor) {
    LPCTSTR guid1Str = _T("6B29FC40-CA47-1067-B31D-00DD010662DA");
    GUID guid1;
    guid1.Data1 = 0x6B29FC40;
    guid1.Data2 = 0xCA47;
    guid1.Data3 = 0x1067;
    guid1.Data4[0] = 0xB3;
    guid1.Data4[1] = 0x1D;
    guid1.Data4[2] = 0x00;
    guid1.Data4[3] = 0xDD;
    guid1.Data4[4] = 0x01;
    guid1.Data4[5] = 0x06;
    guid1.Data4[6] = 0x62;
    guid1.Data4[7] = 0xDA;

    MeaGUID guid2(guid1Str);
    validateEqual(guid1, guid2);

    try {
        MeaGUID guid3(_T(""));
        BOOST_FAIL("MeaGUID did not throw");
    } catch (COleException* e) {
        delete e;
    }

    try {
        MeaGUID guid3(_T("FOO"));
        BOOST_FAIL("MeaGUID did not throw");
    } catch (COleException* e) {
        delete e;
    }

    try {
        MeaGUID guid3(_T("6B29FC40-CA47-1067-B31D-00DD010662D"));
        BOOST_FAIL("MeaGUID did not throw");
    } catch (COleException* e) {
        delete e;
    }
}

BOOST_AUTO_TEST_CASE(TestAssignOperatorGUID) {
    GUID guid1;
    HRESULT hr = CoCreateGuid(&guid1);
    if (FAILED(hr)) {
        AfxThrowOleException(hr);
    }
    MeaGUID guid2;
    guid2 = guid1;
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestAssignOperatorMeaGUID) {
    MeaGUID guid1;
    MeaGUID guid2;
    guid2 = guid1;
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestAssignOperatorString) {
    LPCTSTR guid1Str = _T("6B29FC40-CA47-1067-B31D-00DD010662DA");
    GUID guid1;
    guid1.Data1 = 0x6B29FC40;
    guid1.Data2 = 0xCA47;
    guid1.Data3 = 0x1067;
    guid1.Data4[0] = 0xB3;
    guid1.Data4[1] = 0x1D;
    guid1.Data4[2] = 0x00;
    guid1.Data4[3] = 0xDD;
    guid1.Data4[4] = 0x01;
    guid1.Data4[5] = 0x06;
    guid1.Data4[6] = 0x62;
    guid1.Data4[7] = 0xDA;

    MeaGUID guid2;
    guid2 = guid1Str;
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestAssignGUID) {
    GUID guid1;
    HRESULT hr = CoCreateGuid(&guid1);
    if (FAILED(hr)) {
        AfxThrowOleException(hr);
    }
    MeaGUID guid2;
    MeaGUID guid3(guid2.Assign(guid1));
    validateEqual(guid1, guid2);
    validateEqual(guid1, guid3);
}

BOOST_AUTO_TEST_CASE(TestAssignMeaGUID) {
    MeaGUID guid1;
    MeaGUID guid2;
    MeaGUID guid3(guid2.Assign(guid1));
    validateEqual(guid1, guid2);
    validateEqual(guid1, guid3);
}

BOOST_AUTO_TEST_CASE(TestAssignString) {
    LPCTSTR guid1Str = _T("6B29FC40-CA47-1067-B31D-00DD010662DA");
    GUID guid1;
    guid1.Data1 = 0x6B29FC40;
    guid1.Data2 = 0xCA47;
    guid1.Data3 = 0x1067;
    guid1.Data4[0] = 0xB3;
    guid1.Data4[1] = 0x1D;
    guid1.Data4[2] = 0x00;
    guid1.Data4[3] = 0xDD;
    guid1.Data4[4] = 0x01;
    guid1.Data4[5] = 0x06;
    guid1.Data4[6] = 0x62;
    guid1.Data4[7] = 0xDA;

    MeaGUID guid2;
    MeaGUID guid3(guid2.Assign(guid1Str));
    validateEqual(guid1, guid2);
    validateEqual(guid1, guid3);
}

BOOST_AUTO_TEST_CASE(TestEqualityGUID) {
    GUID guid1;
    HRESULT hr = CoCreateGuid(&guid1);
    if (FAILED(hr)) {
        AfxThrowOleException(hr);
    }
    GUID guid2;
    hr = CoCreateGuid(&guid2);
    if (FAILED(hr)) {
        BOOST_FAIL(hr);
    }

    MeaGUID guid3(guid1);
    BOOST_TEST(guid3 == guid1);
    BOOST_TEST(guid3 != guid2);
}

BOOST_AUTO_TEST_CASE(TestEqualityMeaGUID) {
    MeaGUID guid1;
    MeaGUID guid2;

    MeaGUID guid3(guid1);
    BOOST_TEST(guid3 == guid1);
    BOOST_TEST(guid3 != guid2);
}

BOOST_AUTO_TEST_CASE(TestCast) {
    MeaGUID guid1;

    GUID guid2 = guid1;
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestToString) {
    MeaGUID guid1;
    CString guid2Str(guid1.ToString());
    MeaGUID guid2(guid2Str);
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestOutputOperator) {
    MeaGUID guid1;

    std::ostringstream stream;
    stream << guid1;
    CString guid2Str(stream.str().c_str());
    MeaGUID guid2(guid2Str);
    validateEqual(guid1, guid2);
}

BOOST_AUTO_TEST_CASE(TestLess) {
    std::set<MeaGUID, MeaGUID::less> guidSet;
    std::set<MeaGUID, MeaGUID::less>::iterator iter;
    MeaGUID guid1;
    MeaGUID guid2;
    MeaGUID guid3;

    guidSet.insert(guid1);
    guidSet.insert(guid2);
    guidSet.insert(guid3);

    iter = guidSet.find(guid2);
    BOOST_TEST(*iter == guid2);
    iter = guidSet.find(guid1);
    BOOST_TEST(*iter == guid1);
    iter = guidSet.find(guid3);
    BOOST_TEST(*iter == guid3);
}

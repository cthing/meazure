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
#include <GUID.h>
#include <set>
#include <boost/test/included/unit_test.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


using namespace std;
using namespace boost::unit_test;
using boost::unit_test_framework::test_suite;


namespace {
    void validateGUID(const MeaGUID& guid) {
        GUID value = guid;
        BOOST_CHECK((value.Data1 > (DWORD)0) ||
                    (value.Data2 > (WORD)0) ||
                    (value.Data3 > (WORD)0) ||
                    (value.Data4[0] > (BYTE)0) ||
                    (value.Data4[1] > (BYTE)0) ||
                    (value.Data4[2] > (BYTE)0) ||
                    (value.Data4[3] > (BYTE)0) ||
                    (value.Data4[4] > (BYTE)0) ||
                    (value.Data4[5] > (BYTE)0) ||
                    (value.Data4[6] > (BYTE)0) ||
                    (value.Data4[7] > (BYTE)0));
    }

    void validateEqual(const GUID& guid1, const GUID& guid2) {
        BOOST_CHECK_EQUAL(guid1.Data1, guid2.Data1);
        BOOST_CHECK_EQUAL(guid1.Data2, guid2.Data2);
        BOOST_CHECK_EQUAL(guid1.Data3, guid2.Data3);
        BOOST_CHECK_EQUAL(guid1.Data4[0], guid2.Data4[0]);
        BOOST_CHECK_EQUAL(guid1.Data4[1], guid2.Data4[1]);
        BOOST_CHECK_EQUAL(guid1.Data4[2], guid2.Data4[2]);
        BOOST_CHECK_EQUAL(guid1.Data4[3], guid2.Data4[3]);
        BOOST_CHECK_EQUAL(guid1.Data4[4], guid2.Data4[4]);
        BOOST_CHECK_EQUAL(guid1.Data4[5], guid2.Data4[5]);
        BOOST_CHECK_EQUAL(guid1.Data4[6], guid2.Data4[6]);
        BOOST_CHECK_EQUAL(guid1.Data4[7], guid2.Data4[7]);
    }

    void TestDefaultCtor() {
        MeaGUID guid;
        validateGUID(guid);
    }

    void TestGUIDCtor() {
        GUID guid1;
        HRESULT hr = CoCreateGuid(&guid1);
        if (FAILED(hr)) {
            AfxThrowOleException(hr);
        }
        MeaGUID guid2(guid1);
        validateEqual(guid1, guid2);
    }

    void TestCopyCtor() {
        MeaGUID guid1;
        MeaGUID guid2(guid1);
        validateEqual(guid1, guid2);
    }

    void TestStringCtor() {
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

    void TestAssignOperatorGUID() {
        GUID guid1;
        HRESULT hr = CoCreateGuid(&guid1);
        if (FAILED(hr)) {
            AfxThrowOleException(hr);
        }
        MeaGUID guid2;
        guid2 = guid1;
        validateEqual(guid1, guid2);
    }

    void TestAssignOperatorMeaGUID() {
        MeaGUID guid1;
        MeaGUID guid2;
        guid2 = guid1;
        validateEqual(guid1, guid2);
    }

    void TestAssignOperatorString() {
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

    void TestAssignGUID() {
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

    void TestAssignMeaGUID() {
        MeaGUID guid1;
        MeaGUID guid2;
        MeaGUID guid3(guid2.Assign(guid1));
        validateEqual(guid1, guid2);
        validateEqual(guid1, guid3);
    }

    void TestAssignString() {
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

    void TestEqualityGUID() {
        GUID guid1;
        HRESULT hr = CoCreateGuid(&guid1);
        if (FAILED(hr)) {
            AfxThrowOleException(hr);
        }
        GUID guid2;
        hr = CoCreateGuid(&guid2);
        if (FAILED(hr)) {
            AfxThrowOleException(hr);
        }

        MeaGUID guid3(guid1);
        BOOST_CHECK(guid3 == guid1);
        BOOST_CHECK(guid3.IsEqual(guid1));
        BOOST_CHECK(guid3 != guid2);
        BOOST_CHECK(!guid3.IsEqual(guid2));
    }

    void TestEqualityMeaGUID() {
        MeaGUID guid1;
        MeaGUID guid2;

        MeaGUID guid3(guid1);
        BOOST_CHECK(guid3 == guid1);
        BOOST_CHECK(guid3.IsEqual(guid1));
        BOOST_CHECK(guid3 != guid2);
        BOOST_CHECK(!guid3.IsEqual(guid2));
    }

    void TestCast() {
        MeaGUID guid1;

        GUID guid2 = guid1;
        validateEqual(guid1, guid2);

        LPCTSTR guid3Str = guid1;
        MeaGUID guid3(guid3Str);
        validateEqual(guid1, guid3);
    }

    void TestToString() {
        MeaGUID guid1;
        CString guid2Str(guid1.ToString());
        MeaGUID guid2(guid2Str);
        validateEqual(guid1, guid2);
    }

    void TestLess() {
        std::set<MeaGUID, MeaGUID::less> guidSet;
        std::set<MeaGUID, MeaGUID::less>::iterator iter;
        MeaGUID guid1;
        MeaGUID guid2;
        MeaGUID guid3;

        guidSet.insert(guid1);
        guidSet.insert(guid2);
        guidSet.insert(guid3);

        iter = guidSet.find(guid2);
        BOOST_CHECK_EQUAL(guid2, *iter);
        iter = guidSet.find(guid1);
        BOOST_CHECK_EQUAL(guid1, *iter);
        iter = guidSet.find(guid3);
        BOOST_CHECK_EQUAL(guid3, *iter);
    }
}

test_suite* init_unit_test_suite(int argc, char* argv[]) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    test_suite* suite = BOOST_TEST_SUITE("GUID Tests");
    suite->add(BOOST_TEST_CASE(&TestDefaultCtor));
    suite->add(BOOST_TEST_CASE(&TestGUIDCtor));
    suite->add(BOOST_TEST_CASE(&TestCopyCtor));
    suite->add(BOOST_TEST_CASE(&TestStringCtor));
    suite->add(BOOST_TEST_CASE(&TestAssignOperatorGUID));
    suite->add(BOOST_TEST_CASE(&TestAssignOperatorMeaGUID));
    suite->add(BOOST_TEST_CASE(&TestAssignOperatorString));
    suite->add(BOOST_TEST_CASE(&TestAssignGUID));
    suite->add(BOOST_TEST_CASE(&TestAssignMeaGUID));
    suite->add(BOOST_TEST_CASE(&TestAssignString));
    suite->add(BOOST_TEST_CASE(&TestEqualityGUID));
    suite->add(BOOST_TEST_CASE(&TestEqualityMeaGUID));
    suite->add(BOOST_TEST_CASE(&TestCast));
    suite->add(BOOST_TEST_CASE(&TestToString));
    suite->add(BOOST_TEST_CASE(&TestLess));
    return suite;
}

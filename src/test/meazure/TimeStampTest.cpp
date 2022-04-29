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
#include <TimeStamp.h>
#include <boost/test/included/unit_test.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


using namespace std;
using namespace boost::unit_test;
using boost::unit_test_framework::test_suite;


namespace {
    void TestMakeTimeStamp() {
        BOOST_CHECK_EQUAL(CString(_T("1970-01-01T00:00:00Z")), MeaMakeTimeStamp(0));        // Epoch
        BOOST_CHECK_EQUAL(CString(_T("1971-02-05T01:00:00Z")), MeaMakeTimeStamp(34563600)); // Epoch + 400 d, 1 h
    }

    void TestParseTimeStamp() {
        BOOST_CHECK_EQUAL(0, MeaParseTimeStamp(_T("1970-01-01T00:00:00Z")));
        BOOST_CHECK_EQUAL(34563600, MeaParseTimeStamp(_T("1971-02-05T01:00:00Z")));
    }
}

test_suite* init_unit_test_suite(int argc, char* argv[]) {
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        cerr << "Fatal Error: MFC initialization failed\n";
        return nullptr;
    }

    test_suite* suite = BOOST_TEST_SUITE("TimeStamp Tests");
    suite->add(BOOST_TEST_CASE(&TestMakeTimeStamp));
    suite->add(BOOST_TEST_CASE(&TestParseTimeStamp));
    return suite;
}

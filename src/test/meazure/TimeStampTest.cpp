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
#define BOOST_TEST_MODULE TimeStampTest
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/TimeStamp.h>


struct GlobalFixture {
    GlobalFixture() {
        if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
            BOOST_FAIL("Fatal Error: MFC initialization failed");
        }
    }

    CWinApp theApp;
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);


BOOST_AUTO_TEST_CASE(TestMakeTimeStamp) {
    BOOST_TEST(MeaMakeTimeStamp(0) == _T("1970-01-01T00:00:00Z"));        // Epoch
    BOOST_TEST(MeaMakeTimeStamp(34563600) == _T("1971-02-05T01:00:00Z")); // Epoch + 400 d, 1 h
}

BOOST_AUTO_TEST_CASE(TestParseTimeStamp) {
    BOOST_TEST(MeaParseTimeStamp(_T("1970-01-01T00:00:00Z")) == 0);
    BOOST_TEST(MeaParseTimeStamp(_T("1971-02-05T01:00:00Z")) == 34563600);
}

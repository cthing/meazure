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
#define BOOST_TEST_MODULE ColorsTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/CommandLineInfo.h>


BOOL g_enableLayeredWindows { TRUE };


BOOST_AUTO_TEST_CASE(TestOptions) {
    BOOST_TEST(g_enableLayeredWindows);
    MeaCommandLineInfo cmdLine;
    cmdLine.ParseParam("nl", TRUE, TRUE);
    BOOST_TEST(!g_enableLayeredWindows);
}
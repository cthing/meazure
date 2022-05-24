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
#define BOOST_TEST_MODULE PlotterTest
#include <boost/test/unit_test.hpp>
#include <meazure/graphics/Plotter.h>
#include <vector>
#include <functional>


struct GlobalFixture {
    GlobalFixture() {
        if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
            BOOST_FAIL("Fatal Error: MFC initialization failed");
        }
    }

    CWinApp theApp;
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);

bool operator==(const POINT& lhs, const POINT& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::ostream& operator<<(std::ostream& os, const POINT& point) {
    os << '(' << point.x << ',' << point.y << ')';
    return os;
}


BOOST_AUTO_TEST_CASE(TestPlotLine) {
    std::vector<POINT> points;
    std::function<void(int, int)> addPoint = [&](int x, int y) {
        POINT pt { x, y };
        points.push_back(pt);
    };

    POINT start1 { 1, 2 };
    POINT end1 { 5, 2 };
    points.clear();
    MeaPlotter::PlotLine(start1, end1, addPoint);

    BOOST_TEST(points.size() == 4);
    BOOST_TEST(points[0] == (POINT { 2, 2 }));
    BOOST_TEST(points[1] == (POINT { 3, 2 }));
    BOOST_TEST(points[2] == (POINT { 4, 2 }));
    BOOST_TEST(points[3] == (POINT { 5, 2 }));

    POINT start2 { 1, 2 };
    POINT end2 { 6, 5 };
    points.clear();
    MeaPlotter::PlotLine(start2, end2, addPoint);

    BOOST_TEST(points.size() == 5);
    BOOST_TEST(points[0] == (POINT { 2, 3 }));
    BOOST_TEST(points[1] == (POINT { 3, 3 }));
    BOOST_TEST(points[2] == (POINT { 4, 4 }));
    BOOST_TEST(points[3] == (POINT { 5, 4 }));
    BOOST_TEST(points[4] == (POINT { 6, 5 }));
}

BOOST_AUTO_TEST_CASE(TestPlotCircle) {
    std::vector<POINT> points;
    std::function<void(int, int)> addPoint = [&](int x, int y) {
        POINT pt { x, y };
        points.push_back(pt);
    };

    POINT center { 1, 2 };
    MeaPlotter::PlotCircle(center, 3, addPoint);

    BOOST_TEST(points.size() == 24);
    BOOST_TEST(points[0] == (POINT { 4, 2 }));
    BOOST_TEST(points[1] == (POINT { -2, 2 }));
    BOOST_TEST(points[2] == (POINT { -2, 2 }));
    BOOST_TEST(points[3] == (POINT { 4, 2 }));
    BOOST_TEST(points[4] == (POINT { 1, 5 }));
    BOOST_TEST(points[5] == (POINT { 1, 5 }));
    BOOST_TEST(points[6] == (POINT { 1, -1 }));
    BOOST_TEST(points[7] == (POINT { 1, -1 }));
    BOOST_TEST(points[8] == (POINT { 4, 3 }));
    BOOST_TEST(points[9] == (POINT { -2, 3 }));
    BOOST_TEST(points[10] == (POINT { -2, 1 }));
    BOOST_TEST(points[11] == (POINT { 4, 1 }));
    BOOST_TEST(points[12] == (POINT { 2, 5 }));
    BOOST_TEST(points[13] == (POINT { 0, 5 }));
    BOOST_TEST(points[14] == (POINT { 0, -1 }));
    BOOST_TEST(points[15] == (POINT { 2, -1 }));
    BOOST_TEST(points[16] == (POINT { 3, 4 }));
    BOOST_TEST(points[17] == (POINT { -1, 4 }));
    BOOST_TEST(points[18] == (POINT { -1, 0 }));
    BOOST_TEST(points[19] == (POINT { 3, 0 }));
    BOOST_TEST(points[20] == (POINT { 3, 4 }));
    BOOST_TEST(points[21] == (POINT { -1, 4 }));
    BOOST_TEST(points[22] == (POINT { -1, 0 }));
    BOOST_TEST(points[23] == (POINT { 3, 0 }));
}

BOOST_AUTO_TEST_CASE(TestPlotCrosshair) {
    std::vector<POINT> points;
    std::function<void(int, int)> addPoint = [&](int x, int y) {
        POINT pt { x, y };
        points.push_back(pt);
    };

    SIZE size { 5, 5 };
    SIZE spread { 1, 1 };
    MeaPlotter::PlotCrosshair(size, spread, 2, addPoint);

    BOOST_TEST(points.size() == 32);
    BOOST_TEST(points[0] == (POINT { 1, 0 }));
    BOOST_TEST(points[1] == (POINT { 4, 0 }));
    BOOST_TEST(points[2] == (POINT { 4, 1 }));
    BOOST_TEST(points[3] == (POINT { 1, 1 }));
    BOOST_TEST(points[4] == (POINT { 2, 1 }));
    BOOST_TEST(points[5] == (POINT { 3, 1 }));
    BOOST_TEST(points[6] == (POINT { 3, 2 }));
    BOOST_TEST(points[7] == (POINT { 2, 2 }));
    BOOST_TEST(points[8] == (POINT { 0, 1 }));
    BOOST_TEST(points[9] == (POINT { 0, 4 }));
    BOOST_TEST(points[10] == (POINT { 1, 4 }));
    BOOST_TEST(points[11] == (POINT { 1, 1 }));
    BOOST_TEST(points[12] == (POINT { 1, 2 }));
    BOOST_TEST(points[13] == (POINT { 1, 3 }));
    BOOST_TEST(points[14] == (POINT { 2, 3 }));
    BOOST_TEST(points[15] == (POINT { 2, 2 }));
    BOOST_TEST(points[16] == (POINT { 1, 5 }));
    BOOST_TEST(points[17] == (POINT { 4, 5 }));
    BOOST_TEST(points[18] == (POINT { 4, 4 }));
    BOOST_TEST(points[19] == (POINT { 1, 4 }));
    BOOST_TEST(points[20] == (POINT { 2, 4 }));
    BOOST_TEST(points[21] == (POINT { 3, 4 }));
    BOOST_TEST(points[22] == (POINT { 3, 3 }));
    BOOST_TEST(points[23] == (POINT { 2, 3 }));
    BOOST_TEST(points[24] == (POINT { 5, 1 }));
    BOOST_TEST(points[25] == (POINT { 5, 4 }));
    BOOST_TEST(points[26] == (POINT { 4, 4 }));
    BOOST_TEST(points[27] == (POINT { 4, 1 }));
    BOOST_TEST(points[28] == (POINT { 4, 2 }));
    BOOST_TEST(points[29] == (POINT { 4, 3 }));
    BOOST_TEST(points[30] == (POINT { 3, 3 }));
    BOOST_TEST(points[31] == (POINT { 3, 2 }));
}

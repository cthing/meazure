/*
 * Copyright 2001 C Thing Software
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

 /// @file
 /// @brief Plots the points for drawing various shapes.

#pragma once

#include <cmath>
#include <functional>


/// Plots the points for drawing various shapes (e.g. lines, circles).
///
namespace MeaPlotter {

    /// The window region is composed of single pixel rectangles arranged in a line from the start point to the end
    /// point. The location of each rectangle is determined using the Bresenham algorithm adapted from
    /// "Graphics Gems", Academic Press, 1990, p. 685. The line needs to be created in this brute force way because
    /// relying on the polygon region method produces a horrible looking line.
    /// 
    /// @param start     [in] Start point for the line
    /// @param end       [in] End point for the line
    /// @param addPoint  [in] Function called to record the plotted point (x, y)
    ///
    inline void PlotLine(const POINT& start, const POINT& end, std::function<void(int, int)> addPoint) {
        const int dx = end.x - start.x;
        const int dy = end.y - start.y;

        const int ax = 2 * std::abs(dx);
        const int ay = 2 * std::abs(dy);

        const int sx = (dx < 0) ? -1 : 1;
        const int sy = (dy < 0) ? -1 : 1;

        const auto [endx, endy] = end;

        auto [x, y] = start;

        if (ax > ay) {      /* x dominant */
            int d = ay - (ax / 2);
            while (x != endx) {
                if (d >= 0) {
                    y += sy;
                    d -= ax;
                }
                x += sx;
                d += ay;
                addPoint(x, y);
            }
        } else {            /* y dominant */
            int d = ax - (ay / 2);
            while (y != endy) {
                if (d >= 0) {
                    x += sx;
                    d -= ay;
                }
                y += sy;
                d += ax;
                addPoint(x, y);
            }
        }
    }

    /// The circular window region is composed of single pixel rectangles arranged in a circle. The location of each
    /// rectangle is determined using an algorithm from the paper "A Fast Bresenham Type Algorithm for Drawing Circles"
    /// by John Kennedy, Mathematics Dept., Santa Monica College (http://homepage.smc.edu/kennedy_john/BCIRCLE.PDF,
    /// rkennedy@ix.netcom.com).
    ///
    /// @param center   [in] Center of the circle
    /// @param radius   [in] Radius of the circle, in pixels
    /// @param addPoint [in] Function called to record the plotted point (x, y)
    ///
    inline void PlotCircle(const POINT& center, int radius, std::function<void(int, int)> addPoint) {
        const auto [xc, yc] = center;
        int x = radius;
        int y = 0;
        int deltax = 1 - 2 * radius;
        int deltay = 1;
        int radiusError = 0;

        while (x >= y) {
            addPoint(xc + x, yc + y);       // Octant 1
            addPoint(xc - x, yc + y);       // Octant 4
            addPoint(xc - x, yc - y);       // Octant 5
            addPoint(xc + x, yc - y);       // Octant 8
            addPoint(xc + y, yc + x);       // Octant 2
            addPoint(xc - y, yc + x);       // Octant 3
            addPoint(xc - y, yc - x);       // Octant 6
            addPoint(xc + y, yc - x);       // Octant 7

            y++;
            radiusError += deltay;
            deltay += 2;

            if ((2 * radiusError + deltax) > 0) {
                x--;
                radiusError += deltax;
                deltax += 2;
            }
        }
    }

    /// Plots the points comprising the crosshair. Four petals comprise the top, bottom, left and right of the
    /// crosshair. Each petal of the crosshair is made up of stacked rectangles. Each rectangle is thk high by
    /// 2 * spread wide. Each rectangle is called a layer.
    /// <pre>
    ///           |---| spread
    ///       ********* ---------
    ///       *       * - thk   |
    ///        *     *          |
    ///        *     *          |
    ///         *   *           |
    ///         *   *       5 layers
    ///          * *            |
    ///          * *            |
    ///           *             |
    ///           * -------------
    /// </pre>
    /// 
    /// @param size     [in] total size of the crosshair
    /// @param spread   [in] half the width of the petal at its widest point
    /// @param layers   [in] number of rectangles comprising a crosshair petal
    /// @param addPoint [in] Function called to record the plotted point (x, y)
    ///
    inline void PlotCrosshair(const SIZE& size, const SIZE& spread, int layers, std::function<void(int, int)> addPoint) {
        const int xc = size.cx / 2;
        const int yc = size.cy / 2;
        const int thkx = xc / layers;
        const int thky = yc / layers;

        for (int layer = 0, spreadx = spread.cx, thk = 0; layer < layers && spreadx >= 0; layer++, spreadx--, thk += thky) {
            addPoint(xc - spreadx,     thk);
            addPoint(xc + spreadx + 1, thk);
            addPoint(xc + spreadx + 1, thk + thky);
            addPoint(xc - spreadx,     thk + thky);
        }

        //
        // Left petal
        //
        for (int layer = 0, spready = spread.cy, thk = 0; layer < layers && spready >= 0; layer++, spready--, thk += thkx) {
            addPoint(thk,        yc - spready);
            addPoint(thk,        yc + spready + 1);
            addPoint(thk + thkx, yc + spready + 1);
            addPoint(thk + thkx, yc - spready);
        }

        //
        // Bottom petal
        //
        for (int layer = 0, spreadx = spread.cx, thk = size.cy; layer < layers && spreadx >= 0; layer++, spreadx--, thk -= thky) {
            addPoint(xc - spreadx,     thk);
            addPoint(xc + spreadx + 1, thk);
            addPoint(xc + spreadx + 1, thk - thky);
            addPoint(xc - spreadx,     thk - thky);
        }

        //
        // Right petal
        //
        for (int layer = 0, spready = spread.cy, thk = size.cx; layer < layers && spready >= 0; layer++, spready--, thk -= thkx) {
            addPoint(thk,        yc - spready);
            addPoint(thk,        yc + spready + 1);
            addPoint(thk - thkx, yc + spready + 1);
            addPoint(thk - thkx, yc - spready);
        }
    }
};

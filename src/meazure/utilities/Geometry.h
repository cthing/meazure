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

 /// @file
 /// @brief Classes and convenience methods for working with geometry.

#pragma once

#include <cmath>
#include "NumericUtils.h"
#include "Utils.h"


/// Convenience methods for calculating various geometric values (e.g. circumference, angle).
///
class MeaGeometry {
public:
    MeaGeometry() = delete;
    ~MeaGeometry() = delete;

    /// Calculates the length corresponding to the specified x and y distances using the formula:
    /// \f[
    ///     length=\sqrt{dx^2+dy^2}
    /// \f]
    /// @param dx   [in] X distance.
    /// @param dy   [in] Y distance.
    ///
    /// @return Length corresponding to the vertical and horizontal distances.
    ///
    static double CalcLength(double dx, double dy) {
        return std::sqrt(dx * dx + dy * dy);
    }

    /// Calculates the length between the two specified points using the formula:
    /// \f[
    ///     length=\sqrt{(p2_x-p1_x)^2+(p2_y-p1_y)^2}
    /// \f]
    /// @param p1   [in] Point 1.
    /// @param p2   [in] Point 2.
    ///
    /// @return Length between point 1 and point 2.
    ///
    static double CalcLength(const POINT& p1, const POINT& p2) {
        return CalcLength(static_cast<double>(p2.x - p1.x), static_cast<double>(p2.y - p1.y));
    }

    /// Calculates the circumference of the circle with the specified radius using the formula:
    /// \f[
    ///     circumference=2*\pi*radius
    /// \f]
    /// @param radius   [in] Radius of the circle.
    ///
    /// @return Circumference of the circle.
    ///
    static double CalcCircumference(double radius) {
        return 2.0 * radius * MeaNumericUtils::PI;
    }

    /// Returns the angular sector containing the vector specified by the two points. A circle centered at the
    /// starting point of the vector is divided into eight 45 degree sectors numbered -4 through 4 inclusive.
    /// The sectors are numbered according to the following table of angle ranges. Positive angles go clockwise
    /// from the positive x-axis.
    /// 
    /// <table>
    /// <tr>
    ///     <th>Angle Range (deg)</th>
    ///     <th>Sector</th>
    /// </tr>
    /// <tr>
    ///     <td>[0, 45)</td>
    ///     <td>1</td>
    /// </tr>
    /// <tr>
    ///     <td>[45, 90)</td>
    ///     <td>2</td>
    /// </tr>
    /// <tr>
    ///     <td>[90, 135)</td>
    ///     <td>3</td>
    /// </tr>
    /// <tr>
    ///     <td>[135, 180)</td>
    ///     <td>4</td>
    /// </tr>
    /// <tr>
    ///     <td>[180, 225]</td>
    ///     <td>-4</td>
    /// </tr>
    /// <tr>
    ///     <td>(225, 270]</td>
    ///     <td>-3</td>
    /// </tr>
    /// <tr>
    ///     <td>(270, 315]</td>
    ///     <td>-2</td>
    /// </tr>
    /// <tr>
    ///     <td>(315, 0]</td>
    ///     <td>-1</td>
    /// </tr>
    /// <tr>
    ///     <td>Zero vector</td>
    ///     <td>0</td>
    /// </tr>
    /// </table>
    ///
    /// @param start        [in] Starting point for the vector.
    /// @param end          [in] Ending point for the vector.
    ///
    /// @return The circular sector corresponding to the vector in the range [-4.0, 4.0].
    ///
    static int CalcSector(const POINT& start, const POINT& end) {
        int deltax = end.x - start.x;
        int deltay = end.y - start.y;

        if (deltax == 0 && deltay == 0) {
            return 0;
        }
        if (deltax == 0) {
            return (deltay < 0) ? -3 : 3;
        }
        if (deltay == 0) {
            return (deltax < 0) ? -4 : 1;
        }

        // Note: 45 degrees == PI/4 radians
        double result = atan2(static_cast<double>(deltay), static_cast<double>(deltax)) / MeaNumericUtils::PI4;
        return static_cast<int>(result + std::copysign(1.0, result));
    }

    /// Indicates whether the specified vector is oriented in a vertical direction. A vector is considered
    /// vertically oriented if it is in sector +/-2 or +/-3. See the documentation for the CalcSector method.
    /// 
    /// @param start  [in] Starting point for the vector
    /// @param end    [in] Ending point for the vector
    /// @return <b>true</b> if the specified vector is vertically oriented. Returns <b>false</b> if the
    ///     vector is horizontally oriented or is the empty vector.
    /// 
    static bool IsVerticallyOriented(const POINT& start, const POINT& end) {
        switch (CalcSector(start, end)) {
        case 2:
        case -2:
        case 3:
        case -3:
            return true;
        default:
            return false;
        }
    }

    /// Indicates whether the specified vector is oriented in a horizontal direction. A vector is considered
    /// horizontally oriented if it is in sector +/-1 or +/-4. See the documentation for the CalcSector method.
    /// 
    /// @param start  [in] Starting point for the vector
    /// @param end    [in] Ending point for the vector
    /// @return <b>true</b> if the specified vector is horizontally oriented. Returns <b>false</b> if the
    ///     vector is vertically oriented or is the empty vector.
    /// 
    static bool IsHorizontallyOriented(const POINT& start, const POINT& end) {
        switch (CalcSector(start, end)) {
        case 1:
        case -1:
        case 4:
        case -4:
            return true;
        default:
            return false;
        }
    }

    /// Returns the angle of the vector from the specified start point to the specified end point relative to
    /// the horizontal (x-axis), measured in radians. Using the default coordinate system (i.e. positive y-axis
    /// pointed down from the origin), positive angles are measured clockwise from the horizontal to the vector.
    /// The following formula is used:
    /// \f[
    ///     angle=atan2(end_y-start_y, end_x-start_x)
    /// \f]
    /// @param start    [in] Starting point for the vector whose angle is to be determined.
    /// @param end      [in] Ending point for the vector whose angle is to be determined.
    ///
    /// @return Angle between the x-axis and the line, in radians. Degenerate cases return 0.0.
    ///
    static double CalcAngle(const FPOINT& start, const FPOINT& end) {
        double deltax = end.x - start.x;
        double deltay = end.y - start.y;

        if (MeaNumericUtils::IsFloatingZero(deltax) && MeaNumericUtils::IsFloatingZero(deltay)) {
            return 0.0;
        }
        return std::atan2(deltay, deltax);
    }

    /// Returns the angle, in radians, between the vector from the vertex to point p1 and the vector from the
    /// vertex to point p2. Using the default coordinate system (i.e. positive y-axis pointed down from the origin),
    /// positive angles are measured clockwise from the vertex-p1 line to the vertex-p2 line.
    ///
    /// @param vertex   [in] Intersection point for the two lines.
    /// @param p1       [in] Forms the end point for the first line from the vertex.
    /// @param p2       [in] Forms the end point for the second line from the vertex.
    ///
    /// @return Angle between the vectors vertex-p1 and vertex-p2.
    ///
    static double CalcAngle(const FPOINT& vertex, const FPOINT& p1, const FPOINT& p2) {
        double deltax1 = p1.x - vertex.x;
        double deltax2 = p2.x - vertex.x;
        double deltay1 = p1.y - vertex.y;
        double deltay2 = p2.y - vertex.y;

        double numer = deltay2 * deltax1 - deltay1 * deltax2;
        double denom = deltax2 * deltax1 + deltay1 * deltay2;

        if (MeaNumericUtils::IsFloatingZero(numer) && MeaNumericUtils::IsFloatingZero(denom)) {
            return 0.0;
        }
        return std::atan2(numer, denom);
    }
};

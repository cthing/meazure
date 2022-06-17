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
#include <iostream>
#include "NumericUtils.h"


/// Represents a rectangular size. Unlike the Windows SIZE structure, the dimensions of the MeaFSize structure are
/// double precision values.
///
struct MeaFSize {
    double cx;     ///< Length in the x dimension.
    double cy;     ///< Length in the y dimension.


    /// Constructs a size object initialized to 0.0 width and height.
    ///
    MeaFSize() : MeaFSize(0.0, 0.0) {}

    /// Constructs a size object initialized to the specified dimensions.
    /// 
    /// @param x  [in] Length in the x dimension
    /// @param y  [in] Length in the y dimension
    ///
    MeaFSize(double x, double y) : cx(x), cy(y) {}

    /// Tests equality between this with the specified size.
    /// 
    /// @param other  [in] Other size to compare against this
    /// @return <b>true</b> if this and the specified size have the same dimensions.
    /// 
    bool operator==(const MeaFSize& other) const {
        return MeaNumericUtils::IsEqualF(cx, other.cx) && MeaNumericUtils::IsEqualF(cy, other.cy);
    }

    /// Tests inequality between this with the specified size.
    /// 
    /// @param other  [in] Other size to compare against this
    /// @return <b>true</b> if this and the specified size have the different dimensions.
    /// 
    bool operator!=(const MeaFSize& other) const {
        return !(*this == other);
    }

    /// Adds this and the specified object and returns a new size object.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     f_x \\
    ///     f_y \\
    ///     \end{array} \right]
    ///     =
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    ///     +
    ///     \left[ \begin{array}{c}
    ///     fsize_x \\
    ///     fsize_y \\
    ///     \end{array} \right]
    /// \f]
    /// @param fsize    [in] Size object to add.
    /// @return New object that is the sum of this and the specified object.
    /// 
    MeaFSize operator+(const MeaFSize& fsize) const {
        return MeaFSize(cx + fsize.cx, cy + fsize.cy);
    }

    /// Adds this and the specified value and returns a new size object.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     f_x \\
    ///     f_y \\
    ///     \end{array} \right]
    ///     =
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    ///     +
    ///     \left[ \begin{array}{c}
    ///     val \\
    ///     val \\
    ///     \end{array} \right]
    /// \f]
    /// @param val      [in] Length to add to both dimensions.
    /// @return New object that is the sum of this and the specified value.
    /// 
    MeaFSize operator+(double val) const {
        return MeaFSize(cx + val, cy + val);
    }

    /// Multiplies this and the specified object and returns a new size object.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     f_x \\
    ///     f_y \\
    ///     \end{array} \right]
    ///     =
    ///     \left[ \begin{array}{cc}
    ///     this_x & 0 \\
    ///     0      & this_y \\
    ///     \end{array} \right]
    ///     \left[ \begin{array}{c}
    ///     fsize_x \\
    ///     fsize_y \\
    ///     \end{array} \right]
    /// \f]
    /// @param fsize    [in] Size object to multiply.
    /// @return New object that is the product of this and the specified object.
    /// 
    MeaFSize operator*(const MeaFSize& fsize) const {
        return MeaFSize(cx * fsize.cx, cy * fsize.cy);
    }

    /// Multiplies this and the specified object and returns a new size object.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     f_x \\
    ///     f_y \\
    ///     \end{array} \right]
    ///     =
    ///     \left[ \begin{array}{cc}
    ///     this_x & 0 \\
    ///     0      & this_y \\
    ///     \end{array} \right]
    ///     \left[ \begin{array}{c}
    ///     size_x \\
    ///     size_y \\
    ///     \end{array} \right]
    /// \f]
    /// @param size     [in] Size object to multiply.
    /// @return New object that is the product of this and the specified object.
    /// 
    MeaFSize operator*(const SIZE& size) const {
        return MeaFSize(cx * size.cx, cy * size.cy);
    }

    /// Multiplies this and the specified value and returns a new size object.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     f_x \\
    ///     f_y \\
    ///     \end{array} \right]
    ///     = mult
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    /// \f]
    /// @param mult     [in] Value to multiply both dimensions.
    /// @return New object that is the product of this and the specified value.
    /// 
    MeaFSize operator*(double mult) const {
        return MeaFSize(cx * mult, cy * mult);
    }

    /// Adds the specified size object to this.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    ///     =
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    ///     +
    ///     \left[ \begin{array}{c}
    ///     fsize_x \\
    ///     fsize_y \\
    ///     \end{array} \right]
    /// \f]
    /// @param fsize        [in] Size object to add to this.
    /// @return this object.
    /// 
    MeaFSize& operator+=(const MeaFSize& fsize) {
        cx += fsize.cx;
        cy += fsize.cy;
        return *this;
    }

    /// Subtracts the specified size object from this.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    ///     =
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    ///     -
    ///     \left[ \begin{array}{c}
    ///     fsize_x \\
    ///     fsize_y \\
    ///     \end{array} \right]
    /// \f]
    /// @param fsize        [in] Size object to subtract from this.
    /// @return this object.
    MeaFSize& operator-=(const MeaFSize& fsize) {
        cx -= fsize.cx;
        cy -= fsize.cy;
        return *this;
    }

    /// Multiplies the specified size object and this.
    /// \f[
    ///     \left[ \begin{array}{c}
    ///     this_x \\
    ///     this_y \\
    ///     \end{array} \right]
    ///     =
    ///     \left[ \begin{array}{cc}
    ///     this_x & 0 \\
    ///     0      & this_y \\
    ///     \end{array} \right]
    ///     \left[ \begin{array}{c}
    ///     fsize_x \\
    ///     fsize_y \\
    ///     \end{array} \right]
    /// \f]
    /// @param fsize        [in] Size object to multiply with this.
    /// @return this object.
    MeaFSize& operator*=(const MeaFSize& fsize) {
        cx *= fsize.cx;
        cy *= fsize.cy;
        return *this;
    }
};


/// Provides output of the size in the format "(cx,cy)".
/// 
/// @param os     [in] Output stream to write
/// @param size   [in] Size object to write
/// @return The specified output stream.
/// 
inline std::ostream& operator<<(std::ostream& os, const MeaFSize& size) {
    os << '(' << size.cx << ',' << size.cy << ')';
    return os;
}

/// Calculates the log base 10 on each dimension of the specified
/// size object and returns the results in a new size object.
/// \f[
///     \left[ \begin{array}{c}
///     f_x \\
///     f_y \\
///     \end{array} \right]
///     =
///     \left[ \begin{array}{c}
///     \log fsize_x \\
///     \log fsize_y \\
///     \end{array} \right]
/// \f]
/// @param fsize        [in] Size object whose logarithm is to be obtained.
///
/// @return A new size object whose dimensions are the base 10 log of
///         the specified size object's dimensions.
///
inline MeaFSize log10(const MeaFSize& fsize) {
    return MeaFSize(std::log10(fsize.cx), std::log10(fsize.cy));
}

/// Calculates the floor on each dimension of the specified
/// size object and returns the results in a new size object.
/// \f[
///     \left[ \begin{array}{c}
///     f_x \\
///     f_y \\
///     \end{array} \right]
///     =
///     \left[ \begin{array}{c}
///     \lfloor fsize_x \\
///     \lfloor fsize_y \\
///     \end{array} \right]
/// \f]
/// @param fsize        [in] Size object whose floor is to be obtained.
///
/// @return A new size object whose dimensions are the floor of
///         the specified size object's dimensions.
///
inline MeaFSize floor(const MeaFSize& fsize) {
    return MeaFSize(std::floor(fsize.cx), std::floor(fsize.cy));
}

/// Subtracts the specified size object from the specified
/// value and returns a new size object.
/// \f[
///     \left[ \begin{array}{c}
///     f_x \\
///     f_y \\
///     \end{array} \right]
///     =
///     \left[ \begin{array}{c}
///     val \\
///     val \\
///     \end{array} \right]
///     -
///     \left[ \begin{array}{c}
///     fsize_x \\
///     fsize_y \\
///     \end{array} \right]
/// \f]
/// @param val      [in] The specified size object is
///                 subtracted from this value.
/// @param fsize    [in] Size object to subtract from the
///                 specified value.
///
/// @return New size object formed by the specified size
///         object being subtracted from the specified value.
///
inline MeaFSize operator-(double val, const MeaFSize& fsize) {
    return MeaFSize(val - fsize.cx, val - fsize.cy);
}


/// Represents a rectangular. Unlike the Windows RECT
/// structure, the positions of the MeaFRect structure are
/// double precision values.
///
struct MeaFRect {
    double top;         ///< Top of the rectangle.
    double bottom;      ///< Bottom of the rectangle.
    double left;        ///< Left side of the rectangle.
    double right;       ///< Right side of the rectangle.

    /// Constructs a rectangle object initialized to 0.0 in all coordinates.
    ///
    MeaFRect() : MeaFRect(0.0, 0.0, 0.0, 0.0) {}

    /// Constructs a rectangle object initialized to the specified coordinates.
    /// 
    /// @param t  [in] Initial top coordinate 
    /// @param b  [in] Initial bottom coordinate
    /// @param l  [in] Initial left coordinate
    /// @param r  [in] Initial right coordinate
    /// 
    MeaFRect(double t, double b, double l, double r) : top(t), bottom(b), left(l), right(r) {}

    /// Tests equality between this with the specified rectangle.
    /// 
    /// @param other  [in] Other rectangle to compare against this
    /// @return <b>true</b> if this and the specified rectangle have the same dimensions.
    /// 
    bool operator==(const MeaFRect& other) const {
        return MeaNumericUtils::IsEqualF(top, other.top) &&
               MeaNumericUtils::IsEqualF(bottom, other.bottom) &&
               MeaNumericUtils::IsEqualF(left, other.left) &&
               MeaNumericUtils::IsEqualF(right, other.right);
    }

    /// Tests inequality between this with the specified rectangle.
    /// 
    /// @param other  [in] Other rectangle to compare against this
    /// @return <b>true</b> if this and the specified rectangle have the different dimensions.
    /// 
    bool operator!=(const MeaFRect& other) const {
        return !(*this == other);
    }
};


/// Provides output of the rectangle in the format "[top right bottom left]".
/// 
/// @param os     [in] Output stream to write
/// @param rect   [in] Rectangle object to write
/// @return The specified output stream.
/// 
inline std::ostream& operator<<(std::ostream& os, const MeaFRect& rect) {
    os << '[' << rect.top << ' ' << rect.right << ' ' << rect.bottom << ' ' << rect.left << ']';
    return os;
}


/// Represents a point. Unlike the Windows POINT structure, the positions of the MeaFPoint structure are
/// double precision values.
///
struct MeaFPoint {
    double x;       ///< X position.
    double y;       ///< Y position.

    /// Constructs a point object initialized to (0.0, 0.0).
    ///
    MeaFPoint() : MeaFPoint(0.0, 0.0) {}

    /// Constructs a point object initialized to the specified coordinates.
    /// 
    /// @param xi  [in] Initial x coordinate 
    /// @param yi  [in] Initial y coordinate
    /// 
    MeaFPoint(double xi, double yi) : x(xi), y(yi) {}

    /// Tests equality between this with the specified point.
    /// 
    /// @param other  [in] Other point to compare against this
    /// @return <b>true</b> if this and the specified point have the same dimensions.
    /// 
    bool operator==(const MeaFPoint& other) const {
        return MeaNumericUtils::IsEqualF(x, other.x) && MeaNumericUtils::IsEqualF(y, other.y);
    }

    /// Tests inequality between this with the specified point.
    /// 
    /// @param other  [in] Other point to compare against this
    /// @return <b>true</b> if this and the specified point have the different dimensions.
    /// 
    bool operator!=(const MeaFPoint& other) const {
        return !(*this == other);
    }
};


/// Provides output of the point in the format "(x,y)".
/// 
/// @param os     [in] Output stream to write
/// @param point  [in] Point object to write
/// @return The specified output stream.
/// 
inline std::ostream& operator<<(std::ostream& os, const MeaFPoint& point) {
    os << '(' << point.x << ',' << point.y << ')';
    return os;
}


/// Convenience methods for calculating various geometric values (e.g. circumference, angle).
///
namespace MeaGeometry {

    /// Multiplies the specified value by the specified scale factor.
    /// 
    /// @param value        [in] Value to scale
    /// @param scaleFactor  [in] Multiplier for the value
    /// @return Scaled value rounded to the nearest whole number.
    /// 
    inline int Scale(int value, double scaleFactor) {
        return static_cast<int>(std::round(value * scaleFactor));
    }

    /// Multiplies the top, bottom, left and right coordinates of the specified rectangle by the specified scale factor.
    /// 
    /// @param rect         [in] Rectangle to scale
    /// @param scaleFactor  [in] Multiplier for each coordinate
    /// @return Scaled rectangle with each coordinate rounded to the nearest whole number.
    /// 
    inline RECT Scale(const RECT& rect, double scaleFactor) {
        return RECT {
            Scale(rect.left, scaleFactor),
            Scale(rect.top, scaleFactor),
            Scale(rect.right, scaleFactor),
            Scale(rect.bottom, scaleFactor)
        };
    }

    /// Multiplies both dimensions of the specified size object by the specified scale factor.
    /// 
    /// @param size         [in] Size object to scale
    /// @param scaleFactor  [in] Multiplier for both dimensions
    /// @return Scaled size object with each dimension rounded to the nearest whole number.
    /// 
    inline SIZE Scale(const SIZE& size, double scaleFactor) {
        return SIZE {
            Scale(size.cx, scaleFactor),
            Scale(size.cy, scaleFactor)
        };
    }

    /// Calculates the length corresponding to the specified x and y distances using the formula:
    /// \f[
    ///     length=\sqrt{dx^2+dy^2}
    /// \f]
    /// @param dx   [in] X distance.
    /// @param dy   [in] Y distance.
    ///
    /// @return Length corresponding to the vertical and horizontal distances.
    ///
    inline double CalcLength(double dx, double dy) {
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
    inline double CalcLength(const POINT& p1, const POINT& p2) {
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
    inline double CalcCircumference(double radius) {
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
    inline int CalcSector(const POINT& start, const POINT& end) {
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
    inline bool IsVerticallyOriented(const POINT& start, const POINT& end) {
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
    inline bool IsHorizontallyOriented(const POINT& start, const POINT& end) {
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
    inline double CalcAngle(const MeaFPoint& start, const MeaFPoint& end) {
        double deltax = end.x - start.x;
        double deltay = end.y - start.y;

        if (MeaNumericUtils::IsZeroF(deltax) && MeaNumericUtils::IsZeroF(deltay)) {
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
    inline double CalcAngle(const MeaFPoint& vertex, const MeaFPoint& p1, const MeaFPoint& p2) {
        double deltax1 = p1.x - vertex.x;
        double deltax2 = p2.x - vertex.x;
        double deltay1 = p1.y - vertex.y;
        double deltay2 = p2.y - vertex.y;

        double numer = deltay2 * deltax1 - deltay1 * deltax2;
        double denom = deltax2 * deltax1 + deltay1 * deltay2;

        if (MeaNumericUtils::IsZeroF(numer) && MeaNumericUtils::IsZeroF(denom)) {
            return 0.0;
        }
        return std::atan2(numer, denom);
    }
};

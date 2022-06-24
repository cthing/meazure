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
 /// @brief Constants and convenience methods for working with numbers.

#pragma once

#include <cmath>


/// Constants and convenience methods for working with numbers.
///
namespace MeaNumericUtils {
    
    constexpr double PI = 3.141592653589793;
    constexpr double PI4 = 0.785398163397448;

    /// Tests whether the two specified floating point values are equal.
    /// 
    /// @tparam T One of <b>float</b>, <b>double</b>, or <b>long double</b>
    /// @param f1 [in] Value to test
    /// @param f2 [in] Value to test
    /// 
    /// @return <b>true</b> if the two values are equal within an epsilon of the floating point type compensated
    /// for the magnitude of the values. See
    /// <a href="https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/">Comparing
    /// Floating Point Numbers, 2012 Edition</a> and
    /// <a href="https://embeddeduse.com/2019/08/26/qt-compare-two-floats/">Comparing Two Floating-Point Numbers</a> 
    /// for details on this implementation.
    ///
    template<typename T>
    inline bool IsEqualF(T f1, T f2) {
        return (std::fabs(f1 - f2) <= std::numeric_limits<T>::epsilon())
            ? true
            : std::fabs(f1 - f2) <= (std::numeric_limits<T>::epsilon() * std::fmax(std::fabs(f1), std::fabs(f2)));
    }

    /// Tests whether the specified floating point value is zero.
    /// 
    /// @tparam T One of <b>float</b>, <b>double</b>, or <b>long double</b>
    /// @param f [in] Value to test
    /// 
    /// @return <b>true</b> if the value is equal to zero within an epsilon of the floating point type.
    ///
    template<typename T>
    inline bool IsZeroF(T f) {
        return std::fabs(f) <= std::numeric_limits<T>::epsilon();
    }

    /// Converts the specified value in degrees to a corresponding value in radians.
    /// 
    /// @param deg  [in] Degree value to convert to radians
    /// @return Value in radians
    /// 
    constexpr double DegToRad(double deg) {
        return deg * PI / 180.0;
    }

    /// Converts the specified value in radians to a corresponding value in degrees.
    /// 
    /// @param rad  [in] Radian value to convert to degrees
    /// @return Value in degrees
    /// 
    constexpr double RadToDeg(double rad) {
        return rad * 180.0 / PI;
    }
};

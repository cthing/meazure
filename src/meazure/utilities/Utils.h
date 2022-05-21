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
/// @brief Miscellaneous utility methods.

#pragma once

#include <cmath>


/// Represents a rectangular size. Unlike the Windows SIZE
/// structure, the dimensions of the FSIZE structure are
/// double precision values.
///
struct FSIZE {
    double cx;     ///< Length in the x dimension.
    double cy;     ///< Length in the y dimension.


    /// Constructs a size object initialized to 0.0 width and height.
    ///
    FSIZE() : cx(0.0), cy(0.0) {}

    /// Constructs a size object initialized to the specified dimensions.
    /// 
    /// @param x  [in] Length in the x dimension
    /// @param y  [in] Length in the y dimension
    ///
    FSIZE(double x, double y) : cx(x), cy(y) {}

    /// Adds this and the specified object and returns
    /// a new size object.
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
    /// @return New object that is the sum of this and the
    ///         specified object.
    FSIZE operator+(const FSIZE& fsize) const {
        return FSIZE(cx + fsize.cx, cy + fsize.cy);
    }

    /// Adds this and the specified value and returns
    /// a new size object.
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
    /// @return New object that is the sum of this and the
    ///         specified value.
    FSIZE operator+(double val) const {
        return FSIZE(cx + val, cy + val);
    }

    /// Multiplies this and the specified object and returns
    /// a new size object.
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
    /// @return New object that is the product of this and the
    ///         specified object.
    FSIZE operator*(const FSIZE& fsize) const {
        return FSIZE(cx * fsize.cx, cy * fsize.cy);
    }

    /// Multiplies this and the specified object and returns
    /// a new size object.
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
    /// @return New object that is the product of this and the
    ///         specified object.
    FSIZE operator*(const SIZE& size) const {
        return FSIZE(cx * size.cx, cy * size.cy);
    }

    /// Multiplies this and the specified value and returns
    /// a new size object.
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
    /// @return New object that is the product of this and the
    ///         specified value.
    FSIZE operator*(double mult) const {
        return FSIZE(cx * mult, cy * mult);
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
    FSIZE& operator+=(const FSIZE& fsize) {
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
    FSIZE& operator-=(const FSIZE& fsize) {
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
    FSIZE& operator*=(const FSIZE& fsize) {
        cx *= fsize.cx;
        cy *= fsize.cy;
        return *this;
    }
};


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
inline FSIZE log10(const FSIZE& fsize) {
    return FSIZE(std::log10(fsize.cx), std::log10(fsize.cy));
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
inline FSIZE floor(const FSIZE& fsize) {
    return FSIZE(std::floor(fsize.cx), std::floor(fsize.cy));
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
inline FSIZE operator-(double val, const FSIZE& fsize) {
    return FSIZE(val - fsize.cx, val - fsize.cy);
}


/// Represents a rectangular. Unlike the Windows RECT
/// structure, the positions of the FRECT structure are
/// double precision values.
///
struct FRECT {
    double top;         ///< Top of the rectangle.
    double bottom;      ///< Bottom of the rectangle.
    double left;        ///< Left side of the rectangle.
    double right;       ///< Right side of the rectangle.

    /// Constructs a rectangle object initialized to 0.0 in all coordinates.
    ///
    FRECT() : top(0.0), bottom(0.0), left(0.0), right(0.0) {}

    /// Constructs a rectangle object initialized to the specified coordinates.
    /// 
    /// @param t  [in] Initial top coordinate 
    /// @param b  [in] Initial bottom coordinate
    /// @param l  [in] Initial left coordinate
    /// @param r  [in] Initial right coordinate
    /// 
    FRECT(double t, double b, double l, double r) : top(t), bottom(b), left(l), right(r) {}
};



/// Represents a point. Unlike the Windows POINT
/// structure, the positions of the FPOINT structure are
/// double precision values.
///
struct FPOINT {
    double x;       ///< X position.
    double y;       ///< Y position.

    /// Constructs a point object initialized to (0.0, 0.0).
    ///
    FPOINT(): x(0.0), y(0.0) {}
    
    /// Constructs a point object initialized to the specified coordinates.
    /// 
    /// @param xi  [in] Initial x coordinate 
    /// @param yi  [in] Initial y coordinate
    /// 
    FPOINT(double xi, double yi) : x(xi), y(yi) {}
};


class MeaUtils {

public:
    /// Converts the specified double to a string with the
    /// minimum number of decimal places.
    ///
    /// @param value    [in] Numerical value to convert to a string.
    ///
    /// @return String corresponding to the double value.
    ///
    static CString DblToStr(double value);

    /// Tests whether the specified string is a number. For the
    /// purposes of this method, a number is a base 10 double
    /// precision floating point value.
    ///
    /// @param str      [in] String to test
    /// @param valuep   [out] Value parsed from the string. nullptr
    ///                 if not interested. If the return value is
    ///                 <b>false</b>, valuep's value is undefined.
    ///
    /// @return <b>true</b> if string represents a number.
    ///
    static bool IsNumber(LPCTSTR str, double* valuep = nullptr);

    /// Tests whether the specified string is a boolean value. For
    /// the purpose of this method, the strings "1", "TRUE", "true"
    /// are boolean <b>true</b> values, while "0", "FALSE", "false"
    /// are boolean <b>false</b>.
    ///
    /// @param str      [in] String to test
    /// @param valuep   [out] Value parsed from the string. nullptr
    ///                 if not interested. If the return value is
    ///                 <b>false</b>, valuep's value is undefined.
    ///
    /// @return <b>true</b> if string represents a boolean value.
    ///
    static bool IsBoolean(LPCTSTR str, bool* valuep = nullptr);

    /// Converts standalone LF characters into a CR+LF pair.
    /// Existing CR+LF pairs in the string are not effected.
    ///
    /// @param str      [in] String to process.
    ///
    /// @return Converted string.
    ///
    static CString LFtoCRLF(CString str);

    /// Converts CR+LF pairs to a single LF character.
    ///
    /// @param str      [in] String to process.
    ///
    /// @return Converted string.
    ///
    static CString CRLFtoLF(CString str);

    /// Indicates whether the specified key is being pressed.
    ///
    /// @param keyCode  ASCII character code or VK_* for a virtual key.
    /// 
    static bool IsKeyPressed(int keyCode);

private:
    /// All members of this class are static. No instances
    /// of this class are ever created.
    ///
    MeaUtils() {}

    /// All members of this class are static. No instances
    /// of this class are ever created.
    ///
    ~MeaUtils() {}
};


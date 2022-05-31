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
/// @brief Interface providing measurement units information.

#pragma once

#include "Units.h"
#include <meazure/utilities/Geometry.h>


/// Interface for units related information.
///
struct MeaUnitsProvider {

    /// Returns the current linear measurement units.
    ///
    /// @return Current linear units.
    ///
    virtual MeaLinearUnits* GetLinearUnits() const = 0;

    /// Returns the linear measurement units object corresponding
    /// to the specified identifier string (e.g. "px").
    ///
    /// @param unitsStr     [in] Linear units identifier string.
    ///
    /// @return Linear measurement units object.
    ///
    virtual MeaLinearUnits* GetLinearUnits(const CString& unitsStr) const = 0;

    /// Returns the current angular measurement units.
    ///
    /// @return Current angular units.
    ///
    virtual MeaAngularUnits* GetAngularUnits() const = 0;

    /// Returns the angular measurement units object corresponding
    /// to the specified identifier string (e.g. "deg").
    ///
    /// @param unitsStr     [in] Angular units identifier string.
    ///
    /// @return Angular measurement units object.
    ///
    virtual MeaAngularUnits* GetAngularUnits(const CString& unitsStr) const = 0;

    /// Returns the orientation of the y-axis.
    ///
    /// @return If <b>true</b> is returned, the origin of the coordinate
    ///         system is located at the lower left of the primary screen,
    ///         and the positive y-axis is pointing upward. If <b>false</b>
    ///         is returned, the origin is located at the upper left corner
    ///         and the positive y-axis is pointing down. Note that the
    ///         actual location of the origin is determined by the SetOrigin
    ///         method.
    ///
    virtual bool IsInvertY() const = 0;

    /// Returns the location of the origin of the coordinate system.
    ///
    /// @return Location of the origin of the coordinate system, in pixels.
    ///
    virtual const POINT& GetOrigin() const = 0;

    /// Uses the current linear measurement units to convert the two specified
    /// points from pixels to the current units, and then calculates the
    /// width and height of the rectangle formed by the two points.
    ///
    /// @param p1   [in] One point defining the rectangle, in pixels.
    /// @param p2   [in] The other point defining the rectangle, in pixels.
    ///
    /// @return The width and height of the rectangle formed by the two
    ///         specified points converted to the current linear measurement
    ///         units. Regardless of the relative locations of the two points,
    ///         the width and height are always positive. In addition, the
    ///         width and height are inclusive (e.g. p1.x = 1, p2.x = 3,
    ///         width = p2.x - p1.x + 1 = 3).
    ///
    virtual FSIZE GetWidthHeight(const POINT& p1, const POINT& p2) const = 0;

    /// Converts the specified angle to the current angular units
    /// and formats the result for display.
    /// @param angle    [in] Angle to be converted and displayed, in radians.
    /// @return String containing the converted and formatted angle data.
    virtual CString FormatConvertAngle(double angle) const = 0;

    /// Formats the specified linear measurement value for display.
    /// @param id       [in] Identifies the linear measurement for
    ///                 use in determining the display precision.
    /// @param value    [in] Linear measurement value to format,
    ///                 in current units.
    /// @return String containing the formatted measurement data.
    virtual CString Format(MeaLinearMeasurementId id, double value) const = 0;

    /// Formats the specified angular measurement value for display.
    /// @param id       [in] Identifies the angular measurement for
    ///                 use in determining the display precision.
    /// @param value    [in] Angular measurement value to format,
    ///                 in current units.
    /// @return String containing the formatted measurement data.
    virtual CString Format(MeaAngularMeasurementId id, double value) const = 0;

    /// Converts the specified coordinate from pixels to the desired units.
    /// This conversion takes into account the location of the origin and the
    /// orientation of the y-axis.
    ///
    /// @param pos      [in] Coordinates in pixels to convert to the desired units.
    ///
    /// @return Coordinates converted from pixels to the desired units. The
    ///         conversion takes into account the location of the origin and
    ///         the orientation of the y-axis.
    ///         
    virtual FPOINT ConvertCoord(const POINT& pos) const = 0;

    /// Converts the specified position from pixels to the desired units.
    /// This conversion does not take into account the location of the origin
    /// nor does it compensate for the orientation of the y-axis.
    ///
    /// @param pos      [in] Position in pixels to convert to the desired units.
    ///
    /// @return Position converted from pixels to the desired units. The
    ///         conversion does not take into account the location of the
    ///         origin nor does it compensate for the orientation of the y-axis.
    ///
    virtual FPOINT ConvertPos(const POINT& pos) const = 0;

    /// Converts from the current units to pixels. The conversion does not take into
    /// account the location of the origin nor the orientation of the y-axis.
    ///
    /// @param pos      [in] Position to convert to pixels.
    ///
    /// @return Position converted from the current units to pixels. The conversion
    ///         does not take into account the location of the origin nor the
    ///         orientation of the y-axis.
    ///
    virtual POINT UnconvertPos(const FPOINT& pos) const = 0;

    /// Converts the specified resolution in pixels/inch to the desired units.
    ///
    /// @param res      [in] Resolution in pixels/inch to convert to the desired units.
    ///
    /// @return Resolution converted from pixels/inch to the desired units.
    ///
    virtual FSIZE ConvertRes(const FSIZE& res) const = 0;

    /// Converts the specified angle value from its native radians
    /// to the desired units.
    ///
    /// @param angle    [in] Value to be converted.
    ///
    /// @return Angle value converted to the desired units.
    ///
    virtual double ConvertAngle(double angle) const = 0;

    /// Converts the specified value from the specified units to pixels. A minimum
    /// pixel value is specified in case the resolution is such that the conversion
    /// to pixels results in a value that is too small.
    ///
    /// @param id           [in] Linear units identifier.
    /// @param res          [in] Screen resolution to use to perform the conversion.
    /// @param value        [in] Value to be converted to pixels.
    /// @param minPixels    [in] If the converted pixel value in less than this
    ///                     minimum value, the minimum value is returned.
    ///
    /// @return X and Y pixel values.
    ///
    virtual SIZE ConvertToPixels(MeaLinearUnitsId id, const FSIZE& res, double value, int minPixels) const = 0;

    /// Returns the number of minor ticks to display before a major tick mark is
    /// displayed on the measurement rulers.
    ///
    /// @return Number of minor tick marks to display before displaying a major
    ///         tick mark.
    ///
    virtual int GetMajorTickCount() const = 0;

    /// Returns the increment between minor tick marks on the measurement rulers.
    ///
    /// @param rect     [in] Rectangle in screen coordinates, used to determine
    ///                 the current screen resolution.
    ///
    /// @return Increment for the minor tick marks, in the current units.
    ///
    virtual FSIZE GetMinorIncr(const RECT& rect) const = 0;
};

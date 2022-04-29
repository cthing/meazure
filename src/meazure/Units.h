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
/// @brief Header file for the units manager and its associated units classes.

#pragma once

#include <math.h>
#include <list>
#include <map>
#include <vector>
#include "Label.h"
#include "Profile.h"
#include "Singleton.h"
#include "Utils.h"


class MeaUnitsMgr;


/// Identifiers for linear measurement units.
///
enum MeaLinearUnitsId {
    MeaPixelsId = 0,        ///< Pixel units.
    MeaPointsId = 1,        ///< Point units (1/72 inch).
    MeaTwipsId = 2,         ///< Twip units (1/1440 inch).
    MeaInchesId = 3,        ///< Inch units.
    MeaCentimetersId = 4,   ///< Centimeter units (1/2.54 inch)
    MeaMillimetersId = 5,   ///< Millimeter units (1/25.4 inch)
    MeaPicasId = 6,         ///< Pica units (1/6 inch)
    MeaCustomId = 7         ///< User defined units
};


/// Identifiers for angular measurement units.
///
enum MeaAngularUnitsId {
    MeaDegreesId = 0,            ///< Angles reported in degrees.
    MeaRadiansId = 1             ///< Angles reported in radians.
};


/// Types of linear measurements.
///
enum MeaLinearMeasurementId {
    MeaX = 0,        ///< X coordinate
    MeaY = 1,        ///< Y coordinate
    MeaW = 2,        ///< Width
    MeaH = 3,        ///< Height
    MeaD = 4,        ///< Distance
    MeaAr = 5,       ///< Area
    MeaRx = 6,       ///< Screen resolution in the X direction
    MeaRy = 7        ///< Screen resolution in the Y direction
};


/// Types of angular measurements.
///
enum MeaAngularMeasurementId {
    MeaA = 0         ///< Angle
};


/// The screen resolution can differ in the X and Y directions.
/// When conversions require the use of the screen resolution,
/// a direction must be specified so that the correct resolution
/// is used. These identifiers specify the direction for conversions.
///
enum MeaConvertDir {
    MeaConvertX = 0,        ///< X axis direction.
    MeaConvertY = 1         ///< Y axis direction.
};


/// Measurement units are packaged into a class, one for each set
/// of units (e.g. inches). Each class contains methods for conversion,
/// display precision, and persistence of the state of the class to a
/// given profile object. Related to these classes but not contained by
/// them are the set of unit label classes.
///
/// This class serves as the base class for all measurement units.
///
class MeaUnits {

public:
    static const double kPI;    ///< The value of pi.

    typedef std::vector<int>        DisplayPrecisions;      ///< Number of decimal places to display for each type of measurement.
    typedef std::vector<CString>    DisplayPrecisionNames;  ///< Strings to identify units precisions in profiles.


    /// Persists the state of a units class instance to the specified
    /// profile object.
    ///
    /// @param profile      [in] Profile object into which to persist
    ///                     the state of the units class.
    ///
    virtual void SaveProfile(MeaProfile& profile);

    /// Restores the state of a units class instance from the specified
    /// profile object.
    ///
    /// @param profile      [in] Profile object from which to restore
    ///                     the state of the units class.
    ///
    virtual void LoadProfile(MeaProfile& profile);

    /// Persists the measurement precisions to the specified profile object.
    ///
    /// @param profile      [in] Profile object into which to persist
    ///                     the decimal place precisions.
    ///
    void SavePrecision(MeaProfile& profile);

    /// Restores the measurement precisions from the specified profile object.
    ///
    /// @param profile      [in] Profile object from which to restore
    ///                     the decimal place precisions.
    ///
    void LoadPrecision(MeaProfile& profile);

    /// Called to restores the units to their default state.
    ///
    virtual void MasterReset();

    /// Returns a string describing the units.
    ///
    /// @return String describing he units (e.g. "px')
    ///
    CString GetUnitsStr() const { return m_unitsStr; }

    /// Sets the decimal place precisions for all measurement types.
    ///
    /// @param displayPrecisions    [in] Set of precisions for all measurement types.
    ///
    void SetDisplayPrecisions(const DisplayPrecisions& displayPrecisions) {
        m_displayPrecisions = displayPrecisions;
    }

    /// Returns the current decimal place precisions for all measurement types.
    ///
    /// @return Set of precisions for all measurement types.
    ///
    const DisplayPrecisions& GetDisplayPrecisions() const {
        return m_displayPrecisions;
    }

    /// Returns the default decimal place precisions for all measurement types.
    ///
    /// @return Set of default precisions for all measurement types.
    ///
    const DisplayPrecisions& GetDefaultPrecisions() const {
        return m_defaultPrecisions;
    }

    /// Sets the default display precisions as the current precisions.
    ///
    void RestoreDefaultPrecisions() {
        m_displayPrecisions = m_defaultPrecisions;
    }

    /// Returns the identifying names for the display precisions.
    ///
    /// @return Set of names identifying precisions for all measurement types.
    ///
    const DisplayPrecisionNames& GetDisplayPrecisionNames() const {
        return m_displayPrecisionNames;
    }

protected:
    /// Base class constructor.
    ///
    /// @param unitsStr     [in] Name for the units.
    ///
    explicit MeaUnits(LPCTSTR unitsStr);

    /// Base class destructor.
    ///
    virtual ~MeaUnits();

    /// Adds the specified identifying name to the list of
    /// precision names.
    ///
    /// @param name     [in] Identifying name for the precision.
    ///
    void AddPrecisionName(LPCTSTR name) {
        m_displayPrecisionNames.push_back(name);
    }

    /// Adds the specified precision value to both the list
    /// of current precisions and the list of default precisions.
    ///
    /// @param precision    [in] Number of decimal places to display.
    ///
    void AddPrecision(int precision) {
        m_defaultPrecisions.push_back(precision);
        m_displayPrecisions.push_back(precision);
    }

private:
    DisplayPrecisions m_defaultPrecisions;          ///< Default precisions for all measurement types.
    DisplayPrecisions m_displayPrecisions;          ///< Current precisions for all measurement types.
    DisplayPrecisionNames m_displayPrecisionNames;  ///< Names for all precision values.
    CString m_unitsStr;                             ///< Name for the units.
};


/// There are two major categories of units: linear and angular.
/// The angular units cover the angle measurement units. This is
/// the base class for all angular units.
///
class MeaAngularUnits : public MeaUnits {

public:
    /// Returns the identifier for the angular units.
    ///
    /// @return Angular units ID.
    ///
    MeaAngularUnitsId GetUnitsId() const { return m_unitsId; }

    /// Formats the specified angular measurement value using the
    /// precision for the specified measurement ID.
    ///
    /// @param id       [in] Identifier for the angular units whose precision
    ///                 is to be used to format the specified value.
    /// @param value    [in] Measurement value to be formatted.
    ///
    /// @return Measurement value formatted with the appropriate precision
    ///         and returned as a string.
    ///
    CString Format(MeaAngularMeasurementId id, double value) const;

    /// Converts the specified angle value from its native radians
    /// to the desired units.
    ///
    /// @param angle    [in] Value to be converted.
    ///
    /// @return Angle value converted to the desired units.
    ///
    virtual double ConvertAngle(double angle) const = 0;

protected:
    /// Constructs an angular-based unit.
    ///
    /// @param unitsId      [in] Identifier for the angular units.
    /// @param unitsStr     [in] Name for the units.
    ///
    MeaAngularUnits(MeaAngularUnitsId unitsId, LPCTSTR unitsStr);

    /// Destroys an angular-based unit.
    ///
    virtual ~MeaAngularUnits();

private:
    MeaAngularUnitsId m_unitsId;  ///< Identifier for the units.
};


/// Provides angular measurements in degrees.
///
class MeaDegreeUnits : public MeaAngularUnits {

public:
    /// Constructs a degree measurement unit.
    ///
    MeaDegreeUnits();

    /// Destroys a degree measurement unit.
    ///
    ~MeaDegreeUnits();

    /// Converts the specified angle value from its native radians
    /// to degrees.
    ///
    /// @param angle    [in] Value to be converted from radians to degrees.
    ///
    /// @return Angle value in degrees.
    ///
    virtual double ConvertAngle(double angle) const;
};


/// Provides angular measurements in radians.
///
class MeaRadianUnits : public MeaAngularUnits {

public:
    /// Constructs a radian measurement unit.
    ///
    MeaRadianUnits();

    /// Destroys a radian measurement unit.
    ///
    ~MeaRadianUnits();

    /// Passthru method returning the specified
    /// angle in radians.
    ///
    /// @param angle    [in] Angle value in radians.
    ///
    /// @return Angle value in radians.
    ///
    virtual double ConvertAngle(double angle) const;
};


/// There are two major categories of units: linear and angular.
/// The linear units cover distance related measurements
/// (e.g. coordinates, distances, resolutions). This is the base
/// class for all linear units.
///
class MeaLinearUnits : public MeaUnits {

public:
    /// Returns the identifier for the linear units.
    ///
    /// @return Linear units ID.
    ///
    MeaLinearUnitsId GetUnitsId() const { return m_unitsId; }


    /// By default, the Windows coordinate system is inverted from the
    /// typical Cartesian coordinate system in that the origin is located
    /// in the upper left corner of the primary screen and the positive
    /// y-axis points downward. This method sets whether the y-axis should
    /// be inverted so that the origin is moved to the lower left corner
    /// of the primary screen and the positive y-axis points up. Note that
    /// the actual location of the origin is determined by the SetOrigin
    /// method.
    ///
    /// @param invertY      [in] <b>true</b> inverts the y-axis, placing
    ///                     the origin at the lower left of the primary
    ///                     display screen and making positive y pointing
    ///                     upward.
    ///
    static void SetInvertY(bool invertY) { m_invertY = invertY; }

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
    static bool GetInvertY() { return m_invertY; }

    /// Moves the origin of the coordinate system to the specified point. The
    /// orientation of the axes is not effected by this method. To change the
    /// orientation of the y-axis use the SetInvertY method.
    ///
    /// @param origin   [in] New location for the origin of the coordinate system.
    ///                 in pixels.
    ///
    static void SetOrigin(const POINT& origin) { m_originOffset = origin; }

    /// Returns the location of the origin of the coordinate system.
    ///
    /// @return Location of the origin of the coordinate system, in pixels.
    ///
    static const POINT& GetOrigin() { return m_originOffset; }

    /// Internally all measurements are in pixels. Measurement units based
    /// solely on pixels do not require the use of the screen resolution for
    /// conversion. Measurement units such as inches, require the screen
    /// resolution for conversion from pixels. This method indicates whether
    /// the screen resolution is required for conversion from pixels to the
    /// desired measurement units and vice versa.
    ///
    /// @return <b>true</b> if the screen resolution must be used to convert
    ///         to and from pixels. This base class implementation always
    ///         returns <b>true</b>.
    ///
    virtual bool RequiresRes() const;

    /// Formats the specified linear measurement value using the
    /// precision for the specified measurement ID.
    ///
    /// @param id       [in] Identifier for the linear units whose precision
    ///                 is to be used to format the specified value.
    /// @param value    [in] Measurement value to be formatted.
    ///
    /// @return Measurement value formatted with the appropriate precision
    ///         and returned as a string.
    ///
    CString Format(MeaLinearMeasurementId id, double value) const;

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
    FPOINT ConvertCoord(const POINT& pos) const;

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
    FPOINT ConvertPos(const POINT& pos) const;

    /// Converts the specified resolution in pixels/inch to the desired units.
    ///
    /// @param res      [in] Resolution in pixels/inch to convert to the desired units.
    ///
    /// @return Resolution converted from pixels/inch to the desired units.
    ///
    FSIZE ConvertRes(const FSIZE& res) const;

    /// Converts from the current units to pixels. The conversion takes into account
    /// the location of the origin and the orientation of the y-axis. The conversion
    /// is performed for the specified coordinate located on the specified axis.
    ///
    /// @param dir      [in] Axis on which the conversion should take place.
    /// @param wnd      [in] Window used to determine the screen resolution.
    /// @param pos      [in] Coordinate to convert to pixels.
    ///
    /// @return Coordinate converted from the current units to pixels. The conversion
    ///         takes into account the location of the origin and the orientation of
    ///         the y-axis.
    ///
    double UnconvertCoord(MeaConvertDir dir, const CWnd* wnd, double pos) const;

    /// Converts from the current units to pixels. The conversion takes into account
    /// the location of the origin and the orientation of the y-axis. The conversion
    /// is performed for the specified coordinate located on the specified axis. Since
    /// the specified coordinate might not convert to an integral number of pixels,
    /// this method returns the bounding integral pixel values and indicates whether
    /// an exact integral conversion could be performed.
    ///
    /// @param dir      [in] Axis on which the conversion should take place.
    /// @param wnd      [in] Window used to determine the screen resolution.
    /// @param pos      [in] Coordinate to convert to pixels.
    /// @param c1       [out] Closest integral pixel value below the converted
    ///                 coordinate.
    /// @param c2       [out] Closest integral pixel value above the converted
    ///                 coordinate.
    ///
    /// @return <b>true</b> if the  conversion to pixels was exact.
    ///
    bool UnconvertCoord(MeaConvertDir dir, const CWnd* wnd, double pos, int& c1, int& c2) const;

    /// Converts from the current units to pixels. The conversion takes into account
    /// the location of the origin and the orientation of the y-axis.
    ///
    /// @param pos      [in] Coordinates to convert to pixels.
    ///
    /// @return Coordinates converted from the current units to pixels. The conversion
    ///         takes into account the location of the origin and the orientation of
    ///         the y-axis.
    ///
    POINT UnconvertCoord(const FPOINT& pos) const;

    /// Converts from the current units to pixels. The conversion does not take into
    /// account the location of the origin nor the orientation of the y-axis.
    ///
    /// @param pos      [in] Position to convert to pixels.
    ///
    /// @return Position converted from the current units to pixels. The conversion
    ///         does not take into account the location of the origin nor the
    ///         orientation of the y-axis.
    ///
    POINT UnconvertPos(const FPOINT& pos) const;

    /// Returns the number of minor ticks to display before a major tick mark is
    /// displayed on the measurement rulers.
    ///
    /// @return Number of minor tick marks to display before displaying a major
    ///         tick mark.
    ///
    int GetMajorTickCount() const { return m_majorTickCount; }

    /// Returns the increment between minor tick marks on the measurement rulers.
    ///
    /// @param rect     [in] Rectangle in screen coordinates, used to determine
    ///                 the current screen resolution.
    ///
    /// @return Increment for the minor tick marks, in the current units.
    ///
    FSIZE GetMinorIncr(const RECT& rect) const;

    /// Converts the specified value from the current units to pixels. A minimum
    /// pixel value is specified in case the resolution is such that the conversion
    /// to pixels results in a value that is too small.
    ///
    /// @param res          [in] Screen resolution to use to perform the conversion.
    /// @param value        [in] Value to be converted to pixels.
    /// @param minPixels    [in] If the converted pixel value in less than this
    ///                     minimum value, the minimum value is returned.
    ///
    /// @return X and Y pixel values.
    ///
    SIZE ConvertToPixels(const FSIZE& res, double value, int minPixels) const;

protected:
    /// Constructs the linear units.
    ///
    /// @param unitsId      [in] Identifier for the linear units.
    /// @param unitsStr     [in] Name for the units.
    ///
    MeaLinearUnits(MeaLinearUnitsId unitsId, LPCTSTR unitsStr);

    /// Destroys the linear units.
    ///
    virtual ~MeaLinearUnits();

    /// Returns a resolution conversion factor. For most units this is
    /// the same as the values returned by the FromPixels method. However,
    /// since internally the resolution is expressed as pixels per inch,
    /// the pixel units need to provide their own conversion factor.
    ///
    /// @param res      [in] Resolution to convert.
    ///
    /// @return Resolution conversion factor.
    ///
    virtual FSIZE GetResFromPixels(const FSIZE& res) const;

    /// Returns the X and Y factors to convert from pixels to the
    /// current units. In other words, multiplying the values returned
    /// from this method by pixels results in a value expressed in the
    /// current units.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in units/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const = 0;

    /// Converts from the pixels to the current units. The conversion takes into account
    /// the location of the origin and the orientation of the y-axis. The conversion
    /// is performed for the specified coordinate located on the specified axis.
    ///
    /// @param dir      [in] Axis on which the conversion should take place.
    /// @param wnd      [in] Window used to determine the screen resolution.
    /// @param pos      [in] Coordinate to convert from pixels.
    ///
    /// @return Coordinate converted from pixels to the current units. The conversion
    ///         takes into account the location of the origin and the orientation of
    ///         the y-axis.
    ///
    double ConvertCoord(MeaConvertDir dir, const CWnd* wnd, int pos) const;

    /// In a multiple monitor environment there are multiple screen resolutions,
    /// one set per monitor. Therefore, to determine the a resolution, a screen must
    /// be determined. This method uses the specified position to determined a screen
    /// and thus a resolution. The method compensates for the location of the origin
    /// and the orientation of the y-axis.
    ///
    /// @param pos      [in] Position used to determine a screen and thus a resolution,
    ///                 in the current units.
    ///
    /// @return Screen resolution, in the current units.
    ///
    const FSIZE& FindResFromCoord(const FPOINT& pos) const;

    /// In a multiple monitor environment there are multiple screen resolutions,
    /// one set per monitor. Therefore, to determine the a resolution, a screen must
    /// be determined. This method uses the specified position to determined a screen
    /// and thus a resolution. The method does not compensate for the location of the
    /// origin nor the orientation of the y-axis.
    ///
    /// @param pos      [in] Position used to determine a screen and thus a resolution,
    ///                 in the current units.
    ///
    /// @return Screen resolution, in the current units.
    ///
    const FSIZE& FindResFromPos(const FPOINT& pos) const;

private:
    static const double kTickIncrements[];  ///< Ruler tick increments. The order of magnitude of
                                            ///< these values is adjusted based on the units.
    static const int kNumTickIncrements;    ///< Number of tick mark increments in the
                                            ///< kTickIncrements array.
    static const int kMinSepPixels;         ///< Minimum separation between ruler tick marks, in pixels.
    static const double kMinSepInches;      ///< Minimum separation between ruler tick marks, in inches.

    static POINT m_originOffset;    ///< Offset of the origin from the system origin, in pixels.
    static bool m_invertY;          ///< Indicates if the y-axis direction is inverted.

    MeaLinearUnitsId m_unitsId;     ///< Linear units identifier.
    int m_majorTickCount;           ///< Number of minor ruler tick marks between major tick marks.
};


/// Provides linear measurements in pixels.
///
class MeaPixelUnits : public MeaLinearUnits {

public:
    /// Constructs pixel units.
    ///
    MeaPixelUnits();

    /// Destroys pixel units.
    ///
    ~MeaPixelUnits();

    /// Internally all measurements are in pixels. Measurement units based
    /// solely on pixels do not require the use of the screen resolution for
    /// conversion. Measurement units such as inches, require the screen
    /// resolution for conversion from pixels. This method indicates whether
    /// the screen resolution is required for conversion from pixels to the
    /// desired measurement units and vice versa.
    ///
    /// @return Always returns <b>false</b> since the resolution is not used
    ///         when measuring using pixels.
    ///
    virtual bool RequiresRes() const;

protected:
    /// Returns a resolution conversion factor. Since internally the
    /// resolution is expressed as pixels per inch, the pixel units need
    /// to provide a resolution conversion factor that is effectively
    /// cancels out the conversion.
    ///
    /// @param res      [in] Resolution to convert.
    ///
    /// @return Resolution conversion factor.
    ///
    virtual FSIZE GetResFromPixels(const FSIZE& res) const;

    /// Returns the X and Y factors to convert from pixels to the
    /// current units. Since the current units are pixels, the
    /// factor return is 1.0 in both directions.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return 1.0 in both directions.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;
};


/// Provides linear measurements in points.
///
class MeaPointUnits : public MeaLinearUnits {

public:
    /// Constructs point units.
    ///
    MeaPointUnits();

    /// Destroys point units.
    ///
    ~MeaPointUnits();

protected:
    /// Returns the X and Y factors to convert from pixels to points.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in points.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in points/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;
};


/// Provides linear measurements in picas.
///
class MeaPicaUnits : public MeaLinearUnits {

public:
    /// Constructs pica units.
    ///
    MeaPicaUnits();

    /// Destroys pica units.
    ///
    ~MeaPicaUnits();

protected:
    /// Returns the X and Y factors to convert from pixels to picas.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in picas.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in picas/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;
};


/// Provides linear measurements in twips.
///
class MeaTwipUnits : public MeaLinearUnits {

public:
    /// Constructs twips units.
    ///
    MeaTwipUnits();

    /// Destroys twips units.
    ///
    ~MeaTwipUnits();

protected:
    /// Returns the X and Y factors to convert from pixels to twips.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in twips.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in twips/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;
};


/// Provides linear measurements in inches.
///
class MeaInchUnits : public MeaLinearUnits {

public:
    /// Constructs inch units.
    ///
    MeaInchUnits();

    /// Destroys inch units.
    ///
    ~MeaInchUnits();

protected:
    /// Returns the X and Y factors to convert from pixels to inches.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in inches.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in inches/pixels
    ///         (i.e. the inverse of the resolution).
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;
};


/// Provides linear measurements in centimeters.
///
class MeaCentimeterUnits : public MeaLinearUnits {

public:
    /// Constructs centimeter units.
    ///
    MeaCentimeterUnits();

    /// Destroys centimeter units.
    ///
    ~MeaCentimeterUnits();

protected:
    /// Returns the X and Y factors to convert from pixels to centimeters.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in centimeters.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in centimeters/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;
};


/// Provides linear measurements in millimeters.
///
class MeaMillimeterUnits : public MeaLinearUnits {

public:
    /// Constructs millimeter units.
    ///
    MeaMillimeterUnits();

    /// Destroys millimeter units.
    ///
    ~MeaMillimeterUnits();

protected:
    /// Returns the X and Y factors to convert from pixels to millimeters.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in millimeters.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in millimeters/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;
};


/// Provides linear measurements in user defined units.
///
class MeaCustomUnits : public MeaLinearUnits {

public:
    /// User defined units are based on a conversion factor. This factor
    /// converts from a basis set of units to the custom units. The
    /// basis for the conversion is identified by this enumeration.
    ///
    enum ScaleBasis {
        PixelBasis = 0,         ///< Custom units = factor * pixels.
        InchBasis = 1,          ///< Custom units = factor * inches.
        CentimeterBasis = 2     ///< Custom units = factor * centimeters.
    };


    static const double     kDefScaleFactor;    ///< Default scale factor.
    static const ScaleBasis kDefScaleBasis;     ///< Default basis for the custom units.


    /// Constructs custom units.
    ///
    MeaCustomUnits();

    /// Destroys custom units.
    ///
    ~MeaCustomUnits();

    /// Persists the custom units conversion factor and basis to the
    /// specified profile object.
    ///
    /// @param profile      [in] Profile object into which to persist
    ///                     the custom units.
    ///
    virtual void SaveProfile(MeaProfile& profile);

    /// Restores the custom units conversion factor and basis from the
    /// specified profile object.
    ///
    /// @param profile      [in] Profile object from which to restore
    ///                     the custom units.
    ///
    virtual void LoadProfile(MeaProfile& profile);

    /// Called when the master reset is initiated. Resets the custom
    /// units to its default state.
    ///
    virtual void MasterReset();

    /// Indicates whether the custom units requires a screen resolution
    /// to perform its conversion from pixels.
    ///
    /// @return <b>true</b> if the conversion basis is not based on pixels.
    ///
    virtual bool RequiresRes() const;

    /// Indicates whether custom units have been defined.
    ///
    /// @return <b>true</b> if custom units have been defined. Custom units
    ///         are considered to be defined if they have a name and
    ///         abbreviation.
    ///
    bool HaveCustomUnits() const {
        return !m_name.IsEmpty() && !m_abbrev.IsEmpty();
    }

    /// Returns the name of the custom units. The name is used on the
    /// units menu as well as other places.
    ///
    /// @return Name for the units.
    ///
    CString GetName() const { return m_name; }

    /// Sets a name for the custom units. The name is displayed on the
    /// units menu and in other places.
    ///
    /// @param name     [in] Name for the units.
    ///
    void SetName(const CString& name);

    /// Returns the abbreviation for the custom units. The abbreviation
    /// is used by the data display and in other places.
    ///
    /// @return Abbreviation for the units.
    ///
    CString GetAbbrev() const { return m_abbrev; }

    /// Sets an abbreviation for the custom units. The abbreviation is
    /// displayed by the data display and in other places.
    ///
    /// @param abbrev   [in] Abbreviation for the units.
    ///
    void SetAbbrev(const CString& abbrev);

    /// Returns the conversion basis.
    ///
    /// @return Custom units conversion basis.
    ///
    ScaleBasis GetScaleBasis() const { return m_scaleBasis; }

    /// Returns the scale basis as an identifying string (e.g. "px").
    ///
    /// @return Conversion basis as a string.
    ///
    CString GetScaleBasisStr() const;

    /// Sets the conversion basis.
    ///
    /// @param scaleBasis       [in] Conversion basis.
    ///
    void SetScaleBasis(ScaleBasis scaleBasis) { m_scaleBasis = scaleBasis; }

    /// Sets the conversion basis using a string identifier.
    ///
    /// @param scaleBasisStr    [in] Conversion basis as an identifying string.
    ///
    void SetScaleBasis(CString scaleBasisStr);

    /// Returns the conversion factor for the custom units.
    ///
    /// @return Conversion factor from the conversion basis to
    ///         the custom units.
    ///
    double GetScaleFactor() const { return m_scaleFactor; }

    /// Sets the conversion factor for the custom units.
    ///
    /// @param scaleFactor  [in] Conversion factor from the conversion
    ///                     basis to the custom units.
    ///
    void SetScaleFactor(double scaleFactor) { m_scaleFactor = scaleFactor; }

protected:
    /// Returns the X and Y factors to convert from pixels to the
    /// custom units. In other words, multiplying the values returned
    /// from this method by pixels results in a value expressed in the
    /// custom units.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in custom units/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const;

private:
    CString m_name;             ///< Name for the custom units.
    CString m_abbrev;           ///< Abbreviation for the custom units.
    ScaleBasis m_scaleBasis;    ///< Conversion basis for the custom units.
    double m_scaleFactor;       ///< Conversion factor for the custom units.
};


//
// Labels
//

/// Base class for measurement units labels. The label is
/// shown on the data display. There is a derived class for
/// each type of measurement: angles, lengths, areas and
/// resolutions.
///
class MeaUnitsLabel : public MeaLabel {

public:
    typedef std::map<int, CString>  LabelMap;   ///< Maps a units identifier to the corresponding label string.

    /// Destroys the base class instance for the label.
    ///
    virtual ~MeaUnitsLabel();

    /// Creates the underlying label control.
    ///
    /// @param unitsId      [in] Units identifier.
    /// @param style        [in] Window style for the label as
    ///                     defined by CStatic.
    /// @param topLeft      [in] Location for the top left corner
    ///                     of the label, in pixels.
    /// @param parentWnd    [in] Parent window for the label.
    /// @param id           [in] Caller defined identifier.
    ///
    /// @return <b>true</b> if the label was created successfully.
    ///
    bool Create(UINT unitsId, DWORD style, const POINT& topLeft,
        CWnd* parentWnd, UINT id = 0xffff);

protected:
    /// Base class constructor for the label instance.
    ///
    /// @param format   [in] Format string that defines the
    ///                 type of measurement units (e.g. "sq.").
    /// @param labels   [in] Map of measurement identifiers to
    ///                 label strings.
    /// @param width    [out] Width of the labels, in pixels.
    ///
    MeaUnitsLabel(UINT format, LabelMap& labels, int& width);

    /// Returns the measurement type format string.
    ///
    /// @return Format string identifier.
    ///
    UINT GetFormat() const { return m_format; }

    /// Sets the overall width of the label object to the width
    /// of the widest label string over all the units.
    ///
    /// @return <b>true</b> if the size was set successfully.
    ///
    bool SetSize();

    /// Formats the label string corresponding to the specified
    /// units identifier.
    ///
    /// @param unitsId      [in] Units identifier whose label is
    ///                     to be constructed.
    ///
    /// @return Formatted label string (e.g. "sq. in").
    ///
    CString ConstructLabel(int unitsId) const;

private:
    LabelMap& m_labelsRef;  ///< Maps units identifiers to label strings.
    int& m_widthRef;        ///< Width of the label object, in pixels.
    UINT m_format;          ///< Label format string identifier.
};


/// There are two categories of labels: those for angular
/// measurements and those for linear measurements. This
/// class handles angular measurement labels.
///
class MeaAngularUnitsLabel : public MeaUnitsLabel {

public:
    /// Constructs an angular measurement label.
    ///
    MeaAngularUnitsLabel();

    /// Destroys an angular measurement label.
    ///
    virtual ~MeaAngularUnitsLabel();

    /// Displays the actual text of the label.
    ///
    /// @param anglesId     [in] Angular units identifier.
    ///
    virtual void SetUnits(MeaAngularUnitsId anglesId);

private:
    static LabelMap m_labels;   ///< Maps angular units identifiers to their corresponding label strings.
    static int m_labelWidth;    ///< Width of the label, in pixels.
};


/// There are two categories of labels: those for angular
/// measurements and those for linear measurements. This
/// class is the base class for linear measurement labels.
///
class MeaLinearUnitsLabel : public MeaUnitsLabel {

public:
    /// Displays the actual text of the label.
    ///
    /// @param unitsId      [in] Linear units identifier.
    ///
    virtual void SetUnits(MeaLinearUnitsId unitsId);

    /// Sets the specified label string for the specified units.
    ///
    /// @param unitsId      [in] Identifier for the units whose label
    ///                     string is to be changed.
    /// @param label        [in] New label string for the units.
    ///
    static void ChangeLabel(MeaLinearUnitsId unitsId, const CString& label) {
        m_labels[unitsId] = label;
    }

protected:
    /// Constructs a linear units label.
    ///
    /// @param format   [in] Format string identifier. The format
    ///                 string provides label modifiers for the
    ///                 measurement type (e.g. "sq." for "sq. in").
    /// @param width    [out] Width of the label, in pixels.
    ///
    MeaLinearUnitsLabel(UINT format, int& width);

    /// Destroys a linear units label.
    ///
    virtual ~MeaLinearUnitsLabel();

private:
    static LabelMap m_labels;   ///< Maps linear units identifiers to their corresponding label strings.
};


/// Label for length measurements. Length measurements
/// include coordinates, width, heights and distances.
///
class MeaLengthUnitsLabel : public MeaLinearUnitsLabel {

public:
    /// Constructs a length measurement label.
    ///
    MeaLengthUnitsLabel();

    /// Destroys a length measurement label.
    ///
    virtual ~MeaLengthUnitsLabel();

private:
    static int m_labelWidth;   ///< Width of the label object, in pixels.
};


/// Label for area measurements.
///
class MeaAreaUnitsLabel : public MeaLinearUnitsLabel {

public:
    /// Constructs an area measurement label.
    ///
    MeaAreaUnitsLabel();

    /// Destroys an area measurement label.
    ///
    virtual ~MeaAreaUnitsLabel();

private:
    static int m_labelWidth;   ///< Width of the label object, in pixels.
};


/// Label for resolution measurements.
///
class MeaResUnitsLabel : public MeaLinearUnitsLabel {

public:
    /// Constructs a resolution measurement label.
    ///
    MeaResUnitsLabel();

    /// Destroys a resolution measurement label.
    ///
    virtual ~MeaResUnitsLabel();

private:
    static int m_labelWidth;   ///< Width of the label object, in pixels.
};


/// Manager for all units related operations including creating
/// the units objects and labels, specifying the current units, setting
/// the measurement origin and setting the y-axis orientation.
///
class MeaUnitsMgr : public MeaSingleton_T<MeaUnitsMgr> {

public:
    static constexpr bool kDefHaveWarned { false };             ///< Indicates whether the user has
                                                                ///< already been warned about using the
                                                                ///< operating system reported resolution.
    static constexpr LPCTSTR kDefLinearUnits { _T("px") };      ///< Default units for linear measurements.
    static constexpr LPCTSTR kDefAngularUnits { _T("deg") };    ///< Default units for angular measurements.
    static constexpr bool kDefInvertY { false };                ///< Default orientation of the y-axis.


    /// Persists the state of the units manager to the specified
    /// profile object.
    ///
    /// @param profile      [in] Profile object into which the
    ///                     manager state is persisted.
    ///
    void SaveProfile(MeaProfile& profile);

    /// Restores the state of the units manager from the specified
    /// profile object.
    ///
    /// @param profile      [in] Profile object from which the
    ///                     manager state is restored.
    ///
    void LoadProfile(MeaProfile& profile);

    /// Called when the master application reset is initiated. The
    /// units manager is reset to its default state.
    ///
    void MasterReset();

    /// Sets the current linear measurement units based on the specified
    /// units identifier.
    ///
    /// @param unitsId      [in] Measurement units identifier.
    ///
    void SetLinearUnits(MeaLinearUnitsId unitsId);

    /// Sets the current linear measurement units based on the specified
    /// units identifier string (e.g. "in").
    ///
    /// @param unitsStr     [in] Measurement units identifier string (e.g. "in").
    ///
    void SetLinearUnits(LPCTSTR unitsStr);

    /// Sets the current angular measurement units based on the specified
    /// units identifier.
    ///
    /// @param unitsId      [in] Measurement units identifier.
    ///
    void SetAngularUnits(MeaAngularUnitsId unitsId);

    /// Sets the current angular measurement units based on the specified
    /// units identifier string (e.g. "deg").
    ///
    /// @param unitsStr     [in] Measurement units identifier.
    ///
    void SetAngularUnits(LPCTSTR unitsStr);

    /// Returns the current linear measurement units.
    ///
    /// @return Current linear units.
    ///
    MeaLinearUnits* GetLinearUnits() { return m_currentLinearUnits; }

    /// Returns the linear measurement units object corresponding
    /// to the specified identifier string (e.g. "px").
    ///
    /// @param unitsStr     [in] Linear units identifier string.
    ///
    /// @return Linear measurement units object.
    ///
    MeaLinearUnits* GetLinearUnits(const CString& unitsStr) {
        for (const auto& unitsEntry : m_linearUnitsMap) {
            if (unitsEntry.second->GetUnitsStr() == unitsStr) {
                return unitsEntry.second;
            }
        }
        return nullptr;
    }

    /// Returns the identifier for the current linear measurement units.
    ///
    /// @return Identifier for the current linear measurement units.
    ///
    MeaLinearUnitsId GetLinearUnitsId() const { return m_currentLinearUnits->GetUnitsId(); }

    /// Returns the identifier string for the current linear measurement units
    /// (e.g. "px").
    ///
    /// @return Identifier string for the current linear measurement units.
    ///
    CString GetLinearUnitsStr() const { return m_currentLinearUnits->GetUnitsStr(); }

    /// Returns the current angular measurement units.
    ///
    /// @return Current angular units.
    ///
    MeaAngularUnits* GetAngularUnits() { return m_currentAngularUnits; }

    /// Returns the angular measurement units object corresponding
    /// to the specified identifier string (e.g. "deg").
    ///
    /// @param unitsStr     [in] Angular units identifier string.
    ///
    /// @return Angular measurement units object.
    ///
    MeaAngularUnits* GetAngularUnits(const CString& unitsStr) {
        for (const auto& unitsEntry : m_angularUnitsMap) {
            if (unitsEntry.second->GetUnitsStr() == unitsStr) {
                return unitsEntry.second;
            }
        }
        return nullptr;
    }

    /// Returns the identifier for the current angular measurement units.
    ///
    /// @return Identifier for the current angular measurement units.
    ///
    MeaAngularUnitsId GetAngularUnitsId() const { return m_currentAngularUnits->GetUnitsId(); }

    /// Returns the identifier string for the current angular measurement units
    /// (e.g. "deg").
    ///
    /// @return Identifier string for the current angular measurement units.
    ///
    CString GetAngularUnitsStr() const { return m_currentAngularUnits->GetUnitsStr(); }

    /// Returns the user defined units object. This is not necessarily
    /// the current measurement units object.
    ///
    /// @return User defined measurement units object.
    ///
    MeaCustomUnits* GetCustomUnits() { return &m_customUnits; }

    /// Set the number of decimal places to show on the data display for all types of
    /// linear measurements in the specified units (e.g. coordinates, length, area, etc.).
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are to be set.
    /// @param precisions   [in] Array of decimal places for each measurement type.
    ///
    void SetLinearDisplayPrecisions(MeaLinearUnitsId unitsId, const MeaUnits::DisplayPrecisions& precisions) {
        m_linearUnitsMap[unitsId]->SetDisplayPrecisions(precisions);
    }

    /// Returns the array of decimal places currently set for each linear measurement
    /// type in the specified units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places for each linear measurement type.
    ///
    const MeaUnits::DisplayPrecisions& GetLinearDisplayPrecisions(MeaLinearUnitsId unitsId) {
        return m_linearUnitsMap[unitsId]->GetDisplayPrecisions();
    }

    /// Returns the array of default decimal places for each linear measurement
    /// type in the specified units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places for each linear measurement type.
    ///
    const MeaUnits::DisplayPrecisions& GetLinearDefaultPrecisions(MeaLinearUnitsId unitsId) {
        return m_linearUnitsMap[unitsId]->GetDefaultPrecisions();
    }

    /// Set the number of decimal places to show on the data display for angular
    /// measurements in the specified units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are to be set.
    /// @param precisions   [in] Array of decimal places.
    ///
    void SetAngularDisplayPrecisions(MeaAngularUnitsId unitsId, const MeaUnits::DisplayPrecisions& precisions) {
        m_angularUnitsMap[unitsId]->SetDisplayPrecisions(precisions);
    }

    /// Returns the array of decimal places currently set for the specified angular
    /// measurement units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places.
    ///
    const MeaUnits::DisplayPrecisions& GetAngularDisplayPrecisions(MeaAngularUnitsId unitsId) {
        return m_angularUnitsMap[unitsId]->GetDisplayPrecisions();
    }

    /// Returns the array of default decimal places for the specified angular
    /// measurement units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places.
    ///
    const MeaUnits::DisplayPrecisions& GetAngularDefaultPrecisions(MeaAngularUnitsId unitsId) {
        return m_angularUnitsMap[unitsId]->GetDefaultPrecisions();
    }

    /// By default, the Windows coordinate system is inverted from the
    /// typical Cartesian coordinate system in that the origin is located
    /// in the upper left corner of the primary screen and the positive
    /// y-axis points downward. This method sets whether the y-axis should
    /// be inverted so that the origin is moved to the lower left corner
    /// of the primary screen and the positive y-axis points up. Note that
    /// the actual location of the origin is determined by the SetOrigin
    /// method.
    ///
    /// @param invertY      [in] <b>true</b> inverts the y-axis, placing
    ///                     the origin at the lower left of the primary
    ///                     display screen and making positive y pointing
    ///                     upward.
    ///
    void SetInvertY(bool invertY) const { MeaLinearUnits::SetInvertY(invertY); }

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
    bool GetInvertY() const { return MeaLinearUnits::GetInvertY(); }

    /// Moves the origin of the coordinate system to the specified point. The
    /// orientation of the axes is not effected by this method. To change the
    /// orientation of the y-axis use the SetInvertY method.
    ///
    /// @param origin   [in] New location for the origin of the coordinate system.
    ///                 in pixels.
    ///
    void SetOrigin(const POINT& origin) const { MeaLinearUnits::SetOrigin(origin); }

    /// Returns the location of the origin of the coordinate system.
    ///
    /// @return Location of the origin of the coordinate system, in pixels.
    ///
    const POINT& GetOrigin() const { return MeaLinearUnits::GetOrigin(); }

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
    FSIZE GetWidthHeight(const POINT& p1, const POINT& p2) const;

    /// Creates a new length measurement units label object.
    ///
    /// @return Newly created length measurement units label.
    ///         When no longer needed, the object should be
    ///         destroyed using the DeleteUnitsLabel method
    ///         on the object.
    ///
    MeaLengthUnitsLabel* NewLengthUnitsLabel();

    /// Creates a new angular measurement units label object.
    ///
    /// @return Newly created angular measurement units label.
    ///         When no longer needed, the object should be
    ///         destroyed using the DeleteUnitsLabel method
    ///         on the object.
    ///
    MeaAngularUnitsLabel* NewAngleUnitsLabel();

    /// Creates a new area measurement units label object.
    ///
    /// @return Newly created area measurement units label.
    ///         When no longer needed, the object should be
    ///         destroyed using the DeleteUnitsLabel method
    ///         on the object.
    ///
    MeaAreaUnitsLabel* NewAreaUnitsLabel();

    /// Creates a new resolution measurement units label object.
    ///
    /// @return Newly created resolution measurement units label.
    ///         When no longer needed, the object should be
    ///         destroyed using the DeleteUnitsLabel method
    ///         on the object.
    ///
    MeaResUnitsLabel* NewResUnitsLabel();

    /// Destroys the specified measurement units label.
    ///
    /// @param label    [in] Label object to be deleted.
    ///
    void DeleteUnitsLabel(MeaUnitsLabel* label);

    /// Converts the specified angle to the current angular units
    /// and formats the result for display.
    /// @param angle    [in] Angle to be converted and displayed, in radians.
    /// @return String containing the converted and formatted angle data.
    CString FormatConvertAngle(double angle) const {
        return Format(MeaA, ConvertAngle(angle));
    }

    /// Formats the specified linear measurement value for display.
    /// @param id       [in] Identifies the linear measurement for
    ///                 use in determining the display precision.
    /// @param value    [in] Linear measurement value to format,
    ///                 in current units.
    /// @return String containing the formatted measurement data.
    CString Format(MeaLinearMeasurementId id, double value) const {
        return m_currentLinearUnits->Format(id, value);
    }

    /// Formats the specified angular measurement value for display.
    /// @param id       [in] Identifies the angular measurement for
    ///                 use in determining the display precision.
    /// @param value    [in] Angular measurement value to format,
    ///                 in current units.
    /// @return String containing the formatted measurement data.
    CString Format(MeaAngularMeasurementId id, double value) const {
        return m_currentAngularUnits->Format(id, value);
    }

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
    FPOINT ConvertCoord(const POINT& pos) const {
        return m_currentLinearUnits->ConvertCoord(pos);
    }

    /// Converts from the current units to pixels. The conversion takes into account
    /// the location of the origin and the orientation of the y-axis.
    ///
    /// @param pos      [in] Coordinates to convert to pixels.
    ///
    /// @return Coordinates converted from the current units to pixels. The conversion
    ///         takes into account the location of the origin and the orientation of
    ///         the y-axis.
    ///
    POINT UnconvertCoord(const FPOINT& pos) const {
        return m_currentLinearUnits->UnconvertCoord(pos);
    }

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
    FPOINT ConvertPos(const POINT& pos) const {
        return m_currentLinearUnits->ConvertPos(pos);
    }

    /// Converts from the current units to pixels. The conversion does not take into
    /// account the location of the origin nor the orientation of the y-axis.
    ///
    /// @param pos      [in] Position to convert to pixels.
    ///
    /// @return Position converted from the current units to pixels. The conversion
    ///         does not take into account the location of the origin nor the
    ///         orientation of the y-axis.
    ///
    POINT UnconvertPos(const FPOINT& pos) const {
        return m_currentLinearUnits->UnconvertPos(pos);
    }

    /// Converts the specified resolution in pixels/inch to the desired units.
    ///
    /// @param res      [in] Resolution in pixels/inch to convert to the desired units.
    ///
    /// @return Resolution converted from pixels/inch to the desired units.
    ///
    FSIZE ConvertRes(const FSIZE& res) const {
        return m_currentLinearUnits->ConvertRes(res);
    }

    /// Converts the specified angle value from its native radians
    /// to the desired units.
    ///
    /// @param angle    [in] Value to be converted.
    ///
    /// @return Angle value converted to the desired units.
    ///
    double ConvertAngle(double angle) const {
        return m_currentAngularUnits->ConvertAngle(angle);
    }

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
    SIZE ConvertToPixels(MeaLinearUnitsId id, const FSIZE& res, double value, int minPixels) const {
        return ((*m_linearUnitsMap.find(id)).second)->ConvertToPixels(res, value, minPixels);
    }

private:
    MEA_SINGLETON_DECL(MeaUnitsMgr)

    typedef std::map<MeaLinearUnitsId, MeaLinearUnits*> LinearUnitsMap;     ///< Maps linear units identifiers to the corresponding linear units objects.
    typedef std::map<MeaAngularUnitsId, MeaAngularUnits*> AngularUnitsMap;  ///< Maps angular units identifiers to the corresponding angular units objects.
    typedef std::list<MeaLinearUnitsLabel*> LinearLabelsList;               ///< List of all linear units labels.
    typedef std::list<MeaAngularUnitsLabel*> AngularLabelsList;             ///< List of all angular units labels.

    /// Managers do not have copy semantics so this method is purposely undefined.
    /// 
    MeaUnitsMgr(const MeaUnitsMgr&);

    /// Managers do not have assignment semantics so this method is purposely undefined.
    ///
    MeaUnitsMgr& operator=(const MeaUnitsMgr&);


    MeaPixelUnits m_pixelUnits;                 ///< Pixel units object.
    MeaPointUnits m_pointUnits;                 ///< Point units object.
    MeaTwipUnits m_twipUnits;                   ///< Twips units object.
    MeaInchUnits m_inchUnits;                   ///< Inch units object.
    MeaCentimeterUnits m_cmUnits;               ///< Centimeter units object.
    MeaMillimeterUnits m_mmUnits;               ///< Millimeter units object.
    MeaPicaUnits m_picaUnits;                   ///< Pica units object.
    MeaCustomUnits m_customUnits;               ///< User defined units object.
    MeaDegreeUnits m_degreeUnits;               ///< Degree units object.
    MeaRadianUnits m_radianUnits;               ///< Radian units object.
    MeaLinearUnits* m_currentLinearUnits;       ///< Points to the current linear units object.
    MeaAngularUnits* m_currentAngularUnits;     ///< Points to the current angular units object.
    LinearUnitsMap m_linearUnitsMap;            ///< Map of linear units identifiers to units objects.
    AngularUnitsMap m_angularUnitsMap;          ///< Map of angular units identifiers to units objects.
    LinearLabelsList m_linearLabelsList;        ///< List of linear units labels.
    AngularLabelsList m_angularLabelsList;      ///< List of angular units labels.
    bool m_haveWarned;                          ///< Indicates whether the user has already been warned about using
                                                ///< the operating system reported resolution.
};

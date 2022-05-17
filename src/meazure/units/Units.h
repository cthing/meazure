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
/// @brief Header file for the units classes.

#pragma once

#include <vector>
#include <meazure/ui/ScreenProvider.h>
#include <meazure/profile/Profile.h>
#include <meazure/utilities/Utils.h>


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
    virtual double ConvertAngle(double angle) const override;
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
    virtual double ConvertAngle(double angle) const override;
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
    /// @return <b>true</b> if the conversion to pixels was exact.
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

protected:
    /// Constructs the linear units.
    ///
    /// @param unitsId        [in] Identifier for the linear units.
    /// @param unitsStr       [in] Name for the units.
    /// @param screenProvider [in] Screen information provider
    ///
    MeaLinearUnits(MeaLinearUnitsId unitsId, LPCTSTR unitsStr, const MeaScreenProvider& screenProvider);

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

protected:
    const MeaScreenProvider& m_screenProvider;  ///< Screen information provider

private:
    static POINT m_originOffset;                ///< Offset of the origin from the system origin, in pixels.
    static bool m_invertY;                      ///< Indicates if the y-axis direction is inverted.
    MeaLinearUnitsId m_unitsId;                 ///< Linear units identifier.
    int m_majorTickCount;                       ///< Number of minor ruler tick marks between major tick marks.
};


/// Provides linear measurements in pixels.
///
class MeaPixelUnits : public MeaLinearUnits {

public:
    /// Constructs pixel units.
    /// 
    /// @param screenProvider [in] Screen information provider
    ///
    MeaPixelUnits(const MeaScreenProvider& screenProvider);

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
    virtual bool RequiresRes() const override;

    /// Returns the X and Y factors to convert from pixels to the
    /// current units. Since the current units are pixels, the
    /// factor return is 1.0 in both directions.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return 1.0 in both directions.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;

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
    virtual FSIZE GetResFromPixels(const FSIZE& res) const override;
};


/// Provides linear measurements in points.
///
class MeaPointUnits : public MeaLinearUnits {

public:
    /// Constructs point units.
    /// 
    /// @param screenProvider [in] Screen information provider
    ///
    MeaPointUnits(const MeaScreenProvider& screenProvider);

    /// Destroys point units.
    ///
    ~MeaPointUnits();

    /// Returns the X and Y factors to convert from pixels to points.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in points.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in points/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;
};


/// Provides linear measurements in picas.
///
class MeaPicaUnits : public MeaLinearUnits {

public:
    /// Constructs pica units.
    /// 
    /// @param screenProvider [in] Screen information provider
    ///
    MeaPicaUnits(const MeaScreenProvider& screenProvider);

    /// Destroys pica units.
    ///
    ~MeaPicaUnits();

    /// Returns the X and Y factors to convert from pixels to picas.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in picas.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in picas/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;
};


/// Provides linear measurements in twips.
///
class MeaTwipUnits : public MeaLinearUnits {

public:
    /// Constructs twips units.
    /// 
    /// @param screenProvider [in] Screen information provider
    ///
    MeaTwipUnits(const MeaScreenProvider& screenProvider);

    /// Destroys twips units.
    ///
    ~MeaTwipUnits();

    /// Returns the X and Y factors to convert from pixels to twips.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in twips.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in twips/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;
};


/// Provides linear measurements in inches.
///
class MeaInchUnits : public MeaLinearUnits {

public:
    /// Constructs inch units.
    /// 
    /// @param screenProvider [in] Screen information provider
    ///
    MeaInchUnits(const MeaScreenProvider& screenProvider);

    /// Destroys inch units.
    ///
    ~MeaInchUnits();

    /// Returns the X and Y factors to convert from pixels to inches.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in inches.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in inches/pixels
    ///         (i.e. the inverse of the resolution).
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;
};


/// Provides linear measurements in centimeters.
///
class MeaCentimeterUnits : public MeaLinearUnits {

public:
    /// Constructs centimeter units.
    /// 
    /// @param screenProvider [in] Screen information provider
    ///
    MeaCentimeterUnits(const MeaScreenProvider& screenProvider);

    /// Destroys centimeter units.
    ///
    ~MeaCentimeterUnits();

    /// Returns the X and Y factors to convert from pixels to centimeters.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in centimeters.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in centimeters/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;
};


/// Provides linear measurements in millimeters.
///
class MeaMillimeterUnits : public MeaLinearUnits {

public:
    /// Constructs millimeter units.
    /// 
    /// @param screenProvider [in] Screen information provider
    ///
    MeaMillimeterUnits(const MeaScreenProvider& screenProvider);

    /// Destroys millimeter units.
    ///
    ~MeaMillimeterUnits();

    /// Returns the X and Y factors to convert from pixels to millimeters.
    /// In other words, multiplying the values returned from this
    /// method by pixels results in a value expressed in millimeters.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in millimeters/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;
};


/// Provides linear measurements in user defined units.
///
class MeaCustomUnits : public MeaLinearUnits {

public:
    typedef void (*ChangeLabelFunc)(MeaLinearUnitsId, const CString&);

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
    /// @param screenProvider   [in] Screen information provider
    /// @param changeLabelFunc  [in] Function to change the label of the custom units
    ///
    MeaCustomUnits(const MeaScreenProvider& screenProvider, ChangeLabelFunc changeLabelFunc);

    /// Destroys custom units.
    ///
    ~MeaCustomUnits();

    /// Persists the custom units conversion factor and basis to the
    /// specified profile object.
    ///
    /// @param profile      [in] Profile object into which to persist
    ///                     the custom units.
    ///
    virtual void SaveProfile(MeaProfile& profile) override;

    /// Restores the custom units conversion factor and basis from the
    /// specified profile object.
    ///
    /// @param profile      [in] Profile object from which to restore
    ///                     the custom units.
    ///
    virtual void LoadProfile(MeaProfile& profile) override;

    /// Called when the master reset is initiated. Resets the custom
    /// units to its default state.
    ///
    virtual void MasterReset() override;

    /// Indicates whether the custom units requires a screen resolution
    /// to perform its conversion from pixels.
    ///
    /// @return <b>true</b> if the conversion basis is not based on pixels.
    ///
    virtual bool RequiresRes() const override;

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

    /// Returns the X and Y factors to convert from pixels to the
    /// custom units. In other words, multiplying the values returned
    /// from this method by pixels results in a value expressed in the
    /// custom units.
    ///
    /// @param res      [in] Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in custom units/pixels.
    ///
    virtual FSIZE FromPixels(const FSIZE& res) const override;

private:
    ChangeLabelFunc m_changeLabelFunc;  ///< Function to change the label text
    CString m_name;                     ///< Name for the custom units.
    CString m_abbrev;                   ///< Abbreviation for the custom units.
    ScaleBasis m_scaleBasis;            ///< Conversion basis for the custom units.
    double m_scaleFactor;               ///< Conversion factor for the custom units.
};

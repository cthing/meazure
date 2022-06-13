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
/// @brief Header file for the units manager.

#pragma once

#include <list>
#include "Units.h"
#include "UnitsLabels.h"
#include "UnitsProvider.h"
#include <meazure/utilities/Singleton.h>


/// Manager for all units related operations including creating
/// the units objects and labels, specifying the current units, setting
/// the measurement origin and setting the y-axis orientation.
///
class MeaUnitsMgr : public MeaUnitsProvider, public MeaSingleton_T<MeaUnitsMgr> {

public:
    static constexpr bool kDefHaveWarned { false };             ///< Indicates whether the user has
                                                                ///< already been warned about using the
                                                                ///< operating system reported resolution.
    static constexpr LPCTSTR kDefLinearUnits { _T("px") };      ///< Default units for linear measurements.
    static constexpr LPCTSTR kDefAngularUnits { _T("deg") };    ///< Default units for angular measurements.
    static constexpr bool kDefInvertY { false };                ///< Default orientation of the y-axis.


    MeaUnitsMgr(token);
    ~MeaUnitsMgr();

    MeaUnitsMgr(const MeaUnitsMgr&) = delete;
    MeaUnitsMgr& operator=(const MeaUnitsMgr&) = delete;

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
    MeaLinearUnits* GetLinearUnits() const override { return m_currentLinearUnits; }

    /// Returns the linear measurement units object corresponding
    /// to the specified identifier string (e.g. "px").
    ///
    /// @param unitsStr     [in] Linear units identifier string.
    ///
    /// @return Linear measurement units object.
    ///
    MeaLinearUnits* GetLinearUnits(const CString& unitsStr) const override {
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
    MeaAngularUnits* GetAngularUnits() const override { return m_currentAngularUnits; }

    /// Returns the angular measurement units object corresponding
    /// to the specified identifier string (e.g. "deg").
    ///
    /// @param unitsStr     [in] Angular units identifier string.
    ///
    /// @return Angular measurement units object.
    ///
    MeaAngularUnits* GetAngularUnits(const CString& unitsStr) const override {
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
        m_linearUnitsMap.at(unitsId)->SetDisplayPrecisions(precisions);
    }

    /// Returns the array of decimal places currently set for each linear measurement
    /// type in the specified units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places for each linear measurement type.
    ///
    const MeaUnits::DisplayPrecisions& GetLinearDisplayPrecisions(MeaLinearUnitsId unitsId) {
        return m_linearUnitsMap.at(unitsId)->GetDisplayPrecisions();
    }

    /// Returns the array of default decimal places for each linear measurement
    /// type in the specified units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places for each linear measurement type.
    ///
    const MeaUnits::DisplayPrecisions& GetLinearDefaultPrecisions(MeaLinearUnitsId unitsId) {
        return m_linearUnitsMap.at(unitsId)->GetDefaultPrecisions();
    }

    /// Set the number of decimal places to show on the data display for angular
    /// measurements in the specified units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are to be set.
    /// @param precisions   [in] Array of decimal places.
    ///
    void SetAngularDisplayPrecisions(MeaAngularUnitsId unitsId, const MeaUnits::DisplayPrecisions& precisions) {
        m_angularUnitsMap.at(unitsId)->SetDisplayPrecisions(precisions);
    }

    /// Returns the array of decimal places currently set for the specified angular
    /// measurement units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places.
    ///
    const MeaUnits::DisplayPrecisions& GetAngularDisplayPrecisions(MeaAngularUnitsId unitsId) {
        return m_angularUnitsMap.at(unitsId)->GetDisplayPrecisions();
    }

    /// Returns the array of default decimal places for the specified angular
    /// measurement units.
    ///
    /// @param unitsId      [in] Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places.
    ///
    const MeaUnits::DisplayPrecisions& GetAngularDefaultPrecisions(MeaAngularUnitsId unitsId) {
        return m_angularUnitsMap.at(unitsId)->GetDefaultPrecisions();
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
    bool IsInvertY() const override { return MeaLinearUnits::IsInvertY(); }

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
    const POINT& GetOrigin() const override { return MeaLinearUnits::GetOrigin(); }

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
    MeaFSize GetWidthHeight(const POINT& p1, const POINT& p2) const override;

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
    CString FormatConvertAngle(double angle) const override {
        return Format(MeaA, ConvertAngle(angle));
    }

    /// Formats the specified linear measurement value for display.
    /// @param id       [in] Identifies the linear measurement for
    ///                 use in determining the display precision.
    /// @param value    [in] Linear measurement value to format,
    ///                 in current units.
    /// @return String containing the formatted measurement data.
    CString Format(MeaLinearMeasurementId id, double value) const override {
        return m_currentLinearUnits->Format(id, value);
    }

    /// Formats the specified angular measurement value for display.
    /// @param id       [in] Identifies the angular measurement for
    ///                 use in determining the display precision.
    /// @param value    [in] Angular measurement value to format,
    ///                 in current units.
    /// @return String containing the formatted measurement data.
    CString Format(MeaAngularMeasurementId id, double value) const override {
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
    MeaFPoint ConvertCoord(const POINT& pos) const override {
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
    POINT UnconvertCoord(const MeaFPoint& pos) const {
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
    MeaFPoint ConvertPos(const POINT& pos) const override {
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
    POINT UnconvertPos(const MeaFPoint& pos) const override {
        return m_currentLinearUnits->UnconvertPos(pos);
    }

    /// Converts the specified resolution in pixels/inch to the desired units.
    ///
    /// @param res      [in] Resolution in pixels/inch to convert to the desired units.
    ///
    /// @return Resolution converted from pixels/inch to the desired units.
    ///
    MeaFSize ConvertRes(const MeaFSize& res) const override {
        return m_currentLinearUnits->ConvertRes(res);
    }

    /// Converts the specified angle value from its native radians
    /// to the desired units.
    ///
    /// @param angle    [in] Value to be converted.
    ///
    /// @return Angle value converted to the desired units.
    ///
    double ConvertAngle(double angle) const override {
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
    SIZE ConvertToPixels(MeaLinearUnitsId id, const MeaFSize& res, double value, int minPixels) const override {
        return ((*m_linearUnitsMap.find(id)).second)->ConvertToPixels(res, value, minPixels);
    }

    /// Returns the number of minor ticks to display before a major tick mark is
    /// displayed on the measurement rulers.
    ///
    /// @return Number of minor tick marks to display before displaying a major
    ///         tick mark.
    ///
    int GetMajorTickCount() const override { return m_majorTickCount; }

    /// Returns the increment between minor tick marks on the measurement rulers.
    ///
    /// @param rect     [in] Rectangle in screen coordinates, used to determine
    ///                 the current screen resolution.
    ///
    /// @return Increment for the minor tick marks, in the current units.
    ///
    MeaFSize GetMinorIncr(const RECT& rect) const override;

private:
    /// Ruler tick increments. The order of magnitude of these values is adjusted based on the units.
    static constexpr double kTickIncrements[] = { 10.0, 25.0, 50.0, 75.0, 100.0 };

    /// Number of tick mark increments in the kTickIncrements array.
    static constexpr int kNumTickIncrements = sizeof(kTickIncrements) / sizeof(*kTickIncrements);

    /// Minimum separation between ruler tick marks, in pixels.
    static constexpr int kMinSepPixels = 5;

    ///< Minimum separation between ruler tick marks, in inches.
    static constexpr double kMinSepInches = 0.1;

    typedef std::map<MeaLinearUnitsId, MeaLinearUnits*> LinearUnitsMap;     ///< Maps linear units identifiers to the corresponding linear units objects.
    typedef std::map<MeaAngularUnitsId, MeaAngularUnits*> AngularUnitsMap;  ///< Maps angular units identifiers to the corresponding angular units objects.
    typedef std::list<MeaLinearUnitsLabel*> LinearLabelsList;               ///< List of all linear units labels.
    typedef std::list<MeaAngularUnitsLabel*> AngularLabelsList;             ///< List of all angular units labels.
    

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
    int m_majorTickCount;                       ///< Number of minor ruler tick marks between major tick marks.
};

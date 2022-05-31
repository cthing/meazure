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
/// @brief Position context information.
 
#pragma once

#include "PositionScreen.h"
#include "PositionLogWriter.h"
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ScreenProvider.h>
#include <meazure/units/Units.h>
#include <meazure/utilities/Geometry.h>
#include <meazure/utilities/GUID.h>


/// Represents all information of interest about the system at the time a position is recorded. This includes the
/// list of all display screens attached to the system. The desktop information is saved in the position log file.
/// To minimize repetition, positions reference the desktop information in effect when they were recorded. This
/// means that one desktop information section can serve many positions.
///
class MeaPositionDesktop {

public:
    /// Constructs a desktop information object.
    /// 
    /// @param unitsProvider   [in] Units information and conversion provider
    /// @param screenProvider  [in] Screen information provider
    ///
    MeaPositionDesktop(const MeaUnitsProvider& unitsProvider, const MeaScreenProvider& screenProvider);

    /// Constructs a desktop information object with the specified unique ID.
    ///
    /// @param guidStr          [in] GUID ID for the desktop information object.
    /// @param unitsProvider    [in] Units information and conversion provider
    /// @param screenProvider   [in] Screen information provider
    ///
    explicit MeaPositionDesktop(LPCTSTR guidStr, const MeaUnitsProvider& unitsProvider,
                                const MeaScreenProvider& screenProvider);

    /// Constructs a desktop information object as a copy of the specified object.
    ///
    /// @param desktop      [in] Desktop information object to copy.
    ///
    MeaPositionDesktop(const MeaPositionDesktop& desktop) { Copy(desktop); }

    /// Destroys a desktop information object.
    ///
    ~MeaPositionDesktop() {}

    /// Assigns the specified desktop information object to this object.
    ///
    /// @param desktop      [in] Desktop information object to be assigned.
    ///
    /// @return This object.
    ///
    MeaPositionDesktop& operator=(const MeaPositionDesktop& desktop) {
        return Copy(desktop);
    }

    /// Returns the linear units in effect when this object was created.
    /// @return Linear units object in effect when this object was created.
    MeaLinearUnits* GetLinearUnits() { return m_linearUnits; }

    /// Returns the linear units in effect when this object was created.
    /// @return Linear units object in effect when this object was created.
    const MeaLinearUnits* GetLinearUnits() const { return m_linearUnits; }

    /// Returns the angular units in effect when this object was created.
    /// @return Angular units object in effect when this object was created.
    MeaAngularUnits* GetAngularUnits() { return m_angularUnits; }

    /// Returns the angular units in effect when this object was created.
    /// @return Angular units object in effect when this object was created.
    const MeaAngularUnits* GetAngularUnits() const { return m_angularUnits; }

    /// Indicates whether the y-axis was inverted when this object was created.
    /// @return <b>true</b> if the y-axis was inverted when this object was created.
    bool GetInvertY() const { return m_invertY; }

    /// Returns the location of the origin when this object was created.
    /// @return The location of the origin when this object was created.
    const FPOINT& GetOrigin() const { return m_origin; }

    /// Returns the unique ID for this object.
    /// @return GUID ID for this object.
    const MeaGUID& GetId() const { return m_id; }

    /// Sets a unique ID for this object.
    /// @param guidStr      [in] GUID ID to set for this object.
    void SetId(LPCTSTR guidStr) { m_id = guidStr; }

    /// Returns the name for custom units.
    /// @return Name for custom units.
    CString GetCustomName() const { return m_customName; }

    /// Returns the abbreviation for custom units.
    /// @return Abbreviation for custom units.
    CString GetCustomAbbrev() const { return m_customAbbrev; }

    /// Returns the units basis for custom units (i.e. pixels, inches or centimeters).
    /// @return The units basis for custom units.
    CString GetCustomBasisStr() const { return m_customBasisStr; }

    /// Returns the conversion factor for custom units.
    /// @return Conversion factor.
    double GetCustomFactor() const { return m_customFactor; }

    /// Returns the set of display precisions for custom units.
    /// @return Set of custom units display precisions.
    MeaUnits::DisplayPrecisions GetCustomPrecisions() const { return m_customPrecisions; }

    /// Loads desktop elements of the log file.
    ///
    /// @param desktopNode      [in] Desktop node in the DOM.
    ///
    void Load(const MeaXMLNode* desktopNode);

    /// Saves the desktop information
    ///
    /// @param writer       [in] Provides ability to write a position to the log.
    /// @param indent       [in] Output indentation level.
    /// @throw CFileException is there was a problem saving the information
    ///
    void Save(MeaPositionLogWriter& writer, int indent) const;

    /// Compares the specified desktop information object with this to determine equality.
    ///
    /// @param desktop      [in] Desktop information object to compare with this.
    ///
    /// @return <b>true</b> if the specified object and this are equal.
    ///
    bool operator==(const MeaPositionDesktop& desktop) const {
        return (MeaNumericUtils::IsFloatingEqual(m_origin.x, desktop.m_origin.x) &&
            MeaNumericUtils::IsFloatingEqual(m_origin.y, desktop.m_origin.y) &&
            (m_invertY == desktop.m_invertY) &&
            MeaNumericUtils::IsFloatingEqual(m_size.cx, desktop.m_size.cx) &&
            MeaNumericUtils::IsFloatingEqual(m_size.cy, desktop.m_size.cy) &&
            (m_linearUnits == desktop.m_linearUnits) &&
            (m_angularUnits == desktop.m_angularUnits) &&
            (m_screens == desktop.m_screens) &&
            (m_customName == desktop.m_customName) &&
            (m_customAbbrev == desktop.m_customAbbrev) &&
            (m_customBasisStr == desktop.m_customBasisStr) &&
            MeaNumericUtils::IsFloatingEqual(m_customFactor, desktop.m_customFactor) &&
            (m_customPrecisions == desktop.m_customPrecisions));
    }

    /// Compares the specified desktop information object with this to determine inequality.
    ///
    /// @param desktop     [in] Desktop information object to compare with this.
    ///
    /// @return <b>true</b> if the specified object and this are not equal.
    ///
    bool operator!=(const MeaPositionDesktop& desktop) const { return !(*this == desktop); }

private:
    typedef std::list<MeaPositionScreen> PositionScreenList;   ///< List of all display screens attached to the system.


    /// Copies the specified desktop information object into this object.
    ///
    /// @param desktop      [in] Desktop information object to copy.
    ///
    /// @return This object.
    ///
    MeaPositionDesktop& Copy(const MeaPositionDesktop& desktop) {
        if (&desktop != this) {
            m_unitsProvider = desktop.m_unitsProvider;
            m_id = desktop.m_id;
            m_origin = desktop.m_origin;
            m_invertY = desktop.m_invertY;
            m_size = desktop.m_size;
            m_linearUnits = desktop.m_linearUnits;
            m_angularUnits = desktop.m_angularUnits;
            m_screens = desktop.m_screens;

            m_customName = desktop.m_customName;
            m_customAbbrev = desktop.m_customAbbrev;
            m_customBasisStr = desktop.m_customBasisStr;
            m_customFactor = desktop.m_customFactor;
            m_customPrecisions = desktop.m_customPrecisions;
        }
        return *this;
    }

    /// Sets the linear units for the object based on the specified units name.
    ///
    /// @param unitsStr     [in] Linear units name.
    ///
    void SetLinearUnits(const CString& unitsStr);

    /// Sets the angular units for the object based on the specified units name.
    ///
    /// @param unitsStr     [in] Angular units name.
    ///
    void SetAngularUnits(const CString& unitsStr);

    /// Loads the display precisions values for the custom units.
    ///
    /// @param displayPrecisionNode     [in] displayPrecisions element in the DOM.
    ///
    void LoadCustomPrecisions(const MeaXMLNode* displayPrecisionNode);

    /// Saves the display precisions for custom units
    ///
    /// @param writer       [in] Provides ability to write a position to the log.
    /// @param indent       [in] Output indentation level.
    /// @throws CFileException if there was a problem saving the information
    ///
    void SaveCustomPrecisions(MeaPositionLogWriter& writer, int indent) const;


    const MeaUnitsProvider* m_unitsProvider;    ///< Units information object
    MeaGUID m_id;                       ///< ID for use by a Position object to reference this object.
    FPOINT m_origin;                    ///< Origin in the units that were in effect when the desktop object was created.
    bool m_invertY;                     ///< Is origin inverted.
    FSIZE m_size;                       ///< Desktop size in units in effect when desktop object created.
    MeaLinearUnits* m_linearUnits;      ///< Current linear units.
    MeaAngularUnits* m_angularUnits;    ///< Current angle units.
    PositionScreenList m_screens;               ///< List of display screens.
    CString m_customName;               ///< Custom units name.
    CString m_customAbbrev;             ///< Custom units abbreviation.
    CString m_customBasisStr;           ///< Custom scale factor basis.
    double m_customFactor;              ///< Custom scale factor.
    MeaUnits::DisplayPrecisions m_customPrecisions; ///< Custom display precisions.
};


/// Interface for a class that implements reference counting of desktop information objects.
///
struct MeaPositionDesktopRefCounter {
    /// Increments the reference count for the specified desktop information object.
    ///
    /// @param id  [in] ID of the desktop info object whose reference count is to be incremented.
    ///
    virtual void AddDesktopRef(const MeaGUID& id) = 0;

    /// Decrements the reference count for the specified desktop information object.
    ///
    /// @param id  [in] ID of the desktop info object whose reference count is to be decremented.
    ///
    virtual void ReleaseDesktopRef(const MeaGUID& id) = 0;
};


/// References a desktop information object. Position objects hold a reference to the desktop on which they
/// were recorded. References to desktop information object are reference counted so that unreferenced
/// desktop information is not written to the log file. This class hides the reference counting details from
/// the position object.
/// 
class MeaPositionDesktopRef {
public:

    /// Constructs a reference to the specified desktop information object.
    /// 
    /// @param counter  [in] Reference count manager
    /// @param desktop  [in] Desktop information object to reference
    /// 
    MeaPositionDesktopRef(MeaPositionDesktopRefCounter* counter, const MeaPositionDesktop& desktop) :
        m_counter(counter), m_id(desktop.GetId()) {
        m_counter->AddDesktopRef(m_id);
    }

    /// Constructs a reference to the specified desktop information object. This constructor is used during
    /// deserialization of the position log file.
    /// 
    /// @param counter  [in] Reference count manager
    /// @param idStr [in] GUID identifier for a desktop information object
    /// 
    MeaPositionDesktopRef(MeaPositionDesktopRefCounter* counter, const CString& idStr) :
        m_counter(counter), m_id(idStr) {
        m_counter->AddDesktopRef(m_id);
    }

    /// Makes a copy of the specified desktop information reference.
    /// 
    /// @param ref  [in] Desktop information object to copy
    /// 
    MeaPositionDesktopRef(const MeaPositionDesktopRef& ref) : m_counter(ref.m_counter), m_id(ref.m_id) {
        m_counter->AddDesktopRef(m_id);
    }

    ~MeaPositionDesktopRef() {
        try {
            m_counter->ReleaseDesktopRef(m_id);
        } catch (...) {
            assert(false);
        }
    }

    MeaPositionDesktopRef& operator=(const MeaPositionDesktopRef& ref) {
        if (&ref != this) {
            m_counter->ReleaseDesktopRef(m_id);

            m_counter = ref.m_counter;
            m_id = ref.m_id;

            m_counter->AddDesktopRef(m_id);
        }

        return *this;
    }

    bool operator==(const MeaPositionDesktopRef& ref) const { return m_id == ref.m_id; }
    bool operator!=(const MeaPositionDesktopRef& ref) const { return m_id != ref.m_id; }

    operator MeaGUID() const { return m_id; }
    operator LPCTSTR() const { return static_cast<LPCTSTR>(m_id); }

private:
    MeaPositionDesktopRefCounter* m_counter;
    MeaGUID m_id;
};

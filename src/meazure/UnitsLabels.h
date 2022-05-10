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

#include <map>
#include "Label.h"
#include "Units.h"



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

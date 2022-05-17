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
/// @brief Display precision property page header file.

#pragma once

#include <meazure/resource.h>
#include <meazure/ui/NumberField.h>
#include <meazure/units/Units.h>
#include <map>


/// Property page for setting display precisions for each set of measurement
/// units. Each measurement unit has associated with it a set of display
/// precisions, one for each measurement (e.g. width, height, x, y).
///
class MeaPrecisionPrefs : public CPropertyPage {
    DECLARE_DYNCREATE(MeaPrecisionPrefs)

public:
    /// Constructs an instance of the precision property page.
    ///
    MeaPrecisionPrefs();

    /// Destroys an instance of the precision property page.
    ///
    ~MeaPrecisionPrefs();

    enum { IDD = IDD_PREF_PRECISION };

    /// Sets the display precisions for the specified linear measurement units.
    ///
    /// @param unitsId      [in] Linear units whose precisions are being set.
    /// @param precisions   [in] Set of display precisions for the specified units.
    ///
    void SetLinearPrecisions(MeaLinearUnitsId unitsId, const MeaUnits::DisplayPrecisions& precisions) {
        m_linearPrecisions[unitsId] = precisions;
    }

    /// Sets the display precisions for the specified angular measurement units.
    ///
    /// @param unitsId      [in] Angular units whose precisions are being set.
    /// @param precisions   [in] Set of display precisions for the specified units.
    ///
    void SetAngularPrecisions(MeaAngularUnitsId unitsId, const MeaUnits::DisplayPrecisions& precisions) {
        m_angularPrecisions[unitsId] = precisions;
    }

    /// Obtains the display precisions for the specified linear measurement units.
    ///
    /// @param unitsId      [in] Linear units whose precisions are to be obtained.
    ///
    /// @return Set of display precisions for the specified units.
    ///
    const MeaUnits::DisplayPrecisions& GetLinearPrecisions(MeaLinearUnitsId unitsId) {
        return m_linearPrecisions[unitsId];
    }

    /// Obtains the display precisions for the specified angular measurement units.
    ///
    /// @param unitsId      [in] Angular units whose precisions are to be obtained.
    ///
    /// @return Set of display precisions for the specified units.
    ///
    const MeaUnits::DisplayPrecisions& GetAngularPrecisions(MeaAngularUnitsId unitsId) {
        return m_angularPrecisions[unitsId];
    }

    /// Called by the custom units property page when the display precision button on
    /// that page is pressed. This method selects the custom units display precisions.
    ///
    void SelectCustomUnits();

    /// Called when the property page is created. This method
    /// subclasses a number of controls on the page, with our
    /// implementations.
    ///
    /// @return TRUE, indicating that the input focus should be
    ///         set to the first control on the page.
    ///
    virtual BOOL OnInitDialog();

protected:
    /// Performs Dynamic Data Exchange (DDX) for the page.
    ///
    /// @param pDX  [in] DDX object for exchanging data between the page UI
    ///             and the member variables.
    /// 
    virtual void DoDataExchange(CDataExchange* pDX) override;

    /// Called when the centimeter radio button is pressed.
    afx_msg void OnCm();

    /// Called when the custom units radio button is pressed.
    afx_msg void OnCustom();

    /// Called when the degrees radio button is pressed.
    afx_msg void OnDeg();

    /// Called when the inches radio button is pressed.
    afx_msg void OnIn();

    /// Called when the millimeters radio button is pressed.
    afx_msg void OnMm();

    /// Called when the picas radio button is pressed.
    afx_msg void OnPc();

    /// Called when the points radio button is pressed.
    afx_msg void OnPt();

    /// Called when the pixels radio button is pressed.
    afx_msg void OnPx();

    /// Called when the radians radio button is pressed.
    afx_msg void OnRad();

    /// Called when the twips radio button is pressed.
    afx_msg void OnTp();

    /// Called when the input focus is lost from the angle text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusA();

    /// Called when the input focus is lost from the area text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusAr();

    /// Called when the input focus is lost from the distance text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusD();

    /// Called when the input focus is lost from the height text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusH();

    /// Called when the input focus is lost from the x-axis resolution text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusRx();

    /// Called when the input focus is lost from the y-axis resolution text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusRy();

    /// Called when the input focus is lost from the width text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusW();

    /// Called when the input focus is lost from the x position text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusX();

    /// Called when the input focus is lost from the y position text field.
    /// When the input focus is lost, the value in the text field is set.
    /// 
    afx_msg void OnKillFocusY();

    /// Called when the Default button is pressed.
    /// 
    afx_msg void OnDefault();

    /// Called when any display precision values are changed so that the
    /// modified flag can be set.
    /// 
    afx_msg void OnChange();

    DECLARE_MESSAGE_MAP()

private:
    /// Maps a measurement ID to a label control.
    typedef std::map<int, CStatic*> Labels;

    /// Maps a measurement ID to a numeric text field.
    typedef std::map<int, MeaNumberField*> Fields;

    /// Maps a measurement ID to the spin control on a text field.
    typedef std::map<int, CSpinButtonCtrl*> Spins;

    /// Maps a measurement ID to a set of display precisions.
    typedef std::map<int, MeaUnits::DisplayPrecisions> Precisions;


    static constexpr int kMaxPlaces { 6 };        ///< Maximum allowable number of decimal places


    /// Purposely undefined.
    ///
    MeaPrecisionPrefs(const MeaPrecisionPrefs&);

    /// Purposely undefined.
    ///
    MeaPrecisionPrefs& operator=(const MeaPrecisionPrefs&);

    /// Ensures radio button behavior among the units radio buttons.
    /// That is, when the specified button is set, all other buttons
    /// in the radio group are unset.
    ///
    /// @param buttonId     [in] Radio button to set.
    ///
    void SetCheck(UINT buttonId) const;

    /// Enables the linear units controls, disables the angular units
    /// controls, and displays the precision values for the currently
    /// selected linear units.
    ///
    void ShowLinearPrecisions();

    /// Enables the angular units controls, disables the linear units
    /// controls, and displays the precision values for the currently
    /// selected angular units.
    ///
    void ShowAngularPrecisions();

    /// Enables or disables the linear units controls.
    ///
    void EnableLinearDisplay(bool enable);

    /// Enables or disables the angular units controls.
    ///
    void EnableAngularDisplay(bool enable);

    /// Redisplays the display precisions for the currently selected
    /// linear units.
    ///
    void UpdateLinearDisplay();

    /// Redisplays the display precisions for the currently selected
    /// angular units.
    ///
    void UpdateAngularDisplay();

    /// Obtains the value in the specified linear measurement text field,
    /// validates it, and returns it.
    ///
    /// @param id   [in] ID of the linear measurement precision text field.
    ///
    /// @return Value from the text field.
    ///
    int GetLinearValue(MeaLinearMeasurementId id) {
        return GetValue(m_linearFields[id]);
    }

    /// Obtains the value in the specified angular measurement text field,
    /// validates it, and returns it.
    ///
    /// @param id   [in] ID of the angular measurement precision text field.
    ///
    /// @return Value from the text field.
    ///
    int GetAngularValue(MeaAngularMeasurementId id) {
        return GetValue(m_angularFields[id]);
    }

    /// Obtains the value in the specified measurement text field,
    /// validates it, and returns it.
    ///
    /// @param field    [in] Measurement precision text field.
    ///
    /// @return Value from the text field.
    ///
    int GetValue(MeaNumberField* field);


    Precisions m_linearPrecisions;          ///< Precisions for all linear measurements.
    Precisions m_angularPrecisions;         ///< Precisions for all angular measurements.
    Labels m_linearLabels;                  ///< Labels for all linear measurements.
    Labels m_angularLabels;                 ///< Labels for all angular measurements.
    Fields m_linearFields;                  ///< Text fields for all linear measurements.
    Fields m_angularFields;                 ///< Text fields for all angular measurements.
    Spins m_linearSpins;                    ///< Spin controls for all linear measurements.
    Spins m_angularSpins;                   ///< Spin controls for all angular measurements.
    MeaLinearUnitsId m_curLinearUnitsId;    ///< ID of the currently selected linear units.
    MeaAngularUnitsId m_curAngularUnitsId;  ///< ID of the currently selected angular units.
    bool m_lockout;                         ///< Prevents circular notification of text field changes.
};

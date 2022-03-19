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

#include "NumberField.h"
#include "Units.h"
#include <map>


/// Property page for setting display precisions for each set of measurement
/// units. Each measurement unit has associated with it a set of display
/// precisions, one for each measurement (e.g. width, height, x, y).
///
class MeaPrecisionPrefs : public CPropertyPage
{
    DECLARE_DYNCREATE(MeaPrecisionPrefs)

public:
    /// Constructs an instance of the precision property page.
    ///
    MeaPrecisionPrefs();

    /// Destroys an instance of the precision property page.
    ///
    ~MeaPrecisionPrefs();


    //{{AFX_DATA(MeaPrecisionPrefs)
    enum { IDD = IDD_PREF_PRECISION };
        // NOTE - ClassWizard will add data members here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DATA


    /// Sets the display precisions for the specified linear measurement units.
    ///
    /// @param unitsId      [in] Linear units whose precisions are being set.
    /// @param precisions   [in] Set of display precisions for the specified units.
    ///
    void    SetLinearPrecisions(MeaLinearUnitsId unitsId, const MeaUnits::DisplayPrecisions& precisions) {
        m_linearPrecisions[unitsId] = precisions;
    }

    /// Sets the display precisions for the specified angular measurement units.
    ///
    /// @param unitsId      [in] Angular units whose precisions are being set.
    /// @param precisions   [in] Set of display precisions for the specified units.
    ///
    void    SetAngularPrecisions(MeaAngularUnitsId unitsId, const MeaUnits::DisplayPrecisions& precisions) {
        m_angularPrecisions[unitsId] = precisions;
    }

    /// Obtains the display precisions for the specified linear measurement units.
    ///
    /// @param unitsId      [in] Linear units whose precisions are to be obtained.
    ///
    /// @return Set of display precisions for the specified units.
    ///
    const MeaUnits::DisplayPrecisions&  GetLinearPrecisions(MeaLinearUnitsId unitsId) {
        return m_linearPrecisions[unitsId];
    }

    /// Obtains the display precisions for the specified angular measurement units.
    ///
    /// @param unitsId      [in] Angular units whose precisions are to be obtained.
    ///
    /// @return Set of display precisions for the specified units.
    ///
    const MeaUnits::DisplayPrecisions&  GetAngularPrecisions(MeaAngularUnitsId unitsId) {
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

    //{{AFX_VIRTUAL(MeaPrecisionPrefs)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    /// @fn DoDataExchange(CDataExchange* pDX)
    /// Performs Dynamic Data Exchange (DDX) for the page.
    ///
    /// @param pDX  [in] DDX object for exchanging data between the page UI
    ///             and the member variables.

protected:
    //{{AFX_MSG(MeaPrecisionPrefs)
    afx_msg void OnCm();
    afx_msg void OnCustom();
    afx_msg void OnDeg();
    afx_msg void OnIn();
    afx_msg void OnMm();
    afx_msg void OnPc();
    afx_msg void OnPt();
    afx_msg void OnPx();
    afx_msg void OnRad();
    afx_msg void OnTp();
    afx_msg void OnKillFocusA();
    afx_msg void OnKillFocusAr();
    afx_msg void OnKillFocusD();
    afx_msg void OnKillFocusH();
    afx_msg void OnKillFocusRx();
    afx_msg void OnKillFocusRy();
    afx_msg void OnKillFocusW();
    afx_msg void OnKillFocusX();
    afx_msg void OnKillFocusY();
    afx_msg void OnDefault();
    afx_msg void OnChange();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    /// @fn OnCm()
    /// Called when the centimeter radio button is pressed.

    /// @fn OnCustom()
    /// Called when the custom units radio button is pressed.

    /// @fn OnDeg()
    /// Called when the degrees radio button is pressed.

    /// @fn OnIn()
    /// Called when the inches radio button is pressed.

    /// @fn OnMm()
    /// Called when the millimeters radio button is pressed.

    /// @fn OnPc()
    /// Called when the picas radio button is pressed.

    /// @fn OnPt()
    /// Called when the points radio button is pressed.

    /// @fn OnPx()
    /// Called when the pixels radio button is pressed.

    /// @fn OnRad()
    /// Called when the radians radio button is pressed.

    /// @fn OnTp()
    /// Called when the twips radio button is pressed.

    /// @fn OnKillFocusA()
    /// Called when the input focus is lost from the angle text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusAr()
    /// Called when the input focus is lost from the area text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusD()
    /// Called when the input focus is lost from the distance text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusH()
    /// Called when the input focus is lost from the height text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusRx()
    /// Called when the input focus is lost from the x-axis resolution text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusRy()
    /// Called when the input focus is lost from the y-axis resolution text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusW()
    /// Called when the input focus is lost from the width text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusX()
    /// Called when the input focus is lost from the x position text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnKillFocusY()
    /// Called when the input focus is lost from the y position text field.
    /// When the input focus is lost, the value in the text field is set.

    /// @fn OnDefault()
    /// Called when the Default button is pressed.

    /// @fn OnChange()
    /// Called when any display precision values are changed so that the
    /// modified flag can be set.

private:
    /// Maps a measurement ID to a label control.
    typedef std::map<int, CStatic*>                     Labels;

    /// Maps a measurement ID to a numeric text field.
    typedef std::map<int, MeaNumberField*>              Fields;

    /// Maps a measurement ID to the spin control on a text field.
    typedef std::map<int, CSpinButtonCtrl*>             Spins;

    /// Maps a measurement ID to a set of display precisions.
    typedef std::map<int, MeaUnits::DisplayPrecisions>  Precisions;


    /// Purposely undefined.
    ///
    MeaPrecisionPrefs(const MeaPrecisionPrefs&);

    /// Purposely undefined.
    ///
    MeaPrecisionPrefs& operator=(const MeaPrecisionPrefs&);


    static const int kMaxPlaces;        ///< Maximum allowable number of decimal places


    /// Ensures radio button behavior among the units radio buttons.
    /// That is, when the specified button is set, all other buttons
    /// in the radio group are unset.
    ///
    /// @param buttonId     [in] Radio button to set.
    ///
    void    SetCheck(UINT buttonId) const;


    /// Enables the linear units controls, disables the angular units
    /// controls, and displays the precision values for the currently
    /// selected linear units.
    ///
    void    ShowLinearPrecisions();

    /// Enables the angular units controls, disables the linear units
    /// controls, and displays the precision values for the currently
    /// selected angular units.
    ///
    void    ShowAngularPrecisions();

    
    /// Enables or disables the linear units controls.
    ///
    void    EnableLinearDisplay(bool enable);

    /// Enables or disables the angular units controls.
    ///
    void    EnableAngularDisplay(bool enable);


    /// Redisplays the display precisions for the currently selected
    /// linear units.
    ///
    void    UpdateLinearDisplay();

    /// Redisplays the display precisions for the currently selected
    /// angular units.
    ///
    void    UpdateAngularDisplay();


    /// Obtains the value in the specified linear measurement text field,
    /// validates it, and returns it.
    ///
    /// @param id   [in] ID of the linear measurement precision text field.
    ///
    /// @return Value from the text field.
    ///
    int     GetLinearValue(MeaLinearMeasurementId id) {
        return GetValue(m_linearFields[id]);
    }

    /// Obtains the value in the specified angular measurement text field,
    /// validates it, and returns it.
    ///
    /// @param id   [in] ID of the angular measurement precision text field.
    ///
    /// @return Value from the text field.
    ///
    int     GetAngularValue(MeaAngularMeasurementId id) {
        return GetValue(m_angularFields[id]);
    }

    /// Obtains the value in the specified measurement text field,
    /// validates it, and returns it.
    ///
    /// @param field    [in] Measurement precision text field.
    ///
    /// @return Value from the text field.
    ///
    int     GetValue(MeaNumberField* field);


    Precisions  m_linearPrecisions;         ///< Precisions for all linear measurements.
    Precisions  m_angularPrecisions;        ///< Precisions for all angular measurements.

    Labels      m_linearLabels;             ///< Labels for all linear measurements.
    Labels      m_angularLabels;            ///< Labels for all angular measurements.

    Fields      m_linearFields;             ///< Text fields for all linear measurements.
    Fields      m_angularFields;            ///< Text fields for all angular measurements.

    Spins       m_linearSpins;              ///< Spin controls for all linear measurements.
    Spins       m_angularSpins;             ///< Spin controls for all angular measurements.

    MeaLinearUnitsId    m_curLinearUnitsId;     ///< ID of the currently selected linear units.
    MeaAngularUnitsId   m_curAngularUnitsId;    ///< ID of the currently selected angular units.

    bool        m_lockout;      ///< Prevents circular notification of text field changes.
};


//{{AFX_INSERT_LOCATION}}

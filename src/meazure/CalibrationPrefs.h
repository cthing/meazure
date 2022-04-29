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
/// @brief Calibration preferences property page header file.

#pragma once

#include "MeaAssert.h"
#include "NumberField.h"
#include "RulerSlider.h"
#include "ScreenMgr.h"
#include <map>


/// Property page for screen resolution calibration preferences. Using this
/// property page, the screen resolution can be calibrated using a number of
/// different techniques including direct measurement of the screen.
///
class MeaCalibrationPrefs : public CPropertyPage {

public:
    /// Indicates how the screen resolution is determined.
    ///
    enum ResolutionMode {
        AutoRes = 0,        ///< The operating system is supplying the screen resolution.
        ManualRes = 1       ///< The screen resolution is supplied by the user.
    };

    /// Indicates which unit system was used to manually calibrate the
    /// screen resolution.
    ///
    enum ResolutionUnits {
        UseInches = 0,      ///< The resolution was calibrated using inches.
        UseCentimeters = 1  ///< The resolution was calibrated using centimeters.
    };


    /// This structure represents the characteristics of a screen. There
    /// can be multiple screens connected to a single machine, each with
    /// its own size, resolution can calibration mode.
    struct Screen {
        int m_resMode;      ///< Indicates whether calibration for the screen resolution is manual or automatic.
        int m_resUnits;     ///< If manual resolution calibration, indicates if calibration in inches or centimeters.
        FSIZE m_res;        ///< Screen resolution.
        FSIZE m_size;       ///< Screen size, in inches or centimeters.
    };


    /// Constructs a calibration property page instance.
    ///
    MeaCalibrationPrefs();

    /// Destroys a calibration property page instance.
    ///
    ~MeaCalibrationPrefs();

    /// Called when the property page is created. This method
    /// subclasses a number of controls on the page, with our
    /// implementations.
    ///
    /// @return TRUE, indicating that the input focus should be
    ///         set to the first control on the page.
    ///
    virtual BOOL OnInitDialog() override;

    /// Called when the page becomes active. This method ensures
    /// the calibration information reflects the screen on which
    /// the page is being displayed.
    ///
    /// @return TRUE, indicating that the page is active.
    ///
    virtual BOOL OnSetActive() override;

    /// Called when the page becomes inactive. The data on the page
    /// is validated.
    ///
    /// @return TRUE if the data was valid and the page can become
    ///         inactive. FALSE if the data is not valid and the page
    ///         must remain active.
    ///
    virtual BOOL OnKillActive() override;

    /// Called by the property sheet dialog when it is dragged. This method
    /// updates the UI to reflect the current screen's resolution calibration
    /// information.
    ///
    /// @param x    [in] x coordinate of the upper left corner of the dialog window in screen coordinates.
    /// @param y    [in] y coordinate of the upper left corner of the dialog window in screen coordinates.
    ///
    afx_msg void OnMove(int x, int y);

    /// Overrides the base class UpdateData method. The base class asserts if
    /// a recursive call to UpdateData is attempted. In case there is a recursive
    /// call, we don't want to assert, we just want to ignore the call.
    /// See CWnd::UpdateData for an explanation of the parameter and return value.
    ///
    BOOL UpdateData(BOOL bSaveAndValidate = TRUE);

    /// Maps iterators pointing to the screens attached to the system, to
    /// Screen information structures that describe the screen characteristics.
    ///
    typedef std::map<MeaScreenMgr::ScreenIter, Screen, MeaScreenMgr::less> ScreenMap;

    ScreenMap m_screens;    ///< Map of screen iterators to screen information structures.

protected:
    /// Called when the operating system resolution is selected.
    ///
    afx_msg void OnAutoRes();

    /// Called when manual resolution calibration is selected.
    ///
    afx_msg void OnManRes();

    /// Called when manual resolution calibration using inches is selected.
    ///
    afx_msg void OnCalInches();

    /// Called when manual resolution calibration using centimeters is selected.
    ///
    afx_msg void OnCalCm();

    /// Called whenever any value is changed by the UI. This method sets the
    /// page modified flag.
    ///
    afx_msg void OnChange();

    /// Called when the value is changed in the x-axis resolution text field.
    ///
    afx_msg void OnRxFieldChange();

    /// Called when the value is changed in the y-axis resolution text field.
    ///
    afx_msg void OnRyFieldChange();

    /// Called when the value is changed in the screen width text field.
    ///
    afx_msg void OnWFieldChange();

    /// Called when the value is changed in the screen height text field.
    ///
    afx_msg void OnHFieldChange();

    DECLARE_MESSAGE_MAP()

    /// Performs Dynamic Data Exchange (DDX) for the page.
    ///
    /// @param pDX  [in] DDX object for exchanging data between the page UI
    ///             and the member variables.
    ///
    virtual void DoDataExchange(CDataExchange* pDX) override;

private:
    /// Sets the resolution calibration mode (i.e. automatic or manual).
    ///
    /// @param resMode      [in] Resolution mode (i.e. automatic or manual).
    ///
    void SetResMode(ResolutionMode resMode);

    /// For manual resolution calibration, sets whether the calibration is
    /// performed in inches or centimeters.
    ///
    /// @param resUnits     [in] Units used for manual resolution calibration.
    ///
    void SetResUnits(ResolutionUnits resUnits);

    /// Returns the numeric value in the specified text field.
    ///
    /// @param fieldId      [in] Identifier of the text field.
    /// @param value        [out] Numerical value in the text field.
    ///
    /// @return <b>true</b> if a numerical value greater than zero was
    ///         read and that value differs from that passed in.
    ///
    bool GetFieldValue(UINT fieldId, double& value);

    /// Updates the position of the resolution calibration sliders
    /// based on the current resolution values.
    ///
    void UpdateSliders();

    /// Updates the position of the specified slider based on the
    /// specified resolution.
    ///
    /// @param sliderId     [in] ID of the slider to update.
    /// @param res          [in] Resolution value, in pixels/inch.
    ///
    void UpdateSlider(UINT sliderId, double res);

    /// Same as the DDX UpdateData method except the specified UI
    /// object is not updated. This prevents cyclic updating.
    ///
    /// @param lockoutId        [in] ID of UI object that should not be updated.
    /// @param bSaveAndValidate [in] Indicates whether the UI should be saved to the member variables.
    ///
    /// @return <b>true</b> if data update was successful.
    ///
    bool LockedUpdateData(UINT lockoutId, bool bSaveAndValidate = true);

    /// Indicates whether manual resolution calibration is being done in metrics units.
    ///
    /// @return <b>true</b> if the calibration is being done in centimeters.
    ///
    bool IsMetric() { return GetScreen().m_resUnits == UseCentimeters; }

    /// Performs DDX handling for the resolution text fields. Internally
    /// resolution values are maintained in pixels/inch. If calibration is
    /// being done using centimeters, this method converts to/from inches.
    ///
    /// @param pDX      [in] DDX object for exchanging data between the page UI
    ///                 and the member variables.
    /// @param nIDC     [in] ID of the UI control.
    /// @param resValue [in, out] Resolution in either pixels/inch or pixels/centimeter.
    ///
    void DDX_ResText(CDataExchange* pDX, int nIDC, double& resValue);

    /// Performs DDX handling for the screen dimension text fields. Internally
    /// dimension values are maintained in inches. If calibration is being done
    /// using centimeters, this method converts to/from inches.
    ///
    /// @param pDX      [in] DDX object for exchanging data between the page UI
    ///                 and the member variables.
    /// @param nIDC     [in] ID of the UI control.
    /// @param dimValue [in, out] Dimensions in either inches or centimeters.
    ///
    void DDX_DimText(CDataExchange* pDX, int nIDC, double& dimValue);

    /// Performs data validation on all appropriate text field values. For these
    /// text fields, a value is considered valid if it is greater than zero.
    ///
    /// @return <b>true</b> if all appropriate field values are valid.
    ///
    bool Validate();

    /// Performs data validation on the specified text field. For these test
    /// fields, a value is considered valid if it is greater than zero.
    ///
    /// @param id   [in] ID of the text field whose value is to be validated.
    ///
    /// @return <b>true</b> if the field value is valid.
    ///
    bool ValidateField(UINT id);


    /// Updates the display of all data. This includes updating the text field
    /// values and slider positions.
    ///
    void UpdateView();

    /// Determines the UI based on the current screen information. The information
    /// displayed on this page is dependent on the screen on which it is shown.
    ///
    void UpdateCurrentScreen();

    /// Called when the position of one of the calibration sliders is changed.
    ///
    /// @param pos      [in] New position of the slider. This is the resolution
    ///                 either in pixels/inch or 2 * pixels/centimeter.
    /// @param id       [in] ID of the slider that has been changed.
    ///
    /// @return TRUE to indicate successful processing.
    ///
    LRESULT OnCaliperChange(WPARAM pos, LPARAM id);

    /// Obtains the screen information for the current screen (i.e. the screen
    /// on which the page is located).
    ///
    /// @return Screen information structure.
    ///
    Screen& GetScreen() {
        ScreenMap::iterator citer = m_screens.find(m_currentIter);
        MeaAssert(citer != m_screens.end());
        return (*citer).second;
    }


    MeaNumberField m_rxField;       ///< Numeric text field for the x-axis resolution.
    MeaNumberField m_ryField;       ///< Numeric text field for the y-axis resolution.
    MeaNumberField m_wField;        ///< Numeric text field for the screen width dimension.
    MeaNumberField m_hField;        ///< Numeric text field for the screen height dimension.
    MeaRulerSlider m_rxSlider;      ///< Calibration slider for the x-axis resolution.
    MeaRulerSlider m_rySlider;      ///< Calibration slider for the y-axis resolution.
    UINT m_lockoutId;               ///< ID of the UI object that should not be updated during DDX.
    bool m_inDDX;                   ///< Indicates if currently in DoDataExchange method
    MeaScreenMgr::ScreenIter m_currentIter;     ///< Current screen.
};

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
/// @brief Header file for the data display portion of the application GUI.

#pragma once

#include "NumberField.h"
#include "Label.h"
#include "Messages.h"
#include "Profile.h"
#include "Themes.h"
#include "ImageButton.h"
#include "UnitsMgr.h"
#include <set>


/// Each data text field on the display is specified by one of
/// these identifiers. Internally each identifier is used as a
/// mask and as such, must be consecutive powers of 2.
///
enum MeaFields {
    MeaX1Field = 0x0001,            ///< X1 coordinate data field.
    MeaY1Field = 0x0002,            ///< Y1 coordinate data field.
    MeaX2Field = 0x0004,            ///< X2 coordinate data field.
    MeaY2Field = 0x0008,            ///< Y2 coordinate data field.
    MeaXVField = 0x0010,            ///< Vertex or center X coordinate data field.
    MeaYVField = 0x0020,            ///< Vertex or center Y coordinate data field.
    MeaWidthField = 0x0040,         ///< Width data field.
    MeaHeightField = 0x0080,        ///< Height data field.
    MeaDistanceField = 0x0100,      ///< Distance data field.
    MeaAngleField = 0x0200,         ///< Angle data field.
    MeaAreaField = 0x0400,          ///< Area data field.
    MeaScreenWidthField = 0x0800,   ///< Screen width data field.
    MeaScreenHeightField = 0x1000,  ///< Screen height data field.
    MeaResXField = 0x2000,          ///< Data field for the screen resolution in the X dimension.
    MeaResYField = 0x4000,          ///< Data field for the screen resolution in the Y dimension.
    MeaAspectField = 0x8000         ///< Aspect ratio field.
};

/// The data display is divided into two major sections: Region and Screen.
/// The Region section displays the measurement data from the radio measurement
/// tools (e.g. Point tool, Line Tool). The Screen section displays data about
/// the current monitor (e.g. resolution, width, height). These identifiers are
/// used to identify a data display section for enabling and disabling purposes.
enum MeaSection {
    MeaRegionSection,       ///< Measurement tool data display section.
    MeaScreenSection        ///< Screen data display section.
};


/// The data display is divided into two major sections: Region and Screen.
/// The Region section displays the measurement data from the radio measurement
/// tools (e.g. Point tool, Line Tool). The Screen section displays data about
/// the current monitor (e.g. resolution, width, height).
///
/// This class is used to represent each section.
///
class MeaDataSection : public CButton {

protected:
    /// Called when the Enter key is pressed on one of the text fields.
    ///
    /// @param wParam   [in] 0, 1, -1 as increment.
    /// @param id       [in] ID of the text field.
    ///
    /// @return Always returns TRUE.
    ///
    afx_msg LRESULT OnFieldEntry(WPARAM wParam, LPARAM id);

    /// Called when spin control UDN_DELTAPOS messages are sent.
    ///
    /// @param wParam       [in] Control ID
    /// @param lParam       [in] Pointer to NMHDR structure.
    /// @param pResult      [out] TRUE indicating that the message has been handled
    ///                     and the spin control does not need to process the message.
    ///
    /// @return Always returns TRUE indicating that the message has been handled.
    ///
    afx_msg BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

    /// Called when the calibration warning button is pressed.
    /// The calibration preference panel is displayed.
    ///
    afx_msg void OnCalButton();

    DECLARE_MESSAGE_MAP()
};



/// This class is responsible for the display of all measurement data.
///
/// The data display is divided into two major sections: Region and Screen.
/// The Region section displays the measurement data from the radio measurement
/// tools (e.g. Point tool, Line Tool). The Screen section displays data about
/// the current monitor (e.g. resolution, width, height).
///
/// Within each section are a set of text fields, which display the measurement
/// data. Depending on the measurement tool being used, certain text fields are
/// editable and offer up/down spin controls for fine adjustment of tool crosshair
/// positions.
///
class MeaDataDisplay : public CWnd {

public:
    /// Constructs the data display component. To complete the construction
    /// of the component, the Create method must be called.
    ///
    MeaDataDisplay();

    /// Destroys the data display component.
    ///
    virtual ~MeaDataDisplay();

    /// Creates the window for the data display.
    ///
    /// @param topLeft      [in] Position of the top, left corner of the display.
    /// @param parentWnd    [in] Parent window for the display.
    ///
    /// @return <b>true</b> if successful; false otherwise.
    ///
    bool Create(const POINT& topLeft, CWnd* parentWnd);

    /// Sets the label for the region frame.
    ///
    /// @param labelId      [in] String resource ID for the region frame label.
    ///
    void SetRegionLabel(UINT labelId) {
        CString str;
        str.LoadString(labelId);
        m_regionSection.SetWindowText(str);
    }

    /// Controls the display of the resolution calibration button. Typically, the
    /// screen resolution reported by the operating system is inaccurate. Because
    /// of this inaccuracy, if the operating system resolution is being used for
    /// measurements, a warning button is displayed on the Screen region of the
    /// data display. Pressing the button displays the resolution calibration
    /// preference page. If a manually calibrated resolution is being used, the
    /// calibration warning button is not displayed.
    ///
    /// @param show     [in] Specified whether the calibration warning button should
    ///                 be shown or hidden.
    ///
    void ShowCalButton(bool show) {
        if (::IsWindow(m_calBtn)) {
            m_calBtn.ShowWindow(show ? SW_SHOW : SW_HIDE);
            Invalidate(TRUE);
            UpdateWindow();
        }
    }

    /// This method specifies which Region section text field should be enabled
    /// and, in addition, which fields should be editable. The fields are specified
    /// as an OR'd set of MeaFields identifiers. Fields that are not specified are
    /// disabled.
    ///
    /// @param enableFields     [in] OR'd set of field identifiers for the fields
    ///                         to enable.
    /// @param editableFields   [in] OR'd set of field identifiers for the fields
    ///                         to make editable.
    ///
    void EnableRegionFields(UINT enableFields, UINT editableFields);

    /// This method specifies which Screen section text field should be enabled
    /// and, in addition, which fields should be editable. The fields are specified
    /// as an OR'd set of MeaFields identifiers. Fields that are not specified are
    /// disabled.
    ///
    /// @param enableFields     [in] OR'd set of field identifiers for the fields
    ///                         to enable.
    /// @param editableFields   [in] OR'd set of field identifiers for the fields
    ///                         to make editable.
    ///
    void EnableScreenFields(UINT enableFields, UINT editableFields);

    /// Erases the contents of all region data fields.
    ///
    void ClearRegionData();

    /// Enables the specified data display section.
    ///
    /// @param section      [in] Data display section to enable.
    ///
    void Enable(MeaSection section);

    /// Disables the specified data display section.
    ///
    /// @param section      [in] Data display section to enable.
    ///
    void Disable(MeaSection section);

    /// Indicates whether the specified data display section is enabled.
    ///
    /// @return <b>true</b> if the specified data display section is enabled.
    ///
    bool IsEnabled(MeaSection section) const {
        return ((section == MeaRegionSection) ? m_regionSectionEnabled : m_screenSectionEnabled);
    }

    /// Returns the height of the specified data display section.
    ///
    /// @param section      [in] Data display section whose height is desired.
    ///
    /// @return Height of the specified data display section, in pixels.
    ///
    int GetHeight(MeaSection section) const {
        return ((section == MeaRegionSection) ?
            m_regionSectionRect.Height() : m_screenSectionRect.Height() + kSectionSpacing);
    }

    /// Displays the specified X1, Y1 coordinates.
    /// @param point    [in] Coordinates, in pixels.
    /// @param cpoint   [in] Coordinates, in current units.
    /// 
    void ShowXY1(const POINT& point, const FPOINT& cpoint);

    /// Displays the specified X2, Y2 coordinates.
    /// @param point    [in] Coordinates, in pixels.
    /// @param cpoint   [in] Coordinates, in current units.
    /// 
    void ShowXY2(const POINT& point, const FPOINT& cpoint);

    /// Displays the specified vertex or center X, Y coordinates.
    /// @param point    [in] Coordinates, in pixels.
    /// @param cpoint   [in] Coordinates, in current units.
    /// 
    void ShowXYV(const POINT& point, const FPOINT& cpoint);

    /// Displays the specified width and height.
    /// @param size     [in] Width and height, in current units.
    /// 
    void ShowWH(const FSIZE& size);

    /// Displays a diagonal distance calculated from the specified width and height.
    /// @param size     [in] Width and height, in current units.
    /// 
    void ShowDistance(const FSIZE& size);

    /// Displays the specified distance.
    /// @param dist     [in] Distance, in current units.
    /// 
    void ShowDistance(double dist);

    /// Displays the specified angle.
    /// @param angle    [in] Angle, in current angular units.
    /// 
    void ShowAngle(double angle);

    /// Displays the aspect ratio of the specified width and height (i.e. width/height).
    /// @param size    [in] Width and height, in current units.
    /// 
    void ShowAspect(const FSIZE& size);

    /// Displays the rectangular area calculated from the specified width and height.
    /// @param size     [in] Width and height, in current units.
    /// 
    void ShowRectArea(const FSIZE& size);

    /// Displays the circular area calculated from the specified radius.
    /// @param radius   [in] Radius, in current units.
    /// 
    void ShowCircleArea(double radius);

    /// Displays the specified string as the name for the current monitor.
    /// This helps identify the screen information in multiple monitor
    /// environments.
    /// @param name     [in] Name for the current monitor.
    /// 
    void ShowScreenName(const CString& name);

    /// Displays the specified screen width and height.
    /// @param size     [in] Screen width and height, in the current units.
    /// 
    void ShowScreenWH(const FSIZE& size);

    /// Displays the specified screen resolution.
    /// @param res      [in] Screen resolution in the X and Y dimensions, in the current units.
    /// 
    void ShowScreenRes(const FSIZE& res);

    ///If one of the enabled text fields has the focus, a pointer
    /// to that window is returned.
    ///
    /// @return Pointer to enabled field if it has the focus and <b>nullptr</b>
    ///         otherwise.
    ///
    MeaNumberField* GetFieldFocus() const;

protected:
    /// Called when the Enter key is pressed on one of the text fields.
    ///
    /// @param wParam       [in] 0, 1, -1 as increment.
    /// @param id           [in] ID of the text field.
    ///
    /// @return Always returns TRUE.
    ///
    afx_msg LRESULT OnFieldEntry(WPARAM wParam, LPARAM id);

    /// Determines where the help cursor was clicked and provides
    /// the appropriate ID.
    ///
    /// @param lparam       [in] Pointer coordinates.
    ///
    /// @return Help ID corresponding to section where mouse clicked.
    ///
    afx_msg LRESULT OnHelpHitTest(WPARAM, LPARAM lparam);

    /// Called when spin control UDN_DELTAPOS messages are sent.
    ///
    /// @param wParam       [in] Control ID.
    /// @param lParam       [in] Pointer to NMHDR structure.
    /// @param pResult      [in] FALSE to allow the spin control to change
    ///                     its position or TRUE to indicate that the message
    ///                     has already been processed.
    ///
    /// @return TRUE if the message has been processed or FALSE if the
    ///         spin control needs to process the message.
    ///
    afx_msg BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()

private:
    /// Each data display field is actually composed of a label, a text field,
    /// possibly a spin control, and usually a units label. This class packages
    /// all these pieces into a single data item component. This eases the
    /// burden and reduces the potential for code duplication for the data
    /// display class.
    ///
    class DataItem {

    public:
        /// Constructs a data item. To complete the data item for use, the
        /// appropriate set of Create* methods must be called.
        ///
        DataItem() : m_spin(nullptr), m_unitsLabel(nullptr) {}

        /// Destroys a data item.
        ///
        ~DataItem() {
            try {
                if (m_unitsLabel != nullptr) {
                    MeaUnitsMgr::Instance().DeleteUnitsLabel(m_unitsLabel);
                }
                if (m_spin != nullptr) {
                    delete m_spin;
                }
            } catch (...) {
                MeaAssert(false);
            }
        }

        /// Creates a labeled text entry field optionally with
        /// a spin button control and units label.
        ///
        /// @param labelId      [in] String resource ID for the field label.
        /// @param numChars     [in] Number of characters the field is to display.
        /// @param parentWnd    [in] Parent for the field and its controls.
        /// @param fieldId      [in] ID for the text field.
        /// @param spinId       [in] ID for the spin control.
        ///
        /// @return <b>true</b> if creation succeeds.
        ///
        bool CreateField(UINT labelId, int numChars, CWnd* parentWnd, MeaFields fieldId, int spinId = 0xFFFF);

        /// Creates a spin button control associated with the text field.
        ///
        /// @param parentWnd    [in] Parent window for the spin button.
        /// @param id           [in] ID for the spin control.
        ///
        /// @return <b>true</b> if creation succeeded.
        ///
        bool CreateSpinButton(CWnd* parentWnd, MeaFields id);

        /// Creates a linear units label.
        ///
        /// @param units        [in] Object representing the linear units for the label.
        /// @param parentWnd    [in] Parent window for the label.
        ///
        /// @return <b>true</b> if creation succeeded.
        ///
        bool CreateLengthUnits(const MeaLinearUnits* units, CWnd* parentWnd) {
            m_unitsLabel = MeaUnitsMgr::Instance().NewLengthUnitsLabel();
            return m_unitsLabel->Create(units->GetUnitsId(), WS_CHILD | WS_VISIBLE,
                                        CPoint(0, 0), parentWnd) ? true : false;
        }

        /// Creates an angular units label.
        ///
        /// @param units        [in] Object representing the angular units for the label.
        /// @param parentWnd    [in] Parent window for the label.
        ///
        /// @return <b>true</b> if creation succeeded.
        ///
        bool CreateAngleUnits(const MeaAngularUnits* units, CWnd* parentWnd) {
            m_unitsLabel = MeaUnitsMgr::Instance().NewAngleUnitsLabel();
            return m_unitsLabel->Create(units->GetUnitsId(), WS_CHILD | WS_VISIBLE,
                                        CPoint(0, 0), parentWnd) ? true : false;
        }

        /// Creates an area units label.
        ///
        /// @param units        [in] Object representing the area units for the label.
        /// @param parentWnd    [in] Parent window for the label.
        ///
        /// @return <b>true</b> if creation succeeded.
        ///
        bool CreateAreaUnits(const MeaLinearUnits* units, CWnd* parentWnd) {
            m_unitsLabel = MeaUnitsMgr::Instance().NewAreaUnitsLabel();
            return m_unitsLabel->Create(units->GetUnitsId(), WS_CHILD | WS_VISIBLE,
                                        CPoint(0, 0), parentWnd) ? true : false;
        }

        /// Creates a resolution units label.
        ///
        /// @param units        [in] Object representing the resolution units for the label.
        /// @param parentWnd    [in] Parent window for the label.
        ///
        /// @return <b>true</b> if creation succeeded.
        ///
        bool CreateResUnits(const MeaLinearUnits* units, CWnd* parentWnd) {
            m_unitsLabel = MeaUnitsMgr::Instance().NewResUnitsLabel();
            return m_unitsLabel->Create(units->GetUnitsId(), WS_CHILD | WS_VISIBLE,
                                        CPoint(0, 0), parentWnd) ? true : false;
        }


        /// Enables/disables all elements of the data item.
        ///
        /// @param enable   [in] Indicates whether the data item should be enabled or disabled.
        ///
        void Enable(bool enable);

        /// Controls whether the data item text field is editable or read-only.
        ///
        /// @param readonly     [in] <b>true</b> if the text field should be read-only.
        ///
        void SetReadOnly(bool readonly) {
            m_field.SetReadOnly(readonly);
            m_field.ModifyStyle(readonly ? (WS_GROUP | WS_TABSTOP) : 0, readonly ? 0 : (WS_GROUP | WS_TABSTOP));
            if (m_spin != nullptr) {
                m_spin->EnableWindow(!readonly);
            }
        }

        /// Displays the specified text in the data item's text field.
        ///
        /// @param text     [in] Text to display.
        ///
        void SetText(LPCTSTR text) {
            m_field.SetWindowText(text);
        }

        /// Returns the contents of the data item's text field converted to
        /// a double precision floating point value.
        ///
        /// @return Contents of the text field converted to a double precision
        ///         floating point value.
        ///
        double GetValue() const {
            double value = 0;
            m_field.GetValue(value);
            return value;
        }

        /// Sets the maximum number of characters allowed in the data item's
        /// text field.
        ///
        /// @param numChars     [in] Maximum number of characters allowed in
        ///                     the text field.
        ///
        void SetLimitText(UINT numChars) {
            m_field.SetLimitText(numChars);
        }

        /// Sets the upper and lower bounds of the spin control value. Note
        /// that the spin control can only take integer values. This means
        /// that the spin controls are not useful for fractional units adjustment.
        ///
        /// @param lower    [in] Minimum spin control value, inclusive.
        /// @param upper    [in] Upper spin control value, inclusive.
        ///
        void SetSpinRange(int lower, int upper) {
            if (m_spin != nullptr) {
                m_spin->SetRange32(lower, upper);
            }
        }

        /// Sets a value for the spin control. Note that the spin control can
        /// only take integer values.
        ///
        /// @param pos      [in] Value for the spin control.
        ///
        void SetSpinPos(int pos) {
            if (m_spin != nullptr && m_spin->IsWindowEnabled()) {
                m_spin->SetPos(pos);
            }
        }


        /// Returns the label object for the data item. The label is displayed
        /// to the left of the text field.
        ///
        /// @return Label object.
        ///
        const MeaLabel& GetTitleLabel() const { return m_titleLabel; }

        /// Returns the text field object for the data item.
        ///
        /// @return Text field object.
        ///
        MeaNumberField& GetField() { return m_field; }

        /// Returns the spin control associated with the text field.
        ///
        /// @return Spin control.
        ///
        CSpinButtonCtrl& GetSpin() {
            MeaAssert(m_spin != nullptr);
            return *m_spin;
        }

        /// Returns the units label object associated with the text field. The
        /// units are displayed to the right of the text field.
        ///
        /// @return Units label object.
        ///
        MeaUnitsLabel& GetUnitsLabel() {
            MeaAssert(m_unitsLabel != nullptr);
            return *m_unitsLabel;
        }

    private:
        static constexpr int kSpinWidth { 17 };     ///< Width for the spin control, in pixels.

        MeaLabel m_titleLabel;          ///< Left side label for the data item.
        MeaNumberField m_field;         ///< Data item's text field.
        CSpinButtonCtrl* m_spin;        ///< Spin control associated with the text field or nullptr if no spin
                                        ///< control for this data item.
        MeaUnitsLabel* m_unitsLabel;    ///< Units label or nullptr if no units label for this data item.
    };


    typedef std::set<MeaNumberField*>   Fields;     ///< A set of numerical text fields.

    static constexpr RECT kMargin { 7, 17, 7, 7 };          ///< Margins for the sections, in pixels.
    static constexpr SIZE kControlSpacing { 7, 5 };         ///< Horizontal and vertical spacing for the controls in a data item, in pixels.
    static constexpr int kSectionSpacing { 3 };             ///< Vertical spacing between sections, in pixels.
    static constexpr int kLabelSpacing { 3 };               ///< Spacing between labels and their text fields, in pixels.
    static constexpr unsigned int kLengthChars { 6 };       ///< Number of visible and allowable characters for length fields.
    static constexpr int kAngleChars { 6 };                 ///< Number of visible and allowable characters for the angle field.
    static constexpr int kAspectChars { 17 };               ///< Number of visible and allowable characters for the aspect ratio field.
    static constexpr int kAspectPrecision { FLT_DIG - 1 };  ///< Number of decimal places for the aspect ratio field
    static constexpr int kAreaChars { 17 };                 ///< Number of visible and allowable characters for the area field.
    static constexpr int kResChars { 6 };                   ///< Number of visible and allowable characters for the screen resolution fields.

    /// Creates the section devoted to displaying radio tool
    /// measurements.
    ///
    /// @return <b>true</b> if the section was created successfully.
    ///
    bool CreateRegionSection();

    /// Create the section devoted to displaying screen information.
    ///
    /// @return <b>true</b> if the section was created successfully.
    ///
    bool CreateScreenSection();


    MeaDataSection m_regionSection;     ///< Measurement tool display section.
    MeaDataSection m_screenSection;     ///< Screen information display section.
    bool m_regionSectionProfileEnabled; ///< Indicates if the measurement tool display
                                        ///< section is shown based on the user preferences.
    bool m_screenSectionProfileEnabled; ///< Indicates if the screen information display
                                        ///< section is shown based on the user preferences.
    bool m_regionSectionEnabled;        ///< Indicates if the measurement tool display is shown.
    bool m_screenSectionEnabled;        ///< Indicates if the screen information section is shown.
    CRect m_regionSectionRect;          ///< Rectangle containing the measurement tool display section.
    CRect m_screenSectionRect;          ///< Rectangle containing the screen information section.
    DataItem m_x1;                      ///< X1 coordinate data item.
    DataItem m_y1;                      ///< Y1 coordinate data item.
    DataItem m_x2;                      ///< X2 coordinate data item.
    DataItem m_y2;                      ///< Y2 coordinate data item.
    DataItem m_xv;                      ///< Vertex or center X coordinate data item.
    DataItem m_yv;                      ///< Vertex or center Y coordinate data item.
    DataItem m_width;                   ///< Width data item.
    DataItem m_height;                  ///< Height data item.
    DataItem m_length;                  ///< Distance data item.
    DataItem m_angle;                   ///< Angle data item.
    DataItem m_aspect;                  ///< Aspect ratio.
    DataItem m_area;                    ///< Area data item.
    DataItem m_screenWidth;             ///< Screen width data item.
    DataItem m_screenHeight;            ///< Screen height data item.
    DataItem m_screenResX;              ///< Screen X resolution data item.
    DataItem m_screenResY;              ///< Screen Y resolution data item.
    MeaImageButton m_calBtn;            ///< Calibration warning button.
    Fields m_fields;                    ///< Set of all text fields.
};

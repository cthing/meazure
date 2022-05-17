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
/// @brief Header file for a popup data display window.

#pragma once

#include <meazure/profile/Profile.h>
#include "Layout.h"
#include "ScreenProvider.h"
#include <meazure/units/UnitsProvider.h>


/// A popup data display window. A data display window is attached to each
/// radio tool's crosshair and a corner of the Window Tool. When the user
/// hovers over a crosshair and while the user drags the crosshair, the
/// data display window is shown and provides a continues readout of the
/// measurements appropriate to the tool. The user can set a preference
/// to not display the data windows.
///
class MeaDataWin : public CWnd {

public:
    static constexpr bool kDefArmed { true };       ///< Indicates whether to display data windows by default.
    static constexpr int kNoLabelId { 0xffff };     ///< Indicates a data window label has not been specified.


    /// Constructs a data window. To use the data window, its Create method
    /// must be called and the window must be displayed using the Show method.
    /// 
    /// @param screenProvider   [in] Screen information provider
    /// @param unitsProvider    [in] Units information provider
    ///
    MeaDataWin(const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider);

    /// Destroys a data window.
    ///
    virtual ~MeaDataWin();

    /// Creates the window for the data display. This method must be called
    /// prior to calling the Show method. A data item is displayed in the
    /// window if its label resource ID is a value other than kNoLabelId.
    ///
    /// @param opacity      [in] Opacity for the window, where 0 is completely
    ///                     transparent and 255 is completely opaque.
    /// @param xLabelId     [in] String resource ID for the x coordinate label.
    /// @param yLabelId     [in] String resource ID for the y coordinate label.
    /// @param wLabelId     [in] String resource ID for the width label.
    /// @param hLabelId     [in] String resource ID for the height coordinate label.
    /// @param dLabelId     [in] String resource ID for the distance label.
    /// @param aLabelId     [in] String resource ID for the angle label.
    /// @param parent       [in] Parent window for the data window if it is
    ///                     used as a child window. Typically, this parameter
    ///                     is nullptr, which creates a floating window.
    ///
    /// @return <b>true</b> if the window is successfully created.
    ///
    bool Create(BYTE opacity, UINT xLabelId = kNoLabelId, UINT yLabelId = kNoLabelId,
                UINT wLabelId = kNoLabelId, UINT hLabelId = kNoLabelId,
                UINT dLabelId = kNoLabelId, UINT aLabelId = kNoLabelId,
                const CWnd* parent = nullptr);

    /// Displays the specified x and y coordinates. For the values
    /// to be displayed, a string resource ID value must be specified
    /// for the xLabelId and yLabelId parameters of the Create method.
    ///
    /// @param point    [in] Coordinates to display.
    ///
    void ShowXY(const FPOINT& point) {
        m_xData = m_unitsProvider.Format(MeaX, point.x);
        m_yData = m_unitsProvider.Format(MeaY, point.y);
    }

    /// Displays the specified width and height. For the values
    /// to be displayed, a string resource ID value must be specified
    /// for the wLabelId and hLabelId parameters of the Create method.
    ///
    /// @param size     [in] Dimensions to display.
    ///
    void ShowWH(const FSIZE& size) {
        m_wData = m_unitsProvider.Format(MeaW, size.cx);
        m_hData = m_unitsProvider.Format(MeaH, size.cy);
    }

    /// Displays a diagonal distance based on the specified width and height.
    /// For the value to be displayed, a string resource ID value must be
    /// specified for the dLabelId parameter of the Create method.
    ///
    /// @param dist     [in] Dimensions used to calculate the distance.
    ///
    void ShowDistance(const FSIZE& dist) {
        m_dData = m_unitsProvider.Format(MeaD, MeaLayout::CalcLength(dist.cx, dist.cy));
    }

    /// Displays the specified distance. For the value to be displayed, a
    /// string resource ID value must be specified for the dLabelId
    /// parameter of the Create method.
    ///
    /// @param dist     [in] Distance to display.
    ///
    void ShowDistance(double dist) {
        m_dData = m_unitsProvider.Format(MeaD, dist);
    }

    /// Displays the specified angle. For the value to be displayed, a
    /// string resource ID value must be specified for the aLabelId
    /// parameter of the Create method.
    ///
    /// @param angle    [in] Angle to display.
    ///
    void ShowAngle(double angle) {
        m_aData = m_unitsProvider.FormatConvertAngle(angle);
    }

    /// Displays the currently set data in the window.
    ///
    /// @param centerWin    [in] The data window is displayed
    ///                     adjacent to a corner of this window.
    ///
    void Update(const CWnd& centerWin) {
        CRect rect;
        centerWin.GetWindowRect(rect);
        Update(rect);
    }

    /// Displays the currently set data in the window.
    ///
    /// @param rect     [in] The data window is displayed
    ///                 adjacent to the corner of this rectangle.
    ///
    void Update(const CRect& rect);

    /// Shows the data window, if the window is armed. If the
    /// window is not armed, calling Show does not display the
    /// window.
    ///
    void Show();

    /// Hides the data window.
    ///
    void Hide() { if (m_hWnd != nullptr) ShowWindow(SW_HIDE); }

    /// Flashes the text in the data window once. Typically used
    /// to indicate that a tool position has been recorded.
    ///
    void Strobe();

    /// Sets the opacity of the data window.
    ///
    /// @param opacity      [in] Opacity of the data window
    ///                     where 0 is transparent and 255 is
    ///                     opaque.
    ///
    void SetOpacity(BYTE opacity);

    /// Arms the data window so that it can be displayed when the
    /// Show method is called.
    ///
    /// @param armed        [in] Specifies whether the data window
    ///                     is allowed to be displayed.
    ///
    static void Arm(bool armed) { m_armed = armed; }

    /// Indicates whether the data window can be displayed.
    ///
    /// @return <b>true</b> if the data window is allowed to be displayed
    ///         using the Show method.
    ///
    static bool IsArmed() { return m_armed; }


    /// Persists the display settings of the data window to the
    /// specified profile object.
    ///
    /// @param profile      [in] Profile object into which the
    ///                     data window state is persisted.
    ///
    static void SaveProfile(MeaProfile& profile);

    /// Restores the display settings of the data window from the
    /// specified profile object.
    ///
    /// @param profile      [in] Profile object from which the
    ///                     data window state is restored.
    ///
    static void LoadProfile(MeaProfile& profile);

    /// Called when the master reset is activated. Causes the
    /// data window to load its default settings.
    ///
    static void MasterReset();

protected:
    DECLARE_MESSAGE_MAP()

    /// Performs cleanup of objects created in the Create method.
    ///
    afx_msg void OnDestroy();

    /// Draws the data window.
    ///
    afx_msg void OnPaint();

    /// Called in response to the flash timer so that the data
    /// window can be strobed.
    ///
    /// @param timerId      [in] ID of the timer.
    ///
    afx_msg void OnTimer(UINT_PTR timerId);

private:
    /// Specifies how the data window is drawn in support
    /// of strobing the data window.
    ///
    enum DrawState {
        Normal,         ///< Draw the data window in its normal appearance.
        Inverted        ///< Draw the data window with its background and text colors swapped.
    };


    static bool m_armed;            ///< Indicates if the data window is allowed to be displayed.
    static SIZE m_margin;           ///< Margin between the data text and the window border, in pixels.
    static UINT m_flashInterval;    ///< Window flash interval, in milliseconds.


    /// Calculates the width for the data window based on the width
    /// of the label, the width of the maximum data value and the
    /// display font.
    ///
    /// @return Width of the data window, in pixels.
    ///
    int CalcWidth();

    /// Draws the data window into the specified device context.
    ///
    /// @param dc   [in] Device context into which the data window
    ///             is drawn.
    ///
    void DrawWin(CDC& dc);


    const MeaScreenProvider& m_screenProvider;  ///< Screen information provider
    const MeaUnitsProvider& m_unitsProvider;    ///< Units information provider
    CString m_xLabel;                           ///< Label for the x coordinate data.
    CString m_yLabel;                           ///< Label for the y coordinate data.
    CString m_wLabel;                           ///< Label for the width data.
    CString m_hLabel;                           ///< Label for the height data.
    CString m_dLabel;                           ///< Label for the distance data.
    CString m_aLabel;                           ///< Label for the angle data.
    CString m_xData;                            ///< X coordinate data converted to a string.
    CString m_yData;                            ///< Y coordinate data converted to a string.
    CString m_wData;                            ///< Width data converted to a string.
    CString m_hData;                            ///< Height data converted to a string.
    CString m_dData;                            ///< Distance data converted to a string.
    CString m_aData;                            ///< Angle data converted to a string.
    const CWnd* m_parent;                       ///< Parent window or nullptr if data window is floating.
    CFont m_font;                               ///< Font for the data display.
    LONG m_textHeight;                          ///< Height of the text, in pixels.
    LONG m_winHeight;                           ///< Height of the data window, in pixels.
    int m_dataOffset;                           ///< Position for the data strings, in pixels.
    DrawState m_drawState;                      ///< Indicates how the data window should be drawn.
    int m_flashCount;                           ///< Window flash interval, in milliseconds.
    BYTE m_opacity;                             ///< Data window opacity, 0 (transparent) to 255 (opaque).
};

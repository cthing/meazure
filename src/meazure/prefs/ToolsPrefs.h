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
/// @brief Tool property page header file.

#pragma once

#include <meazure/resource.h>
#include <meazure/ui/Swatch.h>
#include <meazure/graphics/CrossHair.h>
#include <meazure/ui/DataWin.h>


/// Property page for specifying the following tool preferences:
/// <ul>
///     <li>Line color</li>
///     <li>Crosshair normal background</li>
///     <li>Crosshair highlight color</li>
///     <li>Crosshair border color</li>
///     <li>Crosshair opacity</li>
///     <li>Display of popup data window</li>
///     <li>Display of the origin marker</li>
/// </ul>
class MeaToolsPrefs : public CPropertyPage {

public:
    /// Creates an instance of the tool property page.
    /// 
    /// @param screenProvider   [in] Screen information provider
    /// @param unitsProvider    [in] Units information provider
    ///
    MeaToolsPrefs(const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider);

    /// Destroys an instance of the tool property page.
    ///
    ~MeaToolsPrefs();

    enum { IDD = IDD_PREF_TOOLS };

    /// Called when the property page is created. This method
    /// Creates the sample crosshairs, line color swatch, data
    /// window, and initializes the opacity slider.
    ///
    /// @return TRUE, indicating that the input focus should be
    ///         set to the first control on the page.
    ///
    virtual BOOL OnInitDialog();

    BOOL m_showDataWin;     ///< Preference variable indicating whether the data window popup should be displayed.
    int m_opacity;          ///< Preference variable specifying the crosshair opacity where 0 is transparent and 255 is opaque.
    BOOL m_originMarker;    ///< Preference variable indicating whether the origin marker should be displayed.
    COLORREF m_lineColor;   ///< Line color.
    COLORREF m_backColor;   ///< Crosshair background color.
    COLORREF m_borderColor; ///< Crosshair border color.
    COLORREF m_hiliteColor; ///< Crosshair highlight color.

protected:
    /// Performs Dynamic Data Exchange (DDX) for the page.
    ///
    /// @param pDX  [in] DDX object for exchanging data between the page UI
    ///             and the member variables.
    /// 
    virtual void DoDataExchange(CDataExchange* pDX) override;

    /// Called when the line color button is pressed. Displays a
    /// color picker dialog for selecting a new line color. The
    /// selected color is shown in the line color swatch.
    /// 
    afx_msg void OnChangeLineColor();

    /// Called when the default line color button is pressed. Resets
    /// the line color to its default value.
    /// 
    afx_msg void OnDefLineColor();

    /// Called when the crosshair border color button is pressed.
    /// Displays a color picker dialog for selecting a new border
    /// color and displays the selected color on the sample crosshairs.
    /// 
    afx_msg void OnChangeBorderColor();

    /// Called when the default border color button is pressed. The
    /// default border color is set and displayed on the sample crosshairs.
    /// 
    afx_msg void OnDefBorderColor();

    /// Called when the crosshair background color button is pressed.
    /// Displays a color picker dialog for selecting a new background
    /// color and displays the selected color in the normal crosshair
    /// sample.
    /// 
    afx_msg void OnChangeBackColor();

    /// Called when the default background color button is pressed.
    /// The default background color is set and displayed on the normal
    /// crosshair sample.
    /// 
    afx_msg void OnDefBackColor();

    /// Called when the highlight color button is pressed. Displays a
    /// color picker for selecting a new highlight color and displays the
    /// selected color in the highlight crosshair sample.
    /// 
    afx_msg void OnChangeHiliteColor();

    /// Called when the default highlight color button is pressed. The
    /// default highlight color is set and displayed on the highlight crosshair
    /// sample.
    /// 
    afx_msg void OnDefHiliteColor();

    /// Called when the crosshair opacity slider is moved. Sets the new
    /// opacity on the crosshair samples.
    /// 
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    /// Called whenever a value is changed on the property page. Sets
    /// the modified flag.
    /// 
    afx_msg void OnChange();

    DECLARE_MESSAGE_MAP()

private:
    const MeaScreenProvider& m_screenProvider;  ///< Screen information provider
    MeaSwatch m_line;                           ///< Swatch to display the line color.
    MeaCrossHair m_normalCH;                    ///< Sample crosshair to preview the normal background color.
    MeaCrossHair m_hiliteCH;                    ///< Sample crosshair to preview the highlight color.
    MeaDataWin m_dataWin;                       ///< Sample data popup window.
};

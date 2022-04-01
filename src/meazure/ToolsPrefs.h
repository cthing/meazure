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

#include "Swatch.h"
#include "CrossHair.h"
#include "DataWin.h"


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
class MeaToolsPrefs : public CPropertyPage
{
    DECLARE_DYNCREATE(MeaToolsPrefs)

public:
    /// Creates an instance of the tool property page.
    ///
    MeaToolsPrefs();
    
    /// Destroys an instance of the tool property page.
    ///
    ~MeaToolsPrefs();

    //{{AFX_DATA(MeaToolsPrefs)
    enum { IDD = IDD_PREF_TOOLS };
    BOOL    m_showDataWin;
    int     m_opacity;
    BOOL    m_originMarker;
    //}}AFX_DATA

    /// @var m_showDataWin
    /// Preference variable indicating whether the data window popup should be displayed.

    /// @var m_opacity
    /// Preference variable specifying the crosshair opacity where 0 is transparent and
    /// 255 is opaque.

    /// @var m_originMarker
    /// Preference variable indicating whether the origin marker should be displayed.


    COLORREF    m_lineColor;        ///< Line color.
    COLORREF    m_backColor;        ///< Crosshair background color.
    COLORREF    m_borderColor;      ///< Crosshair border color.
    COLORREF    m_hiliteColor;      ///< Crosshair highlight color.


    /// Called when the property page is created. This method
    /// Creates the sample crosshairs, line color swatch, data
    /// window, and initializes the opacity slider.
    ///
    /// @return TRUE, indicating that the input focus should be
    ///         set to the first control on the page.
    ///
    virtual BOOL OnInitDialog();

    //{{AFX_VIRTUAL(MeaToolsPrefs)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

    /// @fn DoDataExchange(CDataExchange* pDX)
    /// Performs Dynamic Data Exchange (DDX) for the page.
    ///
    /// @param pDX  [in] DDX object for exchanging data between the page UI
    ///             and the member variables.

protected:
    //{{AFX_MSG(MeaToolsPrefs)
    afx_msg void OnChangeLineColor();
    afx_msg void OnDefLineColor();
    afx_msg void OnChangeBorderColor();
    afx_msg void OnDefBorderColor();
    afx_msg void OnChangeBackColor();
    afx_msg void OnDefBackColor();
    afx_msg void OnChangeHiliteColor();
    afx_msg void OnDefHiliteColor();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnChange();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    /// @fn OnChangeLineColor()
    /// Called when the line color button is pressed. Displays a
    /// color picker dialog for selecting a new line color. The
    /// selected color is shown in the line color swatch.

    /// @fn OnDefLineColor()
    /// Called when the default line color button is pressed. Resets
    /// the line color to its default value.

    /// @fn OnChangeBorderColor()
    /// Called when the crosshair border color button is pressed.
    /// Displays a color picker dialog for selecting a new border
    /// color and displays the selected color on the sample crosshairs.

    /// @fn OnDefBorderColor()
    /// Called when the default border color button is pressed. The
    /// default border color is set and displayed on the sample crosshairs.

    /// @fn OnChangeBackColor()
    /// Called when the crosshair background color button is pressed.
    /// Displays a color picker dialog for selecting a new background
    /// color and displays the selected color in the normal crosshair
    /// sample.

    /// @fn OnDefBackColor()
    /// Called when the default background color button is pressed.
    /// The default background color is set and displayed on the normal
    /// crosshair sample.

    /// @fn OnChangeHiliteColor()
    /// Called when the highlight color button is pressed. Displays a
    /// color picker for selecting a new highlight color and displays the
    /// selected color in the highlight crosshair sample.

    /// @fn OnDefHiliteColor()
    /// Called when the default highlight color button is pressed. The
    /// default highlight color is set and displayed on the highlight crosshair
    /// sample.

    /// @fn OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
    /// Called when the crosshair opacity slider is moved. Sets the new
    /// opacity on the crosshair samples.

    /// @fn OnChange()
    /// Called whenever a value is changed on the property page. Sets
    /// the modified flag.

private:
    MeaSwatch       m_line;         ///< Swatch to display the line color.
    MeaCrossHair    m_normalCH;     ///< Sample crosshair to preview the normal background color.
    MeaCrossHair    m_hiliteCH;     ///< Sample crosshair to preview the highlight color.
    MeaDataWin      m_dataWin;      ///< Sample data popup window.
};


//{{AFX_INSERT_LOCATION}}

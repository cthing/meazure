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
/// @brief Grid overlay tool header file.

#pragma once

#include <list>

#include "Tool.h"
#include "Line.h"
#include "NumberField.h"


class MeaGridTool;


/// Grid tool configuration dialog. This dialog allows the user to
/// set the grid spacing. The grid spacing can be set independently
/// in the vertical and horizontal directions.
///
class MeaGridDialog : public CDialog
{
public:
    /// Constructs a new instance of the Grid tool dialog. To display
    /// the dialog call its DoModal method.
    ///
    /// @param tool     [in] Grid tool being configured by the dialog.
    ///
    explicit MeaGridDialog(MeaGridTool* tool);

    /// Destroy the Grid tool dialog.
    ///
    virtual ~MeaGridDialog();

    /// Called immediately before the dialog is displayed in response
    /// to a DoModal call.
    ///
    /// @return <b>TRUE</b> to indicate Windows should set the input focus.
    ///
    virtual BOOL OnInitDialog();

protected:
    /// Called when the user clicks the checkbox that controls whether
    /// the vertical and horizontal grid spacings are set independently.
    ///
    afx_msg void OnGridLink();

    /// Called when the value in the horizontal grid spacing text field
    /// is changed.
    ///
    afx_msg void OnHSpaceChange();

    /// Called when the value in the vertical grid spacing text field
    /// is changed.
    ///
    afx_msg void OnVSpaceChange();

    DECLARE_MESSAGE_MAP()

    /// Called by Windows to perform Dynamic Data Exchange between
    /// the dialog's controls and their corresponding member variables.
    ///
    /// @param pDX      [in] Object containing the dialog data exchange
    ///                 context.
    ///
    virtual void DoDataExchange(CDataExchange* pDX);

    /// Called when the dialog's OK button is pressed. The method
    /// verifies that the spacing fields contain valid numerical
    /// values. If not, a dialog is displayed, the erroneous field
    /// is selected and the dialog is not dismissed.
    ///
    virtual void OnOK();

    /// Called when the dialog's Cancel button is pressed. The method
    /// restores the original spacing values.
    ///
    virtual void OnCancel();

private:
    /// Updates the dialog's controls to reflect the member variable
    /// values.
    ///
    void    UpdateDisplay();

    /// Reads the content of the specified text field and attempts
    /// to convert it to a numerical value. In order to succeed, the
    /// contents must represent a number and the number must be in the
    /// range:
    ///
    /// MeaGridTool::kDefMinSpacing \f$\leq\f$ value \f$\leq\f$ MeaGridTool::kDefMaxSpacing
    ///
    /// @param fieldId      [in] Species the text field to read
    /// @param value        [out] Field contents converted to a numerical value
    ///
    /// @return <b>true</b> if the text field's contents were successfully converted
    ///         and the value was within the allowable range. The value parameter
    ///         is changed only if the method returns <b>true</b>.
    ///
    bool    GetFieldValue(UINT fieldId, long& value) const;

    MeaGridTool*        m_tool;         ///< Grid tool being configured by the dialog

    MeaNumberField      m_hSpaceField;  ///< Horizontal grid spacing text field
    MeaNumberField      m_vSpaceField;  ///< Vertical grid spacing text field

    CSpinButtonCtrl*    m_hSpaceSpin;   ///< Horizontal spacing increment / decrement control
    CSpinButtonCtrl*    m_vSpaceSpin;   ///< Vertical spacing increment / decrement spacing

    bool    m_origLinked;               ///< State of the lock checkbox before the dialog was displayed
    bool    m_linked;                   ///< Current state of the lock checkbox
    SIZE    m_origSpacing;              ///< Grid spacing before the dialog was displayed
    SIZE    m_spacing;                  ///< Current grid spacing
};


/// Grid overlay tool. This tool overlays a rectangular grid on
/// the screen. If there are multiple monitors, the grid is overlayed
/// continuously across all monitors. The grid spacing can be changed
/// using the MeaGridDialog.
///
class MeaGridTool : public MeaTool
{
public:
    // Defaults
    //
    static const int        kDefMinSpacing;         ///< Minimum allowable grid spacing in pixels
    static const int        kDefMaxSpacing;         ///< Maximum allowable grid spacing in pixels
    static const int        kDefDefaultSpacing;     ///< Default grid spacing in pixels
    static const bool       kDefGrid;               ///< Default grid display state
    static const bool       kDefLinked;             ///< Default spacing link state

    static const CString    kToolName;              ///< "GridTool"

    /// Constructs a new instance of the Grid overlay tool. To use
    /// a newly constructed Grid tool, the Enable() method must be called.
    ///
    /// @param mgr  [in] Tool manager parent of the Grid tool.
    ///
    explicit MeaGridTool(MeaToolMgr* mgr);

    /// Hides and then destroys the Grid tool.
    ///
    virtual ~MeaGridTool();


    /// Displays the Grid tool.
    ///
    virtual void    Enable();

    /// Hides the Grid tool.
    ///
    virtual void    Disable();


    /// Persists the state of the tool to the specified profile object.
    ///
    /// @param profile  [in] The destination for the state information.
    ///                 Typically an MeaFileProfile or MeaRegistryProfile
    ///                 object.
    ///
    virtual void    SaveProfile(MeaProfile& profile);

    /// Restores the state of the tool from the specified profile object.
    ///
    /// @param profile  [in] The source for the state information. Typically
    ///                 an MeaFileProfile or MeaRegistryProfile object.
    ///
    virtual void    LoadProfile(MeaProfile& profile);


    /// Resets the tool to its default state.
    ///
    virtual void    MasterReset();


    /// Draws the horizontal and vertical grid lines, if the tool
    /// is enabled.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void    Update(MeaUpdateReason reason);

    /// Draws the horizontal grid lines.
    ///
    void            UpdateH();

    /// Draws the vertical grid lines.
    ///
    void            UpdateV();


    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("GridTool").
    ///
    virtual CString GetToolName() const;


    /// Returns the tool's current position which, for the Grid tool,
    /// is always the position (0, 0).
    ///
    /// @return The position (0, 0)
    ///
    virtual const POINT& GetPosition() const;


    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The grid lines are redrawn in their new colors.
    ///
    virtual void ColorsChanged();


    /// Sets the horizontal and vertical grid spacing.
    ///
    /// @param spacing      [in] Horizontal and vertical grid spacing, in pixels
    ///
    void    SetGridSpacing(const SIZE& spacing);

    /// Gets the horizontal and vertical grid spacing.
    ///
    /// @return Horizontal and vertical grid spacing, in pixels.
    ///
    const SIZE& GetGridSpacing() const { return m_gridSpacing; }


    /// Indicates if the grid spacing is linked so that the
    /// horizontal and vertical grid spacing is the same.
    ///
    /// @return <b>true</b> if the horizontal and vertical
    ///         grid spacing are linked.
    ///
    bool    GetLinked() const { return m_linked; }

    /// Sets the grid spacing link state.
    ///
    /// @param linked       [in] <b>true</b> to link the spacing
    ///
    void    SetLinked(bool linked) { m_linked = linked; }

private:
    typedef std::list<MeaLine*> LineList;       ///< List of grid lines

    /// Grid line direction
    ///
    enum LineDir {
        HDir = 0,
        VDir = 1
    };


    /// Creates the vertical or horizontal lines for the grid as needed
    /// and positions them on the grid according to the currently set
    /// grid spacing. Since there are many lines on a grid and line
    /// creation is expensive, lines are created on demand and
    /// reused. The lines are destroyed only when the tool is
    /// destroyed.
    ///
    /// @param dir      [in] Direction of the lines to create
    ///
    void    SetLines(LineDir dir);

    /// Displays the vertical or horizontal grid lines.
    ///
    /// @param dir      [in] Direction of the lines to display
    ///
    void    ShowLines(LineDir dir);

    /// Hides the vertical or horizontal grid lines
    ///
    /// @param dir      [in] Direction of the lines to display
    ///
    void    HideLines(LineDir dir) const;

    /// Hides and then destroys the lines in the specified line list.
    /// The line list is then cleared.
    ///
    /// @param list     [in] List of lines whose contents is to be
    ///                 destroyed. The list is empty following the
    ///                 call to this method.
    ///
    void    DeleteLines(LineList& list) const;

    CSize       m_gridSpacing;      ///< Vertical and horizontal grid spacing, in pixels
    bool        m_linked;           ///< <b>true</b> if the vertical and horizontal grid spacings are linked (i.e. kept equal)
    LineList    m_hlineList;        ///< List of horizontal grid lines
    LineList    m_vlineList;        ///< List of vertical grid lines
};

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
/// @brief Ruler measurement tool header file.

#pragma once

#include "Tool.h"
#include "Ruler.h"
#include <vector>


/// Ruler measurement tool. The Ruler tool provides vertical and
/// horizontal rulers on each screen. The rulers can be dragged
/// to any position on the screen.
///
class MeaRulerTool : public MeaTool, public MeaRulerCallback {

public:
    // Defaults
    //
    static constexpr bool kShowRulers { false };    ///< Indicates if the rulers are shown by default
    static const CString kToolName;                 ///< "RulerTool"

    /// Constructs a new instance of the Ruler tool. To use a
    /// newly constructed Ruler tool, the Enable() method must be
    /// called.
    ///
    /// @param mgr              [in] Tool manager parent of the Ruler tool
    /// @param screenProvider   [in] Screen information provider
    ///
    explicit MeaRulerTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider);

    /// Hides and then destroys the Ruler tool.
    ///
    virtual ~MeaRulerTool();

    /// Displays the Ruler tool.
    ///
    virtual void Enable() override;

    /// Hides the Ruler tool.
    ///
    virtual void Disable() override;

    /// Positions and draws the rulers.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason) override;

    /// Persists the state of the tool to the specified profile object.
    ///
    /// @param profile  [in] The destination for the state information.
    ///                 Typically an MeaFileProfile or MeaRegistryProfile
    ///                 object.
    ///
    virtual void SaveProfile(MeaProfile& profile) override;

    /// Restores the state of the tool from the specified profile object.
    ///
    /// @param profile  [in] The source for the state information. Typically
    ///                 an MeaFileProfile or MeaRegistryProfile object.
    ///
    virtual void LoadProfile(MeaProfile& profile) override;


    /// Resets the tool to its default state.
    ///
    virtual void MasterReset() override;

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("RulerTool").
    ///
    virtual CString GetToolName() const override;

    /// Returns the tool's current position which, for the Ruler tool,
    /// is always the position (0, 0).
    ///
    /// @return The position (0, 0)
    ///
    virtual const POINT& GetPosition() const override;

    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The rulers are redrawn in their new colors.
    ///
    virtual void ColorsChanged() override;

    /// Sets the position of the specified ruler indicator. The ruler
    /// indicator shows the position of a crosshair, pointer or some
    /// other characteristic of a radio tool.
    ///
    /// @param indId            [in] Specifies the indicator to position
    /// @param indicatorPos     [in] Position for the indicator, in pixels
    ///
    void SetIndicator(MeaRuler::IndicatorId indId, const POINT& indicatorPos);

    /// Called when the user drags a ruler to a new position. This method
    /// is responsible for actually repositioning the specified ruler to
    /// the new position.
    ///
    /// @param info     [in] Ruler information structure
    ///
    virtual void OnRulerMove(const RulerInfo* info) override;

private:
    /// Class representing a single screen's set of rulers. A ruler
    /// set consists of a vertical and a horizontal ruler.
    ///
    class RulerSet {

    public:
        RulerSet(const MeaScreenProvider& screenProvider) :
            m_vRuler(screenProvider), m_hRuler(screenProvider), m_xPos(0), m_yPos(0) {}
        ~RulerSet() {}

        MeaRuler m_vRuler;  ///< Vertical ruler
        MeaRuler m_hRuler;  ///< Horizontal ruler
        int m_xPos;         ///< Vertical ruler's position on the screen, in pixels
        int m_yPos;         ///< Horizontal ruler's position on the screen, in pixels
    };


    typedef std::vector<RulerSet*> Rulers;      ///< Collection of rulers

    /// Copy constructor is purposely undefined.
    ///
    MeaRulerTool(const MeaRulerTool&);

    /// Assignment operator is purposely undefined
    ///
    MeaRulerTool& operator=(const MeaRulerTool&);

    /// Repositions the specified ruler to the location specified in
    /// the corresponding member variable.
    ///
    /// @param rulerIdx     [in] Index specifying the ruler to reposition
    ///
    void PositionRulers(int rulerIdx);

    /// Updates the visual state of the specified ruler.
    ///
    /// @param rulerIdx     [in] Index specifying the ruler to update
    ///
    void UpdateRulers(int rulerIdx);

    /// Repositions the specified ruler's indicators to the location
    /// specified in the corresponding member variable.
    ///
    /// @param rulerIdx     [in] Index specifying the ruler whose indicator is
    ///                     to be repositioned
    /// @param indId        [in] ID of the indicator to reposition
    ///
    void PositionIndicators(int rulerIdx, MeaRuler::IndicatorId indId);


    CPoint m_indicatorPos[MeaRuler::NumIndicators];    ///< Position of each ruler indicator, in pixels
    Rulers m_rulers;                                   ///< Collection of all rulers on all screens
};

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

#include <meazure/pch.h>
#include "PointTool.h"
#include "ToolMgr.h"
#include <meazure/utilities/Geometry.h>
#include <meazure/utilities/StringUtils.h>
#include <meazure/resource.h>
#include <meazure/graphics/Colors.h>
#include <cassert>


MeaPointTool::MeaPointTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                           const MeaUnitsProvider& unitsProvider) :
    MeaRadioTool(mgr, screenProvider, unitsProvider),
    MeaCrossHairCallback(),
    m_center(screenProvider.GetCenter()),
    m_crossHair(screenProvider, unitsProvider),
    m_dataWin(screenProvider, unitsProvider) {
    // Set the default tool position. The crosshair is initially
    // is placed at the center of the screen containing the application.
    //
    m_anchorPoint = m_center;
}

MeaPointTool::~MeaPointTool() {
    try {
        Disable();
    } catch (...) {
        assert(false);
    }
}

bool MeaPointTool::Create() {
    // Create the crosshair.
    //
    if (!m_crossHair.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite),
                            MeaColors::GetA(MeaColors::CrossHairOpacity),
                            this, nullptr, IDS_MEA_POINT1)) {
        return false;
    }

    // Create the crosshair's data window.
    //
    m_dataWin.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X1, IDS_MEA_Y1);

    // Position the crosshair.
    //
    SetPosition();

    return true;
}

void MeaPointTool::SaveProfile(MeaProfile& profile) {
    // Save the position of the crosshair.
    //
    MeaFPoint pt = m_unitsProvider.ConvertPos(m_center);
    profile.WriteStr(_T("PointX1"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("PointY1"), MeaStringUtils::DblToStr(pt.y));
}

void MeaPointTool::LoadProfile(MeaProfile& profile) {
    // Use the current position as the default value
    // for those position components that are not
    // specified in the profile.
    //
    MeaFPoint def = m_unitsProvider.ConvertPos(m_center);

    // Load the crosshair position.
    //
    MeaFPoint pt;
    pt.x = profile.ReadDbl(_T("PointX1"), def.x);
    pt.y = profile.ReadDbl(_T("PointY1"), def.y);
    m_center = m_unitsProvider.UnconvertPos(pt);
    m_anchorPoint = m_center;

    SetPosition();
}

void MeaPointTool::EnableCrosshairs() {
    if (m_mgr.CrosshairsEnabled() && IsWindow(m_crossHair)) {
        m_crossHair.Show();
    }
}

void MeaPointTool::DisableCrosshairs() {
    if (IsWindow(m_crossHair)) {
        m_crossHair.Hide();
    }
}

void MeaPointTool::Flash() {
    m_crossHair.Flash();
}

void MeaPointTool::Strobe() {
    m_crossHair.Flash(MeaCrossHair::kStrobeCount);
    m_dataWin.Strobe();
}

void MeaPointTool::Enable() {
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    // Tell the tool manager which data display fields
    // we will be using.
    //
    m_mgr.EnableRegionFields(MeaX1Field | MeaY1Field, MeaX1Field | MeaY1Field);

    if (!IsWindow(m_crossHair)) {
        Create();
    }

    // Show one liner on how to use the tool.
    //
    m_mgr.SetStatus(IDS_MEA_POINT_STATUS);

    // Make the crosshair visible, flash it and update
    // the data display.
    //
    EnableCrosshairs();
    Flash();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaPointTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    DisableCrosshairs();
}

void MeaPointTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        // Convert the pixel location to the current units.
        //
        MeaFPoint center = m_unitsProvider.ConvertCoord(m_center);

        // Display the results of the measurement in
        // the data display fields.
        //
        m_mgr.ShowXY1(m_center, center);

        // The screen information depends on the
        // current position.
        //
        m_mgr.UpdateScreenInfo(m_center);

        // Display the results of the measurement in
        // the crosshair data window.
        //
        m_dataWin.ShowXY(center);
        m_dataWin.Update(m_crossHair);
    }
}

void MeaPointTool::SetPosition(MeaDataFieldId which, int pixels) {
    // Set the specified position component.
    //
    if (which & MeaX1Field) {
        m_center.x = pixels;
    } else if (which & MeaY1Field) {
        m_center.y = pixels;
    } else {
        return;
    }

    // Reposition the tool based on the crosshair locations.
    //
    SetPosition();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaPointTool::SetPosition(const PointMap& points) {
    // Read the position information from the points
    // map, and set the corresponding point.
    //
    PointMap::const_iterator iter = points.find(_T("1"));
    if (iter != points.end()) {
        m_center = (*iter).second;
    }

    // Reposition the tool and update the data display.
    //
    SetPosition();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaPointTool::SetPosition() {
    // Ensure that the positions fall within the
    // limits of the screen containing the point.
    //
    m_center = m_screenProvider.LimitPosition(m_center);

    // Reposition the tool.
    //
    if (IsWindow(m_crossHair)) {
        m_crossHair.SetPosition(m_center);
    }
}

const POINT& MeaPointTool::GetPosition() const {
    return m_center;
}

void MeaPointTool::RecordPosition(MeaPosition& position) const {
    // Convert the pixel locations to the current units and record
    // the position and the name of this tool in the position object.
    //
    position.SetToolName(kToolName);
    position.RecordXY1(m_unitsProvider.ConvertCoord(m_center));
}

void MeaPointTool::IncPosition(MeaDataFieldId which) {
    // Increment the specified position component.
    //
    if (which & MeaX1Field) {
        SetPosition(which, m_center.x + 1);
    } else if (which & MeaY1Field) {
        SetPosition(which, m_center.y + 1);
    }
}

void MeaPointTool::DecPosition(MeaDataFieldId which) {
    // Decrement the specified position component.
    //
    if (which & MeaX1Field) {
        SetPosition(which, m_center.x - 1);
    } else if (which & MeaY1Field) {
        SetPosition(which, m_center.y - 1);
    }
}

void MeaPointTool::ColorsChanged() {
    // Redraw the crosshair in the new colors.
    //
    m_crossHair.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite));

    // Set the new crosshair and data window opacities.
    //
    m_crossHair.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_dataWin.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
}

PCTSTR MeaPointTool::GetToolName() const {
    return kToolName;
}

UINT MeaPointTool::GetLabelId() const {
    return IDS_MEA_POINT;
}

void MeaPointTool::OnCHEnter(const CrossHairInfo* /* info */) {
    // Show the crosshair's data window.
    //
    m_dataWin.Show();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaPointTool::OnCHLeave(const CrossHairInfo* /* info */) {
    // Hide the crosshair's data window.
    //
    m_dataWin.Hide();
}

void MeaPointTool::OnCHMove(const CrossHairInfo* info) {
    m_center = info->center;

    // Shift + drag locks the movement of the crosshair
    // to vertical or horizontal.
    //
    if (info->flags & MK_SHIFT) {
        if (MeaGeometry::IsVerticallyOriented(m_anchorPoint, m_center)) {
            m_center.x = m_anchorPoint.x;
        } else {
            m_center.y = m_anchorPoint.y;
        }
    } else {
        m_anchorPoint = m_center;
    }

    SetPosition();
    Update(MeaUpdateReason::NormalUpdate);
}

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
#include "LineTool.h"
#include "ToolMgr.h"
#include <meazure/utilities/Geometry.h>
#include <meazure/utilities/StringUtils.h>
#include <meazure/resource.h>
#include <meazure/graphics/Colors.h>
#include <cassert>


MeaLineTool::MeaLineTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                         const MeaUnitsProvider& unitsProvider) :
    MeaRadioTool(mgr, screenProvider, unitsProvider),
    MeaCrossHairCallback(),
    m_curPos(&m_point1),
    m_point1CH(screenProvider, unitsProvider),
    m_point2CH(screenProvider, unitsProvider),
    m_dataWin1(screenProvider, unitsProvider),
    m_dataWin2(screenProvider, unitsProvider) {
    // Set the default tool positions. The line is initially
    // placed at the center of the screen containing the application.
    //
    m_point1 = m_screenProvider.GetCenter();
    m_point2 = m_point1;

    // Offset the end points to initially give the line some length.
    //
    m_point1.Offset(-30, -30);
    m_point2.Offset(30, 30);
}

MeaLineTool::~MeaLineTool() {
    try {
        Disable();
        m_curPos = nullptr;
    } catch (...) {
        assert(false);
    }
}

bool MeaLineTool::Create() {
    // Create the crosshairs.
    //
    if (!m_point1CH.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                           MeaColors::Get(MeaColors::CrossHairBack),
                           MeaColors::Get(MeaColors::CrossHairHilite),
                           MeaColors::GetA(MeaColors::CrossHairOpacity),
                           this, nullptr, IDS_MEA_POINT1, kPoint1Id)) {
        return false;
    }
    if (!m_point2CH.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                           MeaColors::Get(MeaColors::CrossHairBack),
                           MeaColors::Get(MeaColors::CrossHairHilite),
                           MeaColors::GetA(MeaColors::CrossHairOpacity),
                           this, nullptr, IDS_MEA_POINT2, kPoint2Id)) {
        return false;
    }

    // Create the line connecting the crosshairs.
    //
    if (!m_line.Create(3, m_screenProvider)) {
        return false;
    }

    // Ensure that the crosshairs are visually on top of the line.
    //
    m_point1CH.SetAbove(&m_line);
    m_point2CH.SetAbove(&m_line);

    // Create the data windows attached to each crosshair.
    //
    m_dataWin1.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X1, IDS_MEA_Y1, MeaDataWin::kNoLabelId,
                      MeaDataWin::kNoLabelId, IDS_MEA_LENGTH);
    m_dataWin2.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X2, IDS_MEA_Y2, MeaDataWin::kNoLabelId,
                      MeaDataWin::kNoLabelId, IDS_MEA_LENGTH);

    // Position the crosshairs and line based
    // on the values of the points.
    //
    SetPosition();

    return true;
}

void MeaLineTool::SaveProfile(MeaProfile& profile) {
    MeaFPoint pt;

    // Save the position of each end point.
    //
    pt = m_unitsProvider.ConvertPos(m_point1);
    profile.WriteStr(_T("LineX1"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("LineY1"), MeaStringUtils::DblToStr(pt.y));
    pt = m_unitsProvider.ConvertPos(m_point2);
    profile.WriteStr(_T("LineX2"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("LineY2"), MeaStringUtils::DblToStr(pt.y));
}

void MeaLineTool::LoadProfile(MeaProfile& profile) {
    // Use the current positions as the default values
    // for those positions that are not specified in the
    // profile.
    //
    MeaFPoint def1 = m_unitsProvider.ConvertPos(m_point1);
    MeaFPoint def2 = m_unitsProvider.ConvertPos(m_point2);

    // Load the end point positions.
    //
    MeaFPoint pt;
    pt.x = profile.ReadDbl(_T("LineX1"), def1.x);
    pt.y = profile.ReadDbl(_T("LineY1"), def1.y);
    m_point1 = m_unitsProvider.UnconvertPos(pt);
    pt.x = profile.ReadDbl(_T("LineX2"), def2.x);
    pt.y = profile.ReadDbl(_T("LineY2"), def2.y);
    m_point2 = m_unitsProvider.UnconvertPos(pt);

    SetPosition();
}

void MeaLineTool::EnableCrosshairs() {
    if (m_mgr.CrosshairsEnabled() && IsWindow(m_point1CH)) {
        m_point1CH.Show();
        m_point2CH.Show();
    }
}

void MeaLineTool::DisableCrosshairs() {
    if (IsWindow(m_point1CH)) {
        m_point1CH.Hide();
        m_point2CH.Hide();
    }
}

void MeaLineTool::Flash() {
    m_point1CH.Flash();
    m_point2CH.Flash();
}

void MeaLineTool::Strobe() {
    m_point1CH.Flash(MeaCrossHair::kStrobeCount);
    m_point2CH.Flash(MeaCrossHair::kStrobeCount);
    m_dataWin1.Strobe();
    m_dataWin2.Strobe();
}

void MeaLineTool::Enable() {
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    // Tell the tool manager which data display fields
    // we will be using.
    //
    m_mgr.EnableRegionFields(MeaX1Field | MeaY1Field |
                             MeaX2Field | MeaY2Field |
                             MeaWidthField | MeaHeightField |
                             MeaDistanceField | MeaAngleField |
                             MeaAspectField | MeaAreaField,
                             MeaX1Field | MeaY1Field |
                             MeaX2Field | MeaY2Field);

    if (!IsWindow(m_line)) {
        Create();
    }

    // Show one liner on how to use the tool.
    //
    m_mgr.SetStatus(IDS_MEA_LINE_STATUS);

    // Make the line and crosshairs visible, flash
    // the crosshairs and update the data display.
    //
    m_line.Show();
    EnableCrosshairs();
    Flash();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaLineTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    m_line.Hide();
    DisableCrosshairs();
}

void MeaLineTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        // Convert the pixel locations to the current units.
        //
        MeaFPoint p1 = m_unitsProvider.ConvertCoord(m_point1);
        MeaFPoint p2 = m_unitsProvider.ConvertCoord(m_point2);
        MeaFSize wh = m_unitsProvider.GetWidthHeight(m_point1, m_point2);

        // Display the results of the measurement in
        // the data display fields.
        //
        m_mgr.ShowXY1(m_point1, p1);
        m_mgr.ShowXY2(m_point2, p2);
        m_mgr.ShowWH(wh);
        m_mgr.ShowDistance(wh);
        m_mgr.ShowAngle(MeaGeometry::CalcAngle(p1, p2));
        m_mgr.ShowAspect(wh);
        m_mgr.ShowRectArea(wh);

        // The screen information depends on the
        // current position.
        //
        m_mgr.UpdateScreenInfo(*m_curPos);

        // Display the results of the measurement in
        // the crosshair data windows.
        //
        m_dataWin1.ShowXY(p1);
        m_dataWin1.ShowDistance(wh);
        m_dataWin1.Update(m_point1CH);

        m_dataWin2.ShowXY(p2);
        m_dataWin2.ShowDistance(wh);
        m_dataWin2.Update(m_point2CH);
    }
}

void MeaLineTool::SetPosition(MeaDataFieldId which, int pixels) {
    // Set the specified position component.
    //
    switch (which) {
    case MeaX1Field:
        m_point1.x = pixels;
        break;
    case MeaY1Field:
        m_point1.y = pixels;
        break;
    case MeaX2Field:
        m_point2.x = pixels;
        break;
    case MeaY2Field:
        m_point2.y = pixels;
        break;
    default:
        return;
    }

    // Reposition the tool based on the crosshair locations.
    //
    if (which & MeaX1Field || which & MeaY1Field) {
        m_point1 = m_screenProvider.LimitPosition(m_point1);
        m_point1CH.SetPosition(m_point1);
        m_curPos = &m_point1;
    } else {
        m_point2 = m_screenProvider.LimitPosition(m_point2);
        m_point2CH.SetPosition(m_point2);
        m_curPos = &m_point2;
    }
    m_line.SetPosition(m_point1, m_point2);

    // Update the data display.
    //
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaLineTool::SetPosition(const PointMap& points) {
    PointMap::const_iterator iter;

    // Read the position information from the points
    // map, and set the corresponding point.
    //
    iter = points.find(_T("1"));
    if (iter != points.end()) {
        m_point1 = (*iter).second;
    }
    iter = points.find(_T("2"));
    if (iter != points.end()) {
        m_point2 = (*iter).second;
    }

    // Reposition the tool and update the data display.
    //
    SetPosition();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaLineTool::SetPosition() {
    // Ensure that the positions fall within the
    // limits of the screen containing the point.
    //
    m_point1 = m_screenProvider.LimitPosition(m_point1);
    m_point2 = m_screenProvider.LimitPosition(m_point2);

    // Reposition the tool.
    //
    if (IsWindow(m_line)) {
        m_point1CH.SetPosition(m_point1);
        m_point2CH.SetPosition(m_point2);
        m_line.SetPosition(m_point1, m_point2);
    }
}

const POINT& MeaLineTool::GetPosition() const {
    return *m_curPos;
}

void MeaLineTool::RecordPosition(MeaPosition& position) const {
    // Convert the pixel locations to the current units.
    //
    MeaFPoint p1 = m_unitsProvider.ConvertCoord(m_point1);
    MeaFPoint p2 = m_unitsProvider.ConvertCoord(m_point2);
    MeaFSize wh = m_unitsProvider.GetWidthHeight(m_point1, m_point2);

    // Save the positions and the name of this tool in the position object.
    //
    position.SetToolName(kToolName);
    position.RecordXY1(p1);
    position.RecordXY2(p2);
    position.RecordWH(wh);
    position.RecordDistance(wh);
    position.RecordRectArea(wh);

    double angle = MeaGeometry::CalcAngle(p1, p2);
    position.RecordAngle(m_unitsProvider.ConvertAngle(angle));
}

void MeaLineTool::IncPosition(MeaDataFieldId which) {
    switch (which) {
    case MeaX1Field:
        SetPosition(which, m_point1.x + 1);
        break;
    case MeaY1Field:
        SetPosition(which, m_point1.y + 1);
        break;
    case MeaX2Field:
        SetPosition(which, m_point2.x + 1);
        break;
    case MeaY2Field:
        SetPosition(which, m_point2.y + 1);
        break;
    default:
        break;
    }
}

void MeaLineTool::DecPosition(MeaDataFieldId which) {
    switch (which) {
    case MeaX1Field:
        SetPosition(which, m_point1.x - 1);
        break;
    case MeaY1Field:
        SetPosition(which, m_point1.y - 1);
        break;
    case MeaX2Field:
        SetPosition(which, m_point2.x - 1);
        break;
    case MeaY2Field:
        SetPosition(which, m_point2.y - 1);
        break;
    default:
        break;
    }
}

void MeaLineTool::ColorsChanged() {
    // Set the crosshair colors.
    //
    m_point1CH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                         MeaColors::Get(MeaColors::CrossHairBack),
                         MeaColors::Get(MeaColors::CrossHairHilite));
    m_point2CH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                         MeaColors::Get(MeaColors::CrossHairBack),
                         MeaColors::Get(MeaColors::CrossHairHilite));

    // Set the opacity of the crosshairs.
    //
    m_point1CH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_point2CH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    // Set the opacity of the data windows.
    //
    m_dataWin1.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_dataWin2.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    // Set the color of the line.
    //
    m_line.SetColor(MeaColors::Get(MeaColors::LineFore));
}

PCTSTR MeaLineTool::GetToolName() const {
    return kToolName;
}

UINT MeaLineTool::GetLabelId() const {
    return IDS_MEA_LINE;
}

void MeaLineTool::OnCHEnter(const CrossHairInfo* info) {
    // Show the data window corresponding to
    // the crosshair that the pointer has entered.
    //
    if (info->id == kPoint1Id) {
        m_dataWin1.Show();
    } else {
        m_dataWin2.Show();
    }
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaLineTool::OnCHLeave(const CrossHairInfo* info) {
    // Hide the data window corresponding to
    // the crosshair that the pointer has just
    // left.
    //
    if (info->id == kPoint1Id) {
        m_dataWin1.Hide();
    } else {
        m_dataWin2.Hide();
    }
}

void MeaLineTool::OnCHMove(const CrossHairInfo* info) {
    // Ctrl + drag moves the all the crosshairs
    // as a single unit.
    //
    if (info->flags & MK_CONTROL) {
        CSize movingDelta;
        CPoint followingPos;

        if (info->id == kPoint1Id) {
            movingDelta = info->center - m_point1;
            followingPos = m_point2 + movingDelta;
        } else {
            movingDelta = info->center - m_point2;
            followingPos = m_point1 + movingDelta;
        }

        movingDelta -= followingPos - m_screenProvider.LimitPosition(followingPos);
        m_point1 += movingDelta;
        m_point2 += movingDelta;

        m_point1CH.SetPosition(m_point1);
        m_point2CH.SetPosition(m_point2);
    }
    // Normally just move the crosshair being dragged.
    //
    else {
        CPoint& movingPoint = (info->id == kPoint1Id) ? m_point1 : m_point2;
        CPoint& fixedPoint = (info->id != kPoint1Id) ? m_point1 : m_point2;

        movingPoint = info->center;

        // Shift + drag locks the movement of the crosshair
        // to vertical or horizontal.
        //
        if (info->flags & MK_SHIFT) {
            if (MeaGeometry::IsVerticallyOriented(m_point1, m_point2)) {
                movingPoint.x = fixedPoint.x;
            } else {
                movingPoint.y = fixedPoint.y;
            }
        }

        if (info->id == kPoint1Id) {
            m_point1CH.SetPosition(m_point1);
            m_curPos = &m_point1;
        } else {
            m_point2CH.SetPosition(m_point2);
            m_curPos = &m_point2;
        }
    }

    // Only reposition the line if the end points have moved.
    //
    POINT p1, p2;
    m_line.GetPosition(p1, p2);
    if (m_point1 != p1 || m_point2 != p2) {
        m_line.SetPosition(m_point1, m_point2);
    }

    Update(MeaUpdateReason::NormalUpdate);
}

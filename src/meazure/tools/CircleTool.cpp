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
#include "CircleTool.h"
#include "ToolMgr.h"
#include <meazure/utilities/Geometry.h>
#include <meazure/utilities/StringUtils.h>
#include <meazure/resource.h>
#include <meazure/graphics/Colors.h>
#include <cassert>


const CString   MeaCircleTool::kToolName(_T("CircleTool"));


MeaCircleTool::MeaCircleTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                             const MeaUnitsProvider& unitsProvider) :
    MeaRadioTool(mgr, screenProvider, unitsProvider),
    MeaCrossHairCallback(),
    m_curPos(&m_center),
    m_center(screenProvider.GetCenter()),
    m_centerCH(screenProvider, unitsProvider),
    m_perimeterCH(screenProvider, unitsProvider),
    m_dataWinCenter(screenProvider, unitsProvider),
    m_dataWinPerimeter(screenProvider, unitsProvider) {
    // Set the default tool positions. The center of the circle
    // is placed at the center of the screen containing the application.
    //
    m_perimeter = m_center;

    // The radius is drawn at a 45 degree angle a small distance.
    //
    m_perimeter.Offset(30, 30);

    m_anchorCenter = m_center;
    m_anchorPerimeter = m_perimeter;
}

MeaCircleTool::~MeaCircleTool() {
    try {
        Disable();
        m_curPos = nullptr;
    } catch (...) {
        assert(false);
    }
}

bool MeaCircleTool::Create() {
    // Create the crosshairs.
    //
    if (!m_centerCH.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                           MeaColors::Get(MeaColors::CrossHairBack),
                           MeaColors::Get(MeaColors::CrossHairHilite),
                           MeaColors::GetA(MeaColors::CrossHairOpacity),
                           this, nullptr, IDS_MEA_VERTEX, kCenterId)) {
        return false;
    }
    if (!m_perimeterCH.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                              MeaColors::Get(MeaColors::CrossHairBack),
                              MeaColors::Get(MeaColors::CrossHairHilite),
                              MeaColors::GetA(MeaColors::CrossHairOpacity),
                              this, nullptr, IDS_MEA_POINT1, kPerimeterId)) {
        return false;
    }

    // Create the circle and the radial line.
    //
    if (!m_circle.Create(m_screenProvider)) {
        return false;
    }
    if (!m_line.Create(3, m_screenProvider)) {
        return false;
    }

    // Ensure that the crosshairs are visually on top of the
    // lines.
    //
    m_centerCH.SetAbove(&m_line);
    m_perimeterCH.SetAbove(&m_line);
    m_perimeterCH.SetAbove(&m_circle);

    // Create the data windows attached to each crosshair.
    //
    m_dataWinCenter.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_XV, IDS_MEA_YV,
                           MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, IDS_MEA_LENGTH);
    m_dataWinPerimeter.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X1, IDS_MEA_Y1,
                              MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, IDS_MEA_LENGTH);

    // Position the crosshairs and circle and line based
    // on the values of the center and perimeter points.
    //
    SetPosition();

    return true;
}

void MeaCircleTool::SaveProfile(MeaProfile& profile) {
    FPOINT pt;

    // Save the position of the center and perimeter points.
    //
    pt = m_unitsProvider.ConvertPos(m_center);
    profile.WriteStr(_T("CircleXV"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("CircleYV"), MeaStringUtils::DblToStr(pt.y));

    pt = m_unitsProvider.ConvertPos(m_perimeter);
    profile.WriteStr(_T("CircleX1"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("CircleY1"), MeaStringUtils::DblToStr(pt.y));
}

void MeaCircleTool::LoadProfile(MeaProfile& profile) {
    // Use the current positions as the default values
    // for those positions that are not specified in the
    // profile.
    //
    FPOINT defC = m_unitsProvider.ConvertPos(m_center);
    FPOINT defP = m_unitsProvider.ConvertPos(m_perimeter);

    FPOINT pt;
    pt.x = profile.ReadDbl(_T("CircleXV"), defC.x);
    pt.y = profile.ReadDbl(_T("CircleYV"), defC.y);
    m_center = m_unitsProvider.UnconvertPos(pt);
    
    pt.x = profile.ReadDbl(_T("CircleX1"), defP.x);
    pt.y = profile.ReadDbl(_T("CircleY1"), defP.y);
    m_perimeter = m_unitsProvider.UnconvertPos(pt);

    m_anchorCenter = m_center;
    m_anchorPerimeter = m_perimeter;

    SetPosition();
}

void MeaCircleTool::EnableCrosshairs() {
    if (m_mgr.CrosshairsEnabled() && IsWindow(m_centerCH)) {
        m_centerCH.Show();
        m_perimeterCH.Show();
    }
}

void MeaCircleTool::DisableCrosshairs() {
    if (IsWindow(m_centerCH)) {
        m_centerCH.Hide();
        m_perimeterCH.Hide();
    }
}

void MeaCircleTool::Flash() {
    m_centerCH.Flash();
    m_perimeterCH.Flash();
}

void MeaCircleTool::Strobe() {
    m_centerCH.Flash(MeaCrossHair::kStrobeCount);
    m_perimeterCH.Flash(MeaCrossHair::kStrobeCount);
    m_dataWinCenter.Strobe();
    m_dataWinPerimeter.Strobe();
}

void MeaCircleTool::Enable() {
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    // Tell the tool manager which data display fields
    // we will be using.
    //
    m_mgr.EnableRegionFields(MeaX1Field | MeaY1Field |
                             MeaXVField | MeaYVField |
                             MeaWidthField | MeaHeightField |
                             MeaDistanceField | MeaAngleField |
                             MeaAreaField,
                             MeaX1Field | MeaY1Field |
                             MeaXVField | MeaYVField);

    if (!IsWindow(m_circle)) {
        Create();
    }

    // Show one liner on how to use the tool.
    //
    m_mgr.SetStatus(IDS_MEA_CIRCLE_STATUS);

    // Make the lines and crosshairs visible, flash
    // the crosshairs and update the data display.
    //
    m_circle.Show();
    m_line.Show();
    EnableCrosshairs();
    Flash();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaCircleTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    m_circle.Hide();
    m_line.Hide();
    DisableCrosshairs();

    m_dataWinCenter.Hide();
    m_dataWinPerimeter.Hide();
}

void MeaCircleTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        // Convert the pixel locations to the current
        // units.
        //
        FPOINT p1 = m_unitsProvider.ConvertCoord(m_center);
        FPOINT p2 = m_unitsProvider.ConvertCoord(m_perimeter);

        // Calculate the bounding square around the circle.
        //
        int radius = m_circle.GetRadius();
        CPoint topLeft(m_center.x - radius, m_center.y - radius);
        CPoint bottomRight(m_center.x + radius, m_center.y + radius);
        FSIZE wh = m_unitsProvider.GetWidthHeight(topLeft, bottomRight);

        // For the Circle tool the distance value is the
        // length of the radius.
        //
        double r = wh.cx / 2.0;

        // Display the results of the measurement in
        // the data display fields.
        //
        m_mgr.ShowXYV(m_center, p1);
        m_mgr.ShowXY1(m_perimeter, p2);
        m_mgr.ShowWH(wh);
        m_mgr.ShowDistance(r);
        m_mgr.ShowAngle(MeaGeometry::CalcAngle(p1, p2));
        m_mgr.ShowCircleArea(r);

        // The screen information depends on the
        // current position.
        //
        m_mgr.UpdateScreenInfo(*m_curPos);

        // Display the results of the measurement in
        // the crosshair data windows.
        //
        m_dataWinCenter.ShowXY(p1);
        m_dataWinCenter.ShowDistance(r);
        m_dataWinCenter.Update(m_centerCH);

        m_dataWinPerimeter.ShowXY(p2);
        m_dataWinPerimeter.ShowDistance(r);
        m_dataWinPerimeter.Update(m_perimeterCH);
    }
}

bool MeaCircleTool::HasRegion() {
    // There is always a rectangular region unless
    // the center and perimeter are coincident.
    //
    CRect rect = GetRegion();
    return (rect.Height() != 0) && (rect.Width() != 0);
}

RECT MeaCircleTool::GetRegion() {
    int radius = m_circle.GetRadius();

    CPoint topLeft(m_center.x - radius, m_center.y - radius);
    CPoint bottomRight(m_center.x + radius, m_center.y + radius);
    CRect rect(topLeft, bottomRight);

    rect.InflateRect(0, 0, 1, 1);

    return rect;
}

void MeaCircleTool::SetPosition(MeaFields which, int pixels) {
    // Set the specified position component.
    //
    switch (which) {
    case MeaXVField:
        m_center.x = pixels;
        break;
    case MeaYVField:
        m_center.y = pixels;
        break;
    case MeaX1Field:
        m_perimeter.x = pixels;
        break;
    case MeaY1Field:
        m_perimeter.y = pixels;
        break;
    default:
        return;
    }

    // Reposition the tool based on the crosshair locations.
    //
    if (which & MeaXVField || which & MeaYVField) {
        m_center = m_screenProvider.LimitPosition(m_center);
        m_centerCH.SetPosition(m_center);
        m_curPos = &m_center;
    } else {
        m_perimeter = m_screenProvider.LimitPosition(m_perimeter);
        m_perimeterCH.SetPosition(m_perimeter);
        m_curPos = &m_perimeter;
    }

    m_circle.SetPosition(m_center, m_perimeter);
    m_line.SetPosition(m_center, m_perimeter);

    // Update the data display.
    //
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaCircleTool::SetPosition(const PointMap& points) {
    PointMap::const_iterator iter;

    // Read the position information from the points
    // map, and set the corresponding point.
    //
    iter = points.find(_T("v"));
    if (iter != points.end()) {
        m_center = (*iter).second;
    }
    iter = points.find(_T("1"));
    if (iter != points.end()) {
        m_perimeter = (*iter).second;
    }

    // Reposition the tool and update the data display.
    //
    SetPosition();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaCircleTool::SetPosition() {
    // Ensure that the positions fall within the
    // limits of the screen containing the point.
    //
    m_center = m_screenProvider.LimitPosition(m_center);
    m_perimeter = m_screenProvider.LimitPosition(m_perimeter);

    // Reposition the tool.
    //
    if (IsWindow(m_circle)) {
        m_centerCH.SetPosition(m_center);
        m_perimeterCH.SetPosition(m_perimeter);
        m_circle.SetPosition(m_center, m_perimeter);
        m_line.SetPosition(m_center, m_perimeter);
    }
}

const POINT& MeaCircleTool::GetPosition() const {
    return *m_curPos;
}

void MeaCircleTool::GetPosition(MeaPositionLogMgr::Position& position) const {
    // Convert the pixel locations to the current units.
    //
    FPOINT p1 = m_unitsProvider.ConvertCoord(m_center);
    FPOINT p2 = m_unitsProvider.ConvertCoord(m_perimeter);

    int radius = m_circle.GetRadius();
    CPoint topLeft(m_center.x - radius, m_center.y - radius);
    CPoint bottomRight(m_center.x + radius, m_center.y + radius);
    FSIZE wh = m_unitsProvider.GetWidthHeight(topLeft, bottomRight);

    double r = wh.cx / 2.0;

    // Save the positions in the position object.
    //
    position.RecordXYV(p1);
    position.RecordXY1(p2);
    position.RecordWH(wh);
    position.RecordDistance(r);
    position.RecordAngle(MeaGeometry::CalcAngle(p1, p2));
    position.RecordCircleArea(r);
}

void MeaCircleTool::IncPosition(MeaFields which) {
    switch (which) {
    case MeaXVField:
        SetPosition(which, m_center.x + 1);
        break;
    case MeaYVField:
        SetPosition(which, m_center.y + 1);
        break;
    case MeaX1Field:
        SetPosition(which, m_perimeter.x + 1);
        break;
    case MeaY1Field:
        SetPosition(which, m_perimeter.y + 1);
        break;
    default:
        break;
    }
}

void MeaCircleTool::DecPosition(MeaFields which) {
    switch (which) {
    case MeaXVField:
        SetPosition(which, m_center.x - 1);
        break;
    case MeaYVField:
        SetPosition(which, m_center.y - 1);
        break;
    case MeaX1Field:
        SetPosition(which, m_perimeter.x - 1);
        break;
    case MeaY1Field:
        SetPosition(which, m_perimeter.y - 1);
        break;
    default:
        break;
    }
}

void MeaCircleTool::ColorsChanged() {
    // Set the crosshair colors.
    //
    m_centerCH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite));
    m_perimeterCH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite));

    // Set the opacity of the crosshairs.
    //
    m_centerCH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_perimeterCH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    // Set the opacity of the data windows.
    //
    m_dataWinCenter.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_dataWinPerimeter.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    // Set the color of the circle and radial lines.
    //
    m_circle.SetColor(MeaColors::Get(MeaColors::LineFore));
    m_line.SetColor(MeaColors::Get(MeaColors::LineFore));
}

CString MeaCircleTool::GetToolName() const {
    return kToolName;
}

UINT MeaCircleTool::GetLabelId() const {
    return IDS_MEA_CIRCLE;
}

void MeaCircleTool::OnCHEnter(const CHInfo* info) {
    // Show the data window corresponding to
    // the crosshair that the pointer has entered.
    //
    if (info->id == kCenterId) {
        m_dataWinCenter.Show();
    } else {
        m_dataWinPerimeter.Show();
    }
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaCircleTool::OnCHLeave(const CHInfo* info) {
    // Hide the data window corresponding to
    // the crosshair that the pointer has just
    // left.
    //
    if (info->id == kCenterId) {
        m_dataWinCenter.Hide();
    } else {
        m_dataWinPerimeter.Hide();
    }
}

void MeaCircleTool::OnCHMove(const CHInfo* info) {
    // Ctrl + drag moves the all the crosshairs
    // as a single unit.
    //
    if (info->flags & MK_CONTROL) {
        CSize movingDelta;
        CPoint followingPos;

        if (info->id == kCenterId) {
            movingDelta = info->centerPoint - m_center;
            followingPos = m_perimeter + movingDelta;
        } else {
            movingDelta = info->centerPoint - m_perimeter;
            followingPos = m_center + movingDelta;
        }

        movingDelta -= followingPos - m_screenProvider.LimitPosition(followingPos);
        m_center += movingDelta;
        m_perimeter += movingDelta;

        m_centerCH.SetPosition(m_center);
        m_perimeterCH.SetPosition(m_perimeter);
    }
    // Normally just move the crosshair being dragged.
    //
    else {
        CPoint& movingPoint = (info->id == kCenterId) ? m_center : m_perimeter;
        CPoint& fixedPoint = (info->id != kCenterId) ? m_center : m_perimeter;

        movingPoint = info->centerPoint;

        // Shift + drag locks the movement of the crosshair
        // to vertical or horizontal.
        //
        if (info->flags & MK_SHIFT) {
            if (MeaGeometry::IsVerticallyOriented(m_center, m_perimeter)) {
                movingPoint.x = fixedPoint.x;
            } else {
                movingPoint.y = fixedPoint.y;
            }
        }

        if (info->id == kCenterId) {
            m_centerCH.SetPosition(m_center);
            m_curPos = &m_center;
        } else {
            m_perimeterCH.SetPosition(m_perimeter);
            m_curPos = &m_perimeter;
        }
    }

    // Move the circle and radial lines to the current points.
    //
    m_circle.SetPosition(m_center, m_perimeter);
    m_line.SetPosition(m_center, m_perimeter);

    Update(MeaUpdateReason::NormalUpdate);
}

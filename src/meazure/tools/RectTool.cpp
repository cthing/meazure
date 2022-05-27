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
#include "RectTool.h"
#include "ToolMgr.h"
#include <meazure/utilities/Geometry.h>
#include <meazure/utilities/StringUtils.h>
#include <meazure/resource.h>
#include <meazure/graphics/Colors.h>
#include <cassert>


const CString MeaRectTool::kToolName(_T("RectTool"));


MeaRectTool::MeaRectTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                         const MeaUnitsProvider& unitsProvider) :
    MeaRadioTool(mgr, screenProvider, unitsProvider),
    MeaCrossHairCallback(),
    m_curPos(&m_point1),
    m_point1CH(screenProvider, unitsProvider),
    m_point2CH(screenProvider, unitsProvider),
    m_dataWin1(screenProvider, unitsProvider),
    m_dataWin2(screenProvider, unitsProvider) {
    // Set the default tool positions. The rectangle initially
    // is placed at the center of the screen containing the app.
    //
    m_point1 = m_screenProvider.GetCenter();
    m_point2 = m_point1;

    // Offset the end points to initially give the rectangle some length.
    //
    m_point1.Offset(-30, -30);
    m_point2.Offset(30, 30);

    m_anchorPoint1 = m_point1;
    m_anchorPoint2 = m_point2;
}

MeaRectTool::~MeaRectTool() {
    try {
        Disable();
        m_curPos = nullptr;
    } catch (...) {
        assert(false);
    }
}

bool MeaRectTool::Create() {
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

    // Create the rectangle.
    //
    if (!m_rectangle.Create(1, 12)) {
        return false;
    }

    // Ensure that the crosshairs are visually on top of the rectangle.
    //
    m_point1CH.SetAbove(&m_rectangle);
    m_point2CH.SetAbove(&m_rectangle);

    // Create the data windows attached to each crosshair.
    //
    m_dataWin1.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X1, IDS_MEA_Y1, IDS_MEA_WIDTH, IDS_MEA_HEIGHT);
    m_dataWin2.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X2, IDS_MEA_Y2, IDS_MEA_WIDTH, IDS_MEA_HEIGHT);

    // Position the crosshairs and rectangle based
    // on the values of the points.
    //
    SetPosition();

    return true;
}

void MeaRectTool::SaveProfile(MeaProfile& profile) {
    FPOINT pt;

    // Save the position of the each crosshairs.
    //
    pt = m_unitsProvider.ConvertPos(m_point1);
    profile.WriteStr(_T("RectX1"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("RectY1"), MeaStringUtils::DblToStr(pt.y));
    pt = m_unitsProvider.ConvertPos(m_point2);
    profile.WriteStr(_T("RectX2"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("RectY2"), MeaStringUtils::DblToStr(pt.y));
}

void MeaRectTool::LoadProfile(MeaProfile& profile) {
    // Use the current positions as the default values
    // for those positions that are not specified in the
    // profile.
    //
    FPOINT def1 = m_unitsProvider.ConvertPos(m_point1);
    FPOINT def2 = m_unitsProvider.ConvertPos(m_point2);

    // Load the crosshair positions.
    //
    FPOINT pt;
    pt.x = profile.ReadDbl(_T("RectX1"), def1.x);
    pt.y = profile.ReadDbl(_T("RectY1"), def1.y);
    m_point1 = m_unitsProvider.UnconvertPos(pt);
    pt.x = profile.ReadDbl(_T("RectX2"), def2.x);
    pt.y = profile.ReadDbl(_T("RectY2"), def2.y);
    m_point2 = m_unitsProvider.UnconvertPos(pt);

    m_anchorPoint1 = m_point1;
    m_anchorPoint2 = m_point2;

    SetPosition();
}

void MeaRectTool::EnableCrosshairs() {
    if (m_mgr.CrosshairsEnabled() && IsWindow(m_point1CH)) {
        m_point1CH.Show();
        m_point2CH.Show();
    }
}

void MeaRectTool::DisableCrosshairs() {
    if (IsWindow(m_point1CH)) {
        m_point1CH.Hide();
        m_point2CH.Hide();
    }
}

void MeaRectTool::Flash() {
    m_point1CH.Flash();
    m_point2CH.Flash();
}

void MeaRectTool::Strobe() {
    m_point1CH.Flash(MeaCrossHair::kStrobeCount);
    m_point2CH.Flash(MeaCrossHair::kStrobeCount);
    m_dataWin1.Strobe();
    m_dataWin2.Strobe();
}

void MeaRectTool::Enable() {
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

    if (!IsWindow(m_rectangle)) {
        Create();
    }

    // Show one liner on how to use the tool.
    //
    m_mgr.SetStatus(IDS_MEA_RECT_STATUS);

    // Make the rectangle and crosshairs visible, flash
    // the crosshairs and update the data display.
    //
    m_rectangle.Show();
    EnableCrosshairs();
    Flash();
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaRectTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    m_rectangle.Hide();
    DisableCrosshairs();

    m_dataWin1.Hide();
    m_dataWin2.Hide();
}

void MeaRectTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        // Convert the pixel locations to the current units.
        //
        FPOINT p1 = m_unitsProvider.ConvertCoord(m_point1);
        FPOINT p2 = m_unitsProvider.ConvertCoord(m_point2);
        FSIZE wh = m_unitsProvider.GetWidthHeight(m_point1, m_point2);

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
        m_dataWin1.ShowWH(wh);
        m_dataWin1.Update(m_point1CH);

        m_dataWin2.ShowXY(p2);
        m_dataWin2.ShowWH(wh);
        m_dataWin2.Update(m_point2CH);
    }
}

bool MeaRectTool::HasRegion() {
    CRect rect = GetRegion();
    return (rect.Height() != 0) && (rect.Width() != 0);
}

RECT MeaRectTool::GetRegion() {
    CRect rect(m_point1, m_point2);
    rect.NormalizeRect();
    rect.InflateRect(0, 0, 1, 1);

    return rect;
}

void MeaRectTool::SetPosition(MeaFields which, int pixels) {
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

    m_rectangle.SetPosition(m_point1, m_point2);

    // Update the data display.
    //
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaRectTool::SetPosition(const PointMap& points) {
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

void MeaRectTool::SetPosition() {
    // Ensure that the positions fall within the
    // limits of the screen containing the point.
    //
    m_point1 = m_screenProvider.LimitPosition(m_point1);
    m_point2 = m_screenProvider.LimitPosition(m_point2);

    // Reposition the tool.
    //
    if (IsWindow(m_rectangle)) {
        m_point1CH.SetPosition(m_point1);
        m_point2CH.SetPosition(m_point2);
        m_rectangle.SetPosition(m_point1, m_point2);
    }
}

const POINT& MeaRectTool::GetPosition() const {
    return *m_curPos;
}

void MeaRectTool::RecordPosition(MeaPosition& position) const {
    // Convert the pixel locations to the current units.
    //
    FPOINT p1 = m_unitsProvider.ConvertCoord(m_point1);
    FPOINT p2 = m_unitsProvider.ConvertCoord(m_point2);
    FSIZE wh = m_unitsProvider.GetWidthHeight(m_point1, m_point2);

    // Save the positions and the name of this tool in the position object.
    //
    position.SetToolName(kToolName);
    position.RecordXY1(p1);
    position.RecordXY2(p2);
    position.RecordWH(wh);
    position.RecordDistance(wh);
    position.RecordAngle(MeaGeometry::CalcAngle(p1, p2));
    position.RecordRectArea(wh);
}

void MeaRectTool::IncPosition(MeaFields which) {
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

void MeaRectTool::DecPosition(MeaFields which) {
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

void MeaRectTool::ColorsChanged() {
    // Redraw the crosshairs in the new colors.
    //
    m_point1CH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                         MeaColors::Get(MeaColors::CrossHairBack),
                         MeaColors::Get(MeaColors::CrossHairHilite));
    m_point2CH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                         MeaColors::Get(MeaColors::CrossHairBack),
                         MeaColors::Get(MeaColors::CrossHairHilite));

    // Set the crosshair and data window opacities.
    //
    m_point1CH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_point2CH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    m_dataWin1.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_dataWin2.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    // Set the rectangle line color.
    //
    m_rectangle.SetColor(MeaColors::Get(MeaColors::LineFore));
}

CString MeaRectTool::GetToolName() const {
    return kToolName;
}

UINT MeaRectTool::GetLabelId() const {
    return IDS_MEA_RECTANGLE;
}

void MeaRectTool::OnCHEnter(const CHInfo* info) {
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

void MeaRectTool::OnCHLeave(const CHInfo* info) {
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

void MeaRectTool::OnCHMove(const CHInfo* info) {
    // Ctrl + drag moves the all the crosshairs
    // as a single unit.
    //
    if (info->flags & MK_CONTROL) {
        CSize movingDelta;
        CPoint followingPos;

        if (info->id == kPoint1Id) {
            movingDelta = info->centerPoint - m_point1;
            followingPos = m_point2 + movingDelta;
        } else {
            movingDelta = info->centerPoint - m_point2;
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
        CPoint& anchorPoint = (info->id != kPoint1Id) ? m_anchorPoint1 : m_anchorPoint2;

        movingPoint = info->centerPoint;

        // Shift + drag locks the movement of the crosshair
        // to vertical or horizontal.
        //
        if (info->flags & MK_SHIFT) {
            if (MeaGeometry::IsVerticallyOriented(anchorPoint, movingPoint)) {
                movingPoint.x = anchorPoint.x;
            } else {
                movingPoint.y = anchorPoint.y;
            }
        } else {
            anchorPoint = movingPoint;
        }

        if (info->id == kPoint1Id) {
            m_point1CH.SetPosition(m_point1);
            m_curPos = &m_point1;
        } else {
            m_point2CH.SetPosition(m_point2);
            m_curPos = &m_point2;
        }
    }

    m_rectangle.SetPosition(m_point1, m_point2);

    Update(MeaUpdateReason::NormalUpdate);
}

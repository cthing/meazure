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

#include "StdAfx.h"
#include "AngleTool.h"
#include "Layout.h"
#include "Resource.h"
#include "Colors.h"
#include "ToolMgr.h"
#include "ScreenMgr.h"


const CString   MeaAngleTool::kToolName(_T("AngleTool"));
const double    MeaAngleTool::kLengthB  = 50.0;
const UINT      MeaAngleTool::kPoint1Id = 1;
const UINT      MeaAngleTool::kPoint2Id = 2;
const UINT      MeaAngleTool::kVertexId = 3;


MeaAngleTool::MeaAngleTool(MeaToolMgr* mgr) : MeaRadioTool(mgr),
    MeaCrossHairCallback(), m_curPos(&m_point1)
{
    // Set the default tool positions. The vertex is placed
    // at the center of the screen containing the application.
    //
    m_vertex = MeaScreenMgr::Instance().GetCenter();
    m_point1 = m_vertex;
    m_point2 = m_vertex;

    // Points 1 and 2 are positioned to form an equal
    // angle around the x-axis.
    //
    m_point1.Offset(80, -50);
    m_point2.Offset(80, 50);

    m_vertexAnchor = m_vertex;
}


MeaAngleTool::~MeaAngleTool()
{
    try {
        Disable();
        m_curPos = NULL;
    }
    catch(...) {
        MeaAssert(false);
    }
}


bool MeaAngleTool::Create()
{
    // Create the crosshairs.
    //
    if (!m_point1CH.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite),
                            MeaColors::GetA(MeaColors::CrossHairOpacity),
                            this, NULL, IDS_MEA_POINT1, kPoint1Id)) {
        return false;
    }
    if (!m_point2CH.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite),
                            MeaColors::GetA(MeaColors::CrossHairOpacity),
                            this, NULL, IDS_MEA_POINT2, kPoint2Id)) {
        return false;
    }
    if (!m_vertexCH.Create(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite),
                            MeaColors::GetA(MeaColors::CrossHairOpacity),
                            this, NULL, IDS_MEA_VERTEX, kVertexId)) {
        return false;
    }

    // Create the lines connecting the vertex to point 1 and
    // point 2 and also create the angle bisector.
    //
    if (!m_line1.Create(3)) {
        return false;
    }
    if (!m_line2.Create(3)) {
        return false;
    }
    if (!m_lineB.Create(3)) {
        return false;
    }

    // Ensure that the crosshairs are visually on top of the
    // lines.
    //
    m_point1CH.SetAbove(&m_line1);
    m_point1CH.SetAbove(&m_line2);
    m_point1CH.SetAbove(&m_lineB);
    m_point2CH.SetAbove(&m_line1);
    m_point2CH.SetAbove(&m_line2);
    m_point2CH.SetAbove(&m_lineB);
    m_vertexCH.SetAbove(&m_line1);
    m_vertexCH.SetAbove(&m_line2);
    m_vertexCH.SetAbove(&m_lineB);

    // Create the data windows attached to each crosshair.
    //
    m_dataWin1.Create(MeaColors::GetA(MeaColors::CrossHairOpacity),IDS_MEA_X1, IDS_MEA_Y1, MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, IDS_MEA_ANGLE);
    m_dataWin2.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X2, IDS_MEA_Y2, MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, IDS_MEA_ANGLE);
    m_dataWinV.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_XV, IDS_MEA_YV, MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, IDS_MEA_ANGLE);

    // Position the crosshairs and lines based on the values
    // of point 1, 2 and vertex.
    //
    SetPosition();

    return true;
}


void MeaAngleTool::SaveProfile(MeaProfile& profile)
{   
    FPOINT pt;
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    // Save the position of point 1, point 2 and the vertex.
    //
    pt = unitsMgr.ConvertPos(m_point1);
    profile.WriteStr(_T("AngleX1"), MeaUtils::DblToStr(pt.x));
    profile.WriteStr(_T("AngleY1"), MeaUtils::DblToStr(pt.y));
    pt = unitsMgr.ConvertPos(m_point2);
    profile.WriteStr(_T("AngleX2"), MeaUtils::DblToStr(pt.x));
    profile.WriteStr(_T("AngleY2"), MeaUtils::DblToStr(pt.y));
    pt = unitsMgr.ConvertPos(m_vertex);
    profile.WriteStr(_T("AngleXV"), MeaUtils::DblToStr(pt.x));
    profile.WriteStr(_T("AngleYV"), MeaUtils::DblToStr(pt.y));
}


void MeaAngleTool::LoadProfile(MeaProfile& profile)
{
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    // Use the current positions as the default values
    // for those positions that are not specified in
    // the profile.
    //
    FPOINT def1 = unitsMgr.ConvertPos(m_point1);
    FPOINT def2 = unitsMgr.ConvertPos(m_point2);
    FPOINT defV = unitsMgr.ConvertPos(m_vertex);

    // Load the position of point 1, point 2 and the vertex.
    //
    FPOINT pt;
    pt.x = profile.ReadDbl(_T("AngleX1"), def1.x);
    pt.y = profile.ReadDbl(_T("AngleY1"), def1.y);
    m_point1 = unitsMgr.UnconvertPos(pt);
    pt.x = profile.ReadDbl(_T("AngleX2"), def2.x);
    pt.y = profile.ReadDbl(_T("AngleY2"), def2.y);
    m_point2 = unitsMgr.UnconvertPos(pt);
    pt.x = profile.ReadDbl(_T("AngleXV"), defV.x);
    pt.y = profile.ReadDbl(_T("AngleYV"), defV.y);
    m_vertex = unitsMgr.UnconvertPos(pt);

    m_vertexAnchor = m_vertex;

    SetPosition();
}


void MeaAngleTool::EnableCrosshairs()
{
    if (m_mgr->CrosshairsEnabled() && IsWindow(m_point1CH)) {
        m_point1CH.Show();
        m_point2CH.Show();
        m_vertexCH.Show();
    }
}


void MeaAngleTool::DisableCrosshairs()
{
    if (IsWindow(m_point1CH)) {
        m_point1CH.Hide();
        m_point2CH.Hide();
        m_vertexCH.Hide();
    }
}


void MeaAngleTool::Flash()
{
    m_point1CH.Flash();
    m_point2CH.Flash();
    m_vertexCH.Flash();
}


void MeaAngleTool::Strobe()
{
    m_point1CH.Flash(MeaCrossHair::kStrobeCount);
    m_point2CH.Flash(MeaCrossHair::kStrobeCount);
    m_vertexCH.Flash(MeaCrossHair::kStrobeCount);
    m_dataWin1.Strobe();
    m_dataWin2.Strobe();
    m_dataWinV.Strobe();
}


void MeaAngleTool::Enable()
{
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    // Tell the tool manager which data display fields
    // we will be using.
    //
    m_mgr->EnableRegionFields(MeaX1Field | MeaY1Field |
                                MeaX2Field | MeaY2Field |
                                MeaXVField | MeaYVField |
                                MeaAngleField,
                                MeaX1Field | MeaY1Field |
                                MeaX2Field | MeaY2Field |
                                MeaXVField | MeaYVField);

    if (!IsWindow(m_line1)) {
        Create();
    }

    // Show one liner on how to use the tool.
    //
    m_mgr->SetStatus(IDS_MEA_ANGLE_STATUS);

    // Make the lines and crosshairs visible, flash
    // the crosshairs and update the data display.
    //
    m_line1.Show();
    m_line2.Show();
    m_lineB.Show();
    EnableCrosshairs();
    Flash();
    Update(MeaUpdateReason::NormalUpdate);
}


void MeaAngleTool::Disable()
{
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    m_line1.Hide();
    m_line2.Hide();
    m_lineB.Hide();
    DisableCrosshairs();
}


void MeaAngleTool::Update(MeaUpdateReason reason)
{
    if (IsEnabled()) {
        MeaTool::Update(reason);

        UpdateBisector();

        MeaUnitsMgr& units = MeaUnitsMgr::Instance();
        
        // Convert the pixel locations to the current
        // units.
        //
        FPOINT p1 = units.ConvertCoord(m_point1);
        FPOINT p2 = units.ConvertCoord(m_point2);
        FPOINT v = units.ConvertCoord(m_vertex);

        // Calculate the angle based on the locations
        // of the crosshairs in the current units.
        //
        double angle = MeaLayout::GetAngle(v, p1, p2);
        
        // Display the results of the measurement in
        // the data display fields.
        //
        m_mgr->ShowXY1(m_point1, p1);
        m_mgr->ShowXY2(m_point2, p2);
        m_mgr->ShowXYV(m_vertex, v);
        m_mgr->ShowAngle(angle);

        // The screen information depends on the
        // current position.
        //
        m_mgr->UpdateScreenInfo(*m_curPos);

        // Display the results of the measurement in
        // the crosshair data windows.
        //
        m_dataWin1.ShowXY(p1);
        m_dataWin1.ShowAngle(angle);
        m_dataWin1.Update(m_point1CH);

        m_dataWin2.ShowXY(p2);
        m_dataWin2.ShowAngle(angle);
        m_dataWin2.Update(m_point2CH);

        m_dataWinV.ShowXY(v);
        m_dataWinV.ShowAngle(angle);
        m_dataWinV.Update(m_vertexCH);
    }
}


void MeaAngleTool::SetPosition(MeaFields which, int pixels)
{
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
    case MeaXVField:
        m_vertex.x = pixels;
        break;
    case MeaYVField:
        m_vertex.y = pixels;
        break;
    default:
        return;
    }

    // Reposition the tool based on the crosshair locations.
    //
    if (which & MeaX1Field || which & MeaY1Field) {
        m_point1 = MeaScreenMgr::Instance().LimitPosition(m_point1);
        m_point1CH.SetPosition(m_point1);
        m_curPos = &m_point1;
        m_line1.SetPosition(m_vertex, m_point1);
    } else if (which & MeaX2Field || which & MeaY2Field) {
        m_point2 = MeaScreenMgr::Instance().LimitPosition(m_point2);
        m_point2CH.SetPosition(m_point2);
        m_curPos = &m_point2;
        m_line2.SetPosition(m_vertex, m_point2);
    } else {
        m_vertex = MeaScreenMgr::Instance().LimitPosition(m_vertex);
        m_vertexCH.SetPosition(m_vertex);
        m_curPos = &m_vertex;
        m_line1.SetPosition(m_vertex, m_point1);
        m_line2.SetPosition(m_vertex, m_point2);
    }

    // Update the data display.
    //
    Update(MeaUpdateReason::NormalUpdate);
}


void MeaAngleTool::SetPosition(const PointMap& points)
{
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
    iter = points.find(_T("v"));
    if (iter != points.end()) {
        m_vertex = (*iter).second;
    }
    
    // Reposition the tool and update the data display.
    //
    SetPosition();
    Update(MeaUpdateReason::NormalUpdate);
}


void MeaAngleTool::SetPosition()
{
    MeaScreenMgr& mgr = MeaScreenMgr::Instance();

    // Ensure that the positions fall within the
    // limits of the screen containing the point.
    //
    m_point1 = mgr.LimitPosition(m_point1);
    m_point2 = mgr.LimitPosition(m_point2);
    m_vertex = mgr.LimitPosition(m_vertex);

    // Reposition the tool.
    //
    if (IsWindow(m_line1)) {
        m_point1CH.SetPosition(m_point1);
        m_point2CH.SetPosition(m_point2);
        m_vertexCH.SetPosition(m_vertex);
        m_line1.SetPosition(m_vertex, m_point1);
        m_line2.SetPosition(m_vertex, m_point2);
    }
}


const POINT& MeaAngleTool::GetPosition() const
{
    return *m_curPos;
}


void MeaAngleTool::GetPosition(MeaPositionLogMgr::Position& position) const
{
    MeaUnitsMgr& units = MeaUnitsMgr::Instance();
    
    // Convert the pixel locations to the current units.
    //
    FPOINT p1 = units.ConvertCoord(m_point1);
    FPOINT p2 = units.ConvertCoord(m_point2);
    FPOINT v = units.ConvertCoord(m_vertex);

    // Save the positions in the position object.
    //
    position.RecordXY1(p1);
    position.RecordXY2(p2);
    position.RecordXYV(v);

    // Compute the angle and save it in the
    // position object.
    //
    double angle = MeaLayout::GetAngle(v, p1, p2);
    position.RecordAngle(angle);
}


void MeaAngleTool::IncPosition(MeaFields which)
{
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
    case MeaXVField:
        SetPosition(which, m_vertex.x + 1);
        break;
    case MeaYVField:
        SetPosition(which, m_vertex.y + 1);
        break;
    default:
        break;
    }
}


void MeaAngleTool::DecPosition(MeaFields which)
{
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
    case MeaXVField:
        SetPosition(which, m_vertex.x - 1);
        break;
    case MeaYVField:
        SetPosition(which, m_vertex.y - 1);
        break;
    default:
        break;
    }
}


void MeaAngleTool::ColorsChanged()
{
    // Set the crosshair colors.
    //
    m_point1CH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite));
    m_point2CH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite));
    m_vertexCH.SetColors(MeaColors::Get(MeaColors::CrossHairBorder),
                            MeaColors::Get(MeaColors::CrossHairBack),
                            MeaColors::Get(MeaColors::CrossHairHilite));

    // Set the opacity of the crosshairs.
    //
    m_point1CH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_point2CH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_vertexCH.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    // Set the opacity of the data windows.
    //
    m_dataWin1.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_dataWin2.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
    m_dataWinV.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));

    // Set the color of the lines.
    //
    m_line1.SetColor(MeaColors::Get(MeaColors::LineFore));
    m_line2.SetColor(MeaColors::Get(MeaColors::LineFore));
    m_lineB.SetColor(MeaColors::Get(MeaColors::LineFore));
}


CString MeaAngleTool::GetToolName() const
{
    return kToolName;
}


UINT MeaAngleTool::GetLabelId() const
{
    return IDS_MEA_ANGLE_LBL;
}


void MeaAngleTool::OnCHEnter(const CHInfo *info)
{
    // Show the data window corresponding to
    // the crosshair that the pointer has entered.
    //
    switch (info->id) {
    case kPoint1Id:
        m_dataWin1.Show();
        break;
    case kPoint2Id:
        m_dataWin2.Show();
        break;
    case kVertexId:
        m_dataWinV.Show();
        break;
    default:
        MeaAssert(false);
        break;
    }

    Update(MeaUpdateReason::NormalUpdate);
}


void MeaAngleTool::OnCHLeave(const CHInfo *info)
{
    // Hide the data window corresponding to
    // the crosshair that the pointer has just
    // left.
    //
    switch (info->id) {
    case kPoint1Id:
        m_dataWin1.Hide();
        break;
    case kPoint2Id:
        m_dataWin2.Hide();
        break;
    case kVertexId:
        m_dataWinV.Hide();
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void MeaAngleTool::OnCHMove(const CHInfo *info)
{
    // Ctrl + drag moves the all the crosshairs
    // as a single unit.
    //
    if (info->flags & MK_CONTROL) {
        CSize movingDelta;
        CPoint followingPos1;
        CPoint followingPos2;

        if (info->id == kPoint1Id) {
            movingDelta = info->centerPoint - m_point1;
            followingPos1 = m_point2 + movingDelta;
            followingPos2 = m_vertex + movingDelta;
        } else if (info->id == kPoint2Id) {
            movingDelta = info->centerPoint - m_point2;
            followingPos1 = m_point1 + movingDelta;
            followingPos2 = m_vertex + movingDelta;
        } else {
            movingDelta = info->centerPoint - m_vertex;
            followingPos1 = m_point1 + movingDelta;
            followingPos2 = m_point2 + movingDelta;
        }

        CSize d1 = followingPos1 - MeaScreenMgr::Instance().LimitPosition(followingPos1);
        CSize d2 = followingPos2 - MeaScreenMgr::Instance().LimitPosition(followingPos2);

        CSize d;
        d.cx = (abs(d1.cx) < abs(d2.cx)) ? d2.cx : d1.cx;
        d.cy = (abs(d1.cy) < abs(d2.cy)) ? d2.cy : d1.cy;

        movingDelta -= d;

        m_point1 += movingDelta;
        m_point2 += movingDelta;
        m_vertex += movingDelta;

        m_point1CH.SetPosition(m_point1);
        m_point2CH.SetPosition(m_point2);
        m_vertexCH.SetPosition(m_vertex);
    }
    // Normally just move the crosshair being dragged.
    //
    else {
        if (info->id == kPoint1Id) {
            m_point1 = info->centerPoint;

            // Shift + drag locks the movement of the crosshair
            // to vertical or horizontal.
            //
            if (info->flags & MK_SHIFT) {
                switch (MeaLayout::GetSector(m_vertex, m_point1)) {
                case 1:
                case -1:
                case 2:
                case -2:
                    m_point1.x = m_vertex.x;
                    break;
                default:
                    m_point1.y = m_vertex.y;
                    break;
                }
            }

            m_point1CH.SetPosition(m_point1);
            m_curPos = &m_point1;
        } else if (info->id == kPoint2Id) {
            m_point2 = info->centerPoint;

            // Shift + drag locks the movement of the crosshair
            // to vertical or horizontal.
            //
            if (info->flags & MK_SHIFT) {
                switch (MeaLayout::GetSector(m_vertex, m_point2)) {
                case 1:
                case -1:
                case 2:
                case -2:
                    m_point2.x = m_vertex.x;
                    break;
                default:
                    m_point2.y = m_vertex.y;
                    break;
                }
            }

            m_point2CH.SetPosition(m_point2);
            m_curPos = &m_point2;
        } else {
            m_vertex = info->centerPoint;

            // Shift + drag locks the movement of the crosshair
            // to vertical or horizontal.
            //
            if (info->flags & MK_SHIFT) {
                switch (MeaLayout::GetSector(m_vertexAnchor, m_vertex)) {
                case 1:
                case -1:
                case 2:
                case -2:
                    m_vertex.x = m_vertexAnchor.x;
                    break;
                default:
                    m_vertex.y = m_vertexAnchor.y;
                    break;
                }
            } else {
                m_vertexAnchor = m_vertex;
            }

            m_vertexCH.SetPosition(m_vertex);
            m_curPos = &m_vertex;
        }
    }

    POINT p1, p2;

    // Move the lines to the current points
    //
    m_line1.GetPosition(p1, p2);
    if (m_vertex != p1 || m_point1 != p2) {
        m_line1.SetPosition(m_vertex, m_point1);
    }

    m_line2.GetPosition(p1, p2);
    if (m_vertex != p1 || m_point2 != p2) {
        m_line2.SetPosition(m_vertex, m_point2);
    }

    Update(MeaUpdateReason::NormalUpdate);
}


void MeaAngleTool::UpdateBisector()
{   
    MeaUnitsMgr& units = MeaUnitsMgr::Instance();

    // Angles are calculated based on the converted
    // positions so that screen resolutions are taken
    // into account.
    //
    FPOINT p1 = units.ConvertCoord(m_point1);
    FPOINT p2 = units.ConvertCoord(m_point2);
    FPOINT v = units.ConvertCoord(m_vertex);

    // The bisector angle is the average of the angle made by
    // each line relative to the x-axis.
    //
    // Bisector angle = (angle 1 + angle 2) / 2
    //
    double alphaB = (MeaLayout::GetAngle(v, p1) + MeaLayout::GetAngle(v, p2)) / 2.0;

    if (units.GetInvertY()) {
        alphaB = -alphaB;
    }

    // Ensure that the bisector is on the acute or obtuse side of the angle.
    //
    CPoint pointB(m_vertex.x + static_cast<LONG>(kLengthB * cos(alphaB)),
                  m_vertex.y + static_cast<LONG>(kLengthB * sin(alphaB)));
    FPOINT pB = units.ConvertCoord(pointB);

    bool angleSign    = MeaLayout::GetAngle(v, p1, p2) >= 0.0;
    bool bisectorSign = MeaLayout::GetAngle(v, p1, pB) >= 0.0;

    // If we need to flip the bisector, add 180 degrees and recalculate
    // its location.
    //
    if ((angleSign && !bisectorSign) || (!angleSign && bisectorSign)) {
        alphaB = alphaB + MeaUnits::kPI;
        pointB.x = m_vertex.x + static_cast<LONG>(kLengthB * cos(alphaB));
        pointB.y = m_vertex.y + static_cast<LONG>(kLengthB * sin(alphaB));
    }

    // Position the bisector line based on the calculated point.
    //
    m_lineB.SetPosition(m_vertex, pointB);
}

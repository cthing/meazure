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
#include "OriginTool.h"
#include "ToolMgr.h"
#include <meazure/graphics/Colors.h>
#include <cassert>


const CString MeaOriginTool::kToolName(_T("OriginTool"));


MeaOriginTool::MeaOriginTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                             const MeaUnitsProvider& unitsProvider) : 
    MeaTool(mgr, screenProvider, unitsProvider) {}

MeaOriginTool::~MeaOriginTool() {
    try {
        Disable();
    } catch (...) {
        assert(false);
    }
}

bool MeaOriginTool::Create() {
    // Create the line for each axis.
    //
    if (!m_xAxis.Create(0, m_screenProvider))
        return false;
    if (!m_yAxis.Create(0, m_screenProvider))
        return false;

    return true;
}

void MeaOriginTool::SaveProfile(MeaProfile& profile) {
    profile.WriteBool(_T("OriginMarker"), IsEnabled());
}

void MeaOriginTool::LoadProfile(MeaProfile& profile) {
    if (profile.ReadBool(_T("OriginMarker"), kShowMarker))
        Enable();
    else
        Disable();
}

void MeaOriginTool::MasterReset() {
#pragma warning(disable: 4127)
    if (kShowMarker) {
        Enable();
    } else {
        Disable();
    }
#pragma warning(default: 4127)
}

void MeaOriginTool::Enable() {
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    if (!IsWindow(m_xAxis)) {
        Create();
    }

    m_xAxis.Show();
    m_yAxis.Show();

    Update(MeaUpdateReason::NormalUpdate);
}

void MeaOriginTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    m_xAxis.Hide();
    m_yAxis.Hide();
}

void MeaOriginTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        POINT origin = m_unitsProvider.GetOrigin();
        bool inverted = m_unitsProvider.IsInvertY();

        if (inverted && (origin.x == 0) && (origin.y == 0)) {
            origin.y = m_screenProvider.GetVirtualRect().Height() - 1;
        }

        // The length of the axes are expressed in inches and are
        // converted to pixels based on the resolution of the display
        // containing the origin.
        //
        MeaFSize res = m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter(origin));
        SIZE length = m_unitsProvider.ConvertToPixels(MeaInchesId, res, 0.25, 10);

        CPoint xEnd(origin.x + length.cx, origin.y);
        CPoint yEnd(origin.x, origin.y + (inverted ? -length.cy : length.cy));

        m_xAxis.SetPosition(origin, xEnd);
        m_yAxis.SetPosition(origin, yEnd);
    }
}

void MeaOriginTool::ColorsChanged() {
    // Redraw the axes in the new line color.
    //
    m_xAxis.SetColor(MeaColors::Get(MeaColors::LineFore));
    m_yAxis.SetColor(MeaColors::Get(MeaColors::LineFore));
}

CString MeaOriginTool::GetToolName() const {
    return kToolName;
}

const POINT& MeaOriginTool::GetPosition() const {
    return m_unitsProvider.GetOrigin();
}

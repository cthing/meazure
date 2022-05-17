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
#include "ScreenTool.h"
#include "ToolMgr.h"


const CString MeaScreenTool::kToolName(_T("ScreenTool"));


MeaScreenTool::MeaScreenTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                             const MeaUnitsProvider& unitsProvider) :
    MeaTool(mgr, screenProvider, unitsProvider), m_rect(0, 0, 0, 0), m_anyOSRes(true) {
    m_res.cx = 0.0;
    m_res.cy = 0.0;
}

MeaScreenTool::~MeaScreenTool() {
    try {
        Disable();
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaScreenTool::LoadProfile(MeaProfile& /*profile*/) {
    // The LoadProfile method is called early in the application
    // startup process. By calling SetPosition from here we can
    // point the tool at the screen on which the application is
    // running. This is a good first guess for initial screen
    // information.
    //
    CPoint pos(0, 0);
    SetPosition(pos);
}

void MeaScreenTool::Enable() {
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    // Tell the manager which display fields we are using.
    //
    m_mgr.EnableScreenFields(MeaScreenWidthField | MeaScreenHeightField | MeaResXField | MeaResYField, 0);
    Update(MeaUpdateReason::NormalUpdate);
}

void MeaScreenTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();
}

void MeaScreenTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        CPoint topLeft(m_rect.TopLeft());
        CPoint bottomRight(m_rect.BottomRight());
        bottomRight.Offset(-1, -1);

        m_mgr.ShowScreenName(m_name);
        m_mgr.ShowScreenWH(m_unitsProvider.GetWidthHeight(topLeft, bottomRight));
        m_mgr.ShowScreenRes(m_unitsProvider.ConvertRes(m_res));
        m_mgr.ShowCalButton(m_anyOSRes);
    }
}

CString MeaScreenTool::GetToolName() const {
    return kToolName;
}

void MeaScreenTool::SetPosition(const POINT& pos) {
    // Get the information for the screen which contains the specified position.
    //
    MeaScreenProvider::ScreenIter iter = m_screenProvider.GetScreenIter(pos);

    const CRect& rect = m_screenProvider.GetScreenRect(iter);
    const FSIZE& res = m_screenProvider.GetScreenRes(iter);
    bool anyOSRes = m_screenProvider.AnyOSRes();

    // Update the cached information, if the information cached in
    // the tool is different from the information for the screen
    // corresponding to the specified position.
    //
    if ((m_rect != rect) || !MEA_DBL_EQL(m_res.cx, res.cx) || !MEA_DBL_EQL(m_res.cy, res.cy) || (m_anyOSRes != anyOSRes)) {
        m_rect = rect;
        m_res = res;
        m_anyOSRes = m_screenProvider.AnyOSRes();
        m_name = m_screenProvider.GetScreenName(iter);

        Update(MeaUpdateReason::NormalUpdate);
    }
}

const POINT& MeaScreenTool::GetPosition() const {
    return m_defaultPos;
}

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
#include "MeaAssert.h"
#include "ToolMgr.h"
#include "StatusBar.h"
#include "ScreenMgr.h"
#include "UnitsMgr.h"


MeaToolMgr::MeaToolMgr(token) :
    MeaSingleton_T<MeaToolMgr>(),
    m_dataDisplay(nullptr),
    m_crosshairsEnabled(kDefCrosshairsEnabled) {
    const MeaScreenProvider& screenProvider = MeaScreenMgr::Instance();
    const MeaUnitsProvider& unitsProvider = MeaUnitsMgr::Instance();

    // Radio tools
    //
    m_cursorTool = new MeaCursorTool(*this, screenProvider, unitsProvider);
    m_pointTool = new MeaPointTool(*this, screenProvider, unitsProvider);
    m_lineTool = new MeaLineTool(*this, screenProvider, unitsProvider);
    m_rectTool = new MeaRectTool(*this, screenProvider, unitsProvider);
    m_circleTool = new MeaCircleTool(*this, screenProvider, unitsProvider);
    m_angleTool = new MeaAngleTool(*this, screenProvider, unitsProvider);
    m_windowTool = new MeaWindowTool(*this, screenProvider, unitsProvider);

    // Non-radio tools
    //
    m_screenTool = new MeaScreenTool(*this, screenProvider, unitsProvider);
    m_rulerTool = new MeaRulerTool(*this, screenProvider, unitsProvider);
    m_gridTool = new MeaGridTool(*this, screenProvider, unitsProvider);
    m_originTool = new MeaOriginTool(*this, screenProvider, unitsProvider);

    // Put the tools in the map.
    //
    m_tools[m_cursorTool->GetToolName()] = m_cursorTool;
    m_tools[m_pointTool->GetToolName()] = m_pointTool;
    m_tools[m_lineTool->GetToolName()] = m_lineTool;
    m_tools[m_rectTool->GetToolName()] = m_rectTool;
    m_tools[m_circleTool->GetToolName()] = m_circleTool;
    m_tools[m_angleTool->GetToolName()] = m_angleTool;
    m_tools[m_windowTool->GetToolName()] = m_windowTool;
    m_tools[m_screenTool->GetToolName()] = m_screenTool;
    m_tools[m_rulerTool->GetToolName()] = m_rulerTool;
    m_tools[m_gridTool->GetToolName()] = m_gridTool;
    m_tools[m_originTool->GetToolName()] = m_originTool;

    // Start with the cursor tool.
    //
    m_currentRadioTool = m_cursorTool;
}

MeaToolMgr::~MeaToolMgr() {
    try {
        for (const auto& toolEntry : m_tools) {
            delete toolEntry.second;
        }
        m_tools.clear();
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaToolMgr::SetPositionToCursor(MeaFields xfield, MeaFields yfield) {
    POINT point;
    GetCursorPos(&point);
    SetPosition(xfield, point.x);
    SetPosition(yfield, point.y);
}

void MeaToolMgr::SaveProfile(MeaProfile& profile) {
    for (const auto& toolEntry : m_tools) {
        toolEntry.second->SaveProfile(profile);
    }

    profile.WriteStr(_T("CurrentRadioTool"), m_currentRadioTool->GetToolName());

    if (!profile.UserInitiated()) {
        profile.WriteBool(_T("EnableCrosshairs"), m_crosshairsEnabled);
    }
}

void MeaToolMgr::LoadProfile(MeaProfile& profile) {
    for (const auto& toolEntry : m_tools) {
        toolEntry.second->LoadProfile(profile);
    }

    MeaTool* tool = m_tools[profile.ReadStr(_T("CurrentRadioTool"),
        m_currentRadioTool->GetToolName())];
    MeaAssert(tool->IsRadioTool());
    m_currentRadioTool = static_cast<MeaRadioTool*>(tool);

    if (!profile.UserInitiated()) {
        m_crosshairsEnabled = profile.ReadBool(_T("EnableCrosshairs"), m_crosshairsEnabled);
    }
}

void MeaToolMgr::MasterReset() {
    for (const auto& toolEntry : m_tools) {
        toolEntry.second->MasterReset();
    }

    m_currentRadioTool = m_cursorTool;
    m_crosshairsEnabled = kDefCrosshairsEnabled;
}

void MeaToolMgr::SetStatus(int id) const {
    CString str;
    str.LoadStringA(id);

    MeaStatusBar* bar = static_cast<MeaStatusBar*>(static_cast<CFrameWnd*>(AfxGetMainWnd())->GetControlBar(AFX_IDW_STATUS_BAR));
    bar->SetIdleMessage(str);
}

void MeaToolMgr::EnableRegionFields(UINT enableFields, UINT editableFields) {
    CPoint offScreen(MeaScreenMgr::Instance().GetOffScreen());

    if (m_dataDisplay != nullptr) {
        m_dataDisplay->EnableRegionFields(enableFields, editableFields);
    }

    if (!(editableFields & MeaX2Field)) {
        m_rulerTool->SetIndicator(MeaRuler::Ind2, offScreen);
    }

    if (!(editableFields & MeaXVField)) {
        m_rulerTool->SetIndicator(MeaRuler::Ind3, offScreen);
    }
}

void MeaToolMgr::EnableScreenFields(UINT enableFields, UINT editableFields) {
    if (m_dataDisplay != nullptr) {
        m_dataDisplay->EnableScreenFields(enableFields, editableFields);
    }
}

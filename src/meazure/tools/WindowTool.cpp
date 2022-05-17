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
#include "WindowTool.h"
#include "ToolMgr.h"
#include <meazure/utilities/MeaAssert.h>
#include <meazure/ui/Layout.h>
#include <meazure/graphics/Colors.h>
#include <meazure/resource.h>
#include <Hooks/Hooks.h>
#include <windowsx.h>


const CString MeaWindowTool::kToolName(_T("WindowTool"));


MeaWindowTool::MeaWindowTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                             const MeaUnitsProvider& unitsProvider) :
    MeaRadioTool(mgr, screenProvider, unitsProvider),
    m_pointerPos(0, 0),
    m_point1(-1, -1),
    m_point2(-1, -1),
    m_currentWnd(nullptr),
    m_hiliteWnd(nullptr),
    m_dataWin(screenProvider, unitsProvider) {}

MeaWindowTool::~MeaWindowTool() {
    try {
        Disable();
        m_currentWnd = nullptr;
        m_hiliteWnd = nullptr;
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaWindowTool::Create() {
    m_dataWin.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), 0xffff, 0xffff, IDS_MEA_WIDTH, IDS_MEA_HEIGHT);
}

void MeaWindowTool::Enable() {
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    m_currentWnd = nullptr;

    // Tell the tool manager which display fields we are using.
    //
    m_mgr.EnableRegionFields(MeaX1Field | MeaY1Field |
                             MeaX2Field | MeaY2Field |
                             MeaWidthField | MeaHeightField |
                             MeaDistanceField | MeaAngleField |
                             MeaAspectField | MeaAreaField, 0);

    // Create the window rectangle
    //
    if (!IsWindow(m_rectangle)) {
        m_rectangle.Create(2, 0);
        m_rectangle.SetPosition(m_point1, m_point2);
    }

    if (!IsWindow(m_dataWin)) {
        Create();
    }

    // Show a one liner on how to use the tool.
    //
    m_mgr.SetStatus(IDS_MEA_WIN_STATUS);

    // Start monitoring the mouse pointer.
    //
    MeaEnableMouseHook();

    // Display the rectangle and data window.
    //
    m_rectangle.Show();
    m_dataWin.Show();

    Update(MeaUpdateReason::NormalUpdate);
}

void MeaWindowTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

#ifdef _DEBUG
    BOOL ret =
#endif
        MeaDisableMouseHook();
    MeaAssert(ret);

    m_dataWin.Hide();
    m_rectangle.Hide();

    MeaTool::Disable();
}

void MeaWindowTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        if ((reason == MeaUpdateReason::UnitsChanged) || (reason == MeaUpdateReason::OriginChanged) ||
            (reason == MeaUpdateReason::AllChanged)) {
            m_dataWin.Hide();
            m_dataWin.Show();
        }
        if ((reason == MeaUpdateReason::DataWinArming) || (reason == MeaUpdateReason::AllChanged)) {
            if (MeaDataWin::IsArmed()) {
                m_dataWin.Show();
            } else {
                m_dataWin.Hide();
            }
        }

        // Convert the window dimensions to the current units.
        //
        FPOINT p1 = m_unitsProvider.ConvertCoord(m_point1);
        FPOINT p2 = m_unitsProvider.ConvertCoord(m_point2);
        FSIZE wh = m_unitsProvider.GetWidthHeight(m_point1, m_point2);

        // Display the measurement information.
        //
        m_mgr.ShowXY1(m_point1, p1);
        m_mgr.ShowXY2(m_point2, p2);
        m_mgr.ShowWH(wh);
        m_mgr.ShowDistance(wh);
        m_mgr.ShowAngle(MeaLayout::GetAngle(p1, p2));
        m_mgr.ShowAspect(wh);
        m_mgr.ShowRectArea(wh);

        // Update the screen information based on the
        // mouse pointer's location.
        //
        m_mgr.UpdateScreenInfo(m_pointerPos);

        // Display the data window information.
        //
        CRect rect;
        ::GetWindowRect(m_currentWnd, rect);
        rect.InflateRect(3, 3);
        m_dataWin.ShowWH(wh);
        m_dataWin.Update(rect);
    }
}

void MeaWindowTool::Strobe() {
    m_dataWin.Strobe();
}

bool MeaWindowTool::HasCrosshairs() const {
    return false;
}

bool MeaWindowTool::HasRegion() {
    CRect rect = GetRegion();
    return (rect.Height() != 0) && (rect.Width() != 0);
}

RECT MeaWindowTool::GetRegion() {
    CRect rect(m_point1, m_point2);
    rect.NormalizeRect();
    rect.InflateRect(0, 0, 1, 1);

    return rect;
}

const POINT& MeaWindowTool::GetPosition() const {
    return m_pointerPos;
}

void MeaWindowTool::GetPosition(MeaPositionLogMgr::Position& position) const {
    // Convert the pixel locations to the current units.
    //
    FPOINT p1 = m_unitsProvider.ConvertCoord(m_point1);
    FPOINT p2 = m_unitsProvider.ConvertCoord(m_point2);
    FSIZE wh = m_unitsProvider.GetWidthHeight(m_point1, m_point2);

    // Save the positions in the position object.
    //
    position.RecordXY1(p1);
    position.RecordXY2(p2);
    position.RecordWH(wh);
    position.RecordDistance(wh);
    position.RecordAngle(MeaLayout::GetAngle(p1, p2));
    position.RecordRectArea(wh);
}

CString MeaWindowTool::GetToolName() const {
    return kToolName;
}

UINT MeaWindowTool::GetLabelId() const {
    return IDS_MEA_WINDOW;
}

void MeaWindowTool::ColorsChanged() {
    m_rectangle.SetColor(MeaColors::Get(MeaColors::LineFore));
    m_dataWin.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
}

void MeaWindowTool::OnMouseHook(WPARAM /* wParam */, LPARAM lParam) {
    // Cache the pointer position.
    //
    m_pointerPos.x = GET_X_LPARAM(lParam);
    m_pointerPos.y = GET_Y_LPARAM(lParam);

    // Attempt to locate a window under the pointer and
    // if found, position the rectangle over it.
    //
    if (FindWindow()) {
        m_rectangle.SetPosition(m_point1, m_point2);
        Update(MeaUpdateReason::NormalUpdate);
    }
}

bool MeaWindowTool::FindWindow() {
    CPoint pos(m_pointerPos);

    // Look in the window hierarchy for a window under the pointer.
    //
    m_hiliteWnd = WindowFromPoint(pos);
    ScreenToClient(m_hiliteWnd, &pos);
    HWND hWnd2 = ChildWindowFromPointEx(m_hiliteWnd, pos,
        CWP_SKIPINVISIBLE | CWP_SKIPTRANSPARENT);

    if (hWnd2 != nullptr) {
        m_hiliteWnd = hWnd2;
        EnumChildWindows(m_hiliteWnd, EnumChildProc, reinterpret_cast<LPARAM>(this));
    }

    // If we have found a window, obtain its dimensions.
    //
    bool ret = ((m_hiliteWnd != m_currentWnd) && (m_hiliteWnd != m_rectangle.m_hWnd));

    if (ret) {
        if (m_hiliteWnd != nullptr) {
            RECT rect;

            GetWindowRect(m_hiliteWnd, &rect);
            m_point1.x = rect.left;
            m_point1.y = rect.top;
            m_point2.x = rect.right - 1;
            m_point2.y = rect.bottom - 1;
        } else {
            m_point1.x = 0;
            m_point1.y = 0;
            m_point2.x = 0;
            m_point2.y = 0;
        }

        m_currentWnd = m_hiliteWnd;
    }

    return ret;
}

BOOL CALLBACK MeaWindowTool::EnumChildProc(HWND hwnd, LPARAM lParam) {
    MeaWindowTool* tool = reinterpret_cast<MeaWindowTool*>(lParam);

    CRect rect;
    POINT pt;

    // Determine if the mouse pointer is within the window rectangle.
    //
    GetWindowRect(hwnd, rect);
    GetCursorPos(&pt);
    if (rect.PtInRect(pt)) {
        tool->m_hiliteWnd = hwnd;
    }
    return TRUE;
}

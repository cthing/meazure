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
#include "RulerTool.h"
#include "MeaAssert.h"
#include "Colors.h"


const CString   MeaRulerTool::kToolName(_T("RulerTool"));


MeaRulerTool::MeaRulerTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider) :
    MeaTool(mgr, screenProvider), MeaRulerCallback() {
    // Position each indicator completely off any screen.
    //
    int i;
    CPoint offScreen(m_screenProvider.GetOffScreen());

    for (i = 0; i < MeaRuler::NumIndicators; i++) {
        m_indicatorPos[i] = offScreen;
    }

    // Create a set of rulers for each display screen and add
    // it to the collection of ruler sets.
    //
    int numScreens = m_screenProvider.GetNumScreens();
    for (i = 0; i < numScreens; i++) {
        m_rulers.push_back(new RulerSet(m_screenProvider));
    }
}

MeaRulerTool::~MeaRulerTool() {
    try {
        Disable();

        for (unsigned int i = 0; i < m_rulers.size(); i++) {
            delete m_rulers[i];
        }
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaRulerTool::SaveProfile(MeaProfile& profile) {
    profile.WriteBool(_T("Rulers"), IsEnabled());

    unsigned int i;

    // Save the position of each ruler.
    //
    for (i = 0; i < m_rulers.size(); i++) {
        CString tag;
        int xPos, yPos;
        RulerSet* rulerSet = m_rulers[i];

        xPos = (IsWindow(rulerSet->m_vRuler)) ? rulerSet->m_vRuler.GetPosition() : rulerSet->m_xPos;
        yPos = (IsWindow(rulerSet->m_hRuler)) ? rulerSet->m_hRuler.GetPosition() : rulerSet->m_yPos;

        tag.Format(_T("RulerSet%d-"), i);
        profile.WriteInt(tag + _T("XPos"), xPos);
        profile.WriteInt(tag + _T("YPos"), yPos);
    }
}

void MeaRulerTool::LoadProfile(MeaProfile& profile) {
    MeaScreenProvider::ScreenIter iter;
    unsigned int i;

    // Load the position of each ruler.
    //
    for (i = 0, iter = m_screenProvider.GetScreenIter(); !m_screenProvider.AtEnd(iter); ++iter, i++) {
        CString tag;

        const CRect& rect = m_screenProvider.GetScreenRect(iter);

        tag.Format(_T("RulerSet%d-"), i);
        m_rulers[i]->m_xPos = static_cast<int>(profile.ReadInt(tag + _T("XPos"), rect.left));
        m_rulers[i]->m_yPos = static_cast<int>(profile.ReadInt(tag + _T("YPos"), rect.top));
    }

    if (profile.ReadBool(_T("Rulers"), kShowRulers)) {
        Enable();
    } else {
        Disable();
    }
}

void MeaRulerTool::MasterReset() {
#pragma warning(disable: 4127)
    if (kShowRulers) {
        Enable();
    } else {
        Disable();
    }
#pragma warning(default: 4127)
}

void MeaRulerTool::Enable() {
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    MeaScreenProvider::ScreenIter iter;
    int i;

    // Display the rulers on all screens. If the rulers have
    // never been displayed before, create them.
    //
    for (i = 0, iter = m_screenProvider.GetScreenIter(); !m_screenProvider.AtEnd(iter); ++iter, i++) {
        if (!IsWindow(m_rulers[i]->m_vRuler)) {
            const CRect& rect = m_screenProvider.GetScreenRect(iter);

            m_rulers[i]->m_vRuler.Create(MeaColors::Get(MeaColors::RulerBorder),
                                         MeaColors::Get(MeaColors::RulerBack),
                                         MeaColors::GetA(MeaColors::RulerOpacity),
                                         MeaRuler::Vertical, rect, this, nullptr, i);
            m_rulers[i]->m_hRuler.Create(MeaColors::Get(MeaColors::RulerBorder),
                                         MeaColors::Get(MeaColors::RulerBack),
                                         MeaColors::GetA(MeaColors::RulerOpacity),
                                         MeaRuler::Horizontal, rect, this, nullptr, i);

            PositionRulers(i);
            for (int j = 0; j < MeaRuler::NumIndicators; j++) {
                PositionIndicators(i, static_cast<MeaRuler::IndicatorId>(j));
            }
        }

        m_rulers[i]->m_vRuler.Show();
        m_rulers[i]->m_hRuler.Show();
    }
}

void MeaRulerTool::Disable() {
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    for (unsigned int i = 0; i < m_rulers.size(); i++) {
        m_rulers[i]->m_vRuler.Hide();
        m_rulers[i]->m_hRuler.Hide();
    }
}

void MeaRulerTool::Update(MeaUpdateReason reason) {
    if (IsEnabled()) {
        MeaTool::Update(reason);

        for (int i = 0; i < static_cast<int>(m_rulers.size()); i++) {
            PositionRulers(i);
            for (int j = 0; j < MeaRuler::NumIndicators; j++)
                PositionIndicators(i, static_cast<MeaRuler::IndicatorId>(j));
            UpdateRulers(i);
        }
    }
}

CString MeaRulerTool::GetToolName() const {
    return kToolName;
}

const POINT& MeaRulerTool::GetPosition() const {
    return m_defaultPos;
}

void MeaRulerTool::SetIndicator(MeaRuler::IndicatorId indId,
                                const POINT& indicatorPos) {
    m_indicatorPos[indId] = indicatorPos;
    for (int i = 0; i < static_cast<int>(m_rulers.size()); i++) {
        PositionIndicators(i, indId);
    }
}

void MeaRulerTool::ColorsChanged() {
    for (unsigned int i = 0; i < m_rulers.size(); i++) {
        // Redraw the rulers in the new colors.
        //
        m_rulers[i]->m_vRuler.SetColors(MeaColors::Get(MeaColors::RulerBorder),
                                        MeaColors::Get(MeaColors::RulerBack));
        m_rulers[i]->m_hRuler.SetColors(MeaColors::Get(MeaColors::RulerBorder),
                                        MeaColors::Get(MeaColors::RulerBack));

        // Set the opacities of the rulers.
        //
        m_rulers[i]->m_vRuler.SetOpacity(MeaColors::GetA(MeaColors::RulerOpacity));
        m_rulers[i]->m_hRuler.SetOpacity(MeaColors::GetA(MeaColors::RulerOpacity));
    }
}

void MeaRulerTool::PositionRulers(int rulerIdx) {
    m_rulers[rulerIdx]->m_hRuler.SetPosition(m_rulers[rulerIdx]->m_yPos);
    m_rulers[rulerIdx]->m_vRuler.SetPosition(m_rulers[rulerIdx]->m_xPos);
}

void MeaRulerTool::UpdateRulers(int rulerIdx) {
    m_rulers[rulerIdx]->m_hRuler.Update();
    m_rulers[rulerIdx]->m_vRuler.Update();
}

void MeaRulerTool::PositionIndicators(int rulerIdx, MeaRuler::IndicatorId indId) {
    if (IsEnabled()) {
        m_rulers[rulerIdx]->m_hRuler.SetIndicator(indId, m_indicatorPos[indId].x);
        m_rulers[rulerIdx]->m_vRuler.SetIndicator(indId, m_indicatorPos[indId].y);
    }
}

void MeaRulerTool::OnRulerMove(const RulerInfo* info) {
    MeaAssert(info != nullptr);

    RulerSet* rulerSet = m_rulers[info->id];

    if (info->ruler->GetOrientation() == MeaRuler::Horizontal) {
        rulerSet->m_yPos = info->position;
        info->ruler->SetPosition(rulerSet->m_yPos);
    } else {
        rulerSet->m_xPos = info->position;
        info->ruler->SetPosition(rulerSet->m_xPos);
    }
}

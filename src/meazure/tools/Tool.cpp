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
#include "Tool.h"
#include <crtdbg.h>
#include <cassert>


const CPoint MeaTool::m_defaultPos;


MeaTool::MeaTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider) :
    m_mgr(mgr),
    m_screenProvider(screenProvider),
    m_unitsProvider(unitsProvider),
    m_enabled(false) {}

void MeaTool::SaveProfile(MeaProfile& /* profile */) {}

void MeaTool::LoadProfile(MeaProfile& /* profile */) {}

void MeaTool::MasterReset() {}

void MeaTool::Enable() {
    assert(!m_enabled);
    m_enabled = true;
}

void MeaTool::Disable() {
    m_enabled = false;
}

void MeaTool::Update(MeaUpdateReason /* reason */) {
    assert(m_enabled);
}

bool MeaTool::IsRadioTool() {
    return false;
}

void MeaTool::SetPosition(MeaDataFieldId, int) {}

void MeaTool::IncPosition(MeaDataFieldId) {}

void MeaTool::DecPosition(MeaDataFieldId) {}

void MeaTool::ColorsChanged() {}

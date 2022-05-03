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
#include "Tool.h"
#include "MeaAssert.h"
#include <crtdbg.h>


POINT MeaTool::m_defaultPos { 0, 0 };


MeaTool::MeaTool(MeaToolMgr* mgr) :
    m_mgr(mgr), m_enabled(false) {}

MeaTool::~MeaTool() {
    m_mgr = nullptr;
}

void MeaTool::SaveProfile(MeaProfile& /* profile */) {}

void MeaTool::LoadProfile(MeaProfile& /* profile */) {}

void MeaTool::MasterReset() {}

void MeaTool::Enable() {
    MeaAssert(!m_enabled);
    m_enabled = true;
}

void MeaTool::Disable() {
    m_enabled = false;
}

void MeaTool::Update(MeaUpdateReason /* reason */) {
    MeaAssert(m_enabled);
}

bool MeaTool::IsRadioTool() {
    return false;
}

void MeaTool::SetPosition(MeaFields, int) {}

void MeaTool::IncPosition(MeaFields) {}

void MeaTool::DecPosition(MeaFields) {}

void MeaTool::ColorsChanged() {}

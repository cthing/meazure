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
#include "Resource.h"
#include "RadioTool.h"


MeaRadioTool::MeaRadioTool(MeaToolMgr* mgr) : MeaTool(mgr) {}

MeaRadioTool::~MeaRadioTool() {}

bool MeaRadioTool::IsRadioTool() {
    return true;
}

UINT MeaRadioTool::GetLabelId() const {
    return IDS_MEA_EMPTY;
}

bool MeaRadioTool::HasCrosshairs() const {
    return true;
}

void MeaRadioTool::EnableCrosshairs() {}

void MeaRadioTool::DisableCrosshairs() {}

void MeaRadioTool::Flash() {}

void MeaRadioTool::Strobe() {}

bool MeaRadioTool::HasRegion() {
    return false;
}

RECT MeaRadioTool::GetRegion() {
    CRect rect(0, 0, 0, 0);

    return rect;
}

void MeaRadioTool::SetPosition(const PointMap& /*points*/) {}

void MeaRadioTool::OnMouseHook(WPARAM /* wParam */, LPARAM /* lParam */) {}

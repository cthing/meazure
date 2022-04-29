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
#include "Graphic.h"


MeaGraphic::MeaGraphic() : CWnd(), m_id(0xFFFF), m_visible(false), m_parent(nullptr) {}

MeaGraphic::~MeaGraphic() {
    m_parent = nullptr;
}

bool MeaGraphic::Create(LPCTSTR classname, const SIZE& size, const CWnd* parent, UINT id) {
    m_id = id;
    m_parent = parent;

    return CreateEx(0, classname, "", ((parent == nullptr) ? WS_POPUP : WS_CHILD), 0, 0, size.cx, size.cy,
                    ((parent == nullptr) ? *AfxGetMainWnd() : *parent), 0) ? true : false;
}

void MeaGraphic::Show() {
    if (m_hWnd != nullptr) {
        ShowWindow(SW_SHOWNOACTIVATE);
        m_visible = true;
    }
}

void MeaGraphic::Hide() {
    if (m_hWnd != nullptr) {
        m_visible = false;
        ShowWindow(SW_HIDE);
    }
}

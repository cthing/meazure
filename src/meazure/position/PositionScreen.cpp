/*
 * Copyright 2022 C Thing Software
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
#include "PositionScreen.h"


MeaPositionScreen::MeaPositionScreen(const MeaScreenProvider::ScreenIter& screenIter,
                                     const MeaUnitsProvider& unitsProvider,
                                     const MeaScreenProvider& screenProvider) {
    const CRect& rect = screenProvider.GetScreenRect(screenIter);
    MeaFPoint p1 = unitsProvider.ConvertCoord(rect.TopLeft());
    MeaFPoint p2 = unitsProvider.ConvertCoord(rect.BottomRight());

    m_rect.top = p1.y;
    m_rect.bottom = p2.y;
    m_rect.left = p1.x;
    m_rect.right = p2.x;

    m_primary = screenProvider.IsPrimary(screenIter);
    m_manualRes = screenProvider.IsManualRes(screenIter);
    m_res = unitsProvider.ConvertRes(screenProvider.GetScreenRes(screenIter));
    m_desc = screenProvider.GetScreenName(screenIter);
}

void MeaPositionScreen::Load(const MeaXMLNode* screenNode) {
    CString valueStr;

    screenNode->GetAttributes().GetValueBool(_T("primary"), m_primary);
    screenNode->GetAttributes().GetValueStr(_T("desc"), m_desc);

    for (MeaXMLNode::NodeIter_c iter = screenNode->GetChildIter(); !screenNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;
        const MeaXMLAttributes& attrs = node->GetAttributes();

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("rect")) {
                attrs.GetValueDbl(_T("top"), m_rect.top);
                attrs.GetValueDbl(_T("bottom"), m_rect.bottom);
                attrs.GetValueDbl(_T("left"), m_rect.left);
                attrs.GetValueDbl(_T("right"), m_rect.right);
            } else if (node->GetData() == _T("resolution")) {
                attrs.GetValueDbl(_T("x"), m_res.cx);
                attrs.GetValueDbl(_T("y"), m_res.cy);
                attrs.GetValueBool(_T("manual"), m_manualRes);
            }
        }
    }
}

void MeaPositionScreen::Save(MeaXMLWriter& writer) const {
    writer.StartElement(_T("screen"))
        .AddAttribute(_T("desc"), m_desc)
        .AddAttribute(_T("primary"), (m_primary ? _T("true") : _T("false")));

    writer.StartElement(_T("rect"))
        .AddAttribute(_T("top"), m_rect.top)
        .AddAttribute(_T("bottom"), m_rect.bottom)
        .AddAttribute(_T("left"), m_rect.left)
        .AddAttribute(_T("right"), m_rect.right)
        .EndElement();

    writer.StartElement(_T("resolution"))
        .AddAttribute(_T("x"), m_res.cx)
        .AddAttribute(_T("y"), m_res.cy)
        .AddAttribute(_T("manual"), (m_manualRes ? _T("true") : _T("false")))
        .EndElement();

    writer.EndElement();        // screen
}

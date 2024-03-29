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
#include "PositionDesktop.h"
#include <meazure/utilities/StringUtils.h>


MeaPositionDesktop::MeaPositionDesktop(const MeaUnitsProvider& unitsProvider, const MeaScreenProvider& screenProvider) :
    MeaPositionDesktop(nullptr, unitsProvider, screenProvider) {}

MeaPositionDesktop::MeaPositionDesktop(PCTSTR guidStr, const MeaUnitsProvider& unitsProvider,
                                       const MeaScreenProvider& screenProvider) :
    m_unitsProvider(&unitsProvider),
    m_id(guidStr),
    m_linearUnits(unitsProvider.GetLinearUnits()),
    m_angularUnits(unitsProvider.GetAngularUnits()),
    m_invertY(unitsProvider.IsInvertY()),
    m_origin(unitsProvider.ConvertPos(unitsProvider.GetOrigin())),
    m_customFactor(0.0)
{

    const CRect& size = screenProvider.GetVirtualRect();
    CPoint bottomRight(size.BottomRight());
    bottomRight.Offset(-1, -1);

    m_size = unitsProvider.GetWidthHeight(size.TopLeft(), bottomRight);

    if (m_linearUnits->GetUnitsId() == MeaCustomId) {
        MeaCustomUnits* custom = static_cast<MeaCustomUnits*>(m_linearUnits);

        m_customName = custom->GetName();
        m_customAbbrev = custom->GetAbbrev();
        m_customBasisStr = custom->GetScaleBasisStr();
        m_customFactor = custom->GetScaleFactor();
        m_customPrecisions = custom->GetDefaultPrecisions();
    }

    MeaScreenProvider::ScreenIter iter;
    for (iter = screenProvider.GetScreenIter(); !screenProvider.AtEnd(iter); ++iter) {
        MeaPositionScreen screen(iter, unitsProvider, screenProvider);

        m_screens.push_back(screen);
    }
}

void MeaPositionDesktop::SetLinearUnits(const CString& unitsStr) {
    m_linearUnits = m_unitsProvider->GetLinearUnits(unitsStr);
    assert(m_linearUnits != nullptr);
}

void MeaPositionDesktop::SetAngularUnits(const CString& unitsStr) {
    m_angularUnits = m_unitsProvider->GetAngularUnits(unitsStr);
    assert(m_angularUnits != nullptr);
}

void MeaPositionDesktop::Load(const MeaXMLNode* desktopNode) {
    CString valueStr;

    for (MeaXMLNode::NodeIter_c iter = desktopNode->GetChildIter(); !desktopNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;
        const MeaXMLAttributes& attrs = node->GetAttributes();

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("units")) {
                attrs.GetValueStr(_T("length"), valueStr);
                SetLinearUnits(valueStr);
                attrs.GetValueStr(_T("angle"), valueStr);
                SetAngularUnits(valueStr);
            } else if (node->GetData() == _T("customUnits")) {
                attrs.GetValueStr(_T("name"), m_customName);
                attrs.GetValueStr(_T("abbrev"), m_customAbbrev);
                attrs.GetValueStr(_T("scaleBasis"), m_customBasisStr);
                attrs.GetValueDbl(_T("scaleFactor"), m_customFactor);
            } else if (node->GetData() == _T("origin")) {
                attrs.GetValueDbl(_T("xoffset"), m_origin.x);
                attrs.GetValueDbl(_T("yoffset"), m_origin.y);
                attrs.GetValueBool(_T("invertY"), m_invertY);
            } else if (node->GetData() == _T("size")) {
                attrs.GetValueDbl(_T("x"), m_size.cx);
                attrs.GetValueDbl(_T("y"), m_size.cy);
            } else if (node->GetData() == _T("screens")) {
                m_screens.clear();
                for (MeaXMLNode::NodeIter_c screenIter = node->GetChildIter(); 
                     !node->AtEnd(screenIter); ++screenIter) {
                    MeaPositionScreen screen;
                    screen.Load(*screenIter);
                    m_screens.push_back(screen);
                }
            } else if (node->GetData() == _T("displayPrecisions")) {
                MeaXMLNode::NodeIter_c precIter = node->GetChildIter();
                LoadCustomPrecisions(*precIter);
            }
        }
    }
}

void MeaPositionDesktop::Save(MeaXMLWriter& writer) const {
    writer.StartElement(_T("desktop"))
        .AddAttribute(_T("id"), m_id.ToString());

    writer.StartElement(_T("units"))
        .AddAttribute(_T("length"), m_linearUnits->GetUnitsStr())
        .AddAttribute(_T("angle"), m_angularUnits->GetUnitsStr())
        .EndElement();

    if (m_linearUnits->GetUnitsId() == MeaCustomId) {
        writer.StartElement(_T("customUnits"))
            .AddAttribute(_T("name"), m_customName)
            .AddAttribute(_T("abbrev"), m_customAbbrev)
            .AddAttribute(_T("scaleBasis"), m_customBasisStr)
            .AddAttribute(_T("scaleFactor"), m_customFactor)
            .EndElement();
    }

    writer.StartElement(_T("origin"))
        .AddAttribute(_T("xoffset"), m_origin.x)
        .AddAttribute(_T("yoffset"), m_origin.y)
        .AddAttribute(_T("invertY"), (m_invertY ? _T("true") : _T("false")))
        .EndElement();

    writer.StartElement(_T("size"))
        .AddAttribute(_T("x"), m_size.cx)
        .AddAttribute(_T("y"), m_size.cy)
        .EndElement();

    writer.StartElement(_T("screens"));
    for (const auto& screen : m_screens) {
        screen.Save(writer);
    }
    writer.EndElement();        // screens

    if (m_linearUnits->GetUnitsId() == MeaCustomId) {
        writer.StartElement(_T("displayPrecisions"));
        SaveCustomPrecisions(writer);
        writer.EndElement();    // displayPrecisions
    }



    writer.EndElement();        // desktop
}

void MeaPositionDesktop::LoadCustomPrecisions(const MeaXMLNode* displayPrecisionNode) {
    typedef std::map<CString, int> PrecisionMap;
    typedef PrecisionMap::const_iterator PrecisionIter;

    PrecisionMap precMap;

    for (MeaXMLNode::NodeIter_c measurementIter = displayPrecisionNode->GetChildIter();
                                !displayPrecisionNode->AtEnd(measurementIter); ++measurementIter) {
        MeaXMLNode* measurementNode = *measurementIter;
        const MeaXMLAttributes& attrs = measurementNode->GetAttributes();

        if (measurementNode->GetData() == _T("measurement")) {
            CString name;
            int places;

            attrs.GetValueStr(_T("name"), name);
            attrs.GetValueInt(_T("decimalPlaces"), places);

            precMap[name] = places;
        }
    }

    const MeaUnits::DisplayPrecisionNames& precisionNames = m_linearUnits->GetDisplayPrecisionNames();
    const MeaUnits::DisplayPrecisions& precisions = m_linearUnits->GetDisplayPrecisions();
    unsigned int i;

    m_customPrecisions.clear();

    for (i = 0; i < precisionNames.size(); i++) {
        PrecisionIter precIter = precMap.find(precisionNames[i]);

        m_customPrecisions.push_back((precIter != precMap.end()) ? (*precIter).second : precisions[i]);
    }
}

void MeaPositionDesktop::SaveCustomPrecisions(MeaXMLWriter& writer) const {
    const MeaUnits::DisplayPrecisionNames& precisionNames = m_linearUnits->GetDisplayPrecisionNames();
    unsigned int i;

    writer.StartElement(_T("displayPrecision"))
        .AddAttribute(_T("units"), m_linearUnits->GetUnitsStr());

    for (i = 0; i < m_customPrecisions.size(); i++) {
        writer.StartElement(_T("measurement"))
            .AddAttribute(_T("name"), precisionNames[i])
            .AddAttribute(_T("decimalPlaces"), m_customPrecisions[i])
            .EndElement();
    }

    writer.EndElement();        // displayPrecision
}


std::ostream& operator<<(std::ostream& os, const MeaPositionDesktopRef& ref) {
    os << ref.ToString();
    return os;
}

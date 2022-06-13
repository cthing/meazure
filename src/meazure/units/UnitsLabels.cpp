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
#include "UnitsLabels.h"
#include <meazure/resource.h>
#include <meazure/ui/Layout.h>


/**************************************************************************
 * MeaUnitsLabel
 **************************************************************************/

MeaUnitsLabel::MeaUnitsLabel(UINT format, const LabelMap& labels, int& width) :
    MeaLabel(),
    m_labelsRef(labels),
    m_widthRef(width),
    m_format(format) {}

MeaUnitsLabel::~MeaUnitsLabel() {}

bool MeaUnitsLabel::Create(UINT unitsId, DWORD style, const POINT& topLeft,
                           CWnd* parentWnd, UINT id) {
    if (!MeaLabel::Create(ConstructLabel(unitsId), style, topLeft, parentWnd, id)) {
        return false;
    }

    return SetSize();
}

bool MeaUnitsLabel::SetSize() {
    if (m_widthRef < 0) {
        CDC* dc = GetDC();
        if (dc == nullptr) {
            return false;
        }

        if (dc->SelectStockObject(DEFAULT_GUI_FONT) == nullptr) {
            ReleaseDC(dc);
            return false;
        }

        // Get the bounding box for each label setting the width to the max.
        //
        for (const auto& labelEntry : m_labelsRef) {
            CSize sz = dc->GetTextExtent(ConstructLabel(labelEntry.first));
            if (sz.cx > m_widthRef) {
                m_widthRef = sz.cx;
            }
        }

        ReleaseDC(dc);
    }

    CRect rect;

    GetWindowRect(&rect);
    MeaLayout::SetWindowSize(*this, m_widthRef, rect.Height());

    return true;
}

CString MeaUnitsLabel::ConstructLabel(int unitsId) const {
    CString labelStr;

    labelStr.Format(m_format, static_cast<PCTSTR>(m_labelsRef.at(unitsId)));
    return labelStr;
}


//*************************************************************************
// MeaLinearUnitsLabel
//*************************************************************************

MeaUnitsLabel::LabelMap MeaLinearUnitsLabel::m_labels;

MeaLinearUnitsLabel::MeaLinearUnitsLabel(UINT format, int& width) :
    MeaUnitsLabel(format, m_labels, width) {

    // Static initialization cannot be used because an HINSTANCE is not yet available for reading the string table.
    if (m_labels.empty()) {
        m_labels[MeaPixelsId].LoadString(IDS_MEA_PIXELS);
        m_labels[MeaPointsId].LoadString(IDS_MEA_POINTS);
        m_labels[MeaTwipsId].LoadString(IDS_MEA_TWIPS);
        m_labels[MeaInchesId].LoadString(IDS_MEA_INCHES);
        m_labels[MeaCentimetersId].LoadString(IDS_MEA_CM);
        m_labels[MeaMillimetersId].LoadString(IDS_MEA_MM);
        m_labels[MeaPicasId].LoadString(IDS_MEA_PICAS);
        m_labels[MeaCustomId].LoadString(IDS_MEA_CUSTOM_UNITS);
    }
}

MeaLinearUnitsLabel::~MeaLinearUnitsLabel() {}

void MeaLinearUnitsLabel::SetUnits(MeaLinearUnitsId unitsId) {
    // For pixels (i.e. resolution independent units), report
    // the resolution not as px/px but as px/in.
    //
    if (unitsId == MeaPixelsId && GetFormat() == IDS_MEA_RES_LABEL_FMT) {
        unitsId = MeaInchesId;
    }

    SetWindowText(ConstructLabel(unitsId));
}


//*************************************************************************
// MeaAngularUnitsLabel
//*************************************************************************

MeaUnitsLabel::LabelMap MeaAngularUnitsLabel::m_labels;
int                     MeaAngularUnitsLabel::m_labelWidth = -1;

MeaAngularUnitsLabel::MeaAngularUnitsLabel() :
    MeaUnitsLabel(IDS_MEA_DEF_LABEL_FMT, m_labels, m_labelWidth) {

    // Static initialization cannot be used because an HINSTANCE is not yet available for reading the string table.
    if (m_labels.empty()) {
        m_labels[MeaDegreesId].LoadString(IDS_MEA_DEGREES);
        m_labels[MeaRadiansId].LoadString(IDS_MEA_RADIANS);
    }
}

MeaAngularUnitsLabel::~MeaAngularUnitsLabel() {}

void MeaAngularUnitsLabel::SetUnits(MeaAngularUnitsId anglesId) {
    SetWindowText(ConstructLabel(anglesId));
}


//*************************************************************************
// MeaLengthUnitsLabel
//*************************************************************************

int MeaLengthUnitsLabel::m_labelWidth = -1;

MeaLengthUnitsLabel::MeaLengthUnitsLabel() :
    MeaLinearUnitsLabel(IDS_MEA_DEF_LABEL_FMT, m_labelWidth) {}

MeaLengthUnitsLabel::~MeaLengthUnitsLabel() {}


//*************************************************************************
// MeaAreaUnitsLabel
//*************************************************************************

int MeaAreaUnitsLabel::m_labelWidth = -1;


MeaAreaUnitsLabel::MeaAreaUnitsLabel() :
    MeaLinearUnitsLabel(IDS_MEA_AREA_LABEL_FMT, m_labelWidth) {}

MeaAreaUnitsLabel::~MeaAreaUnitsLabel() {}


//*************************************************************************
// MeaResUnitsLabel
//*************************************************************************

int MeaResUnitsLabel::m_labelWidth = -1;


MeaResUnitsLabel::MeaResUnitsLabel() :
    MeaLinearUnitsLabel(IDS_MEA_RES_LABEL_FMT, m_labelWidth) {}

MeaResUnitsLabel::~MeaResUnitsLabel() {}

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
#include "MeaAssert.h"
#include "GridTool.h"
#include "Colors.h"
#include "ScreenMgr.h"


const CString MeaGridTool::kToolName(_T("GridTool"));


MeaGridTool::MeaGridTool(MeaToolMgr* mgr) : MeaTool(mgr),
    m_gridSpacing(kDefDefaultSpacing, kDefDefaultSpacing),
    m_linked(kDefLinked)
{
}


MeaGridTool::~MeaGridTool()
{
    try {
        Disable();
    }
    catch(...) {
        MeaAssert(false);
    }
}


void MeaGridTool::SaveProfile(MeaProfile& profile)
{   
    profile.WriteBool(_T("ScrnGrid"), IsEnabled());
    profile.WriteInt(_T("ScrnGridSpaceX"), m_gridSpacing.cx);
    profile.WriteInt(_T("ScrnGridSpaceY"), m_gridSpacing.cy);
    profile.WriteBool(_T("ScrnGridLinked"), m_linked);
}


void MeaGridTool::LoadProfile(MeaProfile& profile)
{
    m_linked = profile.ReadBool(_T("ScrnGridLinked"), m_linked);
    m_gridSpacing.cx = static_cast<long>(profile.ReadInt(_T("ScrnGridSpaceX"), m_gridSpacing.cx));
    m_gridSpacing.cy = static_cast<long>(profile.ReadInt(_T("ScrnGridSpaceY"), m_gridSpacing.cy));
    if (profile.ReadBool(_T("ScrnGrid"), kDefGrid)) {
        Enable();
    } else {
        Disable();
    }
}


void MeaGridTool::MasterReset()
{
    m_linked = kDefLinked;
    m_gridSpacing.cx = kDefDefaultSpacing;
    m_gridSpacing.cy = kDefDefaultSpacing;

#pragma warning(disable: 4127)
    if (kDefGrid) {
        Enable();
    } else {
        Disable();
    }
#pragma warning(default: 4127)
}


void MeaGridTool::Enable()
{
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();
    Update(MeaUpdateReason::NormalUpdate);
}


void MeaGridTool::Disable()
{
    if (!IsEnabled()) {
        return;
    }

    MeaTool::Disable();

    HideLines(VDir);
    HideLines(HDir);
}


void MeaGridTool::Update(MeaUpdateReason reason)
{
    if (IsEnabled()) {
        MeaTool::Update(reason);

        // To prevent visual artifacts and to improve
        // performance, hide the grid lines, move then
        // as needed, and then show the lines.
        //
        HideLines(HDir);
        HideLines(VDir);

        SetLines(HDir);
        SetLines(VDir);

        ShowLines(HDir);
        ShowLines(VDir);
    }
}


void MeaGridTool::UpdateH()
{
    if (IsEnabled()) {
        MeaTool::Update(MeaUpdateReason::NormalUpdate);

        // To prevent visual artifacts and to improve
        // performance, hide the grid lines, move then
        // as needed, and then show the lines.
        //
        HideLines(HDir);
        SetLines(HDir);
        ShowLines(HDir);
    }
}


void MeaGridTool::UpdateV()
{
    if (IsEnabled()) {
        MeaTool::Update(MeaUpdateReason::NormalUpdate);

        // To prevent visual artifacts and to improve
        // performance, hide the grid lines, move then
        // as needed, and then show the lines.
        //
        HideLines(VDir);
        SetLines(VDir);
        ShowLines(VDir);
    }
}



CString MeaGridTool::GetToolName() const
{
    return kToolName;
}


const POINT& MeaGridTool::GetPosition() const
{
    return m_defaultPos;
}


void MeaGridTool::SetGridSpacing(const SIZE& spacing)
{
    int v;

    // Checks the specified spacing value against the minimum and maximum spacing values.
    // The returned value is clamped to be between the min and max spacing.
    auto validateSpace = [](LONG s) { return (s < kDefMinSpacing) ? kDefMinSpacing : ((s > kDefMaxSpacing) ? kDefMaxSpacing : s); };

    v = validateSpace(spacing.cx);
    if (m_gridSpacing.cx != v) {
        m_gridSpacing.cx = v;
        UpdateH();
    }

    v = validateSpace(spacing.cy);
    if (m_gridSpacing.cy != v) {
        m_gridSpacing.cy = v;
        UpdateV();
    }
}


void MeaGridTool::ColorsChanged()
{
    // Draw the horizontal lines in the new color.
    //
    for (auto line : m_hlineList) {
        line->SetColor(MeaColors::Get(MeaColors::LineFore));
    }

    // Draw the vertical lines in the new color.
    //
    for (auto line : m_vlineList) {
        line->SetColor(MeaColors::Get(MeaColors::LineFore));
    }
}


void MeaGridTool::SetLines(LineDir dir)
{
    int numLinesReq, numLinesAct;
    const CRect& virtRect = MeaScreenMgr::Instance().GetVirtualRect();

    if (dir == VDir) {
        // New lines are created on demand and added to the list
        // for positioning.
        //
        numLinesReq = virtRect.Height() / m_gridSpacing.cy + 1;
        numLinesAct = static_cast<int>(m_hlineList.size());

        if (numLinesAct < numLinesReq) {
            int delta = numLinesReq - numLinesAct;
            for (int i = 0; i < delta; i++) {
                MeaLine *line = new MeaLine();
                line->Create(0);
                m_hlineList.push_back(line);
            }
        }

        // Position the lines.
        //
        LineList::const_iterator iter = m_hlineList.begin();
        POINT startPoint, endPoint;

        for (startPoint.x = virtRect.left, startPoint.y = virtRect.top, endPoint.x = virtRect.right, endPoint.y = virtRect.top;
                startPoint.y <= virtRect.bottom;
                startPoint.y += m_gridSpacing.cy, endPoint.y = startPoint.y) {
            (*iter++)->SetPosition(startPoint, endPoint);
        }
    } else {
        // New lines are created on demand and added to the list
        // for positioning.
        //
        numLinesReq = virtRect.Width() / m_gridSpacing.cx + 1;
        numLinesAct = static_cast<int>(m_vlineList.size());

        if (numLinesAct < numLinesReq) {
            int delta = numLinesReq - numLinesAct;
            for (int i = 0; i < delta; i++) {
                MeaLine *line = new MeaLine();
                line->Create(0);
                m_vlineList.push_back(line);
            }
        }

        // Position the lines.
        //
        LineList::const_iterator iter = m_vlineList.begin();
        POINT startPoint, endPoint;

        for (startPoint.x = virtRect.left, startPoint.y = virtRect.top, endPoint.x = virtRect.left, endPoint.y = virtRect.bottom;
                startPoint.x <= virtRect.right;
                startPoint.x += m_gridSpacing.cx, endPoint.x = startPoint.x) {
            (*iter++)->SetPosition(startPoint, endPoint);
        }
    }
}


void MeaGridTool::HideLines(LineDir dir) const
{
    if (dir == VDir) {
        for (auto line : m_hlineList) {
            line->Hide();
        }
    } else {
        for (auto line : m_vlineList) {
            line->Hide();
        }
    }
}


void MeaGridTool::ShowLines(LineDir dir)
{
    int c;
    const CRect& virtRect = MeaScreenMgr::Instance().GetVirtualRect();

    if (dir == VDir) {
        LineList::const_iterator iter = m_hlineList.begin();

        for (c = virtRect.top; c <= virtRect.bottom; c += m_gridSpacing.cy) {
            (*iter++)->Show();
        }
    } else {
        LineList::const_iterator iter = m_vlineList.begin();

        for (c = virtRect.left; c <= virtRect.right; c += m_gridSpacing.cx) {
            (*iter++)->Show();
        }
    }
}


void MeaGridTool::DeleteLines(LineList& lineList) const
{
    for (auto line : lineList) {
        line->Hide();
        delete line;
    }
    lineList.clear();
}


//=========================================================================
// MeaGridDialog
//=========================================================================


BEGIN_MESSAGE_MAP(MeaGridDialog, CDialog)
    ON_BN_CLICKED(IDC_GRID_LINK, OnGridLink)
    ON_EN_CHANGE(IDC_GRID_HSPACE, OnHSpaceChange)
    ON_EN_CHANGE(IDC_GRID_VSPACE, OnVSpaceChange)
END_MESSAGE_MAP()


MeaGridDialog::MeaGridDialog(MeaGridTool* tool) : CDialog(IDD_GRID_SPACING),
    m_tool(tool), m_hSpaceSpin(nullptr), m_vSpaceSpin(nullptr),
    m_origLinked(tool->GetLinked()), m_linked(tool->GetLinked()),
    m_origSpacing(tool->GetGridSpacing()), m_spacing(tool->GetGridSpacing())
{
}


MeaGridDialog::~MeaGridDialog()
{
    m_tool = nullptr;
    m_hSpaceSpin = nullptr;
    m_vSpaceSpin = nullptr;
}


void MeaGridDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    int linked = m_linked ? TRUE : FALSE;
    DDX_Check(pDX, IDC_GRID_LINK, linked);
    m_linked = linked ? true : false;
    DDX_Text(pDX, IDC_GRID_HSPACE, m_spacing.cx);
    DDX_Text(pDX, IDC_GRID_VSPACE, m_spacing.cy);
}


BOOL MeaGridDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the styles on the fields
    //
    m_hSpaceField.SetValueType(MeaNumberField::PosValues | MeaNumberField::IntValues);
    m_vSpaceField.SetValueType(MeaNumberField::PosValues | MeaNumberField::IntValues);

    // Replace the dialog CEdit controls with our subclassed field control.
    //
    m_hSpaceField.SubclassDlgItem(IDC_GRID_HSPACE, this);
    m_vSpaceField.SubclassDlgItem(IDC_GRID_VSPACE, this);

    // Initialize the spin controls.
    //
    m_hSpaceSpin    = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_GRID_HSPACE_SPIN));
    m_vSpaceSpin    = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_GRID_VSPACE_SPIN));

    m_hSpaceSpin->SetRange(MeaGridTool::kDefMinSpacing, MeaGridTool::kDefMaxSpacing);
    m_vSpaceSpin->SetRange(MeaGridTool::kDefMinSpacing, MeaGridTool::kDefMaxSpacing);

    UDACCEL uda;
    uda.nSec = 0;
    uda.nInc = 1;
    m_hSpaceSpin->SetAccel(1, &uda);
    m_vSpaceSpin->SetAccel(1, &uda);

    // Load the controls with the member data.
    //
    UpdateDisplay();

    return TRUE;
}


void MeaGridDialog::UpdateDisplay()
{
    if (m_linked) {
        m_spacing.cy = m_spacing.cx;
        UpdateData(FALSE);      // Load the dialog controls
    }
    m_vSpaceField.SetReadOnly(m_linked);
    MeaAssert(m_vSpaceSpin != nullptr);
    m_vSpaceSpin->EnableWindow(!m_linked);

    m_tool->SetLinked(m_linked);
    m_tool->SetGridSpacing(m_spacing);
}


void MeaGridDialog::OnGridLink() 
{
    UpdateData(TRUE);   // Read the dialog controls
    UpdateDisplay();
}


void MeaGridDialog::OnHSpaceChange()
{
    if (GetFieldValue(IDC_GRID_HSPACE, m_spacing.cx)) {
        UpdateDisplay();
    }
}


void MeaGridDialog::OnVSpaceChange()
{
    if (GetFieldValue(IDC_GRID_VSPACE, m_spacing.cy)) {
        UpdateDisplay();
    }
}


bool MeaGridDialog::GetFieldValue(UINT fieldId, long& value) const
{
    CWnd* field = GetDlgItem(static_cast<int>(fieldId));

    if (field != nullptr) {
        CString str;

        field->GetWindowText(str);
        if (!str.IsEmpty()) {
            long v = atol(str);
            if ((v >= MeaGridTool::kDefMinSpacing) && (v <= MeaGridTool::kDefMaxSpacing) && (value != v)) {
                value = v;
                return true;
            }
        }
    }

    return false;
}


void MeaGridDialog::OnOK()
{
    // If a field value is invalid, show an error dialog, set the
    // focus to the text field containing the invalid value and
    // highlight the contents of the field.
    //
    long v;

    v = 0;
    if (!GetFieldValue(IDC_GRID_HSPACE, v)) {
        AfxMessageBox(IDS_MEA_GRID_FIELD_VALUE, MB_OK | MB_ICONSTOP);
        m_hSpaceField.SetFocus();
        m_hSpaceField.SetSel(0, -1);
        return;
    }

    v = 0;  
    if (!GetFieldValue(IDC_GRID_VSPACE, v)) {
        AfxMessageBox(IDS_MEA_GRID_FIELD_VALUE, MB_OK | MB_ICONSTOP);
        m_vSpaceField.SetFocus();
        m_vSpaceField.SetSel(0, -1);
        return;
    }

    CDialog::OnOK();
}


void MeaGridDialog::OnCancel()
{
    m_spacing = m_origSpacing;
    m_linked = m_origLinked;
    UpdateData(FALSE);      // Load the dialog controls
    UpdateDisplay();

    CDialog::OnCancel();
}

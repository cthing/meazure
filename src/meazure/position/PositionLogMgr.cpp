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
#include <meazure/resource.h>
#include "PositionLogMgr.h"
#include "PositionLogDlg.h"
#include "PositionSaveDlg.h"
#include "PositionLogWriter.h"
#include <meazure/tools/ToolMgr.h>
#include <meazure/tools/Tool.h>
#include <meazure/utilities/NumericUtils.h>
#include <meazure/utilities/Geometry.h>
#include <meazure/utilities/StringUtils.h>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <cassert>


MeaPositionLogMgr::MeaPositionLogMgr(token) :
    MeaSingleton_T<MeaPositionLogMgr>(),
    m_observer(nullptr),
    m_saveDialog(nullptr),
    m_loadDialog(nullptr),
    m_saveDlgTitle(reinterpret_cast<PCSTR>(IDS_MEA_SAVE_LOG_DLG)),
    m_loadDlgTitle(reinterpret_cast<PCSTR>(IDS_MEA_LOAD_LOG_DLG)),
    m_modified(false),
    m_manageDialog(nullptr) {
    m_title.Format(_T("%s Position Log File"), static_cast<PCTSTR>(AfxGetAppName()));
}

MeaPositionLogMgr::~MeaPositionLogMgr() {
    try {
        delete m_saveDialog;
        delete m_loadDialog;

        m_manageDialog = nullptr;
        m_observer = nullptr;
    } catch (...) {
        assert(false);
    }
}

void MeaPositionLogMgr::SaveProfile(MeaProfile& profile) const {
    if (!profile.UserInitiated()) {
        profile.WriteStr(_T("LastLogDir"), static_cast<PCTSTR>(m_initialDir));
    }
}

void MeaPositionLogMgr::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        m_initialDir = profile.ReadStr(_T("LastLogDir"), static_cast<PCTSTR>(m_initialDir));
    }
}

void MeaPositionLogMgr::MasterReset() {
    m_initialDir.Empty();
}

void MeaPositionLogMgr::ManagePositions() {
    if (m_manageDialog != nullptr) {
        m_manageDialog->SetFocus();
    } else {
        m_manageDialog = new MeaPositionLogDlg();
        m_manageDialog->Create();
        m_manageDialog->ShowWindow(SW_SHOW);
    }
}

MeaPositionSaveDlg* MeaPositionLogMgr::CreateSaveDialog() {
    if (m_saveDialog == nullptr) {
        m_saveDialog = new MeaPositionSaveDlg(kExt, nullptr, kFilter);
        m_saveDialog->m_ofn.lpstrTitle = m_saveDlgTitle;
        m_saveDialog->m_ofn.lpstrInitialDir = m_initialDir;
        m_saveDialog->m_title = m_title;
        m_saveDialog->m_desc = m_desc;
    }
    return m_saveDialog;
}

CFileDialog* MeaPositionLogMgr::CreateLoadDialog() {
    if (m_loadDialog == nullptr) {
        m_loadDialog = new CFileDialog(TRUE, kExt, nullptr,
            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, kFilter);
        m_loadDialog->m_ofn.lpstrTitle = m_loadDlgTitle;
        m_loadDialog->m_ofn.lpstrInitialDir = m_initialDir;
    }
    return m_loadDialog;
}

void MeaPositionLogMgr::RecordPosition() {
    MeaPosition* position = new MeaPosition(RecordDesktopInfo());
    MeaToolMgr::Instance().RecordPosition(*position);
    m_positions.Add(position);

    MeaToolMgr::Instance().StrobeTool();

    m_modified = true;

    if (m_observer != nullptr) {
        m_observer->PositionAdded(m_positions.Size() - 1);
    }
}

void MeaPositionLogMgr::ReplacePosition(int posIndex) {
    MeaPosition* position = new MeaPosition(RecordDesktopInfo());
    MeaToolMgr::Instance().RecordPosition(*position);
    m_positions.Set(posIndex, position);

    MeaToolMgr::Instance().StrobeTool();

    m_modified = true;

    if (m_observer != nullptr) {
        m_observer->PositionReplaced(posIndex);
    }
}

void MeaPositionLogMgr::DeletePosition(int posIndex) {
    m_positions.Delete(posIndex);

    m_modified = HavePositions();

    if (m_observer != nullptr) {
        m_observer->PositionDeleted(posIndex);
    }
}

void MeaPositionLogMgr::DeletePositions() {
    ClearPositions();
    ::MessageBeep(MB_OK);

    m_modified = false;

    if (m_observer != nullptr) {
        m_observer->PositionsDeleted();
    }
}

void MeaPositionLogMgr::ClearPositions() {
    m_positions.DeleteAll();
    m_desktopInfoMap.clear();
    m_refCountMap.clear();
}

void MeaPositionLogMgr::ShowPosition(unsigned int posIndex) {
    if (posIndex >= m_positions.Size()) {
        return;
    }

    MeaToolMgr& toolMgr = MeaToolMgr::Instance();
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    MeaPosition& position = m_positions.Get(posIndex);

    // Change the radio tool, if needed. Note that if the position
    // used the cursor tool, we display it using the point tool so
    // that the cursor is not pulled out from under the user. In
    // addition, if the position used the window tool, we display
    // it using the rectangle tool.
    //
    CString toolName(position.GetToolName());
    if (toolName == MeaCursorTool::kToolName) {
        toolName = MeaPointTool::kToolName;
    } else if (toolName == MeaWindowTool::kToolName) {
        toolName = MeaRectTool::kToolName;
    }

    if (toolName != toolMgr.GetToolName()) {
        toolMgr.SetRadioTool(toolName);
    }

    // Change the units if needed. If these are custom units perform
    // additional configuration.
    //
    const MeaPositionDesktop& desktopInfo = GetDesktopInfo(position.GetDesktopRef().GetId());
    CString unitsStr = desktopInfo.GetLinearUnits()->GetUnitsStr();
    CString anglesStr = desktopInfo.GetAngularUnits()->GetUnitsStr();

    if (unitsStr != unitsMgr.GetLinearUnitsStr()) {
        MeaLinearUnitsId unitsId = desktopInfo.GetLinearUnits()->GetUnitsId();

        if (unitsId == MeaCustomId) {
            MeaCustomUnits* custom = unitsMgr.GetCustomUnits();

            custom->SetName(desktopInfo.GetCustomName());
            custom->SetAbbrev(desktopInfo.GetCustomAbbrev());
            custom->SetScaleBasis(desktopInfo.GetCustomBasisStr());
            custom->SetScaleFactor(desktopInfo.GetCustomFactor());
            custom->SetDisplayPrecisions(desktopInfo.GetCustomPrecisions());
        }

        unitsMgr.SetLinearUnits(unitsStr);
        toolMgr.UpdateTools(MeaUpdateReason::UnitsChanged);
    }

    if (anglesStr != unitsMgr.GetAngularUnitsStr()) {
        unitsMgr.SetAngularUnits(anglesStr);
        toolMgr.UpdateTools(MeaUpdateReason::UnitsChanged);
    }

    // Set the origin and y-axis orientation.
    //
    bool invertY = desktopInfo.IsInvertY();
    if ((invertY && !unitsMgr.IsInvertY()) || (!invertY && unitsMgr.IsInvertY())) {
        unitsMgr.SetInvertY(invertY);
        toolMgr.UpdateTools();
    }

    POINT newOrigin = unitsMgr.UnconvertCoord(desktopInfo.GetOrigin());

    if ((newOrigin.x != unitsMgr.GetOrigin().x) || (newOrigin.y != unitsMgr.GetOrigin().y)) {
        unitsMgr.SetOrigin(newOrigin);
        toolMgr.UpdateTools(MeaUpdateReason::OriginChanged);
    }

    // Show the points.
    //
    MeaRadioTool::PointMap toolPoints;

    for (const auto& pointEntry : position.GetPoints()) {
        toolPoints[pointEntry.first] = unitsMgr.UnconvertCoord(pointEntry.second);
    }

    toolMgr.SetPosition(toolPoints);
}

MeaPositionDesktopRef MeaPositionLogMgr::RecordDesktopInfo() {
    MeaPositionDesktop desktopInfo(MeaUnitsMgr::Instance(), MeaScreenMgr::Instance());

    for (const auto& desktopEntry : m_desktopInfoMap) {
        if (desktopInfo == desktopEntry.second) {
            return MeaPositionDesktopRef(this, desktopEntry.second);
        }
    }

    m_desktopInfoMap.emplace(desktopInfo.GetId(), desktopInfo);
    return MeaPositionDesktopRef(this, desktopInfo);
}

const MeaPositionDesktop& MeaPositionLogMgr::GetDesktopInfo(const MeaGUID& id) const {
    DesktopInfoMap::const_iterator iter = m_desktopInfoMap.find(id);
    assert(iter != m_desktopInfoMap.end());  // Validator ensures this
    return (*iter).second;
}

void MeaPositionLogMgr::AddDesktopRef(const MeaGUID& id) {
    RefCountMap::iterator iter = m_refCountMap.find(id);
    if (iter == m_refCountMap.end()) {
        m_refCountMap[id] = 1;
    } else {
        (*iter).second++;
    }
}

void MeaPositionLogMgr::ReleaseDesktopRef(const MeaGUID& id) {
    RefCountMap::iterator iter = m_refCountMap.find(id);
    if (iter != m_refCountMap.end()) {
        if (--(*iter).second <= 0) {
            m_refCountMap.erase(iter);
        }
    }
}

bool MeaPositionLogMgr::IsPositionFile(PCTSTR filename) {
    TCHAR ext[_MAX_EXT];
    int i = 0;

    _tsplitpath_s(filename, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
    if (ext[i] == _T('.')) {
        i++;
    }
    return (_tcsicmp(&ext[i], kExt) == 0);
}

bool MeaPositionLogMgr::SaveIfModified() {
    bool result = true;

    if (IsModified()) {
        switch (AfxMessageBox(IDS_MEA_ASK_SAVE, MB_YESNOCANCEL)) {
        case IDCANCEL:
            result = false;     // Don't continue
            break;
        case IDYES:
            if (!Save(false)) {
                result = false; // Could not save so don't continue
            }
            break;
        default:
            break;
        }
    }

    return result;
}

bool MeaPositionLogMgr::Save(bool askPathname) {
    bool needPathname = (m_pathname.IsEmpty() || askPathname);

    if (needPathname) {
        MeaPositionSaveDlg* dlg = CreateSaveDialog();

        if (dlg->DoModal() == IDOK) {
            m_pathname = dlg->GetPathName();
            TCHAR drive[_MAX_DRIVE];
            TCHAR dir[_MAX_DIR];

            //
            // Remember the directory for persisting.
            //
            _tsplitpath_s(m_pathname, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
            m_initialDir = drive;
            m_initialDir += dir;

            // Save the log file title and description.
            //
            m_title = dlg->m_title;
            m_desc = dlg->m_desc;
        } else {
            return false;
        }
    }

    //
    // Save the positions
    //
    m_writeStream.exceptions(std::ios::failbit | std::ios::badbit);
    
    try {
        m_writeStream.open(MeaStringUtils::ACPtoUTF8(m_pathname), std::ios::out | std::ios::trunc);

        MeaXMLWriter writer(m_writeStream);
        MeaPositionLogWriter positionWriter(writer, *this);

        positionWriter.Save();

        Close();
    } catch (const std::ofstream::failure& e) {
        Close();

        CString errStr(e.what());
        CString msg;
        msg.Format(IDS_MEA_NO_SAVE_LOG, static_cast<PCTSTR>(errStr));
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
        return false;
    }

    m_modified = false;

    if (m_observer != nullptr) {
        m_observer->LogSaved();
    }

    return true;
}

bool MeaPositionLogMgr::Load(PCTSTR pathname) {
    // If there is a modified set of positions, ask the user if
    // they should be saved before loading a new set.
    //
    if (!SaveIfModified()) {
        return false;
    }

    if (pathname != nullptr) {
        m_pathname = pathname;
    } else {
        CFileDialog* dlg = CreateLoadDialog();

        if (dlg->DoModal() != IDOK) {
            return false;
        }
        m_pathname = dlg->GetPathName();
    }

    bool status = false;
    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];

    //
    // Remember the directory for persisting.
    //
    _tsplitpath_s(m_pathname, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
    m_initialDir = drive;
    m_initialDir += dir;

    //
    // Delete old positions.
    //
    ClearPositions();

    //
    // Parse the contents of the log file
    //
    MeaXMLParser parser(this, true);

    try {
        parser.ParseFile(m_pathname);
        status = true;
    } catch (MeaXMLParserException&) {
        // Handled by the parser.
    } catch (...) {
        CString msg(reinterpret_cast<PCSTR>(IDS_MEA_INVALID_LOGFILE));
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
    }

    if (status) {
        //
        // Process the log file DOM
        //
        ProcessDOM(parser.GetDOM());

        m_modified = false;

        if (m_observer != nullptr) {
            m_observer->LogLoaded();
        }
    }

    return status;
}

void MeaPositionLogMgr::Close() {
    try {
        if (m_writeStream.is_open()) {
            m_writeStream.close();
        }
    } catch (...) {
        assert(false);
    }
}

void MeaPositionLogMgr::ProcessDOM(const MeaXMLNode* dom) {
    for (MeaXMLNode::NodeIter_c iter = dom->GetChildIter(); !dom->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("info")) {
                ProcessInfoNode(node);
            } else if (node->GetData() == _T("desktops")) {
                for (MeaXMLNode::NodeIter_c desktopIter = node->GetChildIter();
                                            !node->AtEnd(desktopIter); ++desktopIter) {
                    ProcessDesktopNode(*desktopIter);
                }
            } else if (node->GetData() == _T("positions")) {
                for (MeaXMLNode::NodeIter_c posIter = node->GetChildIter();
                                            !node->AtEnd(posIter); ++posIter) {
                    ProcessPositionNode(*posIter);
                }
            }
        }
    }
}

void MeaPositionLogMgr::ProcessInfoNode(const MeaXMLNode* infoNode) {
    for (MeaXMLNode::NodeIter_c iter = infoNode->GetChildIter(); !infoNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("title")) {
                m_title = MeaStringUtils::LFtoCRLF(node->GetChildData());
            } else if (node->GetData() == _T("desc")) {
                m_desc = MeaStringUtils::LFtoCRLF(node->GetChildData());
            }
        }
    }
}

void MeaPositionLogMgr::ProcessDesktopNode(const MeaXMLNode* desktopNode) {
    CString valueStr;

    desktopNode->GetAttributes().GetValueStr(_T("id"), valueStr);

    try {
        MeaPositionDesktop desktopInfo(valueStr, MeaUnitsMgr::Instance(), MeaScreenMgr::Instance());

        desktopInfo.Load(desktopNode);

        const MeaGUID& guid = desktopInfo.GetId();
        m_desktopInfoMap.emplace(guid, desktopInfo);
    } catch (COleException* ex) {
        ex->Delete();

        CString msg;
        msg.Format(IDS_MEA_INVALID_DESKTOPID, static_cast<PCTSTR>(valueStr));
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
    }
}

void MeaPositionLogMgr::ProcessPositionNode(const MeaXMLNode* positionNode) {
    CString idStr;
    CString toolStr;
    CString dateStr;

    positionNode->GetAttributes().GetValueStr(_T("desktopRef"), idStr);
    positionNode->GetAttributes().GetValueStr(_T("tool"), toolStr);
    positionNode->GetAttributes().GetValueStr(_T("date"), dateStr);

    try {
        MeaPositionDesktopRef desktopRef(this, idStr);
        MeaPosition* position = new MeaPosition(desktopRef, toolStr, dateStr);

        position->Load(positionNode);

        m_positions.Add(position);
    } catch (COleException* ex) {
        ex->Delete();

        CString msg;
        msg.Format(IDS_MEA_INVALID_DESKTOPREF, static_cast<PCTSTR>(idStr));
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
    }
}

xercesc::InputSource* MeaPositionLogMgr::ResolveEntity(const CString& pathname) {
    CStringW widePathname(pathname);
    return new xercesc::LocalFileInputSource(reinterpret_cast<const XMLCh* const>(static_cast<PCWSTR>(widePathname)));
}

CString MeaPositionLogMgr::GetFilePathname() {
    return m_pathname;
}

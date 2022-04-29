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
#include "PositionLogMgr.h"
#include "PositionLogDlg.h"
#include "PositionSaveDlg.h"
#include "ToolMgr.h"
#include "Tool.h"
#include "VersionInfo.h"
#include "TimeStamp.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "Utils.h"


MEA_SINGLETON_DEF(MeaPositionLogMgr);   ///< Managers are singletons.


MeaPositionLogMgr::MeaPositionLogMgr() :
    MeaXMLParserHandler(), MeaSingleton_T<MeaPositionLogMgr>(),
    m_observer(nullptr),
    m_saveDialog(nullptr),
    m_loadDialog(nullptr),
    m_saveDlgTitle(reinterpret_cast<LPCSTR>(IDS_MEA_SAVE_LOG_DLG)),
    m_loadDlgTitle(reinterpret_cast<LPCSTR>(IDS_MEA_LOAD_LOG_DLG)),
    m_stdioOpen(false),
    m_modified(false),
    m_manageDialog(nullptr) {
    m_title.Format(_T("%s Position Log File"), static_cast<LPCTSTR>(AfxGetAppName()));
}

MeaPositionLogMgr::~MeaPositionLogMgr() {
    try {
        delete m_saveDialog;
        delete m_loadDialog;

        m_manageDialog = nullptr;
        m_observer = nullptr;
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaPositionLogMgr::SaveProfile(MeaProfile& profile) const {
    if (!profile.UserInitiated()) {
        profile.WriteStr(_T("LastLogDir"), static_cast<LPCTSTR>(m_initialDir));
    }
}

void MeaPositionLogMgr::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        m_initialDir = profile.ReadStr(_T("LastLogDir"), static_cast<LPCTSTR>(m_initialDir));
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
    m_positions.Add(new Position(this, RecordDesktopInfo()));

    MeaToolMgr::Instance().StrobeTool();

    m_modified = true;

    if (m_observer != nullptr) {
        m_observer->PositionAdded(m_positions.Size() - 1);
    }
}

void MeaPositionLogMgr::ReplacePosition(int posIndex) {
    m_positions.Set(posIndex, new Position(this, RecordDesktopInfo()));

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
    if (posIndex < m_positions.Size()) {
        Position& position = m_positions.Get(posIndex);
        position.Show();
    }
}

MeaGUID MeaPositionLogMgr::RecordDesktopInfo() {
    DesktopInfo desktopInfo;

    for (const auto& desktopEntry : m_desktopInfoMap) {
        if (desktopInfo == desktopEntry.second) {
            return desktopEntry.first;
        }
    }

    const MeaGUID& guid = desktopInfo.GetId();
    m_desktopInfoMap[guid] = desktopInfo;
    return guid;
}

MeaPositionLogMgr::DesktopInfo& MeaPositionLogMgr::GetDesktopInfo(const MeaGUID& id) {
    DesktopInfoMap::iterator iter = m_desktopInfoMap.find(id);
    MeaAssert(iter != m_desktopInfoMap.end());  // Validator ensures this
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

bool MeaPositionLogMgr::IsPositionFile(LPCTSTR filename) {
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
    int indent = 0;

    if (!Open(m_pathname, CFile::modeWrite | CFile::modeCreate)) {
        return false;
    }

    Write(indent, _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
    Write(indent, _T("<!DOCTYPE positionLog SYSTEM \"https://www.cthing.com/dtd/PositionLog1.dtd\">\n"));
    Write(indent, _T("<positionLog version=\"%d\">\n"), g_versionInfo.GetLogFileMajor());
    indent++;
    WriteInfoSection(indent);
    WriteDesktopsSection(indent);
    WritePositionsSection(indent);
    indent--;
    Write(indent, _T("</positionLog>\n"));

    Close();

    m_modified = false;

    if (m_observer != nullptr) {
        m_observer->LogSaved();
    }

    return true;
}

bool MeaPositionLogMgr::Load(LPCTSTR pathname) {
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
    // Load the positions
    //
    if (Open(m_pathname, CFile::modeRead)) {
        //
        // Delete old positions.
        //
        ClearPositions();

        //
        // Parse the contents of the log file
        //
        MeaXMLParser parser(this, true);

        try {
            UINT numBytes;

            do {
                void* buf = parser.GetBuffer(kChunkSize);
                numBytes = m_stdioFile.Read(buf, kChunkSize);
                parser.ParseBuffer(numBytes, numBytes == 0);
            } while (numBytes > 0);

            status = true;
        } catch (MeaXMLParserException&) {
            // Handled by the parser.
        } catch (MeaLogFileException&) {
            CString msg(reinterpret_cast<LPCSTR>(IDS_MEA_INVALID_LOGFILE));
            MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
        } catch (...) {
            CString msg(reinterpret_cast<LPCSTR>(IDS_MEA_NO_POSITIONS));
            MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
        }

        Close();

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
    } else {
        m_pathname.Empty();
    }

    return status;
}

bool MeaPositionLogMgr::Open(const CString& pathname, UINT mode) {
    CFileException fe;

    m_stdioOpen = false;

    if (m_stdioFile.Open(pathname, mode, &fe) == FALSE) {
        TCHAR errStr[256];
        CString msg;
        fe.GetErrorMessage(errStr, 256);
        msg.Format(((mode & CFile::modeRead) ? IDS_MEA_NO_LOAD_LOG : IDS_MEA_NO_SAVE_LOG), errStr);
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
        return false;
    }

    m_stdioOpen = true;

    return true;
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
                m_title = ProcessDataNodes(node);
            } else if (node->GetData() == _T("desc")) {
                m_desc = ProcessDataNodes(node);
            }
        }
    }
}

void MeaPositionLogMgr::ProcessDesktopNode(const MeaXMLNode* desktopNode) {
    CString valueStr;
    bool def;

    desktopNode->GetAttributes().GetValueStr(_T("id"), valueStr, def);

    try {
        DesktopInfo desktopInfo(valueStr);

        desktopInfo.Load(desktopNode);

        const MeaGUID& guid = desktopInfo.GetId();
        m_desktopInfoMap[guid] = desktopInfo;
    } catch (COleException* ex) {
        ex->Delete();

        CString msg;
        msg.Format(IDS_MEA_INVALID_DESKTOPID, static_cast<LPCTSTR>(valueStr));
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
    }
}

void MeaPositionLogMgr::ProcessPositionNode(const MeaXMLNode* positionNode) {
    CString idStr;
    CString toolStr;
    CString dateStr;
    bool def;

    positionNode->GetAttributes().GetValueStr(_T("desktopRef"), idStr, def);
    positionNode->GetAttributes().GetValueStr(_T("tool"), toolStr, def);
    positionNode->GetAttributes().GetValueStr(_T("date"), dateStr, def);

    try {
        Position* position = new Position(this, idStr, toolStr, dateStr);

        position->Load(positionNode);

        m_positions.Add(position);
    } catch (COleException* ex) {
        ex->Delete();

        CString msg;
        msg.Format(IDS_MEA_INVALID_DESKTOPREF, static_cast<LPCTSTR>(idStr));
        MessageBox(*AfxGetMainWnd(), msg, nullptr, MB_OK | MB_ICONERROR);
    }
}

CString MeaPositionLogMgr::ProcessDataNodes(const MeaXMLNode* elementNode) {
    CString data;

    for (MeaXMLNode::NodeIter_c iter = elementNode->GetChildIter(); !elementNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;

        if (node->GetType() == MeaXMLNode::Type::Data) {
            data += node->GetData();
        }
    }

    return MeaUtils::LFtoCRLF(data);
}

void MeaPositionLogMgr::WriteInfoSection(int indent) {
    TCHAR nameBuffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    GetComputerName(nameBuffer, &size);

    Write(indent, _T("<info>\n"));
    indent++;
    Write(indent, _T("<title>%s</title>\n"), static_cast<LPCTSTR>(MeaXMLParser::Encode(MeaUtils::CRLFtoLF(m_title))));
    Write(indent, _T("<created date=\"%s\"/>\n"), static_cast<LPCTSTR>(MeaMakeTimeStamp(time(nullptr))));
    Write(indent, _T("<generator name=\"%s\" version=\"%s\" build=\"%d\"/>\n"),
          static_cast<LPCTSTR>(AfxGetAppName()),
          static_cast<LPCTSTR>(g_versionInfo.GetProductVersion()),
          g_versionInfo.GetProductBuild());
    Write(indent, _T("<machine name=\"%s\"/>\n"), nameBuffer);
    if (!m_desc.IsEmpty()) {
        Write(indent, _T("<desc>%s</desc>\n"), static_cast<LPCTSTR>(MeaXMLParser::Encode(MeaUtils::CRLFtoLF(m_desc))));
    }
    indent--;
    Write(indent, _T("</info>\n"));
}

void MeaPositionLogMgr::WriteDesktopsSection(int indent) {
    Write(indent, _T("<desktops>\n"));
    indent++;

    for (const auto& refCountEntry : m_refCountMap) {
        GetDesktopInfo(refCountEntry.first).Save(*this, indent);
    }

    indent--;
    Write(indent, _T("</desktops>\n"));
}

void MeaPositionLogMgr::WritePositionsSection(int indent) {
    Write(indent, _T("<positions>\n"));
    indent++;
    m_positions.Save(indent);
    indent--;
    Write(indent, _T("</positions>\n"));
}

void MeaPositionLogMgr::Write(int indentLevel, LPCTSTR format, ...) {
    CString indent(_T(' '), indentLevel * 4);
    CString str;
    va_list args;

    va_start(args, format);
    str.FormatV(format, args);
    va_end(args);

    m_stdioFile.WriteString(indent + MeaXMLParser::ToUTF8(str));
}

void MeaPositionLogMgr::ParseEntity(MeaXMLParser& parser,
                            const CString& pathname) {
    CFile entityFile;
    CFileException fe;

    if (!entityFile.Open(pathname, CFile::modeRead, &fe)) {
        AfxThrowFileException(fe.m_cause, fe.m_lOsError, pathname);
    }

    MeaXMLParser entityParser(parser);

    // Read the contents of the entity file into a parsing buffer.
    //
    int size = static_cast<int>(entityFile.GetLength());
    void* buf = entityParser.GetBuffer(size);
    UINT count = entityFile.Read(buf, size);

    // Parse the entity file
    //
    entityParser.ParseBuffer(count, true);

    entityFile.Close();
}

CString MeaPositionLogMgr::GetFilePathname() {
    return m_stdioOpen ? m_stdioFile.GetFilePath() : m_pathname;
}


//*************************************************************************
// Screen
//*************************************************************************


MeaPositionLogMgr::Screen::Screen(const MeaScreenMgr::ScreenIter& screenIter) {
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();
    MeaScreenMgr& screenMgr = MeaScreenMgr::Instance();

    const CRect& rect = screenMgr.GetScreenRect(screenIter);
    FPOINT p1 = unitsMgr.ConvertCoord(rect.TopLeft());
    FPOINT p2 = unitsMgr.ConvertCoord(rect.BottomRight());

    m_rect.top = p1.y;
    m_rect.bottom = p2.y;
    m_rect.left = p1.x;
    m_rect.right = p2.x;

    m_primary = screenMgr.IsPrimary(screenIter);
    m_manualRes = screenMgr.IsManualRes(screenIter);
    m_res = unitsMgr.ConvertRes(screenMgr.GetScreenRes(screenIter));
    m_desc = screenMgr.GetScreenName(screenIter);
}

void MeaPositionLogMgr::Screen::Load(const MeaXMLNode* screenNode) {
    CString valueStr;
    bool def;

    screenNode->GetAttributes().GetValueBool(_T("primary"), m_primary, def);
    screenNode->GetAttributes().GetValueStr(_T("desc"), m_desc, def);

    for (MeaXMLNode::NodeIter_c iter = screenNode->GetChildIter(); !screenNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;
        const MeaXMLAttributes& attrs = node->GetAttributes();

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("rect")) {
                attrs.GetValueDbl(_T("top"), m_rect.top, def);
                attrs.GetValueDbl(_T("bottom"), m_rect.bottom, def);
                attrs.GetValueDbl(_T("left"), m_rect.left, def);
                attrs.GetValueDbl(_T("right"), m_rect.right, def);
            } else if (node->GetData() == _T("resolution")) {
                attrs.GetValueDbl(_T("x"), m_res.cx, def);
                attrs.GetValueDbl(_T("y"), m_res.cy, def);
                attrs.GetValueBool(_T("manual"), m_manualRes, def);
            }
        }
    }
}

void MeaPositionLogMgr::Screen::Save(MeaPositionLogMgr& mgr, int indent) const {
    mgr.Write(indent, _T("<screen desc=\"%s\" primary=\"%s\">\n"), static_cast<LPCTSTR>(m_desc),
              (m_primary ? _T("true") : _T("false")));
    indent++;
    mgr.Write(indent, _T("<rect top=\"%s\" bottom=\"%s\" left=\"%s\" right=\"%s\"/>\n"),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_rect.top)),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_rect.bottom)),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_rect.left)),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_rect.right)));
    mgr.Write(indent, _T("<resolution x=\"%s\" y=\"%s\" manual=\"%s\"/>\n"),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_res.cx)),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_res.cy)),
              (m_manualRes ? _T("true") : _T("false")));
    indent--;
    mgr.Write(indent, _T("</screen>\n"));
}


//*************************************************************************
// DesktopInfo
//*************************************************************************


MeaPositionLogMgr::DesktopInfo::DesktopInfo() {
    Init();
}

MeaPositionLogMgr::DesktopInfo::DesktopInfo(LPCTSTR guidStr) : m_id(guidStr) {
    Init();
}

void MeaPositionLogMgr::DesktopInfo::Init() {
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();
    MeaScreenMgr& screenMgr = MeaScreenMgr::Instance();

    const CRect& size = screenMgr.GetVirtualRect();
    CPoint bottomRight(size.BottomRight());
    bottomRight.Offset(-1, -1);

    m_linearUnits = unitsMgr.GetLinearUnits();
    m_angularUnits = unitsMgr.GetAngularUnits();
    m_invertY = unitsMgr.GetInvertY();
    m_origin = unitsMgr.ConvertPos(unitsMgr.GetOrigin());
    m_size = unitsMgr.GetWidthHeight(size.TopLeft(), bottomRight);

    if (m_linearUnits->GetUnitsId() == MeaCustomId) {
        MeaCustomUnits* custom = static_cast<MeaCustomUnits*>(m_linearUnits);

        m_customName = custom->GetName();
        m_customAbbrev = custom->GetAbbrev();
        m_customBasisStr = custom->GetScaleBasisStr();
        m_customFactor = custom->GetScaleFactor();
        m_customPrecisions = custom->GetDefaultPrecisions();
    } else {
        m_customName = _T("");
        m_customAbbrev = _T("");
        m_customBasisStr = _T("");
        m_customFactor = 0.0;
        m_customPrecisions.clear();
    }

    MeaScreenMgr::ScreenIter iter;
    for (iter = screenMgr.GetScreenIter(); !screenMgr.AtEnd(iter); ++iter) {
        Screen screen(iter);

        m_screens.push_back(screen);
    }
}

void MeaPositionLogMgr::DesktopInfo::SetLinearUnits(const CString& unitsStr) {
    m_linearUnits = MeaUnitsMgr::Instance().GetLinearUnits(unitsStr);
    MeaAssert(m_linearUnits != nullptr);
}

void MeaPositionLogMgr::DesktopInfo::SetAngularUnits(const CString& unitsStr) {
    m_angularUnits = MeaUnitsMgr::Instance().GetAngularUnits(unitsStr);
    MeaAssert(m_angularUnits != nullptr);
}

void MeaPositionLogMgr::DesktopInfo::Load(const MeaXMLNode* desktopNode) {
    CString valueStr;
    bool def;

    for (MeaXMLNode::NodeIter_c iter = desktopNode->GetChildIter(); !desktopNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;
        const MeaXMLAttributes& attrs = node->GetAttributes();

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("units")) {
                attrs.GetValueStr(_T("length"), valueStr, def);
                SetLinearUnits(valueStr);
                attrs.GetValueStr(_T("angle"), valueStr, def);
                SetAngularUnits(valueStr);
            } else if (node->GetData() == _T("customUnits")) {
                attrs.GetValueStr(_T("name"), m_customName, def);
                attrs.GetValueStr(_T("abbrev"), m_customAbbrev, def);
                attrs.GetValueStr(_T("scaleBasis"), m_customBasisStr, def);
                attrs.GetValueDbl(_T("scaleFactor"), m_customFactor, def);
            } else if (node->GetData() == _T("origin")) {
                attrs.GetValueDbl(_T("xoffset"), m_origin.x, def);
                attrs.GetValueDbl(_T("yoffset"), m_origin.y, def);
                attrs.GetValueBool(_T("invertY"), m_invertY, def);
            } else if (node->GetData() == _T("size")) {
                attrs.GetValueDbl(_T("x"), m_size.cx, def);
                attrs.GetValueDbl(_T("y"), m_size.cy, def);
            } else if (node->GetData() == _T("screens")) {
                m_screens.clear();
                for (MeaXMLNode::NodeIter_c screenIter = node->GetChildIter();
                     !node->AtEnd(screenIter); ++screenIter) {
                    Screen screen;
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

void MeaPositionLogMgr::DesktopInfo::Save(MeaPositionLogMgr& mgr, int indent) const {
    mgr.Write(indent, _T("<desktop id=\"%s\">\n"), static_cast<LPCTSTR>(m_id));
    indent++;
    mgr.Write(indent, _T("<units length=\"%s\" angle=\"%s\"/>\n"),
        static_cast<LPCTSTR>(m_linearUnits->GetUnitsStr()),
        static_cast<LPCTSTR>(m_angularUnits->GetUnitsStr()));

    if (m_linearUnits->GetUnitsId() == MeaCustomId) {
        mgr.Write(indent, _T("<customUnits name=\"%s\" abbrev=\"%s\" scaleBasis=\"%s\" scaleFactor=\"%f\"/>\n"),
                  static_cast<LPCTSTR>(m_customName),
                  static_cast<LPCTSTR>(m_customAbbrev),
                  static_cast<LPCTSTR>(m_customBasisStr), m_customFactor);
    }

    mgr.Write(indent, _T("<origin xoffset=\"%s\" yoffset=\"%s\" invertY=\"%s\"/>\n"),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_origin.x)),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_origin.y)),
              (m_invertY ? _T("true") : _T("false")));
    mgr.Write(indent, _T("<size x=\"%s\" y=\"%s\"/>\n"),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_size.cx)),
              static_cast<LPCTSTR>(MeaUtils::DblToStr(m_size.cy)));

    mgr.Write(indent, _T("<screens>\n"));
    indent++;
    for (const auto& screen : m_screens) {
        screen.Save(mgr, indent);
    }
    indent--;
    mgr.Write(indent, _T("</screens>\n"));

    if (m_linearUnits->GetUnitsId() == MeaCustomId) {
        mgr.Write(indent, _T("<displayPrecisions>\n"));
        indent++;
        SaveCustomPrecisions(mgr, indent);
        indent--;
        mgr.Write(indent, _T("</displayPrecisions>\n"));
    }
    indent--;
    mgr.Write(indent, _T("</desktop>\n"));
}

void MeaPositionLogMgr::DesktopInfo::LoadCustomPrecisions(const MeaXMLNode* displayPrecisionNode) {
    typedef std::map<CString, int> PrecisionMap;
    typedef PrecisionMap::const_iterator PrecisionIter;

    bool def;
    PrecisionMap precMap;

    for (MeaXMLNode::NodeIter_c measurementIter = displayPrecisionNode->GetChildIter();
                                !displayPrecisionNode->AtEnd(measurementIter); ++measurementIter) {
        MeaXMLNode* measurementNode = *measurementIter;
        const MeaXMLAttributes& attrs = measurementNode->GetAttributes();

        if (measurementNode->GetData() == _T("measurement")) {
            CString name;
            int places;

            attrs.GetValueStr(_T("name"), name, def);
            attrs.GetValueInt(_T("decimalPlaces"), places, def);

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

void MeaPositionLogMgr::DesktopInfo::SaveCustomPrecisions(MeaPositionLogMgr& mgr, int indent) const {
    const MeaUnits::DisplayPrecisionNames& precisionNames = m_linearUnits->GetDisplayPrecisionNames();
    unsigned int i;

    mgr.Write(indent, _T("<displayPrecision units=\"%s\">\n"),
        static_cast<LPCTSTR>(m_linearUnits->GetUnitsStr()));
    indent++;
    for (i = 0; i < m_customPrecisions.size(); i++) {
        mgr.Write(indent, _T("<measurement name=\"%s\" decimalPlaces=\"%d\"/>\n"),
            static_cast<LPCTSTR>(precisionNames[i]), m_customPrecisions[i]);
    }
    indent--;
    mgr.Write(indent, _T("</displayPrecision>\n"));
}


//*************************************************************************
// Position
//*************************************************************************


MeaPositionLogMgr::Position::Position() :
    m_mgr(nullptr),
    m_fieldMask(0),
    m_width(0.0),
    m_height(0.0),
    m_distance(0.0),
    m_area(0.0),
    m_angle(0.0) {}

MeaPositionLogMgr::Position::Position(MeaPositionLogMgr* mgr,
                                      const MeaGUID& desktopInfoId) :
    m_mgr(mgr),
    m_fieldMask(0),
    m_width(0.0),
    m_height(0.0),
    m_distance(0.0),
    m_area(0.0),
    m_angle(0.0),
    m_desktopInfoId(desktopInfoId),
    m_toolName(MeaToolMgr::Instance().GetToolName()),
    m_timestamp(MeaMakeTimeStamp(time(nullptr))) {
    MeaToolMgr::Instance().GetPosition(*this);

    if (m_mgr != nullptr) {
        m_mgr->AddDesktopRef(m_desktopInfoId);
    }
}

MeaPositionLogMgr::Position::Position(MeaPositionLogMgr* mgr, const CString& desktopInfoIdStr,
                              const CString& toolName, const CString& timestamp) :
    m_mgr(mgr),
    m_fieldMask(0),
    m_width(0.0),
    m_height(0.0),
    m_distance(0.0),
    m_area(0.0),
    m_angle(0.0),
    m_desktopInfoId(desktopInfoIdStr),
    m_toolName(toolName),
    m_timestamp(timestamp) {
    if (m_mgr != nullptr) {
        m_mgr->AddDesktopRef(m_desktopInfoId);
    }
}

MeaPositionLogMgr::Position::Position(const Position& position) : m_mgr(nullptr),
m_fieldMask(0) {
    Copy(position);
}

MeaPositionLogMgr::Position::~Position() {
    try {
        if (m_mgr != nullptr) {
            m_mgr->ReleaseDesktopRef(m_desktopInfoId);
        }
    } catch (...) {
        MeaAssert(false);
    }
}

MeaPositionLogMgr::Position& MeaPositionLogMgr::Position::Copy(const Position& position) {
    if (&position != this) {
        if (m_mgr != nullptr) {
            m_mgr->ReleaseDesktopRef(m_desktopInfoId);
        }

        m_mgr = position.m_mgr;
        m_desktopInfoId = position.m_desktopInfoId;
        m_toolName = position.m_toolName;
        m_timestamp = position.m_timestamp;
        m_desc = position.m_desc;

        m_fieldMask = position.m_fieldMask;
        m_points = position.m_points;
        m_width = position.m_width;
        m_height = position.m_height;
        m_distance = position.m_distance;
        m_area = position.m_area;
        m_angle = position.m_angle;

        if (m_mgr != nullptr) {
            m_mgr->AddDesktopRef(m_desktopInfoId);
        }
    }

    return *this;
}

void MeaPositionLogMgr::Position::RecordXY1(const FPOINT& point) {
    m_fieldMask |= MeaX1Field | MeaY1Field;

    m_points[_T("1")] = point;
}

void MeaPositionLogMgr::Position::RecordXY2(const FPOINT& point) {
    m_fieldMask |= MeaX2Field | MeaY2Field;

    m_points[_T("2")] = point;
}

void MeaPositionLogMgr::Position::RecordXYV(const FPOINT& point) {
    m_fieldMask |= MeaXVField | MeaYVField;

    m_points[_T("v")] = point;
}

void MeaPositionLogMgr::Position::RecordWH(const FSIZE& size) {
    m_fieldMask |= MeaWidthField | MeaHeightField;

    m_width = size.cx;
    m_height = size.cy;
}

void MeaPositionLogMgr::Position::RecordDistance(const FSIZE& size) {
    m_fieldMask |= MeaDistanceField;

    m_distance = MeaLayout::CalcLength(size.cx, size.cy);
}

void MeaPositionLogMgr::Position::RecordDistance(double dist) {
    m_fieldMask |= MeaDistanceField;

    m_distance = dist;
}

void MeaPositionLogMgr::Position::RecordAngle(double angle) {
    m_fieldMask |= MeaAngleField;

    m_angle = MeaUnitsMgr::Instance().ConvertAngle(angle);
}

void MeaPositionLogMgr::Position::RecordRectArea(const FSIZE& size) {
    m_fieldMask |= MeaAreaField;

    m_area = size.cx * size.cy;
}

void MeaPositionLogMgr::Position::RecordCircleArea(double radius) {
    m_fieldMask |= MeaAreaField;

    m_area = MeaUnits::kPI * radius * radius;
}

void MeaPositionLogMgr::Position::Show() const {
    MeaToolMgr& toolMgr = MeaToolMgr::Instance();
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    // Change the radio tool, if needed. Note that if the position
    // used the cursor tool, we display it using the point tool so
    // that the cursor is not pulled out from under the user. In
    // addition, if the position used the window tool, we display
    // it using the rectangle tool.
    //
    CString toolName(m_toolName);
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
    const DesktopInfo& desktopInfo = m_mgr->GetDesktopInfo(m_desktopInfoId);
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
    bool invertY = desktopInfo.GetInvertY();
    if ((invertY && !unitsMgr.GetInvertY()) || (!invertY && unitsMgr.GetInvertY())) {
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

    for (const auto& pointEntry : m_points) {
        toolPoints[pointEntry.first] = unitsMgr.UnconvertCoord(pointEntry.second);
    }

    toolMgr.SetPosition(toolPoints);
}

void MeaPositionLogMgr::Position::Load(const MeaXMLNode* positionNode) {
    bool def;

    for (MeaXMLNode::NodeIter_c iter = positionNode->GetChildIter(); !positionNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("desc")) {
                SetDesc(ProcessDataNodes(node));
            } else if (node->GetData() == _T("points")) {
                for (MeaXMLNode::NodeIter_c pointIter = node->GetChildIter(); !node->AtEnd(pointIter); ++pointIter) {
                    MeaXMLNode* pointNode = *pointIter;
                    const MeaXMLAttributes& attrs = pointNode->GetAttributes();

                    if ((pointNode->GetType() == MeaXMLNode::Type::Element) && (pointNode->GetData() == _T("point"))) {
                        CString name;
                        FPOINT pt;
                        attrs.GetValueStr(_T("name"), name, def);
                        attrs.GetValueDbl(_T("x"), pt.x, def);
                        attrs.GetValueDbl(_T("y"), pt.y, def);
                        AddPoint(name, pt);
                    }
                }
            } else if (node->GetData() == _T("properties")) {
                for (MeaXMLNode::NodeIter_c pointIter = node->GetChildIter(); !node->AtEnd(pointIter); ++pointIter) {
                    MeaXMLNode* pointNode = *pointIter;
                    const MeaXMLAttributes& attrs = pointNode->GetAttributes();

                    if (pointNode->GetType() == MeaXMLNode::Type::Element) {
                        if (pointNode->GetData() == _T("width")) {
                            attrs.GetValueDbl(_T("value"), m_width, def);
                            m_fieldMask |= MeaWidthField;
                        } else if (pointNode->GetData() == _T("height")) {
                            attrs.GetValueDbl(_T("value"), m_height, def);
                            m_fieldMask |= MeaHeightField;
                        } else if (pointNode->GetData() == _T("distance")) {
                            attrs.GetValueDbl(_T("value"), m_distance, def);
                            m_fieldMask |= MeaDistanceField;
                        } else if (pointNode->GetData() == _T("area")) {
                            attrs.GetValueDbl(_T("value"), m_area, def);
                            m_fieldMask |= MeaAreaField;
                        } else if (pointNode->GetData() == _T("angle")) {
                            attrs.GetValueDbl(_T("value"), m_angle, def);
                            m_fieldMask |= MeaAngleField;
                        }
                    }
                }
            }
        }
    }
}

void MeaPositionLogMgr::Position::Save(int indent) const {
    if (m_mgr == nullptr) {
        return;
    }

    m_mgr->Write(indent, _T("<position desktopRef=\"%s\" tool=\"%s\" date=\"%s\">\n"),
                 static_cast<LPCTSTR>(m_desktopInfoId),
                 static_cast<LPCTSTR>(m_toolName),
                 static_cast<LPCTSTR>(m_timestamp));
    indent++;
    if (!m_desc.IsEmpty()) {
        m_mgr->Write(indent, _T("<desc>%s</desc>\n"),
                     static_cast<LPCTSTR>(MeaXMLParser::Encode(MeaUtils::CRLFtoLF(m_desc))));
    }

    m_mgr->Write(indent, _T("<points>\n"));
    indent++;
    for (const auto& pointEntry : m_points) {
        m_mgr->Write(indent, _T("<point name=\"%s\" x=\"%s\" y=\"%s\"/>\n"),
                     static_cast<LPCTSTR>(pointEntry.first),
                     static_cast<LPCTSTR>(MeaUtils::DblToStr(pointEntry.second.x)),
                     static_cast<LPCTSTR>(MeaUtils::DblToStr(pointEntry.second.y)));
    }
    indent--;
    m_mgr->Write(indent, _T("</points>\n"));

    m_mgr->Write(indent, _T("<properties>\n"));
    indent++;
    if (m_fieldMask & MeaWidthField) {
        m_mgr->Write(indent, _T("<width value=\"%s\"/>\n"),
                     static_cast<LPCTSTR>(MeaUtils::DblToStr(m_width)));
    }
    if (m_fieldMask & MeaHeightField) {
        m_mgr->Write(indent, _T("<height value=\"%s\"/>\n"),
                     static_cast<LPCTSTR>(MeaUtils::DblToStr(m_height)));
    }
    if (m_fieldMask & MeaDistanceField) {
        m_mgr->Write(indent, _T("<distance value=\"%s\"/>\n"),
                     static_cast<LPCTSTR>(MeaUtils::DblToStr(m_distance)));
    }
    if (m_fieldMask & MeaAreaField) {
        m_mgr->Write(indent, _T("<area value=\"%s\"/>\n"),
                     static_cast<LPCTSTR>(MeaUtils::DblToStr(m_area)));
    }
    if (m_fieldMask & MeaAngleField) {
        m_mgr->Write(indent, _T("<angle value=\"%s\"/>\n"),
                     static_cast<LPCTSTR>(MeaUtils::DblToStr(m_angle)));
    }
    indent--;
    m_mgr->Write(indent, _T("</properties>\n"));

    indent--;
    m_mgr->Write(indent, _T("</position>\n"));
}


//*************************************************************************
// Positions
//*************************************************************************


MeaPositionLogMgr::Positions::Positions() {}

MeaPositionLogMgr::Positions::~Positions() {
    try {
        DeleteAll();
    } catch (...) {
        MeaAssert(false);
    }
}

void MeaPositionLogMgr::Positions::Add(Position* position) {
    int posIndex = Size();
    m_posMap[posIndex] = position;
}

void MeaPositionLogMgr::Positions::Set(int posIndex, Position* position) {
    PositionMap::iterator iter = m_posMap.find(posIndex);
    if (iter == m_posMap.end()) {
        throw new std::out_of_range("Positions::Set posIndex out of range");
    }

    delete (*iter).second;
    (*iter).second = position;
}

MeaPositionLogMgr::Position& MeaPositionLogMgr::Positions::Get(int posIndex) {
    PositionMap::const_iterator iter = m_posMap.find(posIndex);
    if (iter == m_posMap.end()) {
        throw new std::out_of_range("Positions::Get posIndex out of range");
    }

    return *(*iter).second;
}

void MeaPositionLogMgr::Positions::Delete(int posIndex) {
    // Find the position entry corresponding to the specified
    // position index.
    //
    PositionMap::iterator iter = m_posMap.find(posIndex);
    if (iter == m_posMap.end()) {
        throw new std::out_of_range("Positions::Delete posIndex out of range");
    }

    // Delete the position object associated with the index.
    //
    delete (*iter).second;

    // Move each position object "down" one index.
    //
    for (iter++; iter != m_posMap.end(); ++iter, posIndex++) {
        m_posMap[posIndex] = (*iter).second;
    }

    // Delete the last entry in the map since it has been
    // moved "down" one index.
    //
    m_posMap.erase(static_cast<int>(m_posMap.size()) - 1);
}

void MeaPositionLogMgr::Positions::DeleteAll() {
    for (const auto& posEntry : m_posMap) {
        delete posEntry.second;
    }
    m_posMap.clear();
}

void MeaPositionLogMgr::Positions::Save(int indent) const {
    for (const auto& posEntry : m_posMap) {
        posEntry.second->Save(indent);
    }
}

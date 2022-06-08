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

 /// @file
 /// @brief Header file for position recording, saving and loading manager.

#pragma once

#include "Position.h"
#include "PositionCollection.h"
#include "PositionScreen.h"
#include "PositionDesktop.h"
#include "PositionLogWriter.h"
#include <meazure/units/Units.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/utilities/Geometry.h>
#include <meazure/utilities/GUID.h>
#include <meazure/utilities/Singleton.h>
#include <meazure/utilities/NumericUtils.h>
#include <meazure/xml/XMLParser.h>
#include <meazure/ui/ScreenMgr.h>
#include <meazure/ui/ScreenProvider.h>
#include <list>
#include <map>
#include <stdexcept>
#include <fstream>


class MeaPositionSaveDlg;
class MeaPositionLogDlg;
class MeaPositionLogObserver;


/// Manages the recording, saving and loading of tool positions. The positions are saved to an XML format file.
///
class MeaPositionLogMgr :
    public MeaXMLParserHandler,
    public MeaPositionDesktopRefCounter,
    public MeaPositionLogWriter,
    public MeaSingleton_T<MeaPositionLogMgr> {

public:
    MeaPositionLogMgr(token);
    ~MeaPositionLogMgr();

    MeaPositionLogMgr(const MeaPositionLogMgr&) = delete;
    MeaPositionLogMgr& operator=(const MeaPositionLogMgr&) = delete;

    /// Records the position of the current radio tool.
    ///
    void RecordPosition();

    /// Replaces the specified position list entry with the
    /// current radio tool position.
    ///
    /// @param posIndex     [in] Zero based index of the position to replace.
    ///
    void ReplacePosition(int posIndex);

    /// Deletes the position recorded at the specified index.
    ///
    /// @param posIndex     [in] Zero based index of the position to delete.
    ///
    void DeletePosition(int posIndex);

    /// Deletes all positions.
    ///
    void DeletePositions();

    /// Indicates if there are any recorded positions.
    ///
    /// @return <b>true</b> if there are recorded positions.
    ///
    bool HavePositions() const { return !m_positions.Empty(); }

    /// Returns the number of positions that have been recorded or loaded.
    ///
    /// @return Number of recorded or loaded positions.
    ///
    unsigned int NumPositions() const { return m_positions.Size(); }

    /// Indicates if new positions have been recorded or the information
    /// associated with a position has changed since the last time the
    /// positions were saved in the log file.
    ///
    bool IsModified() const { return m_modified; }

    /// Returns the position object at the specified index.
    ///
    /// @param posIndex     [in] Zero based index of the position to return.
    ///
    /// @return Position at the specified index.
    ///
    MeaPosition& GetPosition(int posIndex) { return m_positions.Get(posIndex); }

    /// Works with the tool manager to set the radio tool and
    /// its position based on the specified position in the list.
    ///
    /// @param posIndex     [in] Zero based index of the position to display.
    ///
    void ShowPosition(unsigned int posIndex);

    /// Loads the specified position log file.
    ///
    /// @param pathname     [in] Pathname of file to load or nullptr if a file dialog should be shown.
    ///
    /// @return <b>true</b> if loaded, false if canceled or unable to load.
    ///
    bool Load(LPCTSTR pathname = nullptr);

    /// Saves the recorded positions to a log file.
    ///
    /// @param askPathname  [in] <b>true</b> means ask user to supply a pathname even if there is already a pathname.
    ///
    /// @return <b>true</b> if saved, false if canceled or unable to save.
    ///
    bool Save(bool askPathname);

    /// If there are positions that have not been saved, ask the
    /// user if they should be saved. Called before the app exits or
    /// a load will destroy the unsaved positions.
    ///
    /// @return <b>true</b> if the pending operation should proceed
    ///         (e.g. exit or load), <b>false</b> if pending operation
    ///         should be canceled.
    ///
    bool SaveIfModified();

    /// When a change occurs in this manager, the registered observer
    /// object is informed.
    ///
    /// @param observer     [in] Object to register as an observer of the
    ///                     position log manager.
    ///
    void SetObserver(MeaPositionLogObserver* observer) { m_observer = observer; }

    /// Displays the position management dialog.
    ///
    void ManagePositions();

    /// Persists the state of the log manager itself to the
    /// specified profile object.
    ///
    /// @param profile      [in] Profile object into which the position
    ///                     log manager should persist itself.
    ///
    void SaveProfile(MeaProfile& profile) const;

    /// Restores the state of the log manager from the specified
    /// profile object.
    ///
    /// @param profile      [in] Profile object from which the position
    ///                     log manager will restore itself.
    ///
    void LoadProfile(MeaProfile& profile);

    /// Resets the position manager to its default state.
    ///
    void MasterReset();

    /// Called to resolve an external entity (e.g. DTD).
    ///
    /// @param pathname [in] Pathname of the external entity.
    /// @return Input source for the external entity.
    ///
    virtual xercesc::InputSource* ResolveEntity(const CString& pathname) override;

    /// Returns the pathname of the currently parsed log file.
    ///
    /// @return Pathname of the currently parsed position log file.
    ///
    virtual CString GetFilePathname() override;

    /// Tests whether the specified filename represents a position
    /// log file.
    ///
    /// @param filename     [in] File to test
    ///
    /// @return <b>true</b> if the specified file is a position log file.
    ///
    static bool IsPositionFile(LPCTSTR filename);

private:
    typedef std::map<MeaGUID, MeaPositionDesktop, MeaGUID::less> DesktopInfoMap;   ///< Maps GUID to a desktop information object.
    typedef std::map<MeaGUID, int, MeaGUID::less> RefCountMap;              ///< Maps a GUID to a reference count.


    static constexpr int kChunkSize { 1024 };       ///< Log file parsing buffer allocation increment.
    static constexpr LPCTSTR kExt { _T("mpl") };    ///< Log file suffix.
    static constexpr LPCTSTR kFilter { _T("Meazure Position Log Files (*.mpl)|*.mpl|All Files (*.*)|*.*||") };  ///< File dialog filter string.


    /// Constructs a file save dialog tailored to saving position log files.
    ///
    /// @return Position log file save dialog.
    ///
    MeaPositionSaveDlg* CreateSaveDialog();

    /// Constructs a file open dialog tailored to loading position log files.
    ///
    /// @return Position log file option dialog.
    ///
    CFileDialog* CreateLoadDialog();

    /// Called when the position management dialog is destroyed.
    ///
    void ManageDlgDestroyed() { m_manageDialog = nullptr; }

    /// Writes the general information section of the position log file.
    /// @param indent       [in] Output indentation level.
    /// @throws CFileException if there was a problem writing the information
    /// 
    void WriteInfoSection(int indent);

    /// Writes the desktop information section of the position log file.
    /// @param indent       [in] Output indentation level.
    /// @throws CFileException if there was a problem writing the information
    /// 
    void WriteDesktopsSection(int indent);

    /// Writes the positions section of the position log file.
    /// 
    /// @param writer       [in] Provides ability to write a position to the log.
    /// @param indent       [in] Output indentation level.
    /// @throws CFileException if there was a problem writing the information
    /// 
    void WritePositionsSection(MeaPositionLogWriter& writer, int indent);

    /// Printf-like method for writing to a position log file.
    ///
    /// @param indentLevel      [in] Output indentation level.
    /// @param format           [in] Printf-style format string.
    /// @param ...              [in] Items to write to the file.
    /// @throws CFileException if there was a problem writing the information
    ///
    void Write(int indentLevel, LPCTSTR format, ...) override;

    /// Handles the top level elements of the log file DOM and
    /// supervises the processing of the lower level elements.
    ///
    /// @param dom          [in] Root node of the DOM.
    ///
    void ProcessDOM(const MeaXMLNode* dom);

    /// Handles the info element of the log file.
    ///
    /// @param infoNode     [in] info node in the DOM.
    ///
    void ProcessInfoNode(const MeaXMLNode* infoNode);

    /// Handles the desktop elements of the log file.
    ///
    /// @param desktopNode  [in] desktop node in the DOM.
    ///
    void ProcessDesktopNode(const MeaXMLNode* desktopNode);

    /// Handles the position elements of the log file.
    ///
    /// @param positionNode [in] position node in the DOM
    ///
    void ProcessPositionNode(const MeaXMLNode* positionNode);

    /// Records the current desktop information if the information has not already been recorded.
    ///
    /// @return Reference to the desktop information object that was either created of reused.
    ///
    MeaPositionDesktopRef RecordDesktopInfo();

    /// Returns the desktop information object corresponding to
    /// the specified ID.
    ///
    /// @param id       [in] Desktop information object ID.
    ///
    /// @return Desktop information object corresponding to the ID.
    ///
    MeaPositionDesktop& GetDesktopInfo(const MeaGUID& id);

    /// Increments the reference count for the specified desktop
    /// information object.
    ///
    /// @param id       [in] ID of the desktop info object whose reference
    ///                 count is to be incremented.
    ///
    void AddDesktopRef(const MeaGUID& id) override;

    /// Decrements the reference count for the specified desktop
    /// information object.
    ///
    /// @param id       [in] ID of the desktop info object whose reference
    ///                 count is to be decremented.
    ///
    void ReleaseDesktopRef(const MeaGUID& id) override;

    /// Performs the actual work or deleting all positions.
    ///
    void ClearPositions();

    /// Closes the write stream if it is open and prevents any exceptions from leaking out.
    ///
    void Close();


    MeaPositionLogObserver* m_observer; ///< Position log manager observer.
    DesktopInfoMap m_desktopInfoMap;    ///< Desktop information objects
    RefCountMap m_refCountMap;          ///< Desktop information object reference count.
    MeaPositionCollection m_positions;              ///< Recorded positions.
    MeaPositionSaveDlg* m_saveDialog;   ///< Position log file save dialog.
    CFileDialog* m_loadDialog;          ///< Position log file open dialog.
    CString m_saveDlgTitle;             ///< Title for the file save dialog.
    CString m_loadDlgTitle;             ///< Title for the file open dialog.
    CString m_initialDir;               ///< Initial directory for the file save and open dialogs.
    CString m_pathname;                 ///< Pathname of current position log file.
    std::ofstream m_writeStream;        ///< Stream for writing the position log file.
    CString m_title;                    ///< Title for the positions.
    CString m_desc;                     ///< Description of the positions.
    bool m_modified;                    ///< Have the positions been modified since last save.
    MeaPositionLogDlg* m_manageDialog;  ///< Position management dialog.

    friend class MeaPositionLogDlg;     ///< Position save dialog.
};

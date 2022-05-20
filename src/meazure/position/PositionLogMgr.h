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

#include <list>
#include <map>
#include <stdexcept>
#include <meazure/units/Units.h>
#include <meazure/utilities/Utils.h>
#include <meazure/utilities/XMLParser.h>
#include <meazure/utilities/GUID.h>
#include <meazure/utilities/Singleton.h>
#include <meazure/ui/ScreenMgr.h>


class MeaPositionSaveDlg;
class MeaPositionLogDlg;
class MeaPositionLogObserver;


/// Exception thrown if a problem occurs while reading or writing
/// the position log file.
///
class MeaLogFileException {

public:
    /// Constructor for the exception.
    ///
    MeaLogFileException() {}

    /// Destroys the exception.
    ///
    virtual ~MeaLogFileException() {}
};


/// Manages the recording, saving and loading of tool positions. The
/// positions are saved to an XML format file.
///
class MeaPositionLogMgr : public MeaXMLParserHandler, public MeaSingleton_T<MeaPositionLogMgr> {

public:
    /// Represents a single monitor attached to the system.
    /// There is an instance of this class per monitor.
    ///
    class Screen {

    public:
        /// Constructs a screen object.
        ///
        Screen() : m_primary(false), m_manualRes(false) {
            m_rect.left = 0.0;
            m_rect.right = 0.0;
            m_rect.top = 0.0;
            m_rect.bottom = 0.0;

            m_res.cx = 0.0;
            m_res.cy = 0.0;
        }

        /// Constructs deep copy screen object from the specified
        /// iterator over the list of screens.
        ///
        /// @param screenIter   [in] Iterator from which to construct
        ///                     a copy of the screen.
        ///
        explicit Screen(const MeaScreenMgr::ScreenIter& screenIter);

        /// Constructs a deep copy of the specified screen.
        ///
        /// @param screen       [in] Screen object to be copied.
        ///
        Screen(const Screen& screen) { Copy(screen); }

        /// Destroys a screen object.
        ///
        ~Screen() {}

        /// Loads screen elements of the log file.
        ///
        /// @param screenNode   [in] screen node in the DOM.
        ///
        void Load(const MeaXMLNode* screenNode);

        /// Saves the screen information
        ///
        /// @param mgr      [in] Parent manager.
        /// @param indent   [in] Output indentation level.
        /// @throw CFileException if there was a problem saving the file
        ///
        void Save(MeaPositionLogMgr& mgr, int indent) const;

        /// Assignment operator for a screen object. Makes
        /// a deep copy of the object.
        ///
        /// @param screen       [in] Screen option to assign to this.
        ///
        /// @return This screen object.
        ///
        Screen& operator=(const Screen& screen) {
            return Copy(screen);
        }

        /// Tests for equality between the specified screen object and this.
        ///
        /// @param screen       [in] Screen object to test against this.
        ///
        /// @return <b>true</b> if the specified screen object and this are equal.
        ///
        bool operator==(const Screen& screen) const { return IsEqual(screen); }

        /// Tests for inequality between the specified screen object and this.
        ///
        /// @param screen       [in] Screen object to test against this.
        ///
        /// @return <b>true</b> if the specified screen object and this are not equal.
        ///
        bool operator!=(const Screen& screen) const { return !IsEqual(screen); }

    private:
        /// Makes a deep copy of the specified screen object.
        ///
        /// @param screen       [in] Screen object to copy.
        ///
        /// @return Copy of the specified screen object.
        ///
        Screen& Copy(const Screen& screen) {
            if (&screen != this) {
                m_primary = screen.m_primary;
                m_rect = screen.m_rect;
                m_res = screen.m_res;
                m_manualRes = screen.m_manualRes;
                m_desc = screen.m_desc;
            }
            return *this;
        }

        /// Tests whether the specified screen object is equal to this.
        ///
        /// @param screen       [in] Screen object to test against this.
        ///
        /// @return <b>true</b> if the specified screen object is equal to this.
        ///
        bool IsEqual(const Screen& screen) const {
            return ((m_primary == screen.m_primary) &&
                    MeaUtils::IsFloatingEqual(m_rect.top, screen.m_rect.top) &&
                    MeaUtils::IsFloatingEqual(m_rect.bottom, screen.m_rect.bottom) &&
                    MeaUtils::IsFloatingEqual(m_rect.left, screen.m_rect.left) &&
                    MeaUtils::IsFloatingEqual(m_rect.right, screen.m_rect.right) &&
                    MeaUtils::IsFloatingEqual(m_res.cx, screen.m_res.cx) &&
                    MeaUtils::IsFloatingEqual(m_res.cy, screen.m_res.cy) &&
                    (m_manualRes == screen.m_manualRes) &&
                    (m_desc == screen.m_desc));
        }


        bool m_primary;     ///< Is this the primary screen
        FRECT m_rect;       ///< Screen rectangle expressed in the units that were
                            ///< in effect at the time the screen object was created.
        FSIZE m_res;        ///< Screen resolution expressed in the units that were
                            ///< in effect at the time the screen object was created.
        bool m_manualRes;   ///< Is the resolution calibrated manually.
        CString m_desc;     ///< Description for the screen
    };


    /// Represents all information of interest about the system at the
    /// time a position is recorded. This includes the list of all display
    /// screens attached to the system. The desktop information is saved
    /// in the position log file. To minimize repetition, positions reference
    /// the desktop information in effect when they were recorded. This
    /// means that one desktop information section can serve many positions.
    ///
    class DesktopInfo {

    public:
        /// Constructs a desktop information object.
        ///
        DesktopInfo();

        /// Constructs a desktop information object with the specified unique ID.
        ///
        /// @param guidStr      [in] GUID ID for the desktop information object.
        ///
        explicit DesktopInfo(LPCTSTR guidStr);

        /// Constructs a desktop information object as a copy of the specified object.
        ///
        /// @param desktop      [in] Desktop information object to copy.
        ///
        DesktopInfo(const DesktopInfo& desktop) { Copy(desktop); }

        /// Destroys a desktop information object.
        ///
        ~DesktopInfo() {}

        /// Assigns the specified desktop information object to this object.
        ///
        /// @param desktop      [in] Desktop information object to be assigned.
        ///
        /// @return This object.
        ///
        DesktopInfo& operator=(const DesktopInfo& desktop) {
            return Copy(desktop);
        }

        /// Returns the linear units in effect when this object was created.
        /// @return Linear units object in effect when this object was created.
        MeaLinearUnits* GetLinearUnits() { return m_linearUnits; }

        /// Returns the linear units in effect when this object was created.
        /// @return Linear units object in effect when this object was created.
        const MeaLinearUnits* GetLinearUnits() const { return m_linearUnits; }

        /// Returns the angular units in effect when this object was created.
        /// @return Angular units object in effect when this object was created.
        MeaAngularUnits* GetAngularUnits() { return m_angularUnits; }

        /// Returns the angular units in effect when this object was created.
        /// @return Angular units object in effect when this object was created.
        const MeaAngularUnits* GetAngularUnits() const { return m_angularUnits; }

        /// Indicates whether the y-axis was inverted when this object was created.
        /// @return <b>true</b> if the y-axis was inverted when this object was created.
        bool GetInvertY() const { return m_invertY; }

        /// Returns the location of the origin when this object was created.
        /// @return The location of the origin when this object was created.
        const FPOINT& GetOrigin() const { return m_origin; }

        /// Returns the unique ID for this object.
        /// @return GUID ID for this object.
        const MeaGUID& GetId() const { return m_id; }

        /// Sets a unique ID for this object.
        /// @param guidStr      [in] GUID ID to set for this object.
        void SetId(LPCTSTR guidStr) { m_id = guidStr; }

        /// Returns the name for custom units.
        /// @return Name for custom units.
        CString GetCustomName() const { return m_customName; }

        /// Returns the abbreviation for custom units.
        /// @return Abbreviation for custom units.
        CString GetCustomAbbrev() const { return m_customAbbrev; }

        /// Returns the units basis for custom units (i.e. pixels, inches or centimeters).
        /// @return The units basis for custom units.
        CString GetCustomBasisStr() const { return m_customBasisStr; }

        /// Returns the conversion factor for custom units.
        /// @return Conversion factor.
        double GetCustomFactor() const { return m_customFactor; }

        /// Returns the set of display precisions for custom units.
        /// @return Set of custom units display precisions.
        MeaUnits::DisplayPrecisions GetCustomPrecisions() const { return m_customPrecisions; }

        /// Loads desktop elements of the log file.
        ///
        /// @param desktopNode      [in] Desktop node in the DOM.
        ///
        void Load(const MeaXMLNode* desktopNode);

        /// Saves the desktop information
        ///
        /// @param mgr          [in] Parent manager.
        /// @param indent       [in] Output indentation level.
        /// @throw CFileException is there was a problem saving the information
        ///
        void Save(MeaPositionLogMgr& mgr, int indent) const;

        /// Compares the specified desktop information object with this to determine equality.
        ///
        /// @param di       [in] Desktop information object to compare with this.
        ///
        /// @return <b>true</b> if the specified object and this are equal.
        ///
        bool operator==(const DesktopInfo& di) const { return IsEqual(di); }

        /// Compares the specified desktop information object with this to determine inequality.
        ///
        /// @param di       [in] Desktop information object to compare with this.
        ///
        /// @return <b>true</b> if the specified object and this are not equal.
        ///
        bool operator!=(const DesktopInfo& di) const { return !IsEqual(di); }

    private:
        typedef std::list<Screen> ScreenList;       ///< List of all display screens attached to the system.


        /// Copies the specified desktop information object into this object.
        ///
        /// @param desktop      [in] Desktop information object to copy.
        ///
        /// @return This object.
        ///
        DesktopInfo& Copy(const DesktopInfo& desktop) {
            if (&desktop != this) {
                m_id = desktop.m_id;
                m_origin = desktop.m_origin;
                m_invertY = desktop.m_invertY;
                m_size = desktop.m_size;
                m_linearUnits = desktop.m_linearUnits;
                m_angularUnits = desktop.m_angularUnits;
                m_screens = desktop.m_screens;

                m_customName = desktop.m_customName;
                m_customAbbrev = desktop.m_customAbbrev;
                m_customBasisStr = desktop.m_customBasisStr;
                m_customFactor = desktop.m_customFactor;
                m_customPrecisions = desktop.m_customPrecisions;
            }
            return *this;
        }

        /// Centralized initialization method for the class. All
        /// non-copy constructors call this method.
        ///
        void Init();

        /// Determines whether the specified desktop information object
        /// is equal to this object.
        ///
        /// @param desktop      [in] Desktop information object to compare with this.
        ///
        /// @return <b>true</b> if the specified object and this object are equal.
        ///         Note that the object's unique IDs are not compared.
        ///
        bool IsEqual(const DesktopInfo& desktop) const {
            return (MeaUtils::IsFloatingEqual(m_origin.x, desktop.m_origin.x) &&
                    MeaUtils::IsFloatingEqual(m_origin.y, desktop.m_origin.y) &&
                    (m_invertY == desktop.m_invertY) &&
                    MeaUtils::IsFloatingEqual(m_size.cx, desktop.m_size.cx) &&
                    MeaUtils::IsFloatingEqual(m_size.cy, desktop.m_size.cy) &&
                    (m_linearUnits == desktop.m_linearUnits) &&
                    (m_angularUnits == desktop.m_angularUnits) &&
                    (m_screens == desktop.m_screens) &&
                    (m_customName == desktop.m_customName) &&
                    (m_customAbbrev == desktop.m_customAbbrev) &&
                    (m_customBasisStr == desktop.m_customBasisStr) &&
                    MeaUtils::IsFloatingEqual(m_customFactor, desktop.m_customFactor) &&
                    (m_customPrecisions == desktop.m_customPrecisions));
        }

        /// Sets the linear units for the object based on the specified units name.
        ///
        /// @param unitsStr     [in] Linear units name.
        ///
        void SetLinearUnits(const CString& unitsStr);

        /// Sets the angular units for the object based on the specified units name.
        ///
        /// @param unitsStr     [in] Angular units name.
        ///
        void SetAngularUnits(const CString& unitsStr);

        /// Loads the display precisions values for the custom units.
        ///
        /// @param displayPrecisionNode     [in] displayPrecisions element in the DOM.
        ///
        void LoadCustomPrecisions(const MeaXMLNode* displayPrecisionNode);

        /// Saves the display precisions for custom units
        ///
        /// @param mgr      [in] Parent manager.
        /// @param indent   [in] Output indentation level.
        /// @throws CFileException if there was a problem saving the information
        ///
        void SaveCustomPrecisions(MeaPositionLogMgr& mgr, int indent) const;


        MeaGUID m_id;                       ///< ID for use by a Position object to reference this object.
        FPOINT m_origin;                    ///< Origin in the units that were in effect when the desktop object was created.
        bool m_invertY;                     ///< Is origin inverted.
        FSIZE m_size;                       ///< Desktop size in units in effect when desktop object created.
        MeaLinearUnits* m_linearUnits;      ///< Current linear units.
        MeaAngularUnits* m_angularUnits;    ///< Current angle units.
        ScreenList m_screens;               ///< List of display screens.
        CString m_customName;               ///< Custom units name.
        CString m_customAbbrev;             ///< Custom units abbreviation.
        CString m_customBasisStr;           ///< Custom scale factor basis.
        double m_customFactor;              ///< Custom scale factor.
        MeaUnits::DisplayPrecisions m_customPrecisions; ///< Custom display precisions.
    };


    /// Represents a tool position. A position object references the desktop information
    /// that provides its environment. Since desktop environments do not tend to change
    /// often, many position objects typically reference the same desktop information object.
    /// The position object is ultimately persisted to the position log file along with
    /// the desktop information object.
    ///
    /// A position consists of one or more (x,y) points. The number of points required
    /// for a position depends on the measurement tool whose position is being recorded.
    /// For example, the Point tool requires only one point to completely describe its
    /// position, whereas the Line tool requires two points, one per endpoint of the line.
    /// Each point is named by the tool (e.g. "v", "1", "2").
    ///
    class Position {

    public:
        /// Constructs a position object. This constructor is intended for use solely
        /// by STL collections and is not used to construct new position objects in
        /// general use.
        ///
        Position();

        /// Constructs a position object that references the specified desktop information
        /// object.
        ///
        /// @param mgr              [in] Parent manager.
        /// @param desktopInfoId    [in] GUID representing the desktop information object to
        ///                         be referenced by this position.
        ///
        Position(MeaPositionLogMgr* mgr, const MeaGUID& desktopInfoId);

        /// Constructs a position object that represents the position of the specified tool at
        /// the specified time.
        ///
        /// @param mgr              [in] Parent manager.
        /// @param desktopInfoIdStr [in] GUID expressed as a string representing the desktop
        ///                         information object referenced by this position.
        /// @param toolName         [in] Name of the measurement tool whose position is represented
        ///                         by this position object.
        /// @param timestamp        [in] Identifies when this position was recorded.
        ///
        Position(MeaPositionLogMgr* mgr, const CString& desktopInfoIdStr, const CString& toolName, const CString& timestamp);

        /// Copy constructor.
        ///
        /// @param position     [in] Position object to be copied into a new position object.
        ///
        Position(const Position& position);

        /// Destroys a position object.
        ///
        ~Position();

        /// Performs assignment of the specified position object to this position object.
        ///
        /// @param position     [in] Object to be copied to this.
        ///
        /// @return This object.
        ///
        Position& operator=(const Position& position) {
            return Copy(position);
        }

        /// Places a description on the position.
        ///
        /// @param desc     [in] Descriptive text for the position.
        ///
        void SetDesc(const CString& desc) { m_desc = desc; }

        /// Returns the description of the position.
        ///
        /// @return Description of the position or the empty string if
        ///         there is no description.
        ///
        CString GetDesc() const { return m_desc; }

        /// Returns the timestamp indicating when this position was recorded.
        ///
        /// @return Timestamp for the position in ISO 8601 format.
        ///
        CString GetTimeStamp() const { return m_timestamp; }

        /// Adds the specified point to the position using the specified name
        /// to identify the point.
        ///
        /// @param name     [in] Name to assign the point.
        /// @param pt       [in] Point to be stored in the position.
        ///
        void AddPoint(LPCTSTR name, const FPOINT& pt) { m_points[name] = pt; }

        /// Records the specified point as an x1, y1 point.
        /// @param point        [in] Point to record, in the current units.
        void RecordXY1(const FPOINT& point);

        /// Records the specified point as an x2, y2 point.
        /// @param point        [in] Point to record, in the current units.
        void RecordXY2(const FPOINT& point);

        /// Records the specified point as an xv, yv point.
        /// @param point        [in] Point to record, in the current units.
        void RecordXYV(const FPOINT& point);

        /// Records the specified width and height.
        /// @param size         [in] Width and height to record, in the current units.
        void RecordWH(const FSIZE& size);

        /// Records a distance. The distance is calculated
        /// by the method using the formula:
        /// \f[
        ///     distance=\sqrt{size_x^2+size_y^2}
        /// \f]
        /// @param size         [in] Width and height used to calculate the distance, in the current units.
        void RecordDistance(const FSIZE& size);

        /// Records the specified distance.
        /// @param dist         [in] Distance to record, in the current units.
        void RecordDistance(double dist);

        /// Records the angle.
        /// @param angle        [in] Angle to record, in the current angular units.
        void RecordAngle(double angle);

        /// Records a rectangular area. The area is calculated
        /// by the formula:
        /// \f[
        ///     area=size_w*size_h
        /// \f]
        /// @param size         [in] WWidth and height used to calculate the area, in the current units.
        void RecordRectArea(const FSIZE& size);

        /// Records a circular area. The area is calculated using
        /// the formula:
        /// \f[
        ///     area=\pi*radius^2
        /// \f]
        /// @param radius       [in] Radius used to calculate the area, in the current units.
        void RecordCircleArea(double radius);

        /// Tells the tool manager to display the position using the
        /// appropriate tool.
        ///
        void Show() const;

        /// Loads the position elements of the log file.
        ///
        /// @param positionNode     [in] Position node in the DOM.
        ///
        void Load(const MeaXMLNode* positionNode);

        /// Saves the position in the position log file.
        ///
        /// @param indent       [in] Output indentation level.
        /// @throw CFileException if there was a problem saving the position
        ///
        void Save(int indent) const;

    private:
        typedef std::map<CString, FPOINT> PointMap;     ///< Maps a point name to the coordinates of the point.


        /// Copies the specified position to this object.
        ///
        /// @param position     [in] Position to be copied.
        ///
        /// @return This object.
        ///
        Position& Copy(const Position& position);


        UINT m_fieldMask;           ///< Data fields defined for this position. Different tools provide different amounts of data.
        PointMap m_points;          ///< Location of the current tool, , in the units in effect when the position was recorded.
        double m_width;             ///< Width of rectangle or bounding box, in the units in effect when the position was recorded.
        double m_height;            ///< Height of rectangle or bounding box, in the units in effect when the position was recorded.
        double m_distance;          ///< Length of line or diagonal, in the units in effect when the position was recorded.
        double m_area;              ///< Area of rectangle or bounding box, in the units in effect when the position was recorded.
        double m_angle;             ///< Angle of diagonal, line, or angle tool, in the units in effect when the position was recorded.
        MeaPositionLogMgr* m_mgr;   ///< Parent manager.
        MeaGUID m_desktopInfoId;    ///< Id of desktop information object referenced by this position.
        CString m_toolName;         ///< Name of measurement tool whose position is represented by this object.
        CString m_timestamp;        ///< Date and time the position was recorded.
        CString m_desc;             ///< Description for position.
    };

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
    Position& GetPosition(int posIndex) { return m_positions.Get(posIndex); }

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
    void    SaveProfile(MeaProfile& profile) const;

    /// Restores the state of the log manager from the specified
    /// profile object.
    ///
    /// @param profile      [in] Profile object from which the position
    ///                     log manager will restore itself.
    ///
    void    LoadProfile(MeaProfile& profile);

    /// Resets the position manager to its default state.
    ///
    void    MasterReset();

    /// Called during the XML parsing of the log file, to parse an
    /// external entity such as a DTD.
    ///
    /// @param parser       [in] XML parser.
    /// @param pathname     [in] Pathname of the external entity.
    ///
    virtual void    ParseEntity(MeaXMLParser& parser, const CString& pathname) override;

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
    /// Represents a collection of positions. A position log consists
    /// of a collection of positions. In turn, a position consists of
    /// on or more points depending on the measurement tool.
    ///
    class Positions {

    public:
        /// Constructs a position collection object.
        ///
        Positions();

        /// Destroys a position collection object.
        ///
        ~Positions();

        /// Indicates if there are any positions stored in the object.
        ///
        /// @return <b>true</b> if there are positions.
        ///
        bool Empty() const { return m_posMap.empty(); }

        /// Returns the number of positions stored in the object.
        ///
        /// @return Number of positions.
        ///
        unsigned int Size() const { return static_cast<unsigned int>(m_posMap.size()); }

        /// Adds the specified position to the collection of positions.
        ///
        /// @param position     [in] Position to add to the collection.
        ///
        void Add(Position* position);

        /// Places the specified position at the specified location in
        /// the collection.
        ///
        /// @param posIndex     [in] Zero based index indicating where in
        ///                     the collection to place the position.
        /// @param position     [in] Position object to insert in the collection.
        /// @throws std::out_of_range if the specified position is out of bounds
        ///
        void Set(int posIndex, Position* position);

        /// Returns the position object at the specified location in the collection.
        ///
        /// @param posIndex     [in] Zero based index into the collection.
        ///
        /// @return Position object located at the specified location in the
        ///         collection.
        /// @throws std::out_of_range if the specified position is out of bounds
        ///
        Position& Get(int posIndex);

        /// Removes the position object from the specified location in the
        /// collection and destroys the object.
        ///
        /// @param posIndex     [in] Zero based index indicating where in
        ///                     the collection to delete a position.
        /// @throws std::out_of_range if the specified position is out of bounds
        ///
        void Delete(int posIndex);

        /// Removes all positions from the collection and destroys the
        /// position objects.
        ///
        void DeleteAll();

        /// Saves all positions in the collection to the log file.
        ///
        /// @param indent       [in] Output indentation level.
        /// @throws CFileException if there was a problem saving the position
        ///
        void Save(int indent) const;

    private:
        typedef std::map<int, Position*> PositionMap;       ///< Maps indices to position objects.

        PositionMap m_posMap;       ///< Collection of positions.
    };

private:
    typedef std::map<MeaGUID, DesktopInfo, MeaGUID::less> DesktopInfoMap;   ///< Maps GUID to a desktop information object.
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
    void WriteInfoSection(int indent);

    /// Writes the desktop information section of the position log file.
    /// @param indent       [in] Output indentation level.
    /// @throws CFileException if there was a problem writing the information
    void WriteDesktopsSection(int indent);

    /// Writes the positions section of the position log file.
    /// @param indent       [in] Output indentation level.
    /// @throws CFileException if there was a problem writing the information
    void WritePositionsSection(int indent);

    /// Opens the specified position log file either for reading or writing.
    ///
    /// @param pathname     [in] Pathname of position log file to open.
    /// @param mode         [in] one of CFile::modeRead or CFile::modeWrite | CFile::modeCreate
    ///
    /// @return <b>true</b> if file opened successfully.
    ///
    bool Open(const CString& pathname, UINT mode);

    /// Closes the currently open position log file.
    ///
    void Close() { m_stdioFile.Close(); m_stdioOpen = false; }

    /// Printf-like method for writing to a position log file.
    ///
    /// @param indentLevel      [in] Output indentation level.
    /// @param format           [in] Printf-style format string.
    /// @param ...              [in] Items to write to the file.
    /// @throws CFileException if there was a problem writing the information
    ///
    void Write(int indentLevel, LPCTSTR format, ...);

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

    /// Concatenates all child data nodes into a single string.
    ///
    /// @param elementNode  [in] Parent node of data nodes
    ///
    /// @return String concatenation of all child data nodes.
    ///
    static CString ProcessDataNodes(const MeaXMLNode* elementNode);

    /// Records the current desktop information if the information
    /// has not already been recorded.
    ///
    /// @return ID of the desktop information object that was either
    ///         created of reused.
    ///
    MeaGUID RecordDesktopInfo();

    /// Returns the desktop information object corresponding to
    /// the specified ID.
    ///
    /// @param id       [in] Desktop information object ID.
    ///
    /// @return Desktop information object corresponding to the ID.
    ///
    DesktopInfo& GetDesktopInfo(const MeaGUID& id);

    /// Increments the reference count for the specified desktop
    /// information object.
    ///
    /// @param id       [in] ID of the desktop info object whose reference
    ///                 count is to be incremented.
    ///
    void AddDesktopRef(const MeaGUID& id);

    /// Decrements the reference count for the specified desktop
    /// information object.
    ///
    /// @param id       [in] ID of the desktop info object whose reference
    ///                 count is to be decremented.
    ///
    void ReleaseDesktopRef(const MeaGUID& id);

    /// Performs the actual work or deleting all positions.
    ///
    void ClearPositions();


    MeaPositionLogObserver* m_observer; ///< Position log manager observer.
    DesktopInfoMap m_desktopInfoMap;    ///< Desktop information objects
    RefCountMap m_refCountMap;          ///< Desktop information object reference count.
    Positions m_positions;              ///< Recorded positions.
    MeaPositionSaveDlg* m_saveDialog;   ///< Position log file save dialog.
    CFileDialog* m_loadDialog;          ///< Position log file open dialog.
    CString m_saveDlgTitle;             ///< Title for the file save dialog.
    CString m_loadDlgTitle;             ///< Title for the file open dialog.
    CString m_initialDir;               ///< Initial directory for the file save and open dialogs.
    CString m_pathname;                 ///< Pathname of current position log file.
    CStdioFile m_stdioFile;             ///< File pointer for an open position log file.
    bool m_stdioOpen;                   ///< Is the position log file open.
    CString m_title;                    ///< Title for the positions.
    CString m_desc;                     ///< Description of the positions.
    bool m_modified;                    ///< Have the positions been modified since last save.
    MeaPositionLogDlg* m_manageDialog;  ///< Position management dialog.

    friend class Screen;                ///< Represents a display screen.
    friend class DesktopInfo;           ///< Desktop information object.
    friend class Position;              ///< A tool position.
    friend class MeaPositionLogDlg;     ///< Position save dialog.
};

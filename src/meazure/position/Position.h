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

/// @file
/// @brief Represents a tool position.

#pragma once

#include "PositionDesktop.h"
#include <meazure/utilities/Geometry.h>
#include <meazure/xml/XMLParser.h>


/// Represents a tool position. A position object references the desktop information that provides its environment.
/// Since desktop environments do not tend to change  often, many position objects typically reference the same
/// desktop information object. The position object is ultimately persisted to the position log file along with
/// the desktop information object.
///
/// A position consists of one or more (x,y) points. The number of points required for a position depends on the
/// measurement tool whose position is being recorded. For example, the Point tool requires only one point to
/// completely describe its position, whereas the Line tool requires two points, one per endpoint of the line.
/// Each point is named by the tool (e.g. "v", "1", "2").
///
class MeaPosition {

public:
    typedef std::map<CString, MeaFPoint> PointMap;     ///< Maps a point name to the coordinates of the point.


    /// Constructs a position object that references the specified desktop information object.
    ///
    /// @param desktopInfoRef   [in] Reference to the desktop information object for this position
    ///
    MeaPosition(MeaPositionDesktopRef desktopInfoRef);

    /// Constructs a position object that represents the position of the specified tool at
    /// the specified time.
    ///
    /// @param desktopInfoRef   [in] Reference to the desktop information object for this position
    /// @param toolName         [in] Name of the measurement tool whose position is represented
    ///                         by this position object.
    /// @param timestamp        [in] Identifies when this position was recorded.
    ///
    MeaPosition(MeaPositionDesktopRef desktopInfoRef, const CString& toolName, const CString& timestamp);

    /// Copy constructor.
    ///
    /// @param position     [in] Position object to be copied into a new position object.
    ///
    MeaPosition(const MeaPosition& position);

    /// Performs assignment of the specified position object to this position object.
    ///
    /// @param position     [in] Object to be copied to this.
    ///
    /// @return This object.
    ///
    MeaPosition& operator=(const MeaPosition& position);

    /// Sets the name of the tool that recorded this position.
    /// 
    /// @param toolName Name of the tool that recorded this position
    /// 
    void SetToolName(const CString& toolName) { m_toolName = toolName; }

    /// Returns the name of the tool that recorded this position.
    /// 
    /// @return Name of the tool that recorded this position.
    /// 
    CString GetToolName() const { return m_toolName; }

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

    /// Returns the identifier of the desktop information object referenced by this position.
    /// 
    /// @return Desktop information identifier referenced by this position.
    ///  
    MeaPositionDesktopRef GetDesktopRef() const { return m_desktopRef; }

    /// Returns the points representing the position.
    /// 
    /// @return Points representing the position as a map of the name of the point (e.g. "1") and its
    ///     coordinates. The point names are meaningful to the tool that recorded the position.
    ///
    const PointMap& GetPoints() const { return m_points; }

    /// Returns the recorded width.
    /// 
    /// @return Recorded width
    /// 
    double GetWidth() const { return m_width; }

    /// Returns the recorded height.
    /// 
    /// @return Recorded height
    /// 
    double GetHeight() const { return m_height; }

    /// Returns the recorded distance.
    /// 
    /// @return Recorded distance
    /// 
    double GetDistance() const { return m_distance; }

    /// Returns the recorded area.
    /// 
    /// @return Recorded area
    /// 
    double GetArea() const { return m_area; }

    /// Returns the recorded angle.
    /// 
    /// @return Recorded angle.
    /// 
    double GetAngle() const { return m_angle; }

    /// Adds the specified point to the position using the specified name
    /// to identify the point.
    ///
    /// @param name     [in] Name to assign the point.
    /// @param pt       [in] Point to be stored in the position.
    ///
    void AddPoint(PCTSTR name, const MeaFPoint& pt) { m_points[name] = pt; }

    /// Records the specified point as an x1, y1 point.
    /// 
    /// @param point        [in] Point to record, in the current units.
    /// 
    void RecordXY1(const MeaFPoint& point);

    /// Records the specified point as an x2, y2 point.
    /// 
    /// @param point        [in] Point to record, in the current units.
    /// 
    void RecordXY2(const MeaFPoint& point);

    /// Records the specified point as an xv, yv point.
    /// 
    /// @param point        [in] Point to record, in the current units.
    /// 
    void RecordXYV(const MeaFPoint& point);

    /// Records the specified width and height.
    /// 
    /// @param size         [in] Width and height to record, in the current units.
    /// 
    void RecordWH(const MeaFSize& size);

    /// Records a distance. The distance is calculated
    /// by the method using the formula:
    /// \f[
    ///     distance=\sqrt{size_x^2+size_y^2}
    /// \f]
    /// 
    /// @param size         [in] Width and height used to calculate the distance, in the current units.
    /// 
    void RecordDistance(const MeaFSize& size);

    /// Records the specified distance.
    /// 
    /// @param dist         [in] Distance to record, in the current units.
    /// 
    void RecordDistance(double dist);

    /// Records the angle.
    /// 
    /// @param angle        [in] Angle to record, in the current angular units.
    /// 
    void RecordAngle(double angle);

    /// Records a rectangular area. The area is calculated
    /// by the formula:
    /// \f[
    ///     area=size_w*size_h
    /// \f]
    /// 
    /// @param size         [in] WWidth and height used to calculate the area, in the current units.
    /// 
    void RecordRectArea(const MeaFSize& size);

    /// Records a circular area. The area is calculated using
    /// the formula:
    /// \f[
    ///     area=\pi*radius^2
    /// \f]
    /// 
    /// @param radius       [in] Radius used to calculate the area, in the current units.
    /// 
    void RecordCircleArea(double radius);

    /// Loads the position elements of the log file.
    ///
    /// @param positionNode     [in] Position node in the DOM.
    ///
    void Load(const MeaXMLNode* positionNode);

    /// Saves the position in the position log file.
    ///
    /// @param writer       [in] Provides ability to write a position to the log.
    ///
    void Save(MeaXMLWriter& writer) const;

    /// Compares the specified position information object with this to determine equality.
    ///
    /// @param position     [in] Position information object to compare with this.
    ///
    /// @return <b>true</b> if the specified object and this are equal.
    ///
    bool operator==(const MeaPosition& position) const {
        return (m_points == position.m_points) &&
            MeaNumericUtils::IsEqualF(m_width, position.m_width) &&
            MeaNumericUtils::IsEqualF(m_height, position.m_height) &&
            MeaNumericUtils::IsEqualF(m_distance, position.m_distance) &&
            MeaNumericUtils::IsEqualF(m_area, position.m_area) &&
            MeaNumericUtils::IsEqualF(m_angle, position.m_angle) &&
            (m_desktopRef == position.m_desktopRef) &&
            (m_toolName == position.m_toolName) &&
            (m_timestamp == position.m_timestamp) &&
            (m_desc == position.m_desc);
    }

    /// Compares the specified position information object with this to determine inequality.
    ///
    /// @param position    [in] Position information object to compare with this.
    ///
    /// @return <b>true</b> if the specified object and this are not equal.
    ///
    bool operator!=(const MeaPosition& position) const { return !(*this == position); }

private:
    UINT m_fieldMask;    ///< Data fields defined for this position. Different tools provide different amounts of data.
    PointMap m_points;   ///< Location of the current tool, in the units in effect when the position was recorded.
    double m_width;      ///< Width of rectangle or bounding box, in the units in effect when the position was recorded.
    double m_height;     ///< Height of rectangle or bounding box, in the units in effect when the position was recorded.
    double m_distance;   ///< Length of line or diagonal, in the units in effect when the position was recorded.
    double m_area;       ///< Area of rectangle or bounding box, in the units in effect when the position was recorded.
    double m_angle;      ///< Angle of diagonal, line, or angle tool, in the units in effect when the position was recorded.
    MeaPositionDesktopRef m_desktopRef; ///< Reference to the desktop information object for this position.
    CString m_toolName;  ///< Name of measurement tool whose position is represented by this object.
    CString m_timestamp; ///< Date and time the position was recorded.
    CString m_desc;      ///< Description for position.
};

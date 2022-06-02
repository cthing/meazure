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
#include "Position.h"
#include <meazure/utilities/NumericUtils.h>
#include <meazure/utilities/TimeStamp.h>
#include <meazure/utilities/StringUtils.h>
#include <meazure/ui/DataFieldId.h>


MeaPosition::MeaPosition(MeaPositionDesktopRef desktopRef) :
    MeaPosition(desktopRef, _T(""), MeaTimeStamp::Make(time(nullptr))) {}

MeaPosition::MeaPosition(MeaPositionDesktopRef desktopRef, const CString& toolName, const CString& timestamp) :
    m_fieldMask(0),
    m_width(0.0),
    m_height(0.0),
    m_distance(0.0),
    m_area(0.0),
    m_angle(0.0),
    m_desktopRef(desktopRef),
    m_toolName(toolName),
    m_timestamp(timestamp) {}

MeaPosition::MeaPosition(const MeaPosition& position) :
    m_fieldMask(position.m_fieldMask),
    m_width(position.m_width),
    m_height(position.m_height),
    m_distance(position.m_distance),
    m_area(position.m_area),
    m_angle(position.m_angle),
    m_desktopRef(position.m_desktopRef),
    m_toolName(position.m_toolName),
    m_timestamp(position.m_timestamp),
    m_desc(position.m_desc),
    m_points(position.m_points) {}

MeaPosition& MeaPosition::operator=(const MeaPosition& position) {
    if (&position != this) {
        m_fieldMask = position.m_fieldMask;
        m_width = position.m_width;
        m_height = position.m_height;
        m_distance = position.m_distance;
        m_area = position.m_area;
        m_angle = position.m_angle;
        m_desktopRef = position.m_desktopRef;
        m_toolName = position.m_toolName;
        m_timestamp = position.m_timestamp;
        m_desc = position.m_desc;
        m_points = position.m_points;
    }

    return *this;
}

void MeaPosition::RecordXY1(const MeaFPoint& point) {
    m_fieldMask |= MeaX1Field | MeaY1Field;

    m_points[_T("1")] = point;
}

void MeaPosition::RecordXY2(const MeaFPoint& point) {
    m_fieldMask |= MeaX2Field | MeaY2Field;

    m_points[_T("2")] = point;
}

void MeaPosition::RecordXYV(const MeaFPoint& point) {
    m_fieldMask |= MeaXVField | MeaYVField;

    m_points[_T("v")] = point;
}

void MeaPosition::RecordWH(const MeaFSize& size) {
    m_fieldMask |= MeaWidthField | MeaHeightField;

    m_width = size.cx;
    m_height = size.cy;
}

void MeaPosition::RecordDistance(const MeaFSize& size) {
    m_fieldMask |= MeaDistanceField;

    m_distance = MeaGeometry::CalcLength(size.cx, size.cy);
}

void MeaPosition::RecordDistance(double dist) {
    m_fieldMask |= MeaDistanceField;

    m_distance = dist;
}

void MeaPosition::RecordAngle(double angle) {
    m_fieldMask |= MeaAngleField;

    m_angle = angle;
}

void MeaPosition::RecordRectArea(const MeaFSize& size) {
    m_fieldMask |= MeaAreaField;

    m_area = size.cx * size.cy;
}

void MeaPosition::RecordCircleArea(double radius) {
    m_fieldMask |= MeaAreaField;

    m_area = MeaNumericUtils::PI * radius * radius;
}

void MeaPosition::Load(const MeaXMLNode* positionNode) {
    bool def;

    for (MeaXMLNode::NodeIter_c iter = positionNode->GetChildIter(); !positionNode->AtEnd(iter); ++iter) {
        MeaXMLNode* node = *iter;

        if (node->GetType() == MeaXMLNode::Type::Element) {
            if (node->GetData() == _T("desc")) {
                SetDesc(MeaStringUtils::LFtoCRLF(node->GetChildData()));
            } else if (node->GetData() == _T("points")) {
                for (MeaXMLNode::NodeIter_c pointIter = node->GetChildIter(); !node->AtEnd(pointIter); ++pointIter) {
                    MeaXMLNode* pointNode = *pointIter;
                    const MeaXMLAttributes& attrs = pointNode->GetAttributes();

                    if ((pointNode->GetType() == MeaXMLNode::Type::Element) && (pointNode->GetData() == _T("point"))) {
                        CString name;
                        MeaFPoint pt;
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

void MeaPosition::Save(MeaPositionLogWriter& writer, int indent) const {
    writer.Write(indent, _T("<position desktopRef=\"%s\" tool=\"%s\" date=\"%s\">\n"),
                 static_cast<LPCTSTR>(m_desktopRef.ToString()),
                 static_cast<LPCTSTR>(m_toolName),
                 static_cast<LPCTSTR>(m_timestamp));
    indent++;
    if (!m_desc.IsEmpty()) {
        writer.Write(indent, _T("<desc>%s</desc>\n"),
                     static_cast<LPCTSTR>(MeaXMLParser::Encode(MeaStringUtils::CRLFtoLF(m_desc))));
    }

    writer.Write(indent, _T("<points>\n"));
    indent++;
    for (const auto& pointEntry : m_points) {
        writer.Write(indent, _T("<point name=\"%s\" x=\"%s\" y=\"%s\"/>\n"),
                     static_cast<LPCTSTR>(pointEntry.first),
                     static_cast<LPCTSTR>(MeaStringUtils::DblToStr(pointEntry.second.x)),
                     static_cast<LPCTSTR>(MeaStringUtils::DblToStr(pointEntry.second.y)));
    }
    indent--;
    writer.Write(indent, _T("</points>\n"));

    writer.Write(indent, _T("<properties>\n"));
    indent++;
    if (m_fieldMask & MeaWidthField) {
        writer.Write(indent, _T("<width value=\"%s\"/>\n"), static_cast<LPCTSTR>(MeaStringUtils::DblToStr(m_width)));
    }
    if (m_fieldMask & MeaHeightField) {
        writer.Write(indent, _T("<height value=\"%s\"/>\n"), static_cast<LPCTSTR>(MeaStringUtils::DblToStr(m_height)));
    }
    if (m_fieldMask & MeaDistanceField) {
        writer.Write(indent, _T("<distance value=\"%s\"/>\n"), static_cast<LPCTSTR>(MeaStringUtils::DblToStr(m_distance)));
    }
    if (m_fieldMask & MeaAreaField) {
        writer.Write(indent, _T("<area value=\"%s\"/>\n"), static_cast<LPCTSTR>(MeaStringUtils::DblToStr(m_area)));
    }
    if (m_fieldMask & MeaAngleField) {
        writer.Write(indent, _T("<angle value=\"%s\"/>\n"), static_cast<LPCTSTR>(MeaStringUtils::DblToStr(m_angle)));
    }
    indent--;
    writer.Write(indent, _T("</properties>\n"));

    indent--;
    writer.Write(indent, _T("</position>\n"));
}

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
#include "XMLWriter.h"
#include <meazure/utilities/StringUtils.h>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <ios>


const char* MeaXMLWriter::kIndent = u8"    ";


void MeaXMLWriter::Reset() {
    while (!m_elementStack.empty()) {
        m_elementStack.pop();
    }

    m_currentState = State::BeforeDoc;
}

void MeaXMLWriter::Flush() {
    m_out.flush();
}

MeaXMLWriter& MeaXMLWriter::StartDocument() {
    HandleEvent(Event::StartDocument);

    WriteUTF8Literal(u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    WriteNewline();
    return *this;
}

MeaXMLWriter& MeaXMLWriter::EndDocument() {
    HandleEvent(Event::EndDocument);

    WriteNewline();
    Flush();

    return *this;
}

MeaXMLWriter& MeaXMLWriter::Doctype(PCTSTR name, PCTSTR systemId) {
    WriteUTF8Literal(u8"<!DOCTYPE ");
    WriteRaw(name);
    WriteUTF8Literal(u8" SYSTEM \"");
    WriteRaw(systemId);
    WriteUTF8Literal(u8"\">");
    WriteNewline();

    return *this;
}

MeaXMLWriter& MeaXMLWriter::StartElement(PCTSTR name) {
    State previousState = HandleEvent(Event::StartElement);

    ElementPtr element = std::make_shared<Element>(name, previousState);
    m_elementStack.push(element);

    return *this;
}

MeaXMLWriter& MeaXMLWriter::EndElement() {
    HandleEvent(Event::EndElement);

    return *this;
}

MeaXMLWriter& MeaXMLWriter::AddAttribute(PCTSTR name, const CString& value) {
    HandleEvent(Event::Attribute);

    AttributePtr attribute = std::make_shared<Attribute>(name, value);
    m_elementStack.top()->m_attributes.push_back(attribute);

    return *this;
}

MeaXMLWriter& MeaXMLWriter::AddAttribute(PCTSTR name, int value) {
    AddAttribute(name, MeaStringUtils::IntToStr(value));

    return *this;
}

MeaXMLWriter& MeaXMLWriter::AddAttribute(PCTSTR name, double value) {
    AddAttribute(name, MeaStringUtils::DblToStr(value));

    return *this;
}

MeaXMLWriter& MeaXMLWriter::Characters(PCTSTR str) {
    HandleEvent(Event::Characters);

    WriteEscaped(MeaStringUtils::CRLFtoLF(str));

    return *this;
}

MeaXMLWriter::State MeaXMLWriter::HandleEvent(Event event) {
    State previousState = m_currentState;
    bool invalidEvent = false;

    switch (m_currentState) {
    case State::BeforeDoc:
        if (event == Event::StartDocument) {
            m_currentState = State::BeforeRoot;
        } else {
            invalidEvent = true;
        }
        break;
    case State::BeforeRoot:
        switch (event) {
        case Event::Characters:
            break;
        case Event::StartElement:
            m_currentState = State::InStartTag;
            break;
        case Event::EndDocument:
            m_currentState = State::AfterDoc;
            break;
        default:
            invalidEvent = true;
            break;
        }
        break;
    case State::InStartTag:
        switch (event) {
        case Event::Attribute:
            break;
        case Event::Characters:
            WriteStartElement(false);
            m_currentState = State::AfterData;
            break;
        case Event::StartElement:
            WriteStartElement(false);
            m_currentState = State::InStartTag;
            break;
        case Event::EndElement:
            WriteStartElement(true);
            m_currentState = (m_elementStack.size() == 0) ? State::AfterRoot : State::AfterTag;
            break;
        case Event::EndDocument:
            WriteStartElement(true);
            m_currentState = State::AfterDoc;
            break;
        default:
            invalidEvent = true;
            break;
        }
        break;
    case State::AfterTag:
        switch (event) {
        case Event::Characters:
            m_currentState = State::AfterData;
            break;
        case Event::StartElement:
            m_currentState = State::InStartTag;
            break;
        case Event::EndElement:
            WriteEndElement();
            m_currentState = (m_elementStack.size() == 0) ? State::AfterRoot : State::AfterTag;
            break;
        default:
            invalidEvent = true;
            break;
        }
        break;
    case State::AfterData:
        switch (event) {
        case Event::Characters:
            break;
        case Event::StartElement:
            m_currentState = State::InStartTag;
            break;
        case Event::EndElement:
            WriteEndElement();
            m_currentState = (m_elementStack.size() == 0) ? State::AfterRoot : State::AfterTag;
            break;
        default:
            invalidEvent = true;
            break;
        }
        break;
    case State::AfterRoot:
        switch (event) {
        case Event::Characters:
            break;
        case Event::EndDocument:
            m_currentState = State::AfterDoc;
            break;
        default:
            invalidEvent = true;
            break;
        }
        break;
    case State::AfterDoc:
        invalidEvent = true;
        break;
    default:
        CStringA msg;
        msg.Format("Unrecognized state %s", GetStateName(m_currentState));
        throw std::ios::failure(msg);
    }

    if (invalidEvent) {
        CStringA msg;
        msg.Format("Event %s not allowed in state %s", GetEventName(event), GetStateName(m_currentState));
        throw std::ios::failure(msg);
    }

    return previousState;
}

PCSTR MeaXMLWriter::GetStateName(State state) {
    switch (state) {
    case State::BeforeDoc:
        return "BeforeDoc";
    case State::BeforeRoot:
        return "BeforeRoot";
    case State::InStartTag:
        return "InStartTag";
    case State::AfterTag:
        return "AfterTag";
    case State::AfterData:
        return "AfterData";
    case State::AfterRoot:
        return "AfterRoot";
    case State::AfterDoc:
        return "AfterDoc";
    default:
        assert(false);
        return _T("");
    }
}

PCSTR MeaXMLWriter::GetEventName(Event event) {
    switch (event) {
    case Event::Attribute:
        return _T("Attribute");
    case Event::Characters:
        return _T("Characters");
    case Event::EndDocument:
        return _T("EndDocument");
    case Event::EndElement:
        return _T("EndElement");
    case Event::StartDocument:
        return _T("StartDocument");
    case Event::StartElement:
        return _T("StartElement");
    default:
        assert(false);
        return _T("");
    }
}

void MeaXMLWriter::WriteStartElement(bool isEmpty) {
    ElementPtr element = m_elementStack.top();

    if ((element->m_state != State::AfterData) && (m_elementStack.size() > 1)) {
        WriteNewline();
        WriteIndent();
    }

    WriteUTF8Literal(u8'<');
    WriteRaw(element->m_name);
    WriteAttributes(element->m_attributes);
    WriteUTF8Literal(isEmpty ? u8"/>" : u8">");

    // If this is an empty tag, act like an end tag has been specified.
    if (isEmpty) {
        m_elementStack.pop();
    }
}

void MeaXMLWriter::WriteEndElement() {
    if (m_currentState != State::AfterData) {
        WriteNewline();
        WriteIndent();
    }

    ElementPtr element = m_elementStack.top();

    WriteUTF8Literal(u8"</");
    WriteRaw(element->m_name);
    WriteUTF8Literal(u8'>');

    m_elementStack.pop();
}

void MeaXMLWriter::WriteAttributes(Attributes attributes) {
    for (AttributePtr attribute : attributes) {
        WriteUTF8Literal(u8' ');
        WriteRaw(attribute->m_name);
        WriteUTF8Literal(u8'=');
        WriteQuoted(attribute->m_value);
    }
}

void MeaXMLWriter::WriteIndent() {
    std::size_t level = m_elementStack.size() - 1;
    while (level-- > 0) {
        WriteUTF8Literal(kIndent);
    }
}


void MeaXMLWriter::WriteQuoted(PCTSTR str) {
    WriteUTF8Literal(u8'"');
    WriteEscaped(str);
    WriteUTF8Literal(u8'"');
}

void MeaXMLWriter::WriteEscaped(PCTSTR str) {
    if (str != nullptr) {
        size_t len = _tcslen(str);
        for (size_t i = 0; i < len; i++) {
            WriteEscaped(str[i]);
        }
    }
}

void MeaXMLWriter::WriteEscaped(TCHAR ch) {
    switch (ch) {
    case _T('&'):
        WriteUTF8Literal(u8"&amp;");
        break;
    case _T('<'):
        WriteUTF8Literal(u8"&lt;");
        break;
    case _T('>'):
        WriteUTF8Literal(u8"&gt;");
        break;
    case _T('\''):
        WriteUTF8Literal(u8"&apos;");
        break;
    case _T('\"'):
        WriteUTF8Literal(u8"&quot;");
        break;
    case _T('\n'):
        WriteNewline();
        break;
    case _T('\t'):
        WriteUTF8Literal(u8'\t');
        break;
    case _T('\r'):
        WriteUTF8Literal(u8'\r');
        break;
    default:
#ifdef _UNICODE
        if (ch > '\u001F' && ch < '\u007F') {
            writeRaw(ch);
        } else {
            if ((ch >= '\u007F' && ch <= '\uD7FF') || (ch >= '\uE000' && ch <= '\uFFFD')) {
                WriteUTF8Literal(u8"&#");
                m_out << static_cast<std::uint16_t>(ch);
                WriteUTF8Literal(u8';');
            } else {
                WriteUTF8Literal(u8"ctrl-");
                m_out << static_cast<std::uint16_t>(ch);
            }
        }
#else
        if (ch > '\x1F' && ch < '\x7F') {
            WriteUTF8Literal(ch);
        } else {
            WCHAR wch = CStringW(&ch, 1)[0];
            if ((wch >= L'\u007F' && wch <= L'\uD7FF') || (wch >= L'\uE000' && wch <= L'\uFFFD')) {
                WriteUTF8Literal(u8"&#");
                m_out << static_cast<std::uint16_t>(wch);
                WriteUTF8Literal(u8';');
            } else {
                WriteUTF8Literal(u8"ctrl-");
                m_out << static_cast<std::uint16_t>(wch);
            }
        }
#endif
        break;
    }
}

void MeaXMLWriter::WriteNewline() {
    WriteUTF8Literal(u8'\n');
}


void MeaXMLWriter::WriteRaw(PCTSTR str) {
    WriteUTF8Literal(MeaStringUtils::ACPtoUTF8(str));
}


void MeaXMLWriter::WriteRaw(TCHAR ch) {
    WriteUTF8Literal(MeaStringUtils::ACPtoUTF8(ch));
}

void MeaXMLWriter::WriteUTF8Literal(PCSTR str) {
    m_out << str;
}

void MeaXMLWriter::WriteUTF8Literal(char ch) {
    m_out << ch;
}

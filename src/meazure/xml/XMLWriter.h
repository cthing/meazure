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
/// @brief Header file for writing XML files.

#pragma once

#include <meazure/utilities/StringUtils.h>
#include <fstream>
#include <iostream>
#include <list>
#include <stack>
#include <memory>


/// This class writes pretty printed XML in UTF-8 encoding. Because this class serves only the needs of this
/// application, it does not support XML namespaces. Typical usage:</p>
/// <ol>
///      <li>Create an MeaXMLWriter specifying the output destination.</li>
///      <li>Begin the XML output by calling the startDocument method.
///      <li>Call XML output methods as needed (e.g. startElement).</li>
///      <li>Complete the XML output by calling the endDocument method to properly
///          terminate the XML document. No XML output methods can be called following the call to endDocument.</li>
///      <li>Optionally, reuse the XmlWriter instance by calling the reset method.</li>
/// </ol>
/// <p>The following is an example of using MeaXMLWriter to write a simple XML document to the standard output:</p>
/// <pre>
/// MeaXMLWriter writer(std::cout);
///
/// writer.StartDocument();
/// writer.StartElement(_T("elem1"));
/// writer.AddAttribute(_T("id"), _T("1234"));
/// writer.Characters(_T("Hello World"));
/// writer.EndElement();
/// writer.EndDocument();
/// </pre>
/// <p>The following XML is displayed on the standard output:</p>
/// <pre>
/// &lt;?xml version="1.0" encoding="UTF-8"?&gt;
/// &lt;elem1 id="1234"&gt;Hello World&lt;/elem1&gt;
/// </pre>
///
class MeaXMLWriter {

public:
    /// Creates an XML writer that writes to the specified writer.
    ///
    /// @param out      [in] Output destination or {@code null} to use the standard output.
    ///
    MeaXMLWriter(std::ostream& out) : m_out(out) {
        Reset();
    }
    
    /// Resets the XML writer to its initial state so that it can be reused. After endDocument(), the reset method
    /// must be called before the XmlWriter can be reused for output.
    ///
    void Reset();

    /// Starts an XML document by writing the XML header. This method writes a header with UTF-8 encoding. Call
    /// This method must be called before any other XML writer output method is called.
    /// 
    /// @return This writer instance
    /// 
    MeaXMLWriter& StartDocument();

    /// Ends the XML output. This method must be called to properly terminate the XML document. Before the XmlWriter
    /// can be reused, the reset() method must be called. This method does not close the underlying output stream.
    /// 
    /// @return This writer instance
    /// 
    MeaXMLWriter& EndDocument();

    /// Writes a Document Type Declaration.
    ///
    /// @param name         [in] Root element name
    /// @param systemId     [in] System identified (must be specified).
    /// @return This writer instance
    /// 
    MeaXMLWriter& Doctype(PCTSTR name, PCTSTR systemId);

    /// Starts a new element. Attributes for the element can be specified using the addAttributes. Each call to
    /// startElement requires a corresponding call to endElement.
    ///
    /// @param name    [in] Name of the element 
    /// @return This writer instance
    /// 
    MeaXMLWriter& StartElement(PCTSTR name);

    /// Writes an end tag for the element that is open at the current level. Each call to startElement()
    /// requires a corresponding call to endElement.
    /// 
    /// @return This writer instance
    ///
    MeaXMLWriter& EndElement();

    /// Adds an attribute to the current start tag.
    ///
    /// @param name    [in] Attribute name
    /// @param value   [in] Attribute value
    /// @return This writer instance
    /// 
    MeaXMLWriter& AddAttribute(PCTSTR name, const CString& value);

    /// Adds an attribute to the current start tag.
    ///
    /// @param name    [in] Attribute name
    /// @param value   [in] Attribute value
    /// @return This writer instance
    /// 
    MeaXMLWriter& AddAttribute(PCTSTR name, int value);

    /// Adds an attribute to the current start tag.
    ///
    /// @param name    [in] Attribute name
    /// @param value   [in] Attribute value
    /// @return This writer instance
    /// 
    MeaXMLWriter& AddAttribute(PCTSTR name, double value);

    /// Writes the specified string as escaped XML data. Line endings are normalized to LF so that the output
    /// stream can translate them to the platform specific ending.
    ///
    /// @param str  [in] String to write
    /// @return This writer instance
    /// 
    MeaXMLWriter& Characters(PCTSTR str);

    /// Flushes the output. This method is especially useful for ensuring that the entire document has been output
    /// without having to close the writer. This method is invoked automatically by the endDocument() method.
    ///
    void Flush();

protected:
    /// Represents an XML element attribute.
    ///
    struct Attribute {
        Attribute(const CString& name, const CString& value) : m_name(name), m_value(value) {}

        CString m_name;
        CString m_value;
    };


    typedef std::shared_ptr<Attribute> AttributePtr;
    typedef std::list<AttributePtr> Attributes;


    /// Writes a start tag and handles the case where the element is empty.
    ///
    /// @param isEmpty  [in] <b>true</b> to write start element as if it were empty
    /// 
    virtual void WriteStartElement(bool isEmpty);

    /// Writes an end tag.
    ///
    virtual void WriteEndElement();
    
    /// Writes out an attribute list, quoting and escaping values. The attributes will be written all on one line.
    ///
    /// @param attributes   [in] The attribute list to write.
    ///
    virtual void WriteAttributes(Attributes attributes);

    /// Indents the output based on the element nesting level.
    ///
    virtual void WriteIndent();

    /// Writes the specified string to the output as an escaped string surrounded by double quotes.
    ///
    /// @param str  [in] String to quote, escape and write
    ///
    virtual void WriteQuoted(PCTSTR str);

    /// Writes the specified string to the output escaping the '&', '<', '>', and quote characters using the
    /// standard XML escape sequences. Control characters and characters outside the ASCII range are escaped using a
    /// numeric character reference. Invalid XML control characters are written as "ctrl-nnnn".
    ///
    /// @param str   [in] String to escape and write
    /// 
    virtual void WriteEscaped(PCTSTR str);

    /// Writes the specified character to the output escaping the '&', '<', '>', and quote characters using the
    /// standard XML escape sequences. Control characters and characters outside the ASCII range are escaped using a
    /// numeric character reference. Invalid XML control characters are written as "ctrl-nnnn".
    ///
    /// @param ch   [in] Character to escape and write
    /// 
    virtual void WriteEscaped(TCHAR ch);

    /// Writes a newline character. The runtime is relied upon to translate the line feed character into the
    /// platform specific line ending appropriate to the output stream (e.g. CR+LF for ofstream on Windows).
    ///
    virtual void WriteNewline();

    /// Writes the specified string to the output without escaping.
    ///
    /// @param str  [in] String to write
    ///
    virtual void WriteRaw(PCTSTR str);

    /// Writes the specified character to the output without escaping.
    ///
    /// @param ch   [in] Character to write
    ///
    virtual void WriteRaw(TCHAR ch);

    /// Writes the specified UTF-8 literal string without any escaping or conversion.
    /// 
    /// @param str  [in] UTF-8 literal string to write
    /// 
    virtual void WriteUTF8Literal(PCSTR str);

    /// Writes the specified UTF-8 literal character without any escaping or conversion.
    /// 
    /// @param str  [in] UTF-8 literal character to write
    /// 
    virtual void WriteUTF8Literal(char ch);

private:
    /// Represents the state of the writer.
    ///
    enum class State {
        BeforeDoc,
        BeforeRoot,
        InStartTag,
        AfterTag,
        AfterData,
        AfterRoot,
        AfterDoc
    };


    /// Represents a writing state transition event.
    ///
    enum class Event {
        Attribute,
        Characters,
        EndDocument,
        EndElement,
        StartDocument,
        StartElement
    };


    /// Represents an XML element.
    ///
    struct Element {
        Element(PCTSTR name, State state): m_name(name), m_state(state) {}

        CString m_name;             ///< Name of the element
        Attributes m_attributes;    ///< Attributes for the element
        State m_state;              ///< Writer state in which this element is being written
    };


    typedef std::shared_ptr<Element> ElementPtr;
    typedef std::stack<ElementPtr> ElementStack;


    /// Heart of the XmlWriter state machine. Based on the current state and the specified event, an action is fired,
    /// if any, and the next state is set. If the event is illegal for the current state, an std::ios_base::failure is
    /// thrown.
    ///
    /// @param event    [in] The event to handle
    /// @return Previous state
    /// @throws std::ios_base::failure If the event is illegal given the current state.
    ///
    State HandleEvent(Event event);

    /// Obtains a string representation of the specified writing state for debugging purposes.
    /// 
    /// @param state    [in] Writing state whose string representation is to be returned
    /// @return String representation of the specified state.
    ///
    static PCSTR GetStateName(State state);

    /// Obtains a string representation of the specified writing event for debugging purposes.
    /// 
    /// @param event    [in] Writing event whose string representation is to be returned
    /// @return String representation of the specified event.
    ///
    static PCSTR GetEventName(Event state);


    static const char* kIndent;                     ///< String for each level of indentation


    std::ostream& m_out;            ///< Output stream to write the XML
    ElementStack m_elementStack;    ///< Stack of open elements
    State m_currentState;           ///< Current state of the writer state machine.
};

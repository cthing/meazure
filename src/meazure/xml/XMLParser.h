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
/// @brief Header file for the Xerces XML parser wrapper and support classes.

#pragma once

#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/sax/DocumentHandler.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/Locator.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/Xerces_autoconf_config.hpp>
#include <map>
#include <list>
#include <stack>
#include <iostream>
#include <cassert>


class MeaXMLParser;

/// Exception thrown when an error occurs during XML parsing.
///
class MeaXMLParserException {};


/// The class contains the attributes associated with an XML start element. In addition to iterating through
/// the attributes, the class provides searching and other attribute manipulation capabilities.
///
class MeaXMLAttributes {

    friend MeaXMLParser;

public:
    /// Constructs an empty instance of the XML attributes class.
    ///
    MeaXMLAttributes() = default;

    /// Copy constructor. A deep copy of the specified attribute object is performed.
    ///
    /// @param attrs    [in] XML attribute object instance to copy.
    ///
    MeaXMLAttributes(const MeaXMLAttributes& attrs) { Assign(attrs); }

    /// Performs a deep assignment of the specified XML attribute object.
    ///
    /// @param attrs    [in] XML attribute object to assign to this.
    /// @return this
    ///
    MeaXMLAttributes& operator=(const MeaXMLAttributes& attrs) { return Assign(attrs); }

    /// Indicates whether there are any attributes present.
    ///
    /// @return <b>true</b> if there are no attributes.
    ///
    bool IsEmpty() const { return m_attributeMap.empty(); }

    /// Returns the value of the specified attribute as a string.
    /// 
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as a string.
    /// @return <b>true</b> if the attribute is found.
    /// 
    bool GetValueStr(PCTSTR name, CString& value) const;

    /// Returns the value of the specified attribute converted to an integer.
    /// 
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as an integer.
    /// @return <b>true</b> if the attribute is found.
    /// 
    bool GetValueInt(PCTSTR name, int& value) const;

    /// Returns the value of the specified attribute converted to a double.
    /// 
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as a double.
    /// @return <b>true</b> if the attribute is found.
    /// 
    bool GetValueDbl(PCTSTR name, double& value) const;

    /// Returns the value of the specified attribute converted to a boolean.
    /// 
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as a boolean. This parameter is set to <b>true</b> if the
    ///                     attribute contains the string "true" or "1". <b>false</b> is returned otherwise.
    /// @return <b>true</b> if the attribute is found.
    /// 
    bool GetValueBool(PCTSTR name, bool& value) const;

    /// Performs a deep copy assignment of the specified attribute object to this.
    ///
    /// @param attrs        [in] Attribute object to assign to this.
    /// @return this
    ///
    MeaXMLAttributes& Assign(const MeaXMLAttributes& attrs);

protected:
    typedef std::map<CString, CString> AttributeMap;

    /// Constructs an instance of the class based on the specified attributes provided by the Xerces parser.
    ///
    /// @param atts  [in] Attributes from Xerces.
    ///
    MeaXMLAttributes(const xercesc::AttributeList& atts);

    AttributeMap m_attributeMap;     ///< Maps attribute names to their values.
};


/// A node in the XML DOM. The MeaXMLParser class can build a DOM from the parsed file. This is a very minimal DOM
/// and does not conform to the W3C DOM spec.
///
class MeaXMLNode {

public:
    typedef std::list<MeaXMLNode*> NodeList;            ///< Represents a list of DOM nodes.
    typedef std::list<const MeaXMLNode*> NodeList_c;    ///< Represents a list of DOM nodes.
    typedef NodeList::const_iterator NodeIter_c;        ///< Constant iterator over the DOM nodes.

    /// Indicates the type of the DOM node.
    ///
    enum class Type {
        Unknown,        ///< Initial type for a DOM node.
        Element,        ///< The node represents an XML element.
        Data            ///< The node represents data contained between XML elements.
    };


    /// Constructs an empty DOM node of unknown type.
    ///
    MeaXMLNode();

    /// Constructs a DOM node representing the specified element.
    ///
    /// @param elementName  [in] The node represents this element.
    /// @param attrs        [in] Attributes associated with the element.
    ///
    MeaXMLNode(const CString& elementName, const MeaXMLAttributes& attrs);

    /// Constructs a DOM node representing the specified data.
    ///
    /// @param data     [in] Data for the node.
    ///
    explicit MeaXMLNode(const CString& data);

    /// Copy constructor.
    ///
    /// @param node     [in] Node to copy.
    ///
    MeaXMLNode(const MeaXMLNode& node) { Assign(node); }

    /// Destroys a node.
    ///
    virtual ~MeaXMLNode();

    /// Returns the type of the node.
    /// 
    /// @return Node type.
    /// 
    Type GetType() const { return m_type; }

    /// Returns the data for the node. The data returned depends on the node type:
    ///
    /// <table cellspacing="0" cellpadding="3">
    /// <tr><th>Node Type</th><th>GetData() Return</th></tr>
    /// <tr><td>Unknown</td><td>""</td></tr>
    /// <tr><td>Element</td><td>Element name</td></tr>
    /// <tr><td>Data</td><td>Data</td></tr>
    /// </table>
    /// @return Data appropriate for the node.
    ///
    const CString& GetData() const { return m_data; }

    /// Concatenates all child data nodes into a single string.
    ///
    /// @return String concatenation of all child data nodes.
    ///
    CString GetChildData() const;

    /// Returns the attributes associated with the node if it is of type Element.
    /// 
    /// @return Attributes associated with the node.
    /// 
    const MeaXMLAttributes& GetAttributes() const { return m_attributes; }

    /// Indicates whether there are any attributes associated with the node if it is of type Element.
    ///
    /// @return <b>true</b> if the node has attributes.
    ///
    bool HasAttributes() const { return !m_attributes.IsEmpty(); }

    /// Performs a deep copy assignment of the specified node to this.
    /// 
    /// @param node     [in] DOM node to assign to this.
    /// @return this
    /// 
    MeaXMLNode& operator=(const MeaXMLNode& node) { return Assign(node); }

    /// Adds the specified DOM node as a child of this node.
    ///
    /// @param child        [in] Node to add as a child.
    ///
    void AddChild(MeaXMLNode* child) {
        assert(child != nullptr);
        child->SetParent(this);
        m_children.push_back(child);
    }

    /// Performs a deep copy assignment of the specified node to this.
    /// 
    /// @param node     [in] DOM node to assign to this.
    /// @return this
    /// 
    MeaXMLNode& Assign(const MeaXMLNode& node);

    /// Returns a constant iterator over the children of this node.
    ///
    /// @return Constant iterator over the children nodes.
    ///
    NodeIter_c GetChildIter() const { return m_children.begin(); }

    /// Indicates whether the specified iterator has reached the end of the list of children nodes.
    ///
    /// @param iter     [in] Constant iter to test.
    /// @return <b>true</b> if the iterator has reached the end of the list of children nodes.
    ///
    bool AtEnd(const NodeIter_c& iter) const { return iter == m_children.end(); }

    /// Attempts to find the specified child element.
    /// 
    /// @param elementName      [in] Name of the element to find
    /// @return The first child element with the specified name or nullptr if not found.
    /// 
    const MeaXMLNode* FindChildElement(PCTSTR elementName) const {
        for (const MeaXMLNode* node : m_children) {
            if (node->GetType() == Type::Element && node->GetData() == elementName) {
                return node;
            }
        }
        return nullptr;
    }

    /// Attempts to find the specified child elements.
    /// 
    /// @param elementName      [in] Name of the elements to find
    /// @return All child elements with the specified name or an empty list if not found.
    /// 
    NodeList_c FindChildElements(PCTSTR elementName) const {
        std::list<const MeaXMLNode*> nodes;
        for (const MeaXMLNode* node : m_children) {
            if (node->GetType() == Type::Element && node->GetData() == elementName) {
                nodes.push_back(node);
            }
        }
        return nodes;
    }

    friend std::ostream& operator<<(std::ostream& os, const MeaXMLNode& node);

private:
    /// Sets the specified node as the parent for this node. This allows moving up the node hierarchy to the root.
    ///
    /// @param parent   [in] Node to set as the parent of this.
    ///
    void SetParent(MeaXMLNode* parent) { m_parent = parent; }


    Type m_type;                    ///< Type for the node.
    CString m_data;                 ///< Either empty, element name, or character data depending on the node type.
    MeaXMLAttributes m_attributes;  ///< Attributes associated with an element node.
    NodeList m_children;            ///< Children of this node.
    MeaXMLNode* m_parent;           ///< Parent of this node.
};

std::ostream& operator<<(std::ostream& os, const MeaXMLNode::Type& type);


/// Mix-in class for XML parser callback methods. A class that wants XML parsing services inherits from this class,
/// overrides the methods of this class for the events of interest, creates an instance of the MeaXMLParser class
/// and points it at the XML file to parse. The parser calls back using the methods of this class to indicate various
/// XML parsing events. This is classic SAX parsing behavior. The class has noop implementations for all methods.
///
struct MeaXMLParserHandler {

    /// Called when a new element is opened (e.g. &lt;foo&gt; or &lt;foo/&gt;).
    ///
    /// @param container    [in] Parent element.
    /// @param elementName  [in] Name of the element being opened.
    /// @param attrs        [in] Attributes associated with the element.
    ///
    virtual void StartElement(const CString& container, const CString& elementName, const MeaXMLAttributes& attrs);

    /// Called when an element is closed (e.g. &lt;/foo&gt; or &lt;foo/&gt;).
    ///
    /// @param container    [in] Parent element.
    /// @param elementName  [in] Name of the element being closed.
    ///
    virtual void EndElement(const CString& container, const CString& elementName);

    /// Called for all character data encountered between elements. Note that this method will be called for all
    /// character data including whitespace between consecutive elements. The DTD must be used to determine what
    /// whitespace is relevant.
    ///
    /// @param container    [in] Name of the closest open element containing the character data.
    /// @param data         [in] Character data.
    ///
    virtual void CharacterData(const CString& container, const CString& data);

    /// Called to resolve an external entity (e.g. DTD).
    ///
    /// @param pathname [in] Pathname of the external entity.
    /// @return Input source for the external entity. The default implementation returns <b>nullptr</b>.
    ///
    virtual xercesc::InputSource* ResolveEntity(const CString& pathname);

    /// Called when there is a parsing error (e.g. syntax, structure).
    /// 
    /// @param error     [in] Error message
    /// @param pathname  [in] Pathname of the file containing the error
    /// @param line      [in] Line number on which the error occurred (1 based)
    /// @param column    [in] Column number in which the error occurred (1 based)
    ///
    virtual void ParsingError(const CString& error, const CString& pathname, int line, int column);

    /// Called when there is a validation error (e.g. unknown element).
    /// 
    /// @param error     [in] Error message
    /// @param pathname  [in] Pathname of the file containing the error
    /// @param line      [in] Line number on which the error occurred (1 based)
    /// @param column    [in] Column number in which the error occurred (1 based)
    ///
    virtual void ValidationError(const CString& error, const CString& pathname, int line, int column);

    /// Returns the pathname of the currently parsed file.
    ///
    /// @return Pathname of the file currently being parsed.
    ///         This class's implementation of this method returns
    ///         the empty string.
    ///
    virtual CString GetFilePathname();
};


/// Provides SAX style XML parsing. This class is a wrapper around the Xerces parser including XML DTD validation.
/// A class that wants XML parsing services inherits from the MeaXMLParserHandler class, overrides the methods of
/// that class for the events of interest, creates an instance of this class and points it at the XML file to parse.
/// The parser calls back using the methods of the MeaXMLParserHandler class to indicate various XML parsing events.
/// This is classic SAX parsing behavior. The class can also build a primitive DOM.
///
class MeaXMLParser : public xercesc::DocumentHandler, public xercesc::EntityResolver, public xercesc::ErrorHandler {

public:
    /// Constructs an XML parser that only builds a DOM.
    ///
    MeaXMLParser();

    /// Constructs an XML parser.
    ///
    /// @param handler      [in] Callback object for parsing events.
    /// @param buildDOM     [in] Indicates whether a DOM should be built.
    ///
    explicit MeaXMLParser(MeaXMLParserHandler* handler, bool buildDOM = false);

    virtual ~MeaXMLParser();

    /// Parses the specified XML file.
    /// 
    /// @param pathname  [in] XML file to parse
    /// 
    void ParseFile(PCTSTR pathname);

    /// Parses the specified XML cotent.
    /// 
    /// @param content   [in] String of XML content to parse
    ///  
    void ParseString(PCTSTR content);

    /// If a DOM was constructed, this method returns its root node.
    ///
    /// @return Root node of the DOM or nullptr if none was constructed.
    ///
    const MeaXMLNode* GetDOM() const { return m_dom; }

private:
    typedef std::stack<CString> ElementStack;       ///< A stack type for elements.
    typedef std::stack<MeaXMLNode*> NodeStack;      ///< A stack type for DOM nodes.
    typedef std::stack<CString> PathnameStack;      ///< A stack type for entity pathnames.

    // DocumentHandler

    /// Receives notification of the beginning of a document. The parser will invoke this method only once, before
    /// any other methods in this interface.
    /// 
    void startDocument() override;

    /// Receives an object for locating the origin of SAX document events. The locator allows the application to
    /// determine the end position of any document-related event, even if the parser is not reporting an error.
    /// Typically, the application will use this information for reporting its own errors (such as character content
    /// that does not match an application's business rules). Note that the locator will return correct information
    /// only during the invocation of the events in this interface. The application should not attempt to use it at
    /// any other time.
    /// 
    /// @param locator   [in] An object that can return the location of any SAX document event. The object is only
    ///                  'on loan' to the client code and no attempt should me made to delete or modify it.
    /// 
    void setDocumentLocator(const xercesc::Locator* const locator) override;

    /// Receives notification of a processing instruction. The parser will invoke this method once for each processing
    /// instruction found. Note that the processing instructions may occur before or after the main document element.
    /// The parser will never report an XML declaration (XML 1.0, section 2.8) or a text declaration (XML 1.0,
    /// section 4.3.1) using this method.
    /// 
    /// @param target   [in] Processing instruction target
    /// @param data     [in] processing instruction data, or null if none was supplied.
    /// 
    void processingInstruction(const XMLCh* const target, const XMLCh* const data) override;

    /// Receives notification of the beginning of an element. The parser will invoke this method at the beginning of
    /// every element in the XML document. There will be a corresponding endElement() event for every startElement()
    /// event (even when the element is empty). All of the element's content will be reported, in order, before the
    /// corresponding endElement() event.
    ///
    /// @param name    [in] Name of the element
    /// @param attrs   [in] Element attributes. Note that the attribute list will contain only attributes with
    ///                explicit values (specified or defaulted). IMPLIED attributes will be omitted.
    /// 
    void startElement(const XMLCh* const name, xercesc::AttributeList& attrs) override;

    /// Receives notification of the end of an element. The parser will invoke this method at the end of every element
    /// in the XML document. There will be a corresponding startElement() event for every endElement() event (even
    /// when the element is empty).
    /// 
    /// @param name  [in] Name of the element
    /// 
    void endElement(const XMLCh* const name) override;

    /// Receives notification of character data. The parser will call this method to report each chunk of character
    /// data. SAX parsers may return all contiguous character data in a single chunk, or they may split it into
    /// several chunks. However, all of the characters in any single event must come from the same external entity.
    /// Note that the parser map report whitespace using the ignorableWhitespace() method rather than this
    /// one.
    /// 
    /// @param chars   [in] Characters from the XML document
    /// @param length  [in] Number of characters read
    /// 
    void characters(const XMLCh* const chars, const XMLSize_t length) override;

    /// Receives notification of ignorable whitespace in element content. The parser uses this method to report each
    /// chunk of ignorable whitespace (see the W3C XML 1.0 recommendation, section 2.10). The parser may return all
    /// contiguous whitespace in a single chunk, or may split it into several chunks. However, all of the characters
    /// in any single event will come from the same external entity.
    /// 
    /// @param chars   [in] Characters from the XML document
    /// @param length  [in] Number of characters read
    ///
    void ignorableWhitespace(const XMLCh* const chars, const XMLSize_t length) override;

    /// Receives notification of the end of a document. The parser will invoke this method only once, and it will
    /// be the last method invoked during the parse. The parser will not invoke this method until it has either
    /// abandoned parsing (because of an unrecoverable error) or reached the end of input.
    /// 
    void endDocument() override;

    /// Resets the document object for reuse. This method helps in reseting the document implementation defaults
    /// each time the document is begun.
    /// 
    void resetDocument() override;

    // EntityResolver

    /// Allows the application to resolve external entities. The parser will call this method before opening any
    /// external entity except the top-level document entity (including the external DTD subset, external entities
    /// referenced within the DTD, and external entities referenced within the document element). The application may
    /// request that the parser resolve the entity itself, that it use an alternative URI, or that it use an entirely
    /// different input source. If the system identifier is a URL, the SAX parser will resolve it fully before
    /// reporting it to the application.
    /// 
    /// @param publicId   [in] Public identifier of the external entity being referenced, or null if none was supplied
    /// @param systemId   [in] System identifier of the external entity being referenced.
    /// @return An InputSource object describing the new input source, or null to request that the parser open a
    ///     regular URI connection to the system identifier. The returned InputSource is owned by the parser which
    ///     is responsible to clean up the memory.
    /// 
    xercesc::InputSource* resolveEntity(const XMLCh* const publicId, const XMLCh* const systemId) override;
    
    // ErrorHandler
    
    /// Receives notification of a warning. The parser will use this method to report conditions that are not errors
    /// or fatal errors as defined by the XML 1.0 recommendation.
    /// 
    /// @param exc   [in] Warning information encapsulated in a SAX parse exception.
    /// 
    void warning(const xercesc::SAXParseException& exc) override;

    /// Receives notification of a recoverable error. This corresponds to the definition of "error" in section 1.2
    /// of the W3C XML 1.0 Recommendation. For example, the parser uses this callback to report the violation of a
    /// validity constraint.
    /// 
    /// @param exc   [in] Error information encapsulated in a SAX parse exception.
    /// 
    void error(const xercesc::SAXParseException& exc) override;

    /// Receives notification of a non-recoverable error. This corresponds to the definition of "fatal error" in
    /// section 1.2 of the W3C XML 1.0 Recommendation. For example, the parser uses this callback to report the
    /// violation of a well-formedness constraint.
    /// 
    /// @param exc   [in] Error information encapsulated in a SAX parse exception.
    /// 
    void fatalError(const xercesc::SAXParseException& exc) override;

    /// Reset the error handler object for reuse. This method helps in reseting the error handler object
    /// implementation defaults each time the parsing is begun.
    /// 
    void resetErrors() override;


    static MeaXMLParserHandler m_noopHandler;   ///< Do nothing handler when only building a DOM
    static const CString m_homeURL1;            ///< URL for cthing.com
    static const CString m_homeURL2;            ///< URL for cthing.com

    MeaXMLParserHandler* m_handler;         ///< XML event callback object.
    bool m_buildDOM;                        ///< Indicates whether a DOM should be built.
    xercesc::SAXParser* m_parser;           ///< Xerces XML parser.
    ElementStack m_elementStack;            ///< Stack of open elements.
    NodeStack m_nodeStack;                  ///< Stack of XML DOM nodes.
    PathnameStack m_pathnameStack;          ///< Stack of pathnames for the entities being parsed.
    MeaXMLNode* m_dom;                     ///< Root node of the DOM being built, or nullptr.
};

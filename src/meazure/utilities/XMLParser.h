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
/// @brief Header file for the expat XML parser wrapper and support classes.

#pragma once

#include "exval.h"
#include <map>
#include <stack>
#include <cassert>


class MeaXMLParser;


/// Exception thrown when an error occurs during XML parsing.
///
class MeaXMLParserException {};


/// The class contains the attributes associated with an XML start element.
/// In addition to iterating through the attributes, the class provides
/// searching and other attribute manipulation capabilities.
///
class MeaXMLAttributes {
    friend class MeaXMLParser;

protected:
    /// Represents an attribute value.
    ///
    struct AttributeValue {
        CString value;      ///< The value of the attribute
        bool    isDefault;  ///< <b>true</b> means that the value is set from the DTD default;
                            ///< <b>false</b> means that the value explicitly specified in the XML file.
    };

public:
    /// Constructs an empty instance of the XML attributes class.
    ///
    MeaXMLAttributes();

    /// Copy constructor. A deep copy of the specified attribute object
    /// is performed.
    ///
    /// @param attrs    [in] XML attribute object instance to copy.
    ///
    MeaXMLAttributes(const MeaXMLAttributes& attrs) { Assign(attrs); }

    /// Destroys an instance of the XML attributes class.
    ///
    virtual ~MeaXMLAttributes();

    /// Performs a deep assignment of the specified XML attribute object.
    ///
    /// @param attrs    [in] XML attribute object to assign to this.
    ///
    /// @return this
    ///
    MeaXMLAttributes& operator=(const MeaXMLAttributes& attrs) { return Assign(attrs); }

    /// Returns the value of the specified attribute as a string.
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as a string.
    /// @param isDefault    [out] <b>true</b> if the value is the DTD default.
    /// @return <b>true</b> if the attribute is found.
    bool GetValueStr(LPCTSTR name, CString& value, bool& isDefault) const;

    /// Returns the value of the specified attribute converted to an integer.
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as an integer.
    /// @param isDefault    [out] <b>true</b> if the value is the DTD default.
    /// @return <b>true</b> if the attribute is found.
    bool GetValueInt(LPCTSTR name, int& value, bool& isDefault) const;

    /// Returns the value of the specified attribute converted to a double.
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as a double.
    /// @param isDefault    [out] <b>true</b> if the value is the DTD default.
    /// @return <b>true</b> if the attribute is found.
    bool GetValueDbl(LPCTSTR name, double& value, bool& isDefault) const;

    /// Returns the value of the specified attribute converted to a boolean.
    /// @param name         [in] Attribute name.
    /// @param value        [out] Attribute value as a boolean. This parameter
    ///                     is set to <b>true</b> if the attribute contains the
    ///                     string "true" or "1". <b>false</b> is returned otherwise.
    /// @param isDefault    [out] <b>true</b> if the value is the DTD default.
    /// @return <b>true</b> if the attribute is found.
    bool GetValueBool(LPCTSTR name, bool& value, bool& isDefault) const;

    /// Performs a deep copy assignment of the specified attribute object
    /// to this.
    ///
    /// @param attrs        [in] Attribute object to assign to this.
    ///
    /// @return this
    ///
    MeaXMLAttributes& Assign(const MeaXMLAttributes& attrs);

protected:
    /// Constructs an instance of the class based on the specified
    /// attributes provided by the expat parser.
    ///
    /// @param atts         [in] Array of attribute name/value pairs from expat.
    ///                     In the array the name is followed by the value which
    ///                     is then followed by another name and so on until a
    ///                     nullptr is encountered.
    /// @param numSpecified [in] Number of attributes in the array that have been
    ///                     explicitly specified versus set by default from the DTD.
    ///                     The atts array is organized such that the explicitly
    ///                     specified attributes come first.
    ///
    MeaXMLAttributes(const XML_Char** atts, int numSpecified);

    std::map<CString, AttributeValue> m_attributeMap;     ///< Maps attribute names to their values.
};


/// A node in the XML DOM. The MeaXMLParser class can build a DOM
/// from the parsed file. This is a very minimal DOM and does not
/// conform to the W3C DOM spec.
///
class MeaXMLNode {

public:
    typedef std::list<MeaXMLNode*>      NodeList;       ///< Represents a list of DOM nodes.
    typedef NodeList::const_iterator    NodeIter_c;     ///< Constant iterator over the DOM nodes.

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
    /// @return Node type.
    Type GetType() const { return m_type; }

    /// Returns the data for the node. The data returned
    /// depends on the node type:
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

    /// Returns the attributes associated with the node if it
    /// is of type Element.
    /// @return Attributes associated with the node.
    const MeaXMLAttributes& GetAttributes() const { return m_attributes; }

    /// Performs a deep copy assignment of the specified node to this.
    /// @param node     [in] DOM node to assign to this.
    /// @return this
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
    /// @param node     [in] DOM node to assign to this.
    /// @return this
    MeaXMLNode& Assign(const MeaXMLNode& node);

    /// Returns a constant iterator over the children of this node.
    ///
    /// @return Constant iterator over the children nodes.
    ///
    NodeIter_c GetChildIter() const { return m_children.begin(); }

    /// Indicates whether the specified iterator has reached the end
    /// of the list of children nodes.
    ///
    /// @param iter     [in] Constant iter to test.
    ///
    /// @return <b>true</b> if the iterator has reached the end of the
    ///         list of children nodes.
    ///
    bool AtEnd(const NodeIter_c& iter) const { return iter == m_children.end(); }

#ifdef MEA_XMLNODE_DEBUG
    /// Dumps the state of the node to the debug output using TRACE
    /// statements.
    ///
    void Dump() const;
#endif

private:
    /// Sets the specified node as the parent for this node. This
    /// allows moving up the node hierarchy to the root.
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


/// Mix-in class for XML parser callback methods. A class that wants XML
/// parsing services mix-in inherits from this class, overrides the methods
/// of this class for the events of interest, creates an instance of the
/// MeaXMLParser class and points it at the XML file to parse. The parser
/// calls back using the methods of this class to indicate various XML parsing
/// events. This is classic SAX parsing behavior. The class contains noop
/// implementations for the method.
///
class MeaXMLParserHandler {

public:
    /// Empty constructor for the handler class. This class should be
    /// thought of more as an interface than as an instantiable class.
    ///
    MeaXMLParserHandler() {}

    /// Empty destructor for the handler class.
    ///
    virtual ~MeaXMLParserHandler() {}

    /// Called when a new element is opened (e.g. &lt;foo&gt; or &lt;foo/&gt;).
    ///
    /// @param container    [in] Parent element.
    /// @param elementName  [in] Name of the element being opened.
    /// @param attrs        [in] Attributes associated with the element.
    ///
    virtual void StartElementHandler(const CString& container, const CString& elementName, 
                                     const MeaXMLAttributes& attrs);

    /// Called when an element is closed (e.g. &lt;/foo&gt; or &lt;foo/&gt;).
    ///
    /// @param container    [in] Parent element.
    /// @param elementName  [in] Name of the element being closed.
    ///
    virtual void EndElementHandler(const CString& container, const CString& elementName);

    /// Called for all character data encountered between elements.
    /// Note that this method will be called for all character data
    /// including whitespace between consecutive elements. The DTD must
    /// must be used to determine what whitespace is relevant.
    ///
    /// @param container    [in] Name of the closest open element containing
    ///                     the character data.
    /// @param data         [in] Character data.
    ///
    virtual void CharacterDataHandler(const CString& container, const CString& data);

    /// Called to parse an external entity.
    ///
    /// @param parser   [in] Current XML parser.
    /// @param pathname [in] Pathname of the external entity.
    ///
    virtual void ParseEntity(MeaXMLParser& parser, const CString& pathname);

    /// Returns the pathname of the currently parsed file.
    ///
    /// @return Pathname of the file currently being parsed.
    ///         This class's implementation of this method returns
    ///         the empty string.
    ///
    virtual CString GetFilePathname();
};


/// Provides SAX style XML parsing. This class is a wrapper around the
/// expat parser and adds XML validation. A class that wants XML parsing
/// services mix-in inherits from the MeaXMLParserHandler class, overrides
/// the methods of that class for the events of interest, creates an instance
/// of this class and points it at the XML file to parse. The parser calls
/// back using the methods of the MeaXMLParserHandler class to indicate various
/// XML parsing events. This is classic SAX parsing behavior. The class can
/// also build a primitive DOM.
///
class MeaXMLParser : public ev::IValidationHandler {

public:
    /// Constructs an XML parser.
    ///
    /// @param handler      [in] Callback object for parsing events.
    /// @param buildDOM     [in] Indicates whether a DOM should be built.
    ///
    explicit MeaXMLParser(MeaXMLParserHandler* handler, bool buildDOM = false);

    /// Creates an external entity parser. When an external is encountered,
    /// the MeaXMLParserHandler::ParseEntity method is called. This method
    /// should instantiate an XML sub-parser using this constructor and passing
    /// the parent parser provided by the callback method.
    ///
    /// @param parentParser     [in] Parent parser for the external entity
    ///                         sub-parser.
    ///
    MeaXMLParser(const MeaXMLParser& parentParser);

    /// Destroys either a primary or a sub-parser.
    ///
    virtual ~MeaXMLParser();

    /// Sets the base path for resolving relative external entities.
    ///
    /// @param path     [in] Base path.
    ///
    void SetBasePath(const CString& path);

    /// Obtains a buffer to load with XML data. This method must
    /// be called prior to each call to ParseBuffer.
    ///
    /// @param len      [in] Size of the buffer, in bytes.
    ///
    /// @return Pointer to the allocated buffer. The class instance
    ///         also holds a pointer to this buffer and owns the memory
    ///         for it. Do not call free or delete on this pointer. A
    ///         subsequent call to the ParseBuffer method will parse the
    ///         contents of this buffer.
    ///
    void* GetBuffer(int len) { return XML_GetBuffer(m_parser, len); }

    /// Parses the XML data contained in the buffer obtained by a previous
    /// call to GetBuffer. Each call to this method must be preceded by
    /// a call to GetBuffer.
    ///
    /// @param len      [in] Actual number of bytes loaded into the buffer.
    /// @param isFinal  [in] Indicates if this is the end of the XML data.
    ///
    void ParseBuffer(int len, bool isFinal);

    /// If a DOM was constructed, this method returns its root node.
    ///
    /// @return Root node of the DOM or nullptr if none was constructed.
    ///
    const MeaXMLNode* GetDOM() const { return m_dom; }

    /// Convenience method for converting the specified string from
    /// UTF8 encoding to a string.in ACP encoding.
    ///
    /// @param str      [in] String in UTF-8 encoding.
    ///
    /// @return String in ACP encoding.
    ///
    static CString FromUTF8(const XML_Char* str);

    /// Convenience method for converting the specified string from
    /// ACP encoding to UTF-8 encoding.
    ///
    /// @param str      [in] String in ACP encoding.
    ///
    /// @return String in UTF8 encoding.
    ///
    static CString ToUTF8(const CString& str);

    /// Replaces all occurrences of '&', '<', '>', '"', and "'"
    /// with the corresponding entities. Attribute values and character
    /// data (i.e. text between start/end elements) must be encoded.
    /// This method should be called <b>before</b> converting the string
    /// to UTF-8.
    ///
    /// @param str      [in] String to encode.
    ///
    /// @return XML encoded string.
    ///
    static CString Encode(const CString& str);

private:
    typedef std::stack<CString> ElementStack;       ///< A stack type for elements.
    typedef std::stack<MeaXMLNode*> NodeStack;      ///< A stack type for DOM nodes.
    typedef std::stack<CString> PathnameStack;      ///< A stack type for entity pathnames.

    /// The parser has not assignment semantics so this method is purposely undefined.
    MeaXMLParser& operator=(const MeaXMLParser&);

    /// Called by the underlying expat parser when an element is opened.
    ///
    /// @param userData     [in] this
    /// @param elementName  [in] Name of the opened element.
    /// @param attrs        [in] Array of attributes arranged as name followed
    ///                     by value and terminated by a nullptr entry.
    ///
    static void StartElementHandler(void* userData, const XML_Char* elementName, const XML_Char** attrs);

    /// Called by the underlying expat parser when an element is closed.
    ///
    /// @param userData     [in] this
    /// @param elementName  [in] Name of the element being closed.
    ///
    static void EndElementHandler(void* userData, const XML_Char* elementName);

    /// Called by the underlying expat parser when any character data is encountered.
    ///
    /// @param userData     [in] this
    /// @param s            [in] Buffer containing character data. The buffer
    ///                     might not be nullptr terminated so the len parameter
    ///                     must be used to determine the end of the buffer.
    /// @param len          [in] Number of bytes of data in the buffer, s.
    ///
    static void CharacterDataHandler(void* userData, const XML_Char* s, int len);

    /// Called by the underlying expat parser when an external entity reference
    /// is encountered.
    ///
    /// @param parser       [in] this
    /// @param context      [in] Expat internal parsing state.
    /// @param base         [in] Base path for the entity previously set by the
    ///                     SetBasePath method.
    /// @param systemId     [in] The XML system identifier for the entity.
    /// @param publicId     [in] The XML public identifier for the entity. This
    ///                     parameter will be nullptr if no public identifier was
    ///                     specified for the entity.
    ///
    /// @return The value 1, to indicate that the entity has been successfully
    ///         parsed and parsing of the remainder of the XML file should continue.
    ///
    static int ExternalEntityRefHandler(XML_Parser parser, const XML_Char* context, const XML_Char* base,
                                        const XML_Char* systemId, const XML_Char* publicId);

    /// Called by the underlying expat parser when a DOCTYPE declaration is
    /// encountered in the XML file.
    ///
    /// @param userData     [in] this
    /// @param doctypeName  [in] Document element name.
    /// @param sysid        [in] The XML system identifier for the DTD.
    /// @param pubid        [in] The public identifier for the DTD, or nullptr if none.
    /// @param has_internal_subset  [in] Non-zero if an internal DTD subset has
    ///                     been specified.
    ///
    static void DoctypeDeclHandler(void* userData, const XML_Char* doctypeName, const XML_Char* sysid,
                                        const XML_Char* pubid, int has_internal_subset);

    /// Called by the underlying expat parser when a DTD element declaration is
    /// encountered.
    ///
    /// @param userData     [in] this
    /// @param name         [in] Name of the element being declared.
    /// @param model        [in] Content model for the element.
    ///
    static void ElementDeclHandler(void* userData, const XML_Char* name, XML_Content* model);

    /// Called by the underlying expat parser when a DTD attribute declaration is
    /// encountered.
    ///
    /// @param userData     [in] this
    /// @param elname       [in] Name of the element whose attribute is being declared.
    /// @param attname      [in] Name of the attribute being declared.
    /// @param att_type     [in] The type for the attribute.
    /// @param dflt         [in] Default value for the attribute, if any.
    /// @param isrequired   [in] Non-zero if the attribute must be specified.
    ///
    static void AttributeDeclHandler(void* userData, const XML_Char* elname, const XML_Char* attname,
                                     const XML_Char* att_type, const XML_Char* dflt, int isrequired);

    /// Called when an XML parsing error occurs. Queries
    /// the parser to determine the error and displays a dialog
    /// with a description of the problem.
    ///
    void HandleParserError();

    /// Called when an XML validation error occurs.
    ///
    /// @param error        [in] Describes the validation error.
    ///
    virtual void HandleValidationError(const ev::ValidationError& error);


    static CString m_homeURL1;          ///< URL for cthing.com
    static CString m_homeURL2;          ///< URL for cthing.com
    XML_Parser m_parser;                ///< The expat XML parser.
    bool m_isSubParser;                 ///< Indicates whether this is an external entity sub-parser.
    ev::Validator* m_validator;         ///< XML validator.
    MeaXMLParserHandler* m_handler;     ///< XML event callback object.
    ElementStack* m_elementStack;       ///< Stack of open elements.
    PathnameStack* m_pathnameStack;     ///< Stack of pathnames for the entities being parsed.
    bool m_haveDTD;                     ///< Indicates if a DTD is referenced by the XML file being parsed.
    const XML_Char* m_context;          ///< Internal expat parser state.
    bool m_buildDOM;                    ///< Indicates whether a DOM is being built.
    MeaXMLNode* m_dom;                  ///< Root node of the DOM being built, or nullptr.
    NodeStack* m_nodeStack;             ///< Stack of XML DOM nodes.
};

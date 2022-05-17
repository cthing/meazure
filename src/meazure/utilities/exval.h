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
/// @brief Header file for the expat compatible XML validator.

#pragma once

#include <expat/expat.h>
#include <time.h>
#include <ostream>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <string>


/// exval uses the namespace ev for everything.
///
namespace ev {

// Encodings are keyed off the same macro as expat.
//
#ifdef XML_UNICODE
/// Text encoding macro.
#define EV_T(x)     L##x
typedef std::wstring    EVString;       ///< String type.
typedef std::wostream   EVOstream;      ///< Output stream type
#else
/// Text encoding macro.
#define EV_T(x)     x
typedef std::string     EVString;       ///< String type.
typedef std::ostream    EVOstream;      ///< Output stream type.
#endif /* XML_UNICODE */


// Forward decls
class State;
class DFA;
class ElementDecl;
class AttributeDecl;


typedef std::set<EVString>      SymbolSet;          ///< Element name set.
typedef SymbolSet::iterator     SymbolSetIter;      ///< Element name set iterator.


/// Class to represent a validation error. An application class inherits
/// from the IValidationHandler interface and overrides the
/// HandleValidationError method. This method is passed a ValidationError
/// instance whenever the validator detects an invalid construct.
///
class ValidationError
{
    friend class Validator;     ///< Only a Validator is allowed to construct a
                                ///< validation error object.

public:
    /// Validation error code.
    ///
    enum Code {
        MultipleDocumentElements,       ///< The Document Element occurs more
                                        ///< than once at the top level.
        MultipleTopLevelElements,       ///< There are multiple top level elements.
        InvalidDocumentElement,         ///< The top level element does not match
                                        ///< DOCTYPE declaration.
        UndeclaredElement,              ///< Element not found in DTD.
        UndeclaredAttribute,            ///< Attribute not found in DTD.
        InvalidElement,                 ///< Element not allowed by content model.
        NoCharacterData,                ///< Element does not allow PCDATA.
        UncontainedCharacterData,       ///< Character data with no containing element.
        MismatchedFixedValue,           ///< Value of \#FIXED attribute does not match DTD.
        MissingRequiredAttribute,       ///< Required attribute missing from element.
        BadAttributeValue,              ///< Attribute value not in set of possible values.
        DuplicateId,                    ///< Duplicate ID values within document.
        UndeclaredNotation,             ///< Notation not found in DTD.
        UndeclaredEntity,               ///< Entity not found in DTD.
        IdNotFound,                     ///< Could not find ID corresponding to IDREF.
        InvalidElementPattern           ///< Element pattern violates content model.
    };


    /// Returns the validation error message.
    ///
    /// @return Validation error message string.
    ///
    const XML_Char* GetMessage() const { return m_errorMsg.c_str(); }
    
    /// Returns the validation error code.
    ///
    /// @return Validation error message code.
    ///
    Code            GetCode() const { return m_errorCode; }

    /// Returns the last element parsed before the error occurred.
    ///
    /// @return Name of the element parsed before the error occurred.
    ///
    const XML_Char* GetCurrentElement() const { return m_currentElement.c_str(); }
    
    /// Returns the current element's container element.
    ///
    /// @return Name of the current element's container element.
    ///
    const XML_Char* GetContainingElement() const { return m_containingElement.c_str(); }


    /// Returns the line number on which the validation error occurred.
    ///
    /// @return Line number where the validation error occurred.
    ///
    int             GetLineNumber() const { return m_lineNumber; }
    
    /// Returns the character position on the line which caused the validation error.
    ///
    /// @return Character position on the line that caused the validation error.
    ///
    int             GetCharacterPosition() const { return m_characterPosition; }
    
    /// Returns the total byte offset into the file to the point where the
    /// validation error occurred.
    ///
    /// @return Number of bytes into the file to the point where the validation
    ///         error occurred.
    ///
    long            GetByteOffset() const { return m_byteOffset; }

protected:
    /// Constructs a validation error object.
    ///
    /// @param parser               [in] Parser object.
    /// @param errorCode            [in] Validation error code.
    /// @param currentElement       [in] Name of the last element parsed.
    /// @param containingElement    [in] Current element's containing element name.
    ///
    ValidationError(XML_Parser parser, Code errorCode,
                    const XML_Char* currentElement,
                    const XML_Char* containingElement);
    
    /// Destroys a validation error object.
    ///
    virtual ~ValidationError() { };

    Code        m_errorCode;            ///< Validation error code.
    EVString    m_errorMsg;             ///< Validation error message.
    EVString    m_currentElement;       ///< Name of the last element parsed.
    EVString    m_containingElement;    ///< Name of the element's containing element.
    int         m_lineNumber;           ///< Line number where the validation error occurred.
    int         m_characterPosition;    ///< Character position on line where the validation error occurred.
    long        m_byteOffset;           ///< Byte offset to point where the validation error occurred.
};


/// An application class mixin inherits from the IValidationHandler interface
/// and overrides the HandleValidationError method. This method is passed
/// a ValidationError instance whenever the validator detects an invalid
/// construct. The implementor of this interface passes its 'this' pointer
/// to the Validator class when it is constructed.
///
class IValidationHandler
{
public:
    IValidationHandler() { }
    virtual ~IValidationHandler() { }

    /// Called when a validation error occurs. The derived class must
    /// implement this method and handle the error according to its
    /// own policy.
    ///
    /// @param error    [in] Validation error object.
    ///
    virtual void HandleValidationError(const ValidationError& error) = 0;
};


/// The star of the show, this call performs XML validation for the expat
/// parser. An application instantiates a Validator object and ties it
/// into the expat parser via the parser's handler functions.
///
/// The validator constructs a DFA for each element's content model using
/// the venerable algorithm 3.5 from the Dragon book. Optimizations bypass
/// the algorithm and directly construct the DFA for simple content models
/// (e.g. singletons, pure choice, pure sequence).
///
/// The validator is a C++ object with methods that you call from expat
/// handlers. The method parameters are of the same types as those passed
/// to the expat handlers.
///
/// To use the validator instantiate an ev::Validator object then call the
/// following methods from the corresponding expat handlers:
///
/// <table align="center" cellspacing="0" cellpadding="3">
///     <tr><th>Validator Method</th>   <th>Call From expat Handler</th></tr>
///     <tr><td>AddElementDecl</td>     <td>XML_ElementDeclHandler</td></tr>
///     <tr><td>AddAttributeDecl</td>   <td>XML_AttlistDeclHandler</td></tr>
///     <tr><td>AddNotationDecl</td>    <td>XML_NotationDeclHandler</td></tr>
///     <tr><td>AddEntityDecl</td>      <td>XML_EntityDeclHandler</td></tr>
///     <tr><td>SetDocumentElement</td> <td>XML_StartDoctypeDeclHandler</td></tr>
///     <tr><td>StartElement</td>       <td>XML_StartElementHandler</td></tr>
///     <tr><td>EndElement</td>         <td>XML_EndElementHandler</td></tr>
///     <tr><td>CharacterData</td>      <td>XML_CharacterDataHandler</td></tr>
/// </table>
///
/// Encoding uses expat's XML_Char encoding based on whether XML_UNICODE is
/// defined. exval defines the types ev::EVString and ev::EVOStream but these
/// are just convenience wrappers around the appropriate STL string and stream
/// classes.
///
class Validator
{
    friend EVOstream& operator<<(EVOstream& stream, const Validator& validator);

public:
    /// Typically an IValidationHandler is passed to the
    /// constructor so that the application can be informed
    /// of validation errors. If it is not passed to the
    /// constructor, the IValidationHandler can be set
    /// later using the SetValidationHandler method.
    ///
    /// @param handler      [in] Validation error handler object.
    ///
    explicit Validator(IValidationHandler *handler = NULL);
    
    /// Destroys a validator instance.
    ///
    virtual ~Validator();


    /// The validation handler can be set after object
    /// construction using this method.
    ///
    /// @param handler      [in] Validation error handler object.
    ///
    void SetValidationHandler(IValidationHandler *handler) {
        m_handler = handler;
    }

    /// Deletes the entire state of the validator bringing it back to
    /// its newly constructed state. All DFA's and other maps are cleared.
    ///
    void Clear();

    /// Resets the state of the validator so that another document with
    /// the same DTD can be validated. This method must be called after
    /// validation is complete or after an error has occurred if the
    /// validator is to be used again.
    ///
    void Reset();

    /// Call this method from the expat XML_ElementDeclHandler to register
    /// an element declaration with the validator. The method takes the name
    /// of the element and its content model as provided by expat.
    ///
    /// @param elementName      [in] Name of the element being declared.
    /// @param contentModel     [in] Content model for the element as constructed by expat.
    ///
    void AddElementDecl(const XML_Char* elementName, const XML_Content *contentModel);

    /// Call this method from the expat XML_AttlistDeclHandler to register
    /// an attribute declaration. The method takes the name of the element
    /// which contains the attribute, the name of the attribute, its type,
    /// default value, and whether the attribute is required. All these
    /// values are provided to the expat handler function.
    ///
    /// @param elementName      [in] Name of the element containing the attribute.
    /// @param attrName         [in] Name of the attribute being declared.
    /// @param attrType         [in] Type of the attribute.
    /// @param defValue         [in] Default value for the attribute, or NULL if no default.
    /// @param isRequired       [in] Non-zero if the attribute is required.
    ///
    void AddAttributeDecl(const XML_Char* elementName, const XML_Char* attrName,
                        const XML_Char* attrType, const XML_Char* defValue,
                        int isRequired);

    /// Call this method from the expat XML_NotationDeclHandler to register
    /// a notation declaration. The method takes the name of the notation as
    /// provided to the expat handler.
    ///
    /// @param notationName     [in] Name for the notation.
    ///
    void AddNotationDecl(const XML_Char* notationName) {
        m_notations.insert(notationName);
    }

    /// Call this method from the expat XML_EntityDeclHandler to register
    /// an entity declaration. The method takes the name of the entity as
    /// provided to the expat handler.
    ///
    /// @param entityName       [in] Name for the entity.
    ///
    void AddEntityDecl(const XML_Char* entityName) {
        m_entities.insert(entityName);
    }

    /// An XML file must have a single top level element, known as the
    /// document element. Call SetDocumentElement from the expat
    /// XML_StartDoctypeDeclHandler to tell the validator which element
    /// is the document element.
    ///
    /// @param elementName      [in] Name of the document element.
    ///
    void        SetDocumentElement(const XML_Char* elementName);
    
    /// Returns the document element name.
    ///
    /// @return Name of the document element.
    ///
    EVString    GetDocumentElement() const { return m_documentElement; }

    /// Returns the element currently being validated.
    ///
    /// @return Name of the element currently being validated.
    ///
    const XML_Char*     GetCurrentElement() const;

    /// Returns the set of elements that are allowable at the current
    /// position in the document.
    ///
    /// @return A set of element names.
    ///
    SymbolSet           GetAllowableElements() const;

    /// Returns the set of elements that are allowable for an ANY element.
    ///
    /// @return A set of element names.
    ///
    const SymbolSet&    GetAnyElements() const { return m_anyElements; }

    /// Call this method from the expat XML_StartElementHandler to
    /// test whether the element is valid according to the content
    /// model of the containing element. In addition, the method
    /// validates the element's attributes. The method takes the
    /// expat parser instance, the name of the element and its list
    /// of attributes.
    ///
    /// @param parser       [in] XML parser.
    /// @param elementName  [in] Name of the element.
    /// @param attributes   [in] Array of attribute name/value pairs
    ///                     for the element.
    ///
    /// @return <b>true</b> if element is valid in the current parsing
    ///         context, <b>false</b> otherwise and the HandleValidationError
    ///         method will be called on the IValidationHandler interface.
    ///
    bool StartElement(XML_Parser parser, const XML_Char* elementName,
                    const XML_Char** attributes);

    /// Call this method from the expat XML_EndElementHandler to
    /// test whether the ending of the element is valid according
    /// to the content model of the containing element. The method
    /// takes the expat parser instance.
    ///
    /// @param parser       [in] XML parser.
    ///
    /// @return Can return <b>false</b> on close of document element
    ///         if, for example, there are unreconciled IDREF attributes.
    ///
    bool EndElement(XML_Parser parser);

    /// Call this method from the XML_CharacterDataHandler to test
    /// whether character data is allowed by the containing element's
    /// content model. The method takes the expat parser instance and
    /// the character data. The method compensates for whitespace that
    /// is element content (i.e. newlines and purely indenting
    /// whitespace). buf does not have to be NULL terminated. len is
    /// the number of bytes in buf.
    ///
    /// @param parser   [in] XML parser.
    /// @param buf      [in] Character data (might not be NULL terminated).
    /// @param len      [in] Number of bytes in buf.
    ///
    /// @return <b>true</b> if content is valid in the current parsing
    ///         context, <b>false</b> otherwise and the HandleValidationError
    ///         method will be called on the IValidationHandler interface.
    ///
    bool CharacterData(XML_Parser parser, const XML_Char* buf, int len);

    /// Indicates whether the specified element allows PCDATA.
    ///
    /// @param elementName  [in] Name of the element to test.
    ///
    /// @return <b>true</b> if the element allows PCDATA.
    ///
    bool IsMixed(const XML_Char* elementName) const {
        // If we cannot find the element (e.g. no DTD), just assume
        // it takes PCDATA
        //
        MixedIter iter = m_mixedElements.find(elementName);
        return ((iter == m_mixedElements.end()) ? true : (*iter).second);
    }

protected:
    typedef std::map<EVString, ElementDecl*>    ElementDeclMap;     ///< Maps an element name to its declaration.
    typedef ElementDeclMap::iterator            ElementDeclIter;    ///< Iterator over the element declaration map.
    typedef ElementDeclMap::const_iterator      ElementDeclIter_c;  ///< Constant iterator over the element declaration map.
    typedef std::stack<const ElementDecl*>      ElementStack;       ///< Open element stack.
    typedef std::map<EVString, bool>            MixedMap;           ///< Indicates whether the specified element name accepts PCDATA.
    typedef MixedMap::const_iterator            MixedIter;          ///< Iterates over the element PCDATA map.
    typedef std::stack<const State*>            StateStack;         ///< Validation DFA state stack.
    typedef std::set<EVString>                  IDSet;              ///< Element ID set.
    typedef IDSet::const_iterator               IDIter_c;           ///< Iterator over the element ID set.
    typedef std::set<EVString>                  IDRefSet;           ///< IDREF set.
    typedef IDRefSet::const_iterator            IDRefIter_c;        ///< Iterator over the IDREF set.
    typedef std::set<EVString>                  NotationSet;        ///< Set of notations.
    typedef NotationSet::const_iterator         NotationIter_c;     ///< Iterator over the set of notations.
    typedef std::set<EVString>                  EntitySet;          ///< Set of entities.
    typedef EntitySet::const_iterator           EntityIter_c;       ///< Iterator over the set of entities.
    typedef std::map<EVString, DFA*>            DFAMap;             ///< Maps the name of an element to its validation DFA.
    typedef DFAMap::iterator                    DFAIter;            ///< Iterator over the DFA map.
    typedef DFAMap::const_iterator              DFAIter_c;          ///< Constant iterator over the DFA map.


    /// Purposely undefined.
    ///
    Validator(const Validator& validator);

    /// Purposely undefined.
    ///
    Validator& operator=(const Validator& validator);


    /// Indicates whether the specified element is the document element.
    ///
    /// @param elementName      [in] Name of element to test.
    ///
    /// @return <b>true</b> if the specified element is the document element.
    ///
    bool    IsDocumentElement(const XML_Char* elementName) const {
        if (m_documentElement.empty())
            return true;
        return (m_documentElement == elementName);
    }

    /// Returns the declaration for the specified element.
    ///
    /// @param elementName  [in] Name of element whose declaration is desired.
    ///
    /// @return Declaration object for the specified element or NULL if the
    ///         declaration could not be found.
    ///
    const ElementDecl* GetElementDecl(const EVString& elementName) const {
        ElementDeclIter_c iter = m_elementDecls.find(elementName);
        return (iter == m_elementDecls.end()) ? NULL : (*iter).second;
    }

    /// Indicates whether the specified set of attributes contains the
    /// specified attribute.
    ///
    /// @param attributes       [in] Set of attribute name/value pairs.
    /// @param attrName         [in] Name of the attribute to look up.
    ///
    /// @return <b>true</b> if the attribute is found in the specified
    ///         array of attributes.
    ///
    bool    HaveAttribute(const XML_Char** attributes, const EVString& attrName) const;


    /// Removes all state objects from the DFA state stack.
    ///
    void            ClearStateStack();
    
    /// Tests whether the DFA state stack is empty.
    ///
    /// @return <b>true</b> if the DFA state stack is empty.
    ///
    bool            IsStateStackEmpty() const       { return m_dfaStateStack.empty(); }

    /// Pushes the specified DFA state onto the state stack.
    ///
    /// @param state    [in] DFA state object.
    ///
    void            PushState(const State *state)   { m_dfaStateStack.push(state); }
    
    /// Pops the top DFA state object off the DFA state stack.
    ///
    void            PopState()                      { m_dfaStateStack.pop(); }
    
    /// Returns the state object at the top of the DFA state stack. The
    /// object is not removed from the stack.
    ///
    /// @return DFA state object or NULL if the stack is empty.
    ///
    const State*    TopState() const {
        return m_dfaStateStack.empty() ? NULL: m_dfaStateStack.top();
    }

    /// Replaces the state object on the top of the DFA state stack
    /// with the specified DFA state.
    ///
    /// @param state    [in] DFA state object.
    ///
    void            SwapTopState(const State *state)    { PopState(); PushState(state); }


    /// Removes all elements from the open element stack.
    ///
    void            ClearElementStack();
    
    /// Tests whether the open element stack is empty.
    ///
    /// @return <b>true</b> if the element stack is empty.
    ///
    bool            IsElementStackEmpty() const             { return m_elementStack.empty(); }
    
    /// Pushes the specified element declaration onto the open element stack.
    /// The stack uses the element declaration so that the appropriate content
    /// model DFA can be used to validate the element content.
    ///
    /// @param state    [in] element declaration.
    ///
    void            PushElement(const ElementDecl *state)   { m_elementStack.push(state); }
    
    /// Pops the top element declaration off the top of the open element stack.
    ///
    void            PopElement()                            { m_elementStack.pop(); }
    
    /// Returns the element declaration at the top of the open element stack.
    /// The declaration is not removed from the stack.
    ///
    /// @return Element declaration or NULL if the stack is empty.
    ///
    const ElementDecl*  TopElement() const {
        return m_elementStack.empty() ? NULL: m_elementStack.top();
    }


    /// Tests whether the specified string consists only of XML
    /// whitespace characters.
    ///
    /// @param str      [in] String buffer not NULL terminated.
    /// @param len      [in] Number of bytes in string buffer.
    ///
    /// @return <b>true</b> if string consists solely of whitespace characters.
    ///
    bool    IsWhitespace(const XML_Char* str, int len) const;

    /// Sends the specified error message to the validation handler.
    ///
    /// @param parser               [in] XML parser.
    /// @param errorCode            [in] Error code defined by ValidationError.
    /// @param currentElement       [in] Name of the element being processed.
    /// @param containingElement    [in] Name of the element containing the
    ///                             current element.
    ///
    void    SendError(XML_Parser parser, ValidationError::Code errorCode,
                      const XML_Char* currentElement = NULL,
                      const XML_Char* containingElement = NULL);

    IValidationHandler  *m_handler;             ///< Validation error handler object.
    EVString            m_documentElement;      ///< XML document element.
    DFAMap              m_dfas;                 ///< DFAs for all elements.
    ElementDeclMap      m_elementDecls;         ///< Element declarations.
    MixedMap            m_mixedElements;        ///< Elements that can contain PCDATA.
    SymbolSet           m_anyElements;          ///< Set of elements allowable in ANY.
    ElementStack        m_elementStack;         ///< Open element stack.
    StateStack          m_dfaStateStack;        ///< DFA stack stack.
    IDSet               m_ids;                  ///< ID set.
    IDRefSet            m_idRefs;               ///< IDREF set.
    NotationSet         m_notations;            ///< Set of notations.
    EntitySet           m_entities;             ///< Set of entities.
    bool                m_foundDocumentElement; ///< Indicates if document element found.
    bool                m_errorShutdown;        ///< Indicates if validation error should stop due to errors.
};


/// Output stream operator for a validator. Used to dump the
/// state of the validator to the specified output stream for
/// debugging purposes.
///
/// @param stream       [in] Output stream.
/// @param validator    [in] Validator object to output.
///
/// @return Output stream.
///
EVOstream& operator<<(EVOstream& stream, const Validator& validator);


} /* namespace ev */

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

#pragma warning(disable: 4702)
#include "exval.h"
#include "MeaAssert.h"
#pragma warning(disable: 4511 4512)
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#pragma warning(default: 4511 4512 4702)


namespace ev {

// Encoding
#ifdef XML_UNICODE
/// String comparison
#define _evcscmp    wcscmp

/// Counted string comparison
#define _evcsncmp   wcsncmp

typedef boost::char_separator<wchar_t>  EVSeparator;    ///< Separator character for tokenizing.
typedef boost::wformat                  EVFormat;       ///< Printf-like output format string.
#else
/// String comparison
#define _evcscmp    strcmp

/// Counted string comparison
#define _evcsncmp   strncmp

typedef boost::char_separator<char>     EVSeparator;    ///< Separator character for tokenizing.
typedef boost::format                   EVFormat;       ///< Printf-like output format string.
#endif /* XML_UNICODE */

typedef boost::tokenizer<EVSeparator>   EVTokenizer;    ///< Tokenizer for parsing entities, enumeration, etc.

/// String type.
#define EVStr   boost::io::str


//*************************************************************************
// Internal class declarations
//*************************************************************************


/// Represents a node in an XML element content model. The content model is
/// represented as a tree of ContentModel objects.
///
class ContentModel
{
    friend EVOstream& operator<<(EVOstream& stream, const ContentModel& contentModel);

public:
    typedef std::list<ContentModel*>    Models;     ///< List of content models.
    typedef Models::const_iterator      Iter_c;     ///< Constant iterator over a list of content models.
    typedef Models::iterator            Iter;       ///< Iterator over a list of content models.


    /// Categorization of the content model. The category is
    /// used to optimize the DFA creation process for the
    /// content model.
    ///
    enum Category {
        EmptyCat,           ///< Element cannot contain any elements or data.
        AnyCat,             ///< The element can contain any element.
        MixedCat,           ///< The element can contain PCDATA.
        PureChoiceCat,      ///< The content model consists solely of OR'd elements.
        PureSeqCat,         ///< The content model consists solely of a sequence of elements.
        ComplexCat          ///< The content model is none of the above.
    };

    /// Identifies a content model node.
    ///
    enum Type {
        EmptyType,          ///< Element cannot contain any elements or data.
        AnyType,            ///< Element can contain any element.
        MixedType,          ///< Element can contain PCDATA.
        NameType,           ///< Content model node is an element name.
        ChoiceType,         ///< Content model is an OR'd choice node.
        SeqType             ///< Content model is a sequence node.
    };

    /// Specifies the cardinality of a content model node.
    ///
    enum Quantity {
        NoneQuant,          ///< No quantity specified.
        OptQuant,           ///< Zero or one.
        RepQuant,           ///< Zero or more.
        PlusQuant           ///< One or more.
    };


    /// Constructs an exval content model based on the specified
    /// expat content model.
    ///
    /// @param model        [in] Element content model constructed by the expat parser.
    /// @param root         [in] <b>true</b> if the root node of the content model
    ///                     tree is being constructed.
    ///
    explicit ContentModel(const XML_Content *model, bool root = true);
    
    /// Destroys a content model instance.
    ///
    virtual ~ContentModel();


    /// Returns the content model category. The category is used
    /// in the DFA construction process to optimize the DFA.
    ///
    /// @return Content model category.
    ///
    Category        GetCategory() const { return m_category; }
    
    /// Returns the content model signature. The signature is string
    /// that uniquely identifies a content model. Two identical content
    /// models will have the same signature. The signature is used
    /// to quickly determine whether two content models are identical
    /// so that DFAs are not constructed needlessly.
    ///
    /// @return The string signature for the content model.
    ///
    EVString        GetSignature() const { return m_signature; }

    /// Returns the type of a content model node.
    ///
    /// @return Type of the content model node.
    ///
    Type            GetType() const { return m_type; }

    /// Returns the cardinality of the content model node.
    ///
    /// @return Content model node cardinality.
    ///
    Quantity        GetQuantity() const { return m_quantity; }

    /// Returns the element name in this content model node.
    ///
    /// @return Element name in content model node.
    ///
    EVString        GetName() const { return m_name; }


    /// Indicates whether this content model node has child content model nodes.
    ///
    /// @return <b>true</b> if the content model node has child node.
    ///
    bool            HasChildren() const { return !m_children.empty(); }

    /// Returns the number of child content model nodes.
    ///
    /// @return Number of child nodes.
    ///
    int             GetNumChildren() const { return static_cast<int>(m_children.size()); }
    
    /// Returns an iterator over the list of child nodes.
    ///
    /// @return Constant iterator over the list of child nodes.
    ///
    Iter_c          GetChildIter() const { return m_children.begin(); }

    /// Returns an iterator over the list of child nodes.
    ///
    /// @return Iterator over the list of child nodes.
    ///
    Iter            GetChildIter() { return m_children.begin(); }

    /// Indicates whether the specified iterator has hit the end of the
    /// child node list.
    ///
    /// @param iter     [in] Constant iterator over the list of child nodes.
    ///
    /// @return <b>true</b> if the end of the child list has been reached.
    ///
    bool            AtEnd(const Iter_c& iter) const { return iter == m_children.end(); }

    /// Indicates whether the specified iterator has hit the end of the
    /// child node list.
    ///
    /// @param iter     [in] Iterator over the list of child nodes.
    ///
    /// @return <b>true</b> if the end of the child list has been reached.
    ///
    bool            AtEnd(const Iter& iter) { return iter == m_children.end(); }
    
    /// Returns the child content model node corresponding to the specified iterator.
    ///
    /// @param iter     [in] Constant iterator over the list of child nodes.
    ///
    /// @return Content model node.
    ///
    ContentModel&   GetChild(const Iter_c& iter) const { return *(*iter); }
    
    /// Returns the child content model node corresponding to the specified iterator.
    ///
    /// @param iter     [in] Iterator over the list of child nodes.
    ///
    /// @return Content model node.
    ///
    ContentModel&   GetChild(const Iter& iter) const { return *(*iter); }

    /// Indicates whether the content model node type is EMPTY.
    /// @return <b>true</b> if the type is EMPTY.
    bool            IsEmpty() const     { return m_type == EmptyType; }

    /// Indicates whether the content model node type is ANY.
    /// @return <b>true</b> if the type is ANY.
    bool            IsAny() const       { return m_type == AnyType; }

    /// Indicates whether the content model node type is mixed.
    /// @return <b>true</b> if the type is mixed.
    bool            IsMixed() const     { return m_type == MixedType; }

    /// Indicates whether the content model node contains an element name.
    /// @return <b>true</b> if the node contains an element name.
    bool            IsName() const      { return m_type == NameType; }

    /// Indicates whether the content model node is an OR'd choice.
    /// @return <b>true</b> if the node is an OR'd choice.
    bool            IsChoice() const    { return m_type == ChoiceType; }

    /// Indicates whether the content model node is a sequence.
    /// @return <b>true</b> if the node is a sequence.
    bool            IsSeq() const       { return m_type == SeqType; }


    /// Indicates whether the content model node has no cardinality associated with it.
    /// @return <b>true</b> if the node has no cardinality.
    bool            IsNone() const      { return m_quantity == NoneQuant; }

    /// Indicates whether the content model node has a cardinality of zero or one.
    /// @return <b>true</b> if the node has a cardinality of zero or one.
    bool            IsOpt() const       { return m_quantity == OptQuant; }

    /// Indicates whether the content model node has a cardinality of zero or more.
    /// @return <b>true</b> if the node has a cardinality of zero or more.
    bool            IsRep() const       { return m_quantity == RepQuant; }

    /// Indicates whether the content model node has a cardinality of one or more.
    /// @return <b>true</b> if the node has a cardinality of one or more.
    bool            IsPlus() const      { return m_quantity == PlusQuant; }

protected:
    /// Purposely undefined.
    ContentModel(const ContentModel& model);

    /// Purposely undefined.
    ContentModel& operator=(const ContentModel& model);

    /// Attempts to optimize the content model by removing
    /// extraneous levels of parenthesis, etc.
    ///
    void        Optimize();

    /// Attempts to remove unnecessary parenthesis levels.
    ///
    /// @param model    [in] Content model to examine.
    ///
    void        Collapse(ContentModel* model);

    /// Constructs the signature string for the content model.
    /// The signature string uniquely identifies the content
    /// model. Identical content models will have identical
    /// signature strings.
    ///
    /// @return Signature string representing the content model.
    ///
    EVString    MakeSignature() const;

    /// Examines the content model to determine whether a DFA
    /// can be constructed directly or whether the DFA construction
    /// algorithm must be run. For example, the DFA for the content
    /// model (a|b|c) can be constructed directly, whereas a more
    /// complex DFA is required for the content model ((a|b)*,a,b,b,c*).
    ///
    /// @return Content model category.
    ///
    Category    Categorize() const;
    
    /// Determines whether the children of the this content model
    /// node are simple name name nodes with no cardinality.
    ///
    /// @return <b>true</b> if the children are simple name nodes
    ///         with no cardinality.
    ///
    bool        HasPureChildren() const {
        for (Iter_c iter = GetChildIter(); !AtEnd(iter); ++iter) {
            const ContentModel& child = GetChild(iter);
            if (child.m_type != NameType || child.m_quantity != NoneQuant) {
                return false;
            }
        }
        return true;
    }

    bool        m_root;         ///< Indicates whether this node is the root of the content model tree.
    Category    m_category;     ///< Category for this content model node.
    EVString    m_signature;    ///< Signature string for the content model.
    Type        m_type;         ///< Type of this content model node.
    Quantity    m_quantity;     ///< Cardinality of this content model node.
    EVString    m_name;         ///< Element name if this is a name type node.
    Models      m_children;     ///< List of child content model nodes.
};


/// Represents a node in the parse tree that is constructed by parsing
/// the content model. Creating a parse tree is the first step in creating
/// a DFA for an element's content model. The parse tree is constructed
/// according to the technique presented in algorithm 3.5 from the Dragon
/// book.
///
class ParseNode
{
    friend EVOstream& operator<<(EVOstream& stream, const ParseNode *node);

private:
    /// Used by STL to compare two ParseNode objects.
    ///
    struct LessOp {
        /// @returns <b>true</b> if node n1's ID is less than node n2's ID.
        ///
        bool operator() (const ParseNode* n1, const ParseNode* n2) const {
            return n1->GetId() < n2->GetId();
        }
    };

public:
    /// Type of the parse node.
    ///
    enum Type {
        End,            ///< Terminal node.
        Epsilon,        ///< The any input node.
        Name,           ///< An element name node.
        Choice,         ///< An OR'd choice node.
        Seq,            ///< A sequence node.
        Rep,            ///< A zero or more node.
        Opt,            ///< A zero or one node.
        Plus            ///< A one or mode node.
    };

    typedef std::set<ParseNode*, LessOp>    NodeSet;    ///< A set of parse nodes.
    typedef NodeSet::iterator               NodeIter;   ///< Iterator over a set of parse nodes.
    typedef NodeSet::const_iterator         NodeIter_c; ///< Constant iterator over a set of parse nodes.


    /// Constructs a parse node of the specified type.
    ///
    /// @param type     [in] Parse node type.
    ///
    explicit ParseNode(Type type);
    
    /// Constructs a parse node of the specified type and
    /// with the specified left child node.
    ///
    /// @param type     [in] Parse node type.
    /// @param left     [in] Left child node.
    ///
    ParseNode(Type type, ParseNode *left);
    
    /// Destroys a parse node instance.
    ///
    virtual ~ParseNode();

    /// Creates a copy of this node.
    ///
    /// @return Copy of this node.
    ///
    ParseNode*  CloneType() const {
        return new ParseNode(m_type);
    }


    /// Returns the type of the parse node.
    ///
    /// @return Parse node type.
    ///
    Type    GetType() const { return m_type; }

    /// Indicates whether the node is a terminal node.
    /// @return <b>true</b> if node is a terminal node.
    bool    IsEnd() const { return m_type == End; }

    /// Indicates whether the node is an any input node.
    /// @return <b>true</b> if the node is an any input node.
    bool    IsEpsilon() const { return m_type == Epsilon; }

    /// Indicates whether the node contains an element name.
    /// @return <b>true</b> if the node contains an element name.
    bool    IsName() const { return m_type == Name; }

    /// Indicates whether the node is an OR'd choice node.
    /// @return <b>true</b> if the node is an OR'd choice node.
    bool    IsChoice() const { return m_type == Choice; }

    /// Indicates whether the node is a sequence node.
    /// @return <b>true</b> if the node is a sequence node.
    bool    IsSeq() const { return m_type == Seq; }

    /// Indicates whether the cardinality of the node is zero of more.
    /// @return <b>true</b> if the cardinality of the node is zero or more.
    bool    IsRep() const { return m_type == Rep; }

    /// Indicates whether the cardinality of the node is zero or one.
    /// @return <b>true</b> if the cardinality of the node is zero or one.
    bool    IsOpt() const { return m_type == Opt; }

    /// Indicates whether the cardinality of the node is one or more.
    /// @return <b>true</b> if the cardinality of the node is one or more.
    bool    IsPlus() const { return m_type == Plus; }


    /// Indicates whether this node has a child on its left branch.
    ///
    /// @return <b>true</b> if a child node exists on the left branch.
    ///
    bool    HaveLeft() const { return m_left != NULL; }

    /// Indicates whether this node has a child on its right branch.
    ///
    /// @return <b>true</b> if a child node exists on the right branch.
    ///
    bool    HaveRight() const { return m_right != NULL; }

    /// Sets the specified parse node as the left side child of this node.
    ///
    void    SetLeft(ParseNode *node) { m_left = node; }

    /// Sets the specified parse node as the right side child of this node.
    ///
    void    SetRight(ParseNode *node) { m_right = node; }

    /// Returns the left branch child node of this node.
    ///
    /// @return Child node on the left branch or NULL if there is no left child.
    ///
    const ParseNode*    GetLeft() const { return m_left; }

    /// Returns the right branch child node of this node.
    ///
    /// @return Child node on the right branch or NULL if there is no right child.
    ///
    const ParseNode*    GetRight() const { return m_right; }

    /// Returns the right branch child node of this node.
    ///
    /// @return Child node on the right branch or NULL if there is no right child.
    ///
    ParseNode*          GetRight() { return m_right; }

    
    /// Sets the input symbol for this parse node.
    ///
    /// @param symbol       [in] Input symbol for the parse node.
    ///
    void        SetSymbol(const XML_Char* symbol) { m_symbol = symbol; }
    
    /// Sets the input symbol for this parse node.
    ///
    /// @param symbol       [in] Input symbol for the parse node.
    ///
    void        SetSymbol(const EVString& symbol) { m_symbol = symbol; }

    /// Returns the input symbol for this parse node.
    ///
    /// @return Input symbol for this parse node.
    ///
    EVString    GetSymbol() const { return m_symbol; }

    /// Fills the specified set with the input symbols for the parse tree.
    ///
    /// @param symbols      [out] Set to be filled with the input symbols.
    ///
    void        GetSymbolSet(SymbolSet& symbols) const;


    /// Pre-constructs the followpos function as described in section 3.9 of
    /// the Dragon book.
    ///
    void    BuildFollowpos();


    /// Resets the ID assigned to the parse nodes.
    ///
    static void ResetId() { m_currentId = 0; }
    
    /// Returns the ID of this parse node.
    ///
    /// @return ID of this parse node.
    ///
    int     GetId() const { return m_id; }


    /// Performs a set union on the two specified sets of nodes.
    /// \f[
    ///     set_1 = set_1 \cup set_2
    /// \f]
    ///
    /// @param set1     [in, out] First set of the union.
    /// @param set2     [in] Second set of the union.
    ///
    static void Union(NodeSet& set1, const NodeSet& set2) {
        for (NodeIter_c iter = set2.begin(); iter != set2.end(); ++iter) {
            set1.insert(*iter);
        }
    }

    /// Tests whether the two specified parse nodes are equal.
    ///
    /// @param n1       [in] First parse node
    /// @param n2       [in] Second parse node
    ///
    /// @return <b>true</b> if n1's ID == n2's ID.
    ///
    static bool EqualOp(const ParseNode *n1, const ParseNode *n2) {
        return n1->GetId() == n2->GetId();
    }

    
    /// Returns the nodes that comprise the firstpos() set as
    /// defined in section 3.9 of the Dragon book.
    ///
    /// @return Set of nodes comprising the firstpos() set.
    ///
    const NodeSet&  GetFirstpos() const { return m_firstPos; }
    
    /// Returns the nodes that comprise the followpos() set as
    /// defined in section 3.9 of the Dragon book.
    ///
    /// @return Set of nodes comprising the followpos() set.
    ///
    const NodeSet&  GetFollowpos() const { return m_followPos; }

protected:
    /// Purposely undefined.
    ParseNode(const ParseNode& node);

    /// Purposely undefined.
    ParseNode& operator=(const ParseNode& node);

    
    /// Implements the nullable() function as defined in section 3.9
    /// of the Dragon book.
    ///
    void    Nullable();
    
    /// Implements the firstpos() function as defined in section 3.9
    /// of the Dragon book.
    ///
    void    Firstpos();
    
    /// Implements the lastpos() function as defined in section 3.9
    /// of the Dragon book.
    ///
    void    Lastpos();
    
    /// Implements the followpos() function as defined in section 3.9
    /// of the Dragon book.
    ///
    void    Followpos();


    static int  m_currentId;        ///< ID to assign to the parse node created next.

    int         m_id;               ///< ID for this parse node.
    Type        m_type;             ///< Parse node's type.
    ParseNode   *m_left;            ///< Parse node's left child node.
    ParseNode   *m_right;           ///< Parse node's right child node.
    EVString    m_symbol;           ///< Input symbol.
    bool        m_nullable;         ///< <b>true</b> if the node is nullable.
    NodeSet     m_firstPos;         ///< Set of nodes that satisfy firstpos() for this node.
    NodeSet     m_lastPos;          ///< Set of nodes that satisfy lastpos() for this node.
    NodeSet     m_followPos;        ///< Set of nodes that satisfy followpos() for this node.
#ifdef EV_TIMING
    clock_t     m_firstPosTime;     ///< Time to compute firstpos().
    clock_t     m_lastPosTime;      ///< Time to compute lastpos().
    clock_t     m_nullableTime;     ///< Time to compute nullable().
    clock_t     m_followPosTime;    ///< Time to compute followpos().
#endif /* EV_TIMING */
};


/// A deterministic finite state automata (DFA) consists of a set of
/// states. The states are connected by directed arcs that represent
/// input symbols. This class represents a state in a DFA.
///
class State
{
    friend EVOstream& operator<<(EVOstream& stream, const State *state);

public:
    /// Identifies the content type for the state.
    ///
    enum Type {
        Normal,     ///< State represents plain element content.
        Empty,      ///< State represents an empty element.
        Any,        ///< State represents an any content model.
        Mixed       ///< State represents PCDATA.
    };

    typedef std::map<EVString, State*>  TransMap;       ///< Represents a transition to a state based on an input symbol.
    typedef TransMap::iterator          TransIter;      ///< Iterator over the state transition map.
    typedef TransMap::const_iterator    TransIter_c;    ///< Constant iterator over the state transition map.


    /// Constructs a DFA state.
    ///
    /// @param dfa          [in] DFA parent for the state.
    /// @param type         [in] Type of state.
    /// @param positions    [in] Set of parse nodes that are represented by this state
    ///
    State(DFA& dfa, Type type, const ParseNode::NodeSet& positions);

    /// Constructs a DFA state.
    ///
    /// @param dfa          [in] DFA parent for the state.
    /// @param type         [in] Type of state.
    /// @param accepting    [in] Can this state be a final state when matching a pattern.
    ///
    State(DFA& dfa, Type type, bool accepting = false);

    /// Destroys a state instance.
    ///
    virtual ~State();


    /// Based on the specified input symbol (i.e. element name), a transition
    /// arc is selected and the state pointed to by that arc is returned.
    ///
    /// @param elementName      [in] Input symbol that selects the transition arc to the next state.
    ///
    /// @return The next state based on following the transition arc selected by the element name.
    ///
    const State* GetNextState(const EVString& elementName) const;


    /// Indicates if the state represents plain element content.
    /// @return <b>true</b> if the state represents plain element content.
    bool    IsNormal() const    { return m_type == Normal; }

    /// Indicates if the state represents an empty element.
    /// @return <b>true</b> if the state represents an empty element.
    bool    IsEmpty() const     { return m_type == Empty; }

    /// Indicates if the state represents any element.
    /// @return <b>true</b> if the state represents any element.
    bool    IsAny() const       { return m_type == Any; }

    /// Indicates if the state represents PCDATA.
    /// @return <b>true</b> if the state represents PCDATA.
    bool    IsMixed() const     { return m_type == Mixed; }


    /// Indicates whether the state has been marked. States are
    /// marked during the building of a complex DFA to assist the
    /// construction process. Marks are not used after the DFA has
    /// been constructed.
    ///
    /// @return <b>true</b> if the state has been marked.
    ///
    bool    IsMarked() const { return m_marked; }
    
    /// Marks the state. States are marked during the building of a
    /// complex DFA to assist the construction process. Marks are not
    /// used after the DFA has been constructed.
    ///
    void    Mark() { m_marked = true; }


    /// Indicates whether this state can be a final state when matching a
    /// content model.
    ///
    /// @return <b>true</b> indicates that a pattern can end in this state.
    ///
    bool    IsAccepting() const { return m_accepting; }
    
    /// Marks the state as allowing a pattern match to end.
    ///
    /// @param accepting    [in] Sets whether this state allows a pattern match to end.
    ///
    void    SetAccepting(bool accepting = true) { m_accepting = accepting; }


    /// Returns the set of parse positions that correspond to this state.
    ///
    /// @return The set of parse nodes that correspond to this state.
    ///
    const ParseNode::NodeSet&   GetPositions() const { return m_positions; }

    /// Adds a transition arc from this state to the specified next state.
    /// The transition occurs if the specified input symbol (i.e. element)
    /// is encountered.
    ///
    /// @param symbol       [in] Input symbol that will cause the transition.
    /// @param nextState    [in] The state pointed to by the transition arc.
    ///
    void    AddTransition(const EVString& symbol, State *nextState) {
        m_transitions[symbol] = nextState;
    }

    /// The set of symbols from all the transition arc from this state.
    ///
    /// @param symbols      [out] Set of symbols (i.e. element names) comprising
    ///                     the input set on the transition arcs.
    ///
    void    GetSymbolSet(SymbolSet& symbols) const {
        for (TransIter_c iter = m_transitions.begin(); iter != m_transitions.end(); ++iter) {
            symbols.insert((*iter).first);
        }
    }


    /// Resets the ID assigned to the states.
    ///
    static void ResetId() { m_currentId = 0; }

    /// Obtains the ID assigned to the state.
    ///
    /// @return ID assigned to the state.
    ///
    int     GetId() const { return m_id; }


    /// Compares this state with the specified state.
    ///
    /// @param rhs      [in] State to compare against this.
    ///
    /// @return <b>true</b> if the specified state equals this state.
    ///
    bool    operator==(const State& rhs) const;

    /// Compares the parse nodes contained in this state with the
    /// specified set of parse nodes.
    ///
    /// @param positions    [in] Set of parse nodes to compare.
    ///
    /// @return <b>true</b> if the specified parse nodes are equal
    ///         to the parse nodes contained in this state.
    ///
    bool    operator==(const ParseNode::NodeSet& positions) {
        return ((positions.size() != m_positions.size()) ? false :
                std::equal(positions.begin(), positions.end(), m_positions.begin(), ParseNode::EqualOp));
    }

protected:
    /// Purposely undefined.
    State(const State& state);

    /// Purposely undefined.
    State& operator=(const State& state);

    static int  m_currentId;        ///< ID to assign to the parse node created next.


    /// Indicates if this state contains a terminal parse node.
    ///
    /// @return <b>true</b> if this state contains a terminal parse node.
    ///
    bool    ContainsEnd() const {
        for (ParseNode::NodeIter_c posIter = m_positions.begin(); posIter != m_positions.end(); ++posIter) {
            if ((*posIter)->GetType() == ParseNode::End) {
                return true;
            }
        }
        return false;
    }

    DFA&                m_dfa;              ///< Parent DFA for this state.
    int                 m_id;               ///< ID for this state.
    Type                m_type;             ///< Type identifier for this state.
    ParseNode::NodeSet  m_positions;        ///< Parse nodes corresponding to this state.
    bool                m_marked;           ///< Used during DFA construction to indicate this state is marked.
    bool                m_accepting;        ///< Indicates if this state allows a match pattern to terminate.
    TransMap            m_transitions;      ///< Transitions from this state to the next states based on element names.
};


/// This class represents a deterministic finite state automaton (DFA) (i.e. a
/// state machine). An element's content model is represented by a DFA. To
/// validate the content of an element, its DFA is used by transitioning among
/// its states based on the contained elements.
///
class DFA
{
    friend EVOstream& operator<<(EVOstream& stream, const DFA *dfa);

public:
    /// Constructs a DFA.
    ///
    /// @param validator    [in] The parent validator for this DFA.
    /// @param contentModel [in] Content model represented by this DFA.
    ///
    DFA(Validator& validator, const ContentModel& contentModel);
    
    /// Destroys an instance of a DFA.
    ///
    virtual ~DFA();


    /// Returns the parent validator for this DFA.
    ///
    /// @return Parent validator for this DFA.
    ///
    Validator&  GetValidator() { return m_validator; }

    /// Returns the start state of the DFA.
    ///
    /// @return Start state of the DFA.
    ///
    const State* GetStartState() const { return *m_states.begin(); }


    /// Resets the ID assigned to the DFAs.
    ///
    static void ResetId() { m_currentId = 0; }

    /// Obtains the ID assigned to the DFA.
    ///
    /// @return ID assigned to the DFA.
    ///
    int     GetId() const { return m_id; }

protected:
    typedef std::list<State*>           StateList;      ///< List of all states in the DFA.
    typedef StateList::iterator         StateIter;      ///< Iterator over all states in the DFA.
    typedef StateList::const_iterator   StateIter_c;    ///< Constant iterator over all states in the DFA.


    /// Purposely undefined.
    DFA(const DFA& dfa);
    
    /// Purposely undefined.
    DFA& operator=(const DFA& dfa);

    static int  m_currentId;        ///< ID to assign to the parse node created next.


    /// Builds the DFA for an element with an EMPTY content model.
    ///
    void        BuildEmptyDFA();
    
    /// Builds the DFA for an element with an ANY content model.
    ///
    void        BuildAnyDFA();
    
    /// Builds the DFA for an element with a content model that accepts PCDATA.
    ///
    /// @param contentModel     [in] The content model to be represented by the DFA.
    ///
    void        BuildMixedDFA(const ContentModel& contentModel);
    
    /// Builds the DFA for an element with an OR'd content model.
    ///
    /// @param contentModel     [in] The content model to be represented by the DFA.
    ///
    void        BuildChoiceDFA(const ContentModel& contentModel);
    
    /// Builds the DFA for an element with a sequenced content model.
    ///
    /// @param contentModel     [in] The content model to be represented by the DFA.
    ///
    void        BuildSeqDFA(const ContentModel& contentModel);
    
    /// Builds the DFA for an element whose content model is a complex combination of
    /// elements.
    ///
    /// @param parseTree        [in] The parse tree representing the complex content model.
    ///
    void        BuildComplexDFA(const ParseNode *parseTree);


    /// Builds a parse tree from the specified element content model.
    ///
    /// @param contentModel     [in] Content model that is to be parsed.
    /// @param top              [in] Is this the top of the parse tree (for use in recursion).
    ///
    /// @return The root of the parse tree.
    ///
    ParseNode*  BuildParseTree(const ContentModel& contentModel, bool top = true);
    
    /// Returns the first unmarked state in the list of states.
    ///
    /// @return First unmarked state in the list of states. Returns NULL
    ///         if there are no unmarked states.
    ///
    State*      GetUnmarkedState();

    /// Tests whether the list of positions correspond to a state
    /// already on the state list.
    ///
    /// @param positions    [in] List of positions to test against the state list.
    ///
    /// @return Pointer to the state if positions correspond to a state on the
    ///         list, NULL otherwise.
    ///
    State*      FindState(const ParseNode::NodeSet& positions);

    int             m_id;               ///< ID for the DFA.
    ParseNode       *m_parseTree;       ///< Parse tree for a complex DFA.
    StateList       m_states;           ///< States that comprise the DFA.
    Validator&      m_validator;        ///< Parent validator for the DFA.

#ifdef EV_TIMING
    clock_t m_complexTotalTime;         ///< Time to construct a complex DFA, in seconds.
    clock_t m_parseTreeTime;            ///< Time to construct the parse tree, in seconds.
    clock_t m_followPosTime;            ///< Time to construct the followpos function, in seconds.
    clock_t m_buildDfaTime;             ///< Time to build the DFA, in seconds.
#endif /* EV_TIMING */
};


/// Represents an element declaration.
///
class ElementDecl
{
    /// Output stream operator for an element declaration. Used to
    /// dump the state of the element declaration to the specified
    /// output stream for debugging purposes.
    ///
    /// @param stream       [in] Output stream.
    /// @param elementDecl  [in] Element declaration to output.
    ///
    /// @return Output stream.
    ///
    friend EVOstream& operator<<(EVOstream& stream, const ElementDecl *elementDecl);

public:
    typedef std::map<EVString, AttributeDecl*>  AttributeMap;       ///< Maps the name of an attribute to its declaration object.
    typedef AttributeMap::const_iterator        AttributeIter_c;    ///< Constant iterator over the element's attributes.


    /// Constructs an element declaration.
    ///
    /// @param validator    [in] Parent validator for the declaration.
    /// @param elementName  [in] Name of the element being declared.
    ///
    ElementDecl(Validator& validator, const XML_Char* elementName);
    
    /// Destroys an element declaration.
    ///
    virtual ~ElementDecl();


    /// Obtains the start state for the DFA representing this element's
    /// content model.
    ///
    /// @return Start state for this element's DFA.
    ///
    const State*    GetStartState() const;
    

    /// Returns the name of the element.
    ///
    /// @return Element name.
    ///
    EVString    GetName() const { return m_elementName; }


    /// Places the specified DFA on the element.
    ///
    /// @param dfa      [in] DFA that represents the element's content model.
    ///
    void    SetDFA(DFA *dfa) { m_dfa = dfa; }


    /// Places the specified attribute declaration on the element.
    ///
    /// @param attrName     [in] Attribute name.
    /// @param attrType     [in] Attribute type.
    /// @param defValue     [in] Default value for the attribute or NULL if there is no default value.
    /// @param isRequired   [in] Indicates whether the attribute must be specified.
    ///
    void    AddAttributeDecl(const XML_Char* attrName,
                            const XML_Char* attrType,
                            const XML_Char* defValue,
                            bool isRequired);

    /// Returns the declaration for the specified attribute name.
    ///
    /// @param attrName     [in] Name of attribute whose declaration is desired.
    ///
    /// @return Attribute declaration corresponding to the specified attribute name.
    ///
    const AttributeDecl* GetAttributeDecl(const XML_Char* attrName) const {
        AttributeIter_c iter = m_attributes.find(attrName);
        return (iter == m_attributes.end()) ? NULL : (*iter).second;
    }

    /// Returns an iterator over the required attributes.
    ///
    /// @return Constant iterator pointed at the start of the
    ///         list of required attributes.
    ///
    AttributeIter_c StartRequiredAttributes() const {
        return m_requiredAttributes.begin();
    }

    /// Returns an iterator pointed at the end of the required
    /// attribute list.
    ///
    /// @return Constant iterator pointed at the end of the
    ///         list of required attributes.
    ///
    AttributeIter_c EndRequiredAttributes() const {
        return m_requiredAttributes.end();
    }


    /// Returns the parent validator for the element declaration.
    ///
    /// @return Parent validator for the element declaration.
    ///
    const Validator&    GetValidator() const { return m_validator; }

protected:
    typedef AttributeMap::iterator  AttributeIter;  ///< Iterator over the element's attributes.


    /// Purposely undefined.
    ElementDecl(const ElementDecl& decl);

    /// Purposely undefined.
    ElementDecl& operator=(const ElementDecl& decl);

    DFA             *m_dfa;                 ///< DFA representing this element's content model.
    EVString        m_elementName;          ///< Name of the element being declared.
    AttributeMap    m_attributes;           ///< Attributes for the element.
    AttributeMap    m_requiredAttributes;   ///< Required attributes for the element.
    Validator&      m_validator;            ///< Parent validator for the element.
};


/// Represents an attribute declaration.
///
class AttributeDecl
{
    friend EVOstream& operator<<(EVOstream& stream, const AttributeDecl *attributeDecl);

public:
    /// Attribute value type.
    ///
    enum Type {
        Enumerated,     ///< Enumeration.
        CDATA,          ///< Character data.
        ENTITY,         ///< Entity reference.
        ENTITIES,       ///< List of entity references.
        ID,             ///< Unique identifier.
        IDREF,          ///< Reference to a unique identifier.
        IDREFS,         ///< List of references to a unique identifier.
        NMTOKEN,        ///< Name token.
        NMTOKENS,       ///< List of name tokens.
        NOTATION        ///< Notation reference.
    };


    /// Constructs an attribute declaration.
    ///
    /// @param elementDecl  [in] Parent element's declaration.
    /// @param attrName     [in] Name of the attribute being declared.
    /// @param attrType     [in] Type of the attribute's value.
    /// @param defValue     [in] Default value for the attribute or NULL if no default.
    /// @param isRequired   [in] Indicates if the attribute must be specified with the element.
    ///
    AttributeDecl(ElementDecl& elementDecl, const XML_Char* attrName,
                    const XML_Char* attrType, const XML_Char* defValue,
                    bool isRequired);

    /// Destroys an attribute declaration.
    ///
    virtual ~AttributeDecl();


    /// Returns the name of the attribute.
    ///
    /// @return Name of the attribute being declared.
    ///
    EVString    GetName() const { return m_name; }
    
    /// Returns the type of the attribute's value.
    ///
    /// @return Type of the attribute's value.
    ///
    Type        GetType() const { return m_type; }


    /// Tests whether the specified value is one of the enumeration
    /// values for an enumerated attribute.
    ///
    /// @param evalue       [in] Value to test
    ///
    /// @return <b>true</b> if the value is a member of the enumeration set.
    ///
    bool        IsEnumValue(const XML_Char* evalue) const {
        return (m_values.find(evalue) != m_values.end());
    }


    /// Indicates whether the attribute has a default value.
    ///
    /// @return <b>true</b> if the attribute has a default value.
    ///
    bool        HasDefault() const { return !m_defValue.empty(); }
    
    /// Returns the default value for the attribute.
    ///
    /// @return Default value for the attribute or the empty string
    ///         if there is no default value.
    ///
    EVString    GetDefault() const { return m_defValue; }


    /// Indicates if the attribute is required.
    /// @return <b>true</b> if the attribute is required.
    bool IsRequired() const { return (m_required && m_defValue.empty()); }

    /// Indicates if the attribute is required and has only one value.
    /// @return <b>true</b> if the attribute is required and its value is fixed.
    bool IsFixed() const { return (m_required && !m_defValue.empty()); }

protected:
    typedef std::set<EVString>          ValueSet;       ///< If the attribute is enumerated, these are the possible values for the enumeration.
    typedef ValueSet::iterator          ValueIter;      ///< Iterator over the attribute's values
    typedef ValueSet::const_iterator    ValueIter_c;    ///< Constant iterator over the attribute's values.


    /// Purposely undefined.
    AttributeDecl(const AttributeDecl& decl);

    /// Purposely undefined.
    AttributeDecl& operator=(const AttributeDecl& decl);


    /// Parses enumerated attribute values: (val1|val2|val3)
    /// and stores them in an internal set of values.
    ///
    /// @param valueStr     [in] Value string to parse.
    ///
    void ParseValues(const XML_Char* valueStr);

    ElementDecl&    m_elementDecl;      ///< Parent element's declaration.
    EVString        m_name;             ///< Attribute name.
    EVString        m_defValue;         ///< Default value, if any.
    Type            m_type;             ///< Attribute value type.
    ValueSet        m_values;           ///< Set of enumeration values.
    bool            m_required;         ///< Indicates if the attribute is required.
};


/// Output stream operator for a content model. Used to dump the
/// state of the content model to the specified output stream for
/// debugging purposes.
///
/// @param stream       [in] Output stream.
/// @param contentModel [in] Content model to output.
///
/// @return Output stream.
///
EVOstream& operator<<(EVOstream& stream, const ContentModel& contentModel);

/// Output stream operator for a parse node. Used to dump the
/// state of the parse node to the specified output stream for
/// debugging purposes.
///
/// @param stream       [in] Output stream.
/// @param node         [in] Parse node to output.
///
/// @return Output stream.
///
EVOstream& operator<<(EVOstream& stream, const ParseNode *node);

/// Output stream operator for a state. Used to dump the
/// state object to the specified output stream for
/// debugging purposes.
///
/// @param stream       [in] Output stream.
/// @param state        [in] State object to output.
///
/// @return Output stream.
///
EVOstream& operator<<(EVOstream& stream, const State *state);

/// Output stream operator for a DFA. Used to dump the
/// state of the DFA to the specified output stream for
/// debugging purposes.
///
/// @param stream       [in] Output stream.
/// @param dfa          [in] DFA to output.
///
/// @return Output stream.
///
EVOstream& operator<<(EVOstream& stream, const DFA *dfa);

/// Output stream operator for an element declaration. Used to
/// dump the state of the element declaration to the specified
/// output stream for debugging purposes.
///
/// @param stream       [in] Output stream.
/// @param elementDecl  [in] Element declaration to output.
///
/// @return Output stream.
///
EVOstream& operator<<(EVOstream& stream, const ElementDecl *elementDecl);

/// Output stream operator for an attribute declaration. Used to
/// dump the state of the attribute declaration to the specified
/// output stream for debugging purposes.
///
/// @param stream           [in] Output stream.
/// @param attributeDecl    [in] Attribute declaration to output.
///
/// @return Output stream.
///
EVOstream& operator<<(EVOstream& stream, const AttributeDecl *attributeDecl);

} /* namespace ev */


using namespace ev;


//*************************************************************************
// Validator
//*************************************************************************

 
Validator::Validator(IValidationHandler *handler) : m_handler(handler),
    m_foundDocumentElement(false), m_errorShutdown(false)
{
}


Validator::~Validator()
{
    try {
        Clear();
        m_handler = NULL;
    }
    catch(...) {
        MeaAssert(false);
    }
}


void Validator::Clear()
{
    Reset();

    for (ElementDeclIter diter = m_elementDecls.begin(); diter != m_elementDecls.end(); ++diter) {
        delete (*diter).second;
    }
    m_elementDecls.clear();

    for (DFAIter fiter = m_dfas.begin(); fiter != m_dfas.end(); ++fiter) {
        delete (*fiter).second;
    }
    m_dfas.clear();
}


void Validator::Reset()
{
    m_errorShutdown = false;
    m_foundDocumentElement = false;

    while (!IsStateStackEmpty()) {
        PopState();
    }

    while (!IsElementStackEmpty()) {
        PopElement();
    }

    m_ids.clear();
    m_idRefs.clear();
}


SymbolSet Validator::GetAllowableElements() const
{
    SymbolSet symbols;

    const State *currentState = TopState();

    if (currentState == NULL) {
        if (!m_documentElement.empty()) {
            symbols.insert(m_documentElement);
        }
    } else {
        if (currentState->IsAny()) {
            symbols = m_anyElements;
        } else {
            currentState->GetSymbolSet(symbols);
        }
    }

    return symbols;
}


void Validator::AddElementDecl(const XML_Char* elementName,
                               const XML_Content *contentModel)
{
    MeaAssert(elementName != NULL);

    ContentModel model(contentModel);

    // Form the signature of the content model and see if
    // we already have a DFA for it.
    //
    const EVString& signature = model.GetSignature();
    DFAIter_c diter = m_dfas.find(signature);
    DFA *dfa;

    if (diter == m_dfas.end()) {
        dfa = new DFA(*this, model);
        m_dfas[signature] = dfa;
    } else {
        dfa = (*diter).second;
    }

    // If it is a mixed element, add it to the
    // set of mixed elements so that clients can
    // query whether a particular element takes
    // PCDATA.
    //
    m_mixedElements[elementName] = model.IsMixed();

    // Create an element declaration for this element, if it
    // has not already been created by a previous attribute
    // declaration.
    //
    ElementDecl *elementDecl;
    ElementDeclIter iter = m_elementDecls.find(elementName);

    if (iter == m_elementDecls.end()) {
        elementDecl = new ElementDecl(*this, elementName);
        m_elementDecls[elementName] = elementDecl;
    } else {
        elementDecl = (*iter).second;
    }

    // Give the element its content DFA
    //
    elementDecl->SetDFA(dfa);

    // Add the element to the ANY set if it is not the document
    // element.
    //
    if (m_documentElement != elementName) {
        m_anyElements.insert(elementName);
    }
}


void Validator::AddAttributeDecl(const XML_Char* elementName,
                                 const XML_Char* attrName,
                                 const XML_Char* attrType,
                                 const XML_Char* defValue,
                                 int isRequired)
{
    // Create an element declaration for this attribute, if it
    // has not already been created by a previous element
    // declaration.
    //
    ElementDecl *elementDecl;
    ElementDeclIter iter = m_elementDecls.find(elementName);

    if (iter == m_elementDecls.end()) {
        elementDecl = new ElementDecl(*this, elementName);
        m_elementDecls[elementName] = elementDecl;
    } else {
        elementDecl = (*iter).second;
    }
    MeaAssert(elementDecl != NULL);

    // Add the attribute decl to the element decl
    //
    elementDecl->AddAttributeDecl(attrName, attrType, defValue,
                                (isRequired ? true : false));
}


void Validator::SetDocumentElement(const XML_Char* elementName)
{
    m_documentElement = elementName;
}


bool Validator::StartElement(XML_Parser parser, const XML_Char* elementName,
                             const XML_Char** attributes)
{
    if (m_errorShutdown) {
        return false;
    }

    // Get the element declaration that corresponds to this element.
    //
    const ElementDecl *elementDecl = GetElementDecl(elementName);
    if (elementDecl == NULL) {
        SendError(parser, ValidationError::UndeclaredElement, elementName);
        return false;
    }

    // Get the start state of the DFA.
    //
    const State *startState = elementDecl->GetStartState();
    MeaAssert(startState != NULL);

    // If the state stack is empty, this element must be the first
    // occurrence of the document element.
    //
    if (IsStateStackEmpty()) {
        if (m_foundDocumentElement) {
            // We have more than one occurrence of the document element.
            //
            if (IsDocumentElement(elementName)) {
                SendError(parser, ValidationError::MultipleDocumentElements, elementName);
                return false;
            }
            SendError(parser, ValidationError::MultipleTopLevelElements, elementName);
            return false;
        } else {
            // Verify that the document element matches that specified
            // in the DOCTYPE declaration.
            //
            m_foundDocumentElement = true;

            if (!IsDocumentElement(elementName)) {
                SendError(parser, ValidationError::InvalidDocumentElement, elementName);
                return false;
            }
        }
    }
    // Verify that the element is valid in the context of the content
    // model.
    //
    else {
        const State *currentState = TopState();
        MeaAssert(currentState != NULL);
        const State *nextState = currentState->GetNextState(elementName);

        if (nextState == NULL) {
            SendError(parser, ValidationError::InvalidElement, elementName, TopElement()->GetName().c_str());
            return false;
        }
        SwapTopState(nextState);
    }

    // Push the start state of the next DFA on the
    // state stack to validate contained elements.
    //
    PushState(startState);

    // Push the element on the element stack so
    // that we track containment.
    //
    PushElement(elementDecl);

    // Validate the specified attributes
    //
    for (const XML_Char** aptr = attributes; *aptr != NULL; aptr += 2) {
        const XML_Char* aname   = aptr[0];
        const XML_Char* avalue  = aptr[1];

        // Verify that the attribute has been declared.
        //
        const AttributeDecl *attrDecl = elementDecl->GetAttributeDecl(aname);
        if (attrDecl == NULL) {
            SendError(parser, ValidationError::UndeclaredAttribute, aname, elementName);
            return false;
        }

        // If the attribute is #FIXED, verify that the
        // value matches that specified in the DTD.
        //
        if (attrDecl->IsFixed() && (attrDecl->GetDefault() != avalue)) {
            SendError(parser, ValidationError::MismatchedFixedValue, aname, elementName);
            return false;
        }

        switch (attrDecl->GetType()) {
        case AttributeDecl::Enumerated:
            // If the attribute has enumerated values, verify that
            // the value specified is within the enumeration.
            //
            if (!attrDecl->IsEnumValue(avalue)) {
                SendError(parser, ValidationError::BadAttributeValue, aname, elementName);
                return false;
            }
            break;
        case AttributeDecl::CDATA:
            break;
        case AttributeDecl::ENTITY:
            // If this is an ENTITY attribute, verify that the entity has
            // has been declared in the DTD.
            //
            if (m_entities.find(avalue) != m_entities.end()) {
                SendError(parser, ValidationError::UndeclaredEntity, aname, elementName);
                return false;
            }
            break;
        case AttributeDecl::ENTITIES:
            // If this is an ENTITIES attribute, verify that each entity has
            // has been declared in the DTD.
            //
            {
                static EVSeparator delim(EV_T(" \t"));
                EVString vstr(avalue);

                EVTokenizer tokens(vstr, delim);
                for (EVTokenizer::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
                    if (m_entities.find(*iter) != m_entities.end()) {
                        SendError(parser, ValidationError::UndeclaredEntity, aname, elementName);
                        return false;
                    }
                }
            }
            break;
        case AttributeDecl::ID:
            // If this is an ID attribute, verify that this ID is unique.
            //
            if (m_ids.find(avalue) != m_ids.end()) {
                SendError(parser, ValidationError::DuplicateId, aname, elementName);
                return false;
            }
            m_ids.insert(avalue);
            break;
        case AttributeDecl::IDREF:
            // Add the ID reference to the set of references. When the document
            // element is ended, all ID references will be reconciled against
            // the ID values defined in the document.
            //
            m_idRefs.insert(avalue);
            break;
        case AttributeDecl::IDREFS:
            // Add the ID references to the set of references. When the document
            // element is ended, all ID references will be reconciled against
            // the ID values defined in the document.
            //
            {
                static EVSeparator delim(EV_T(" \t"));
                EVString vstr(avalue);

                EVTokenizer tokens(vstr, delim);
                for (EVTokenizer::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
                    m_idRefs.insert(*iter);
                }
            }
            break;
        case AttributeDecl::NMTOKEN:
            break;
        case AttributeDecl::NMTOKENS:
            break;
        case AttributeDecl::NOTATION:
            // If this is a NOTATION attribute, verify that the notation has
            // has been declared in the DTD.
            //
            if (m_notations.find(avalue) != m_notations.end()) {
                SendError(parser, ValidationError::UndeclaredNotation, aname, elementName);
                return false;
            }
            break;
        default:
            MeaAssert(false);
            break;
        }
    }

    // Verify that any required attributes are specified
    //
    for (ElementDecl::AttributeIter_c aiter = elementDecl->StartRequiredAttributes();
                aiter != elementDecl->EndRequiredAttributes(); aiter++) {
        if (!HaveAttribute(attributes, (*aiter).first)) {
            SendError(parser, ValidationError::MissingRequiredAttribute, (*aiter).first.c_str(), elementName);
            return false;
        }
    }

    return true;
}


bool Validator::EndElement(XML_Parser parser)
{
    if (m_errorShutdown) {
        return false;
    }

    // Retrieve the current state and verify that it is an accepting state.
    //
    const State* currentState = TopState();
    MeaAssert(currentState != NULL);
    if (!currentState->IsAccepting()) {
        SendError(parser, ValidationError::InvalidElementPattern, TopElement()->GetName().c_str());
        return false;
    }

    // Retrieve the state of the containing element.
    //
    PopState();

    // Retrieve the containing element.
    //
    MeaAssert(!IsElementStackEmpty());
    PopElement();

    // If the state stack is empty, we have closed the document element.
    // Reconcile any IDREF values against their corresponding ID values.
    //
    if (IsStateStackEmpty()) {
        for (IDRefIter_c iter = m_idRefs.begin(); iter != m_idRefs.end(); ++iter) {
            if (m_ids.find(*iter) == m_ids.end()) {
                SendError(parser, ValidationError::IdNotFound, (*iter).c_str());
                return false;
            }
        }
    }

    return true;
}


bool Validator::CharacterData(XML_Parser parser, const XML_Char* buf, int len)
{
    if (m_errorShutdown) {
        return false;
    }

    const State *state = TopState();
    if (state == NULL) {
        SendError(parser, ValidationError::UncontainedCharacterData);
        return false;
    }
    if (!state->IsMixed() && !IsWhitespace(buf, len) && !state->IsAny()) {
        SendError(parser, ValidationError::NoCharacterData, TopElement()->GetName().c_str());
        return false;
    }

    return true;
}


bool Validator::HaveAttribute(const XML_Char** attributes, const EVString& attrName) const
{
    for (const XML_Char** aptr = attributes; *aptr != NULL; aptr += 2) {
        if (attrName == aptr[0]) {
            return true;
        }
    }
    return false;
}


void Validator::SendError(XML_Parser parser,
                          ValidationError::Code errorCode,
                          const XML_Char* currentElement,
                          const XML_Char* containingElement)
{
    m_errorShutdown = true;
    ValidationError error(parser, errorCode, currentElement, containingElement);
    if (m_handler != NULL) {
        m_handler->HandleValidationError(error);
    }
}


const XML_Char* Validator::GetCurrentElement() const
{
    return ((TopState() == NULL) ? EV_T("") : TopElement()->GetName().c_str());
}


bool Validator::IsWhitespace(const XML_Char* str, int len) const
{
    MeaAssert(str != NULL);

    for (; len > 0; len--, str++) {
        if ((*str != EV_T('\x20')) &&
                (*str != EV_T('\x09')) &&
                (*str != EV_T('\x0D')) &&
                (*str != EV_T('\x0A'))) {
            return false;
        }
    }

    return true;
}


//*************************************************************************
// ValidationError
//*************************************************************************


ValidationError::ValidationError(XML_Parser parser, Code errorCode,
                                 const XML_Char* currentElement,
                                 const XML_Char* containingElement) :
    m_errorCode(errorCode),
    m_currentElement((currentElement != NULL) ? currentElement : EV_T("")),
    m_containingElement((containingElement != NULL) ? containingElement : EV_T("")),
    m_lineNumber(XML_GetCurrentLineNumber(parser)),
    m_characterPosition(XML_GetCurrentColumnNumber(parser)),
    m_byteOffset(XML_GetCurrentByteIndex(parser))
{
    switch (m_errorCode) {
    case MultipleDocumentElements:
        m_errorMsg = EVStr(EVFormat(EV_T("Document element '%s' occurs more than once at the top level")) % currentElement);
        break;
    case MultipleTopLevelElements:
        m_errorMsg = EVStr(EVFormat(EV_T("Multiple top level element '%s'")) % currentElement);
        break;
    case InvalidDocumentElement:
        m_errorMsg = EVStr(EVFormat(EV_T("Top level element '%s' does not match DOCTYPE declaration")) % currentElement);
        break;
    case UndeclaredElement:
        m_errorMsg = EVStr(EVFormat(EV_T("Element '%s' not declared in DTD")) % currentElement);
        break;
    case UndeclaredAttribute:
        m_errorMsg = EVStr(EVFormat(EV_T("Attribute '%s' for element '%s' not declared in DTD")) % currentElement % containingElement);
        break;
    case InvalidElement:
        m_errorMsg = EVStr(EVFormat(EV_T("Element '%s' violates content model of containing element '%s'")) % currentElement % containingElement);
        break;
    case NoCharacterData:
        m_errorMsg = EVStr(EVFormat(EV_T("Element '%s' cannot contain character data")) % currentElement);
        break;
    case UncontainedCharacterData:
        m_errorMsg = EV_T("Character data with no containing element");
        break;
    case MismatchedFixedValue:
        m_errorMsg = EVStr(EVFormat(EV_T("#FIXED attribute value does not match DTD for attribute '%s' in element '%s'")) % currentElement % containingElement);
        break;
    case MissingRequiredAttribute:
        m_errorMsg = EVStr(EVFormat(EV_T("#REQUIRED attribute '%s' missing in element '%s'")) % currentElement % containingElement);
        break;
    case BadAttributeValue:
        m_errorMsg = EVStr(EVFormat(EV_T("Attribute value not in set of possible values for attribute '%s' in element '%s'")) % currentElement % containingElement);
        break;
    case DuplicateId:
        m_errorMsg = EVStr(EVFormat(EV_T("Duplicate ID within the same document for attribute '%s' in element '%s'")) % currentElement % containingElement);
        break;
    case UndeclaredNotation:
        m_errorMsg = EVStr(EVFormat(EV_T("Notation not declared in DTD for attribute '%s' in element '%s'")) % currentElement % containingElement);
        break;
    case UndeclaredEntity:
        m_errorMsg = EVStr(EVFormat(EV_T("Entity not declared in DTD for attribute '%s' in element '%s'")) % currentElement % containingElement);
        break;
    case IdNotFound:
        m_errorMsg = EVStr(EVFormat(EV_T("Could not find ID corresponding to IDREF '%s'")) % currentElement);
        break;
    case InvalidElementPattern:
        m_errorMsg = EVStr(EVFormat(EV_T("Element pattern violates the content model of element '%s'")) % currentElement);
        break;
    default:
        break;
    }
}


//*************************************************************************
// ParseNode
//*************************************************************************


int ParseNode::m_currentId = 0;


ParseNode::ParseNode(Type type):
    m_id(++m_currentId),
    m_type(type),
    m_left(NULL),
    m_right(NULL),
    m_nullable(false)
{
#ifdef EV_TIMING
    m_nullableTime  = 0;
    m_firstPosTime  = 0;
    m_lastPosTime   = 0;
    m_followPosTime = 0;
#endif /* EV_TIMING */
}


ParseNode::ParseNode(Type type, ParseNode *left):
    m_id(++m_currentId),
    m_type(type),
    m_left(left),
    m_right(NULL),
    m_nullable(false)
{
#ifdef EV_TIMING
    m_nullableTime  = 0;
    m_firstPosTime  = 0;
    m_lastPosTime   = 0;
    m_followPosTime = 0;
#endif /* EV_TIMING */
}


ParseNode::~ParseNode()
{
    try {
        delete m_left;
        delete m_right;
    }
    catch(...) {
        MeaAssert(false);
    }
}


void ParseNode::Nullable()
{
    if (IsEpsilon() || IsRep() || IsOpt()) {
        m_nullable = true;
    } else if (IsChoice()) {
        m_nullable = (m_left->m_nullable || m_right->m_nullable);
    } else if (IsSeq()) {
        m_nullable = (m_left->m_nullable && m_right->m_nullable);
    } else {
        m_nullable = false;
    }
}


void ParseNode::Firstpos()
{
    if (!IsEpsilon()) {
        if (IsChoice()) {
            m_firstPos = m_left->m_firstPos;
            Union(m_firstPos, m_right->m_firstPos);
        } else if (IsSeq()) {
            if (m_left->m_nullable) {
                m_firstPos = m_left->m_firstPos;
                Union(m_firstPos, m_right->m_firstPos);
            } else {
                m_firstPos = m_left->m_firstPos;
            }
        } else if (IsRep() || IsOpt() || IsPlus()) {
            m_firstPos = m_left->m_firstPos;
        } else {
            m_firstPos.insert(this);
        }
    }
}


void ParseNode::Lastpos()
{
    if (!IsEpsilon()) {
        if (IsChoice()) {
            m_lastPos = m_left->m_lastPos;
            Union(m_lastPos, m_right->m_lastPos);
        } else if (IsSeq()) {
            if (m_right->m_nullable) {
                m_lastPos = m_left->m_lastPos;
                Union(m_lastPos, m_right->m_lastPos);
            } else {
                m_lastPos = m_right->m_lastPos;
            }
        } else if (IsRep() || IsOpt() || IsPlus()) {
            m_lastPos = m_left->m_lastPos;
        } else {
            m_lastPos.insert(this);
        }
    }
}


void ParseNode::Followpos()
{
    if (IsSeq()) {
        const NodeSet& last = m_left->m_lastPos;
        
        for (NodeIter_c iter = last.begin(); iter != last.end(); iter++) {
            Union((*iter)->m_followPos, m_right->m_firstPos);
        }
    }
    else if (IsRep() || IsPlus()) {
        for (NodeIter_c iter = m_lastPos.begin(); iter != m_lastPos.end(); iter++) {
            Union((*iter)->m_followPos, m_firstPos);
        }
    }
}


void ParseNode::BuildFollowpos()
{
    if (m_left != NULL) {
        m_left->BuildFollowpos();
    }
    if (m_right != NULL) {
        m_right->BuildFollowpos();
    }

#ifdef EV_TIMING
    m_nullableTime = clock();
#endif /* EV_TIMING */

    Nullable();

#ifdef EV_TIMING
    m_nullableTime = clock() - m_nullableTime;
    m_firstPosTime = clock();
#endif /* EV_TIMING */

    Firstpos();

#ifdef EV_TIMING
    m_firstPosTime = clock() - m_firstPosTime;
    m_lastPosTime = clock();
#endif /* EV_TIMING */

    Lastpos();

#ifdef EV_TIMING
    m_lastPosTime = clock() - m_lastPosTime;
    m_followPosTime = clock();
#endif /* EV_TIMING */

    Followpos();

#ifdef EV_TIMING
    m_followPosTime = clock() - m_followPosTime;
#endif /* EV_TIMING */
}


void ParseNode::GetSymbolSet(SymbolSet& symbols) const
{
    if (m_left != NULL) {
        m_left->GetSymbolSet(symbols);
    }
    if (m_right != NULL) {
        m_right->GetSymbolSet(symbols);
    }

    if (!m_symbol.empty()) {
        symbols.insert(m_symbol);
    }
}


//*************************************************************************
// State
//*************************************************************************


int State::m_currentId = 0;


State::State(DFA& dfa, Type type, const ParseNode::NodeSet& positions) :
    m_dfa(dfa), m_id(++m_currentId), m_type(type), m_positions(positions),
    m_marked(false), m_accepting(ContainsEnd())
    
{
}


State::State(DFA& dfa, Type type, bool accepting) :
    m_dfa(dfa), m_id(++m_currentId), m_type(type), m_marked(false),
    m_accepting(accepting)
{
}


State::~State()
{
}


bool State::operator==(const State& rhs) const
{
    return ((m_type == rhs.m_type) &&
            (m_positions == rhs.m_positions) &&
            (m_transitions == rhs.m_transitions));
}


const State* State::GetNextState(const EVString& elementName) const
{
    if (IsEmpty()) {
        return NULL;
    }
    if (IsAny()) {
        const SymbolSet& anyElements = m_dfa.GetValidator().GetAnyElements();
        return ((anyElements.find(elementName) == anyElements.end()) ? NULL : this);
    }

    TransIter_c iter = m_transitions.find(elementName);
    return ((iter == m_transitions.end()) ? NULL : (*iter).second);
}


//*************************************************************************
// DFA
//*************************************************************************


int DFA::m_currentId = 0;


DFA::DFA(Validator& validator, const ContentModel& contentModel) :
    m_id(++m_currentId), m_parseTree(NULL), m_validator(validator) 
{
#ifdef EV_TIMING
    m_complexTotalTime = 0;
#endif /* EV_TIMING */

    // Depending on the category, build the appropriate DFA.
    //
    State::ResetId();

    switch (contentModel.GetCategory()) {
    case ContentModel::EmptyCat:
        BuildEmptyDFA();
        break;
    case ContentModel::AnyCat:
        BuildAnyDFA();
        break;
    case ContentModel::MixedCat:
        BuildMixedDFA(contentModel);
        break;
    case ContentModel::PureChoiceCat:
        BuildChoiceDFA(contentModel);
        break;
    case ContentModel::PureSeqCat:
        BuildSeqDFA(contentModel);
        break;
    case ContentModel::ComplexCat:
        {
#ifdef EV_TIMING
            m_complexTotalTime = clock();
#endif /* EV_TIMING */

            // Build a parse tree for the content model and add it to
            // the map of trees.
            //
            ParseNode::ResetId();

#ifdef EV_TIMING
            m_parseTreeTime = clock();
#endif /* EV_TIMING */

            m_parseTree = BuildParseTree(contentModel);
            MeaAssert(m_parseTree != NULL);

#ifdef EV_TIMING
            m_parseTreeTime = clock() - m_parseTreeTime;
#endif /* EV_TIMING */

            // Build the followpos list
            //
#ifdef EV_TIMING
            m_followPosTime = clock();
#endif /* EV_TIMING */
            m_parseTree->BuildFollowpos();
#ifdef EV_TIMING
            m_followPosTime = clock() - m_followPosTime;
#endif /* EV_TIMING */

            // Construct the DFA for this parse tree
            //
#ifdef EV_TIMING
            m_buildDfaTime = clock();
#endif /* EV_TIMING */
            BuildComplexDFA(m_parseTree);
#ifdef EV_TIMING
            m_buildDfaTime = clock() - m_buildDfaTime;

            m_complexTotalTime = clock() - m_complexTotalTime;
#endif /* EV_TIMING */
        }
        break;
    default:
        MeaAssert(false);
        break;
    }
}


DFA::~DFA()
{
    try {
        delete m_parseTree;

        for (StateIter siter = m_states.begin(); siter != m_states.end(); siter++) {
            delete (*siter);
        }
        m_states.clear();
    }
    catch(...) {
        MeaAssert(false);
    }
}


void DFA::BuildEmptyDFA()
{
    m_states.push_back(new State(*this, State::Empty, true));
}


void DFA::BuildAnyDFA()
{
    m_states.push_back(new State(*this, State::Any, true));
}


void DFA::BuildMixedDFA(const ContentModel& contentModel)
{
    State *state = new State(*this, State::Mixed, true);
    m_states.push_back(state);

    for (ContentModel::Iter_c iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); iter++) {
        state->AddTransition(contentModel.GetChild(iter).GetName(), state);
    }
}


void DFA::BuildChoiceDFA(const ContentModel& contentModel)
{
    bool isRep  = contentModel.IsRep();
    bool isPlus = contentModel.IsPlus();
    bool isOpt  = contentModel.IsOpt();

    if (isRep) {
        State *state = new State(*this, State::Normal, true);
        m_states.push_back(state);

        for (ContentModel::Iter_c iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); iter++) {
            state->AddTransition(contentModel.GetChild(iter).GetName(), state);
        }
    }
    else if (isPlus) {
        State *start = new State(*this, State::Normal);
        State *end   = new State(*this, State::Normal, true);
        m_states.push_back(start);
        m_states.push_back(end);

        for (ContentModel::Iter_c iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); iter++) {
            start->AddTransition(contentModel.GetChild(iter).GetName(), end);
            end->AddTransition(contentModel.GetChild(iter).GetName(), end);
        }
    }
    else {
        State *start = new State(*this, State::Normal, isOpt);
        State *end = new State(*this, State::Normal, true);
        m_states.push_back(start);
        m_states.push_back(end);

        for (ContentModel::Iter_c iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); iter++) {
            start->AddTransition(contentModel.GetChild(iter).GetName(), end);
        }
    }
}


void DFA::BuildSeqDFA(const ContentModel& contentModel)
{
    int i;

    bool isRep  = contentModel.IsRep();
    bool isPlus = contentModel.IsPlus();
    bool isOpt  = contentModel.IsOpt();

    State *start = new State(*this, State::Normal, isRep || isOpt);
    State *first = start;
    m_states.push_back(start);

    ContentModel::Iter_c iter;
    for (i = 0, iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); i++, iter++) {
        if (isRep && (i == contentModel.GetNumChildren() - 1)) {
            start->AddTransition(contentModel.GetChild(iter).GetName(), first);
        } else {
            State *end = new State(*this, State::Normal);
            m_states.push_back(end);
            start->AddTransition(contentModel.GetChild(iter).GetName(), end);
            start = end;
        }
    }

    if (!isRep) {
        start->SetAccepting();
    }

    if (isPlus) {
        first = start;
        
        for (i = 0, iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); i++, iter++) {
            if (i == contentModel.GetNumChildren() - 1) {
                start->AddTransition(contentModel.GetChild(iter).GetName(), first);
            } else {
                State *end = new State(*this, State::Normal);
                m_states.push_back(end);
                start->AddTransition(contentModel.GetChild(iter).GetName(), end);
                start = end;
            }
        }
    }
}


void DFA::BuildComplexDFA(const ParseNode *parseTree)
{
    // The first state is comprised of Firstpos(root) positions
    //
    State *state = new State(*this, State::Normal, parseTree->GetFirstpos());
    m_states.push_back(state);

    // Consider each unmarked state
    //
    SymbolSet symbols;
    parseTree->GetSymbolSet(symbols);

    while ((state = GetUnmarkedState()) != NULL) {
        state->Mark();

        // Consider each symbol in the regular expression.
        //
        for (SymbolSetIter symbolSetIter = symbols.begin(); symbolSetIter != symbols.end(); ++symbolSetIter) {
            const ParseNode::NodeSet& positions = state->GetPositions();

            ParseNode::NodeSet posList;

            // Build a union of the positions in the current state that
            // correspond to the current symbol.
            //
            for (ParseNode::NodeIter_c positionIter = positions.begin(); positionIter != positions.end(); positionIter++) {
                if ((*positionIter)->GetSymbol() == (*symbolSetIter)) {
                    const ParseNode::NodeSet& ps = (*positionIter)->GetFollowpos();
                    ParseNode::Union(posList, ps);
                }
            }

            // If we don't already have a state with the current positions,
            // create a new one and add it to the list
            //
            State *nextState = FindState(posList);
            if (!posList.empty() && (nextState == NULL)) {
                nextState = new State(*this, State::Normal, posList);
                m_states.push_back(nextState);
            }

            // Add the transition for the current state.
            //
            if (nextState != NULL) {
                state->AddTransition(*symbolSetIter, nextState);
            }
        }
    }
}


ParseNode* DFA::BuildParseTree(const ContentModel& contentModel, bool top)
{
    // Create a node for the basic type
    //
    ParseNode *rootNode = NULL;

    switch (contentModel.GetType()) {
    default:
    case ContentModel::EmptyType:
    case ContentModel::AnyType:
    case ContentModel::MixedType:
        MeaAssert(false);
        break;
    case ContentModel::NameType:
        rootNode = new ParseNode(ParseNode::Name);
        break;
    case ContentModel::ChoiceType:
        rootNode = new ParseNode(ParseNode::Choice);
        break;
    case ContentModel::SeqType:
        rootNode = new ParseNode(ParseNode::Seq);
        break;
    }

    MeaAssert(rootNode != NULL);

    // If there is a Name given, add it to the node
    //
    if (!contentModel.GetName().empty()) {
        rootNode->SetSymbol(contentModel.GetName());
    }

    // For choice or sequence node process their children.
    //
    for (ContentModel::Iter_c iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); iter++) {
        ParseNode *child = BuildParseTree(contentModel.GetChild(iter), false);

        if (!rootNode->HaveLeft()) {
            rootNode->SetLeft(child);
        } else if (!rootNode->HaveRight()) {
            rootNode->SetRight(child);
        } else {
            ParseNode *t = rootNode->CloneType();
            t->SetLeft(rootNode);
            t->SetRight(child);
            rootNode = t;
        }
    }

    // Ensure that all choice (mixed) and sequence nodes have two children.
    //
    if (rootNode->IsChoice() || rootNode->IsSeq()) {
        if (!rootNode->HaveLeft()) {
            rootNode->SetLeft(new ParseNode(ParseNode::Epsilon));
        }
        if (!rootNode->HaveRight()) {
            rootNode->SetRight(new ParseNode(ParseNode::Epsilon));
        }
    }

    // Now account for any quantity specified for the node.
    //
    switch (contentModel.GetQuantity()) {
    case ContentModel::NoneQuant:
        break;
    case ContentModel::OptQuant:
        rootNode = new ParseNode(ParseNode::Opt, rootNode);
        break;
    case ContentModel::RepQuant:
        rootNode = new ParseNode(ParseNode::Rep, rootNode);
        break;
    case ContentModel::PlusQuant:
        rootNode = new ParseNode(ParseNode::Plus, rootNode);
        break;
    default:
        MeaAssert(false);
        break;
    }

    // If this is the top level root node, add the end marker
    //
    if (top) {
        if ((rootNode->IsSeq()) && (rootNode->GetRight()->IsEpsilon())) {
            delete rootNode->GetRight();
            rootNode->SetRight(new ParseNode(ParseNode::End));
        } else {
            ParseNode *seqNode = new ParseNode(ParseNode::Seq);
            seqNode->SetLeft(rootNode);
            seqNode->SetRight(new ParseNode(ParseNode::End));
            rootNode = seqNode;
        }
    }

    return rootNode;
}


State* DFA::FindState(const ParseNode::NodeSet& positions)
{
    for (StateIter_c iter = m_states.begin(); iter != m_states.end(); iter++) {
        if (*(*iter) == positions) {
            return *iter;
        }
    }

    return NULL;
}


State* DFA::GetUnmarkedState()
{
    for (StateIter_c iter = m_states.begin(); iter != m_states.end(); iter++) {
        if (!(*iter)->IsMarked()) {
            return *iter;
        }
    }

    return NULL;
}


//*************************************************************************
// ContentModel
//*************************************************************************

 
ContentModel::ContentModel(const XML_Content *model, bool root) :
    m_root(root), m_category(ComplexCat)
{
    MeaAssert(model != NULL);

    // Copy the content model node type
    //
    switch (model->type) {
    case XML_CTYPE_EMPTY:
        m_type = EmptyType;
        break;
    case XML_CTYPE_ANY:
        m_type = AnyType;
        break;
    case XML_CTYPE_MIXED:
        m_type = MixedType;
        break;
    case XML_CTYPE_NAME:
        m_type = NameType;
        break;
    case XML_CTYPE_CHOICE:
        m_type = ChoiceType;
        break;
    case XML_CTYPE_SEQ:
        m_type = SeqType;
        break;
    default:
        MeaAssert(false);
        break;
    }

    // Copy the content model node quantity
    //
    switch (model->quant) {
    case XML_CQUANT_NONE:
        m_quantity = NoneQuant;
        break;
    case XML_CQUANT_OPT:
        m_quantity = OptQuant;
        break;
    case XML_CQUANT_REP:
        m_quantity = RepQuant;
        break;
    case XML_CQUANT_PLUS:
        m_quantity = PlusQuant;
        break;
    default:
        MeaAssert(false);
        break;
    }

    // If there is a name associated with this node, copy it.
    //
    if (model->name != NULL) {
        m_name = model->name;
    }

    // Create child nodes as needed
    //
    for (unsigned int i = 0; i < model->numchildren; i++) {
        m_children.push_back(new ContentModel(&model->children[i], false));
    }

    // Perform root node operations such as optimization and
    // categorization.
    //
    if (m_root) {
        m_category = Categorize();
        if (m_category == ComplexCat) {
            Optimize();
            m_category = Categorize();
        }
        m_signature = MakeSignature();
    }
}


ContentModel::~ContentModel()
{
    try {
        for (Iter iter = GetChildIter(); !AtEnd(iter); iter++) {
            delete *iter;
        }
    }
    catch(...) {
        MeaAssert(false);
    }
}


void ContentModel::Optimize()
{
    // Remove unnecessary levels of parenthesis
    //
    Collapse(this);

    // Move quantity on singletons to outer container.
    //
    // (a*) -> (a)*
    //
    if (m_children.size() == 1 && (IsSeq() || IsChoice())) {
        ContentModel* child = *m_children.begin();

        if (IsNone() && child->IsNone()) {
            m_quantity = child->m_quantity;
            child->m_quantity = NoneQuant;
        }
    }
}


void ContentModel::Collapse(ContentModel* model)
{
    if (!model->IsSeq() && !model->IsChoice()) {
        return;
    }

    Iter iter = model->GetChildIter();
    while (!model->AtEnd(iter)) {
        ContentModel* child = *iter;

        Collapse(child);

        // ((a|b)) -> (a|b)
        // ((a,b)) -> (a,b)
        //
        if (model->m_children.size() == 1 && (child->IsSeq() || child->IsChoice())) {
            if (model->m_quantity == child->m_quantity || model->IsNone() || child->IsNone()) {
                model->m_type = child->m_type;
                if (model->IsNone()) {
                    model->m_quantity = child->m_quantity;
                }
                model->m_name = child->m_name;
                model->m_children = child->m_children;
                child->m_children.clear();
                delete child;

                iter = model->m_children.begin();
            } else {
                iter++;
            }
        }
        // ((a|b)|c|d) -> (a|b|c|d)
        // ((a,b),c,d) -> (a,b,c,d)
        //
        else if ((model->IsChoice() && (child->IsChoice() || (child->IsSeq() && child->m_children.size() == 1)) && model->IsRep() && (child->IsNone() || child->IsRep())) ||
                 (model->m_type == child->m_type && model->IsNone() && model->m_quantity == child->m_quantity)) {
            Iter placeIter = iter;
            placeIter++;

            model->m_children.insert(placeIter, child->m_children.begin(), child->m_children.end());
            child->m_children.clear();
            iter = model->m_children.erase(iter);
            delete child;
        } else {
            iter++;
        }
    }
}


EVString ContentModel::MakeSignature() const
{
    EVString sig;

    switch (m_type) {
    case EmptyType:
        sig = EV_T('E');
        break;
    case AnyType:
        sig = EV_T('A');
        break;
    case MixedType:
        sig = EV_T('M');
        break;
    case NameType:
        sig = EV_T('N');
        break;
    case ChoiceType:
        sig = EV_T('C');
        break;
    case SeqType:
        sig = EV_T('S');
        break;
    default:
        MeaAssert(false);
        break;
    }

    switch (m_quantity) {
    case NoneQuant:
        sig += EV_T('-');
        break;
    case OptQuant:
        sig += EV_T('?');
        break;
    case RepQuant:
        sig += EV_T('*');
        break;
    case PlusQuant:
        sig += EV_T('+');
        break;
    default:
        MeaAssert(false);
        break;
    }

    sig += m_name;

    for (Iter_c iter = GetChildIter(); !AtEnd(iter); iter++) {
        sig += GetChild(iter).MakeSignature();
    }

    return sig;
}


ContentModel::Category ContentModel::Categorize() const
{
    // ANY
    //
    if (IsAny()) {
        return AnyCat;
    }

    // EMPTY
    //
    if (IsEmpty()) {
        return EmptyCat;
    }

    // (#PCDATA) or (#PCDATA|a|b|c)*
    //
    if (IsMixed()) {
        return MixedCat;
    }

    // (a|b|c)[?] or (a|b|c)* or (a,b,c)[?] or (a,b,c)*
    //
    if (HasPureChildren()) {
        return IsChoice() ? PureChoiceCat : PureSeqCat;
    }

    return ComplexCat;
}


//*************************************************************************
// ElementDecl
//*************************************************************************


ElementDecl::ElementDecl(Validator& validator, const XML_Char* elementName) :
    m_dfa(NULL),
    m_elementName((elementName != NULL) ? elementName : EV_T("")),
    m_validator(validator)
{
}


ElementDecl::~ElementDecl()
{
    try {
        for (AttributeIter aiter = m_attributes.begin(); aiter != m_attributes.end(); aiter++) {
            delete (*aiter).second;
        }
        m_attributes.clear();
        m_dfa = NULL;
    }
    catch(...) {
        MeaAssert(false);
    }
}


void ElementDecl::AddAttributeDecl(const XML_Char* attrName,
                                   const XML_Char* attrType,
                                   const XML_Char* defValue,
                                   bool isRequired)
{
    AttributeDecl *attr = new AttributeDecl(*this, attrName, attrType,
                                            defValue, isRequired);
    m_attributes[attrName] = attr;

    if (attr->IsRequired()) {
        m_requiredAttributes[attrName] = attr;
    }
}


const State* ElementDecl::GetStartState() const
{
    return m_dfa->GetStartState();
}


//*************************************************************************
// AttributeDecl
//*************************************************************************


AttributeDecl::AttributeDecl(ElementDecl& elementDecl, const XML_Char* attrName,
                             const XML_Char* attrType, const XML_Char* defValue,
                             bool isRequired) : m_elementDecl(elementDecl),
    m_name((attrName != NULL) ? attrName : EV_T("")),
    m_defValue((defValue != NULL) ? defValue : EV_T("")),
    m_required(isRequired)
{
    // Determine the attribute type
    //
    MeaAssert(attrType != NULL);

    if (_evcscmp(attrType, EV_T("CDATA")) == 0) {
        m_type = CDATA;
    } else if (attrType[0] == EV_T('(')) {
        m_type = Enumerated;
        ParseValues(attrType);
    } else if (_evcscmp(attrType, EV_T("ID")) == 0) {
        m_type = ID;
    } else if (_evcscmp(attrType, EV_T("IDREF")) == 0) {
        m_type = IDREF;
    } else if (_evcscmp(attrType, EV_T("IDREFS")) == 0) {
        m_type = IDREFS;
    } else if (_evcscmp(attrType, EV_T("ENTITY")) == 0) {
        m_type = ENTITY;
    } else if (_evcscmp(attrType, EV_T("ENTITIES")) == 0) {
        m_type = ENTITIES;
    } else if (_evcscmp(attrType, EV_T("NMTOKEN")) == 0) {
        m_type = NMTOKEN;
    } else if (_evcscmp(attrType, EV_T("NMTOKENS")) == 0) {
        m_type = NMTOKENS;
    } else if (_evcsncmp(attrType, EV_T("NOTATION"), 8) == 0) {
        m_type = NOTATION;
        ParseValues(&attrType[8]);
    }
}


AttributeDecl::~AttributeDecl()
{
}


void AttributeDecl::ParseValues(const XML_Char* valueStr)
{
    static EVSeparator sep(EV_T("()|"));
    EVString vstr(valueStr);

    EVTokenizer tokens(vstr, sep);
    for (EVTokenizer::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
        m_values.insert(*iter);
    }
}


//*************************************************************************
// Helper functions
//*************************************************************************


EVOstream& ev::operator<<(EVOstream& stream, const Validator& validator)
{
    stream << EV_T("Number of elements: ") << validator.m_elementDecls.size() << std::endl;
    stream << EV_T("Number of DFAs: ") << validator.m_dfas.size() << std::endl;
    stream << std::endl;

    for (Validator::DFAIter_c iter = validator.m_dfas.begin(); iter != validator.m_dfas.end(); ++iter) {
        stream << EV_T("DFA: ") << (*iter).second->GetId() << EV_T(" (") << (*iter).first.c_str() << EV_T(")") << std::endl;
        stream << (*iter).second << std::endl;
    }

    for (Validator::ElementDeclIter_c declIter = validator.m_elementDecls.begin(); declIter != validator.m_elementDecls.end(); ++declIter) {
        stream << EV_T("Element: ") << (*declIter).first.c_str() << std::endl;
        stream << (*declIter).second << std::endl;
    }

    if (!validator.m_notations.empty()) {
        stream << EV_T("Notations: ") << std::endl;
        for (Validator::NotationIter_c niter = validator.m_notations.begin(); niter != validator.m_notations.end(); ++niter) {
            stream << EV_T("    ") << (*niter).c_str() << std::endl;
        }
    }

    if (!validator.m_entities.empty()) {
        stream << EV_T("Entities: ") << std::endl;
        for (Validator::EntityIter_c eiter = validator.m_entities.begin(); eiter != validator.m_entities.end(); ++eiter) {
            stream << EV_T("    ") << (*eiter).c_str() << std::endl;
        }
    }

    return stream;
}


EVOstream& ev::operator<<(EVOstream& stream, const ContentModel& contentModel)
{
    static int level = 0;
    EVString line;
    EVString indent;

    if (contentModel.m_root) {
        stream << EV_T("Category: ");
        switch (contentModel.m_category) {
        case ContentModel::EmptyCat:
            stream << EV_T("Empty");
            break;
        case ContentModel::AnyCat:
            stream << EV_T("Any");
            break;
        case ContentModel::MixedCat:
            stream << EV_T("Mixed");
            break;
        case ContentModel::PureChoiceCat:
            stream << EV_T("PureChoice");
            break;
        case ContentModel::PureSeqCat:
            stream << EV_T("PureSeq");
            break;
        case ContentModel::ComplexCat:
            stream << EV_T("Complex");
            break;
        default:
            MeaAssert(false);
            break;
        }
        stream << std::endl;
    }

    for (int j = 0; j < (level * 4); j++) {
        indent += EV_T(' ');
    }

    line = indent + EV_T("Type: ");
    switch (contentModel.m_type) {
    case ContentModel::EmptyType:
        line += EV_T("EMPTY");
        break;
    case ContentModel::AnyType:
        line += EV_T("ANY");
        break;
    case ContentModel::MixedType:
        line += EV_T("MIXED");
        break;
    case ContentModel::NameType:
        line += EV_T("NAME");
        break;
    case ContentModel::ChoiceType:
        line += EV_T("CHOICE");
        break;
    case ContentModel::SeqType:
        line += EV_T("SEQ");
        break;
    default:
        MeaAssert(false);
        break;
    }
    stream << line.c_str() << std::endl;

    line = indent + EV_T("Quantity: ");
    switch (contentModel.m_quantity) {
    case ContentModel::NoneQuant:
        line += EV_T("NONE");
        break;
    case ContentModel::OptQuant:
        line += EV_T("?");
        break;
    case ContentModel::RepQuant:
        line += EV_T("*");
        break;
    case ContentModel::PlusQuant:
        line += EV_T("+");
        break;
    default:
        MeaAssert(false);
        break;
    }
    stream << line.c_str() << std::endl;

    if (!contentModel.m_name.empty()) {
        line = indent + EV_T("Name: ");
        line += contentModel.m_name;
        stream << line.c_str() << std::endl;
    }

    level++;
    for (ContentModel::Iter_c iter = contentModel.GetChildIter(); !contentModel.AtEnd(iter); iter++) {
        stream << std::endl << contentModel.GetChild(iter);
    }
    level--;

    return stream;
}


EVOstream& ev::operator<<(EVOstream& stream, const DFA* dfa)
{
#ifdef EV_TIMING
    if (dfa->m_complexTotalTime > 0) {
        stream << EV_T("Complex Times") << std::endl;
        stream << EV_T("    Total: ") << dfa->m_complexTotalTime << std::endl;
        stream << EV_T("    Parse Tree: ") << dfa->m_parseTreeTime << std::endl;
        stream << EV_T("    Followpos: ") << dfa->m_followPosTime << std::endl;
        stream << EV_T("    Build DFA: ") << dfa->m_buildDfaTime << std::endl;
    }
#endif /* EV_TIMING */

    if (dfa->m_parseTree != NULL) {
        stream << dfa->m_parseTree << std::endl;
    }

    stream << EV_T("States:") << std::endl;
    for (DFA::StateIter_c siter = dfa->m_states.begin(); siter != dfa->m_states.end(); siter++) {
        stream << (*siter) << std::endl;
    }

    return stream;
}


EVOstream& ev::operator<<(EVOstream& stream, const State* state)
{
    EVString indent(EV_T("    "));

    stream << indent.c_str() << EV_T("State: ") << state->m_id << std::endl;

    stream << indent.c_str() << EV_T("Type: ");
    switch (state->m_type) {
    case State::Normal:
        stream << EV_T("Normal");
        break;
    case State::Empty:
        stream << EV_T("Empty");
        break;
    case State::Any:
        stream << EV_T("Any");
        break;
    case State::Mixed:
        stream << EV_T("Mixed");
        break;
    default:
        MeaAssert(false);
        break;
    }
    stream << std::endl;

    stream << indent.c_str() << EV_T("Accepting: ") << (state->m_accepting ? EV_T("true") : EV_T("false")) << std::endl;

    stream << indent.c_str() << EV_T("Positions: ");
    for (ParseNode::NodeIter_c iter = state->m_positions.begin(); iter != state->m_positions.end(); iter++) {
        stream << (*iter)->GetId() << EV_T(" ");
    }
    stream << std::endl;
    stream << indent.c_str() << EV_T("Transitions: ");
    if (state->m_transitions.size() == 0) {
        stream << EV_T("none");
    }
    stream << std::endl;
    for (State::TransIter_c titer = state->m_transitions.begin(); titer != state->m_transitions.end(); titer++) {
        stream << indent.c_str() << indent.c_str() << (*titer).first.c_str() << EV_T(" -> ") << (*titer).second->GetId() << std::endl;
    }

    return stream;
}


EVOstream& ev::operator<<(EVOstream& stream, const ParseNode *node)
{
    static int level = 0;
    EVString indent;

    for (int j = 0; j < (level * 4); j++) {
        indent += EV_T(' ');
    }

    if (level == 0) {
        SymbolSet symbols;
        node->GetSymbolSet(symbols);
        stream << EV_T("Input Symbols: ");
        for (SymbolSetIter iter = symbols.begin(); iter != symbols.end(); ++iter) {
            stream << (*iter).c_str() << EV_T(" ");
        }
        stream << std::endl;
    }

    stream << indent.c_str() << EV_T("Node: ");
    switch (node->GetType()) {
    case ParseNode::End:
        stream << EV_T("end");
        break;
    case ParseNode::Epsilon:
        stream << EV_T("epsilon");
        break;
    case ParseNode::Name:
        stream << EV_T("name");
        break;
    case ParseNode::Choice:
        stream << EV_T("choice");
        break;
    case ParseNode::Seq:
        stream << EV_T("sequence");
        break;
    case ParseNode::Rep:
        stream << EV_T("repeat");
        break;
    case ParseNode::Opt:
        stream << EV_T("optional");
        break;
    case ParseNode::Plus:
        stream << EV_T("plus");
        break;
    default:
        MeaAssert(false);
        break;
    }
    stream << std::endl;

    stream << indent.c_str() << EV_T("Id: ") << node->m_id << std::endl;

#ifdef EV_TIMING
    stream << indent.c_str() << EV_T("Nullable time:  ") << node->m_nullableTime << std::endl;
    stream << indent.c_str() << EV_T("Firstpos time:  ") << node->m_firstPosTime << std::endl;
    stream << indent.c_str() << EV_T("Lastpos time:   ") << node->m_lastPosTime << std::endl;
    stream << indent.c_str() << EV_T("Followpos time: ") << node->m_followPosTime << std::endl;
#endif /* EV_TIMING */

    EVString symbol = node->GetSymbol();
    if (!symbol.empty()) {
        stream << indent.c_str() << EV_T("Symbol: ") << symbol.c_str() << std::endl;
    }

    stream << indent.c_str() << EV_T("Nullable: ") << (node->m_nullable ? EV_T("true") : EV_T("false")) << std::endl;

    ParseNode::NodeSet set;

    stream << indent.c_str() << EV_T("Firstpos: ");
    set = node->m_firstPos;
    for (ParseNode::NodeIter iter = set.begin(); iter != set.end(); iter++) {
        stream << (*iter)->m_id << EV_T(" ");
    }
    stream << std::endl;

    stream << indent.c_str() << EV_T("Lastpos: ");
    set = node->m_lastPos;
    for (ParseNode::NodeIter iter = set.begin(); iter != set.end(); iter++) {
        stream << (*iter)->m_id << EV_T(" ");
    }
    stream << std::endl;

    stream << indent.c_str() << EV_T("Followpos: ");
    set = node->m_followPos;
    for (ParseNode::NodeIter iter = set.begin(); iter != set.end(); iter++) {
        stream << (*iter)->m_id << EV_T(" ");
    }
    stream << std::endl;

    stream << std::endl;

    level++;
    if (node->HaveLeft()) {
        stream << node->GetLeft();
    }
    if (node->HaveRight()) {
        stream << node->GetRight();
    }
    level--;

    return stream;
}


EVOstream& ev::operator<<(EVOstream& stream, const ElementDecl* elementDecl)
{
    stream << EV_T("    DFA: ") << elementDecl->m_dfa->GetId() << std::endl;
    for (ElementDecl::AttributeIter_c aiter = elementDecl->m_attributes.begin(); aiter != elementDecl->m_attributes.end(); aiter++) {
        stream << (*aiter).second << std::endl;
    }

    return stream;
}


EVOstream& ev::operator<<(EVOstream& stream, const AttributeDecl* attributeDecl)
{
    EVString indent(EV_T("    "));

    stream << indent.c_str() << EV_T("Attribute: ") << attributeDecl->m_name.c_str() << std::endl;
    indent += EV_T("    ");

    stream << indent.c_str() << EV_T("Type: ");
    switch (attributeDecl->m_type) {
    case AttributeDecl::Enumerated:
        stream << EV_T("Enumerated");
        break;
    case AttributeDecl::CDATA:
        stream << EV_T("CDATA");
        break;
    case AttributeDecl::ENTITY:
        stream << EV_T("ENTITY");
        break;
    case AttributeDecl::ENTITIES:
        stream << EV_T("ENTITIES");
        break;
    case AttributeDecl::ID:
        stream << EV_T("ID");
        break;
    case AttributeDecl::IDREF:
        stream << EV_T("IDREF");
        break;
    case AttributeDecl::IDREFS:
        stream << EV_T("IDREFS");
        break;
    case AttributeDecl::NMTOKEN:
        stream << EV_T("NMTOKEN");
        break;
    case AttributeDecl::NMTOKENS:
        stream << EV_T("NMTOKENS");
        break;
    case AttributeDecl::NOTATION:
        stream << EV_T("NOTATION");
        break;
    default:
        MeaAssert(false);
        break;
    }
    stream << std::endl;

    if (!attributeDecl->m_values.empty()) {
        stream << indent.c_str() << EV_T("Values: ");
        for (AttributeDecl::ValueIter_c iter = attributeDecl->m_values.begin(); iter != attributeDecl->m_values.end(); iter++) {
            stream << (*iter).c_str() << EV_T(" ");
        }
        stream << std::endl;
    }

    if (!attributeDecl->m_defValue.empty()) {
        stream << indent.c_str() << EV_T("Default: ") << attributeDecl->m_defValue.c_str() << std::endl;
    }
    stream << indent.c_str() << EV_T("Required: ") << (attributeDecl->m_required ? EV_T("TRUE") : EV_T("FALSE")) << std::endl;

    return stream;
}

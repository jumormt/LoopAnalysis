//
// Created by Xiao on 7/3/2022.
//

#ifndef WTO_H_
#define WTO_H_

#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Util/BasicTypes.h"

namespace SVF {

template<typename GraphT>
class Wto;

template<typename GraphT>
class WtoVertex;

template<typename GraphT>
class WtoCycle;

template<typename GraphT>
class WtoComponentVisitor;


/// \brief Represents the nesting of a node
///
/// The nesting of a node is the list of cycles containing the node, from
/// the outermost to the innermost.
template<typename GraphT>
class WtoNesting {
public:
    typedef typename GraphT::NodeType NodeT;
private:
    typedef std::vector<const NodeT *> NodeRefList;

public:
    typedef typename NodeRefList::const_iterator Iterator;

private:
    NodeRefList _nodes;

public:
    /// \brief Constructor
    WtoNesting() = default;

    /// \brief Copy constructor
    WtoNesting(const WtoNesting &) = default;

    /// \brief Move constructor
    WtoNesting(WtoNesting &&) = default;

    /// \brief Copy assignment operator
    WtoNesting &operator=(const WtoNesting &) = default;

    /// \brief Move assignment operator
    WtoNesting &operator=(WtoNesting &&) = default;

    /// \brief Destructor
    ~WtoNesting() = default;

    /// \brief Add a cycle head in the nesting
    void add(const NodeT *head) { _nodes.push_back(head); }

    /// \brief Begin iterator over the head of cycles
    Iterator begin() const { return _nodes.cbegin(); }

    /// \brief End iterator over the head of cycles
    Iterator end() const { return _nodes.cend(); }

    /// \brief Return the common prefix of the given nestings
    WtoNesting operator^(const WtoNesting &other) const {
        WtoNesting res;
        for (auto this_it = begin(), other_it = other.begin();
             this_it != end() && other_it != other.end();
             ++this_it, ++other_it) {
            if (*this_it == *other_it) {
                res.add(*this_it);
            } else {
                break;
            }
        }
        return res;
    }

private:
    /// \brief Compare the given nestings
    int compare(const WtoNesting &other) const {
        if (this == &other) {
            return 0; // equals
        }

        auto this_it = begin();
        auto other_it = other.begin();
        while (this_it != end()) {
            if (other_it == other.end()) {
                return 1; // `this` is nested within `other`
            } else if (*this_it == *other_it) {
                ++this_it;
                ++other_it;
            } else {
                return 2; // not comparable
            }
        }
        if (other_it == other.end()) {
            return 0; // equals
        } else {
            return -1; // `other` is nested within `this`
        }
    }

public:
    bool operator<(const WtoNesting &other) const {
        return compare(other) == -1;
    }

    bool operator<=(const WtoNesting &other) const {
        return compare(other) <= 0;
    }

    bool operator==(const WtoNesting &other) const {
        return compare(other) == 0;
    }

    bool operator>=(const WtoNesting &other) const {
        return operator<=(other, *this);
    }

    bool operator>(const WtoNesting &other) const {
        return compare(other) == 1;
    }

    /// \brief Dump the nesting, for debugging purpose
    std::string toString() const {
        std::string str;
        raw_string_ostream rawstr(str);
        rawstr << "[";
        for (auto it = begin(), et = end(); it != et;) {
            rawstr << (*it)->toString();
            ++it;
            if (it != et) {
                rawstr << ", ";
            }
        }
        rawstr << "]";
        return rawstr.str();
    }

}; // end class WtoNesting


/// \brief Base class for components of a weak topological order
///
/// This is either a vertex or a cycle.
template<typename GraphT>
class WtoComponent {
public:
    /// Types of SVFIR statements
    /// Gep represents (base + offset) for field sensitivity
    /// ThreadFork/ThreadJoin is to model parameter passings between thread spawners and spawnees.
    enum WtoCT {
        Node, Cycle
    };

    /// \brief Default constructor
    WtoComponent(WtoCT k) : type(k) {};

    /// \brief Copy constructor
    WtoComponent(const WtoComponent &) noexcept = default;

    /// \brief Move constructor
    WtoComponent(WtoComponent &&) noexcept = default;

    /// \brief Copy assignment operator
    WtoComponent &operator=(const WtoComponent &) noexcept = default;

    /// \brief Move assignment operator
    WtoComponent &operator=(WtoComponent &&) noexcept = default;

    /// \brief Accept the given visitor
    virtual void accept(WtoComponentVisitor<GraphT> &) const = 0;

    /// \brief Destructor
    virtual ~WtoComponent() = default;

    inline WtoCT getKind() const {
        return type;
    }

    virtual std::string toString() const = 0;

    WtoCT type;

}; // end class WtoComponent


/// \brief Represents a vertex
template<typename GraphT>
class WtoVertex final : public WtoComponent<GraphT> {
public:
    typedef typename GraphT::NodeType NodeT;

private:
    const NodeT *_node;

public:
    /// \brief Constructor
    explicit WtoVertex(const NodeT *node) : WtoComponent<GraphT>(WtoComponent<GraphT>::Node), _node(node) {}

    /// \brief Return the graph node
    const NodeT *node() const { return _node; }

    /// \brief Accept the given visitor
    void accept(WtoComponentVisitor<GraphT> &v) const override {
        v.visit(*this);
    }

    /// \brief Dump the vertex, for debugging purpose
    std::string toString() const override {
        return _node->toString();
    }

    /// ClassOf
    //@{
    static inline bool classof(const WtoVertex<GraphT> *) {
        return true;
    }

    static inline bool classof(const WtoComponent<GraphT> *c) {
        return c->getKind() == WtoComponent<GraphT>::Node;
    }
    ///@}

}; // end class WtoVertex


/// \brief Represents a cycle
template<typename GraphT>
class WtoCycle final : public WtoComponent<GraphT> {
public:
    typedef typename GraphT::NodeType NodeT;
    typedef WtoComponent<GraphT> WtoComponentT;

private:
    typedef std::unique_ptr<WtoComponentT> WtoComponentPtr;
    typedef std::list<WtoComponentPtr> WtoComponentRefList;

public:
    /// \brief Iterator over the components
    typedef typename WtoComponentRefList::const_iterator Iterator;

private:
    /// \brief Head of the cycle
    const NodeT *_head;

    /// \brief List of components
    WtoComponentRefList _components;

public:
    /// \brief Constructor
    WtoCycle(const NodeT *head, WtoComponentRefList components)
            : WtoComponent<GraphT>(WtoComponent<GraphT>::Cycle), _head(head), _components(std::move(components)) {}

    /// \brief Return the head of the cycle
    const NodeT *head() const { return _head; }

    /// \brief Begin iterator over the components
    Iterator begin() const {
        return _components.cbegin();
    }

    /// \brief End iterator over the components
    Iterator end() const {
        return _components.cend();
    }

    /// \brief Accept the given visitor
    void accept(WtoComponentVisitor<GraphT> &v) const override {
        v.visit(*this);
    }

    /// ClassOf
    //@{
    static inline bool classof(const WtoCycle<GraphT> *) {
        return true;
    }

    static inline bool classof(const WtoComponent<GraphT> *c) {
        return c->getKind() == WtoComponent<GraphT>::Cycle;
    }
    ///@}

    /// \brief Dump the cycle, for debugging purpose
    std::string toString() const override {
        std::string str;
        raw_string_ostream rawstr(str);
        rawstr << "(";
        rawstr << _head->toString() << ", ";
        for (auto it = begin(), et = end(); it != et;) {
            rawstr << (*it)->toString();
            ++it;
            if (it != et) {
                rawstr << ", ";
            }
        }
        rawstr << ")";
        return rawstr.str();
    }

}; // end class WtoCycle


/// \brief Weak topological order visitor
template<typename GraphT>
class WtoComponentVisitor {
public:
    typedef WtoVertex<GraphT> WtoVertexT;
    typedef WtoCycle<GraphT> WtoCycleT;

public:
    /// \brief Default constructor
    WtoComponentVisitor() = default;

    /// \brief Copy constructor
    WtoComponentVisitor(const WtoComponentVisitor &) noexcept = default;

    /// \brief Move constructor
    WtoComponentVisitor(WtoComponentVisitor &&) noexcept = default;

    /// \brief Copy assignment operator
    WtoComponentVisitor &operator=(const WtoComponentVisitor &) noexcept = default;

    /// \brief Move assignment operator
    WtoComponentVisitor &operator=(WtoComponentVisitor &&) noexcept = default;

    /// \brief Visit the given vertex
    virtual void visit(const WtoVertexT &) = 0;

    /// \brief Visit the given cycle
    virtual void visit(const WtoCycleT &) = 0;

    /// \brief Destructor
    virtual ~WtoComponentVisitor() = default;

}; // end class WtoComponentVisitor


/// \brief Weak Topological Ordering
template<typename GraphT>
class Wto {

public:
    typedef typename GraphT::NodeType NodeT;
    typedef typename GraphT::EdgeType EdgeT;
    typedef WtoNesting<GraphT> WtoNestingT;
    typedef WtoComponent<GraphT> WtoComponentT;
    typedef WtoVertex<GraphT> WtoVertexT;
    typedef WtoCycle<GraphT> WtoCycleT;

private:
    typedef std::unique_ptr<WtoComponentT> WtoComponentPtr;
    typedef std::list<WtoComponentPtr> WtoComponentRefList;
    typedef Set<const NodeT *> NodeRefList;

    typedef u32_t Dfn;
    typedef Map<const NodeT *, Dfn> DfnTable;
    typedef std::vector<const NodeT *> Stack;
    typedef std::shared_ptr<WtoNestingT> WtoNestingPtr;
    typedef Map<const NodeT *, WtoNestingPtr> NestingTable;

public:
    /// \brief Iterator over the components
    typedef typename WtoComponentRefList::const_iterator Iterator;

private:
    WtoComponentRefList _components;
    NodeRefList heads;
    NestingTable _nesting_table;
    DfnTable _dfn_table;
    Dfn _num;
    Stack _stack;

private:
    /// \brief Visitor to build the nestings of each node
    class NestingBuilder final
            : public WtoComponentVisitor<GraphT> {
    private:
        WtoNestingPtr _nesting;
        NestingTable &_nesting_table;

    public:
        explicit NestingBuilder(NestingTable &nesting_table)
                : _nesting(std::make_shared<WtoNestingT>()),
                  _nesting_table(nesting_table) {}

        void visit(const WtoCycleT &cycle) override {
            const NodeT *head = cycle.head();
            WtoNestingPtr previous_nesting = _nesting;
            _nesting_table.insert(std::make_pair(head, _nesting));
            _nesting = std::make_shared<WtoNestingT>(*_nesting);
            _nesting->add(head);
            for (auto it = cycle.begin(), et = cycle.end(); it != et; ++it) {
                (*it)->accept(*this);
            }
            _nesting = previous_nesting;
        }

        void visit(const WtoVertexT &vertex) override {
            _nesting_table.insert(
                    std::make_pair(vertex.node(), _nesting));
        }

    }; // end class NestingBuilder

private:
    /// \brief Return the depth-first number of the given node
    Dfn dfn(const NodeT *n) const {
        auto it = _dfn_table.find(n);
        if (it != _dfn_table.end()) {
            return it->second;
        } else {
            return 0;
        }
    }

    /// \brief Set the depth-first number of the given node
    void set_dfn(const NodeT *n, const Dfn &dfn) {
        auto res = _dfn_table.insert(std::make_pair(n, dfn));
        if (!res.second) {
            (res.first)->second = dfn;
        }
    }

    /// \brief Pop a node from the stack
    const NodeT *pop() {
        assert(!_stack.empty() && "empty stack");
        const NodeT *top = _stack.back();
        _stack.pop_back();
        return top;
    }

    /// \brief Push a node on the stack
    void push(const NodeT *n) { _stack.push_back(n); }

    /// \brief Create the cycle component for the given vertex
    WtoComponentPtr component(const NodeT *vertex) {
        heads.insert(vertex);
        WtoComponentRefList partition;
        for (auto it = vertex->OutEdgeBegin(), et = vertex->OutEdgeEnd(); it != et; ++it) {
            const NodeT *succ = (*it)->getDstNode();
            if (dfn(succ) == 0) {
                visit(succ, partition);
            }
        }
        return std::make_unique<WtoCycleT>(vertex, std::move(partition));
    }

    /// \brief Visit the given node
    ///
    /// Algorithm to build a weak topological order of a graph
    Dfn visit(const NodeT *vertex, WtoComponentRefList &partition) {
        Dfn head(0);
        Dfn min(0);
        bool loop;

        push(vertex);
        _num += Dfn(1);
        head = _num;
        set_dfn(vertex, head);
        loop = false;
        for (auto it = vertex->OutEdgeBegin(), et = vertex->OutEdgeEnd(); it != et; ++it) {
            const NodeT *succ = (*it)->getDstNode();
            Dfn succ_dfn = dfn(succ);
            if (succ_dfn == Dfn(0)) {
                min = visit(succ, partition);
            } else {
                min = succ_dfn;
            }
            if (min <= head) {
                head = min;
                loop = true;
            }
        }
        if (head == dfn(vertex)) {
            set_dfn(vertex, UINT_MAX);
            const NodeT *element = pop();
            if (loop) {
                while (element != vertex) {
                    set_dfn(element, 0);
                    element = pop();
                }
                partition.push_front(component(vertex));
            } else {
                partition.push_front(std::make_unique<WtoVertexT>(vertex));
            }
        }
        return head;
    }

    /// \brief Build the nesting table
    void build_nesting() {
        NestingBuilder builder(_nesting_table);
        for (auto it = begin(), et = end(); it != et; ++it) {
            (*it)->accept(builder);
        }
    }

public:
    /// \brief Compute the weak topological order of the given graph
    explicit Wto(const NodeT *entry) : _num(0) {
        visit(entry, _components);
        _dfn_table.clear();
        _stack.clear();
        build_nesting();
    }

    /// \brief No copy constructor
    Wto(const Wto &other) = delete;

    /// \brief Move constructor
    Wto(Wto &&other) = default;

    /// \brief No copy assignment operator
    Wto &operator=(const Wto &other) = delete;

    /// \brief Move assignment operator
    Wto &operator=(Wto &&other) = default;

    /// \brief Destructor
    ~Wto() = default;

    /// \brief Begin iterator over the components
    /// \brief Begin iterator over the components
    Iterator begin() const {
        return _components.cbegin();
    }

    /// \brief End iterator over the components
    Iterator end() const {
        return _components.cend();
    }

    bool isHead(const NodeT *node) const {
        return heads.find(node) != heads.end();
    }

    typename NodeRefList::const_iterator headBegin() const {
        return heads.cbegin();
    }

    /// \brief End iterator over the components
    typename NodeRefList::const_iterator headEnd() const {
        return heads.cend();
    }


    /// \brief Return the nesting of the given node
    const WtoNestingT &nesting(const NodeT *n) const {
        auto it = _nesting_table.find(n);
        assert(it != _nesting_table.end() && "node not found");
        return *(it->second);
    }

    /// \brief Accept the given visitor
    void accept(WtoComponentVisitor<GraphT> &v) {
        for (const auto &c: _components) {
            c->accept(v);
        }
    }

    /// \brief Dump the order, for debugging purpose
    std::string toString() const {
        std::string str;
        raw_string_ostream rawstr(str);
        rawstr << "[";
        for (auto it = begin(), et = end(); it != et;) {
            rawstr << (*it)->toString();
            ++it;
            if (it != et) {
                rawstr << ", ";
            }
        }
        rawstr << "]";
        return rawstr.str();
    }

}; // end class Wto

}

#endif  /* WTO_H_ */
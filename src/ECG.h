//===- ECG.h -- Graph for Finding Elementary Circle ----------------------------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2013-2017>  <Yulei Sui>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//
/*
 * ECG.h
 *
 * A tiny graph for the implementation of EC.h
 *
 * Created by Guanqin and Xiao on 11/6/2021
 */

#ifndef ECG_H
#define ECG_H

#include "MemoryModel/SVFIR.h"

namespace SVF {
    class ECGNode;

    typedef GenericEdge <ECGNode> GenericECGEdgeTy;

    class ECGEdge : public GenericECGEdgeTy {
    public:
        /// Constructor
        ECGEdge(ECGNode *s, ECGNode *d) : GenericECGEdgeTy(s, d, 0) {
        }

        /// Destructor
        ~ECGEdge() {
        }

        typedef GenericNode<ECGNode, ECGEdge>::GEdgeSetTy ECGEdgeSetTy;
        typedef ECGEdgeSetTy SVFGEdgeSetTy;

        /// Overloading operator << for dumping ICFG node ID
        //@{
        friend llvm::raw_ostream &operator<<(llvm::raw_ostream &o, const ECGEdge &edge) {
            o << edge.toString();
            return o;
        }
        //@}

        virtual const std::string toString() const {
            std::string str;
            raw_string_ostream rawstr(str);
            rawstr << "ECGEdge " << " [";
            rawstr << getDstID() << "<--" << getSrcID() << "\t";
            return rawstr.str();
        }
    };

    typedef GenericNode <ECGNode, ECGEdge> GenericECGNodeTy;

    class ECGNode : public GenericECGNodeTy {

    public:

        typedef ECGEdge::ECGEdgeSetTy::iterator iterator;
        typedef ECGEdge::ECGEdgeSetTy::const_iterator const_iterator;

    public:
        /// Constructor
        ECGNode(NodeID i) : GenericECGNodeTy(i, 0) {

        }

        /// Overloading operator << for dumping ICFG node ID
        //@{
        friend llvm::raw_ostream &operator<<(llvm::raw_ostream &o, const ECGNode &node) {
            o << node.toString();
            return o;
        }
        //@}


        virtual const std::string toString() const {
            std::string str;
            raw_string_ostream rawstr(str);
            rawstr << getId();
            return rawstr.str();
        }

    };

    typedef std::vector<std::pair<NodeID, NodeID>> NodePairVector;
    typedef GenericGraph <ECGNode, ECGEdge> GenericECGTy;

    class ECG : public GenericECGTy {

    public:

        typedef Map<NodeID, ECGNode *> ECGNodeIDToNodeMapTy;
        typedef ECGEdge::ECGEdgeSetTy ECGEdgeSetTy;
        typedef ECGNodeIDToNodeMapTy::iterator iterator;
        typedef ECGNodeIDToNodeMapTy::const_iterator const_iterator;

        NodeID totalECGNode;

    public:
        /// Constructor
        ECG() : totalECGNode(0) {

        }

        /// Destructor
        virtual ~ECG() {}

        /// Get a ECG node
        inline ECGNode *getECGNode(NodeID id) const {
            return getGNode(id);
        }

        /// Whether has the ECGNode
        inline bool hasECGNode(NodeID id) const {
            return hasGNode(id);
        }

        /// Whether we has a ECG edge
        //@{
        bool hasECGEdge(ECGNode *src, ECGNode *dst) {
            ECGEdge edge(src, dst);
            ECGEdge *outEdge = src->hasOutgoingEdge(&edge);
            ECGEdge *inEdge = dst->hasIncomingEdge(&edge);
            if (outEdge && inEdge) {
                assert(outEdge == inEdge && "edges not match");
                return true;
            } else
                return false;
        }
        //@}

        /// Get a SVFG edge according to src and dst
        ECGEdge *getECGEdge(const ECGNode *src, const ECGNode *dst) {
            ECGEdge *edge = nullptr;
            size_t counter = 0;
            for (ECGEdge::ECGEdgeSetTy::iterator iter = src->OutEdgeBegin();
                 iter != src->OutEdgeEnd(); ++iter) {
                if ((*iter)->getDstID() == dst->getId()) {
                    counter++;
                    edge = (*iter);
                }
            }
            assert(counter <= 1 && "there's more than one edge between two ECG nodes");
            return edge;
        }

        /// View graph from the debugger
        void view() {
            llvm::ViewGraph(this, "SSE Simple Graph");
        }

        /// Dump graph into dot file
        void dump(const std::string &filename) {
            GraphPrinter::WriteGraphToFile(SVFUtil::outs(), filename, this);
        }

    public:
        /// Remove a SVFG edge
        inline void removeECGEdge(ECGEdge *edge) {
            edge->getDstNode()->removeIncomingEdge(edge);
            edge->getSrcNode()->removeOutgoingEdge(edge);
            delete edge;
        }

        /// Remove a ECGNode
        inline void removeECGNode(ECGNode *node) {
            std::set<ECGEdge *> temp;
            for (ECGEdge *e: node->getInEdges())
                temp.insert(e);
            for (ECGEdge *e: node->getOutEdges())
                temp.insert(e);
            for (ECGEdge *e: temp) {
                removeECGEdge(e);
            }
            removeGNode(node);
        }

        /// Remove node from nodeID
        inline void removeECGNode(NodeID id) {
            if (hasECGNode(id))
                removeECGNode(getECGNode(id));
        }

        /// Add ECG edge
        inline bool addECGEdge(ECGEdge *edge) {
            bool added1 = edge->getDstNode()->addIncomingEdge(edge);
            bool added2 = edge->getSrcNode()->addOutgoingEdge(edge);
            assert(added1 && added2 && "edge not added??");
            return true;
        }

        /// Add a ECG node
        virtual inline void addECGNode(ECGNode *node) {
            addGNode(node->getId(), node);
        }

        /// Add ECG nodes from nodeid vector
        inline void addECGNodesFromVector(NodeVector nodes) {
            for (NodeID nodeId: nodes) {
                if (!IDToNodeMap.count(nodeId)) {
                    addGNode(nodeId, new ECGNode(nodeId));
                }
            }
        }

        /// Add ECG edges from nodeid pair
        inline void addECGEdgesFromSrcDst(NodeID src, NodeID dst) {
            if (!hasECGNode(src)) {
                addGNode(src, new ECGNode(src));
            }
            if (!hasECGNode(dst)) {
                addGNode(dst, new ECGNode(dst));
            }
            if (!hasECGEdge(getECGNode(src), getECGNode(dst))) {
                addECGEdge(new ECGEdge(getECGNode(src), getECGNode(dst)));
                incEdgeNum();
            }
        }

        /// Add ECG edges from nodeid pair vector
        inline void addECGEdgesFromVector(NodePairVector nodePairs) {
            for (auto nodePair: nodePairs) {
                addECGEdgesFromSrcDst(nodePair.first, nodePair.second);
            }
        }

    };
} // end namespace SVF

namespace llvm {
/* !
 * GraphTraits specializations for generic graph algorithms.
 * Provide graph traits for traversing from a constraint node using standard graph traversals.
 */
    template<>
    struct GraphTraits<SVF::ECGNode *> : public GraphTraits<SVF::GenericNode<SVF::ECGNode, SVF::ECGEdge> *> {
    };

/// Inverse GraphTraits specializations for call graph node, it is used for inverse traversal.
    template<>
    struct GraphTraits<Inverse < SVF::ECGNode * > > : public GraphTraits<
            Inverse < SVF::GenericNode<SVF::ECGNode, SVF::ECGEdge> * > > {
};

template<>
struct GraphTraits<SVF::ECG *> : public GraphTraits<SVF::GenericGraph<SVF::ECGNode, SVF::ECGEdge> *> {
    typedef SVF::ECGNode *NodeRef;
};

/*!
 * Write value flow graph into dot file for debugging
 */
template<>
struct DOTGraphTraits<SVF::ECG *> : public DefaultDOTGraphTraits {

    typedef SVF::ECGNode NodeType;
    typedef NodeType::iterator ChildIteratorType;

    DOTGraphTraits(bool isSimple = false) :
            DefaultDOTGraphTraits(isSimple) {
    }

    /// Return name of the graph
    static std::string getGraphName(SVF::ECG *) {
        return "Graph";
    }

    /// Return function name;
    static std::string getNodeLabel(SVF::ECGNode *node, SVF::ECG *) {
        return node->toString();
    }

    static std::string getNodeAttributes(SVF::ECGNode *node, SVF::ECG *) {
        return "shape=circle";
    }

    template<class EdgeIter>
    static std::string getEdgeAttributes(SVF::ECGNode *, EdgeIter EI, SVF::ECG *) {

        //TODO: mark indirect call of Fork with different color
        SVF::ECGEdge *edge = *(EI.getCurrent());
        assert(edge && "No edge found!!");

        return "color=black";
    }
};

} // End namespace llvm

#endif //ECG_H

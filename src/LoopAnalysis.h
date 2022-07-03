//
// Created by Xiao on 7/3/2022.
//

#ifndef LOOPANALYSIS_LOOPANALYSIS_H
#define LOOPANALYSIS_LOOPANALYSIS_H

#include "ECG.h"
#include "WTO.hpp"

namespace SVF {

template<typename GraphT>
struct GraphTrait {
    typedef typename  GraphT::NodeType *NodeRef;
    typedef typename  GraphT::NodeType::iterator SuccessorNodeIterator;
    typedef typename  GraphT::NodeType::iterator PredecessorNodeIterator;

    static SuccessorNodeIterator successor_begin(NodeRef node) {
        return node->OutEdgeBegin();
    }

    static SuccessorNodeIterator successor_end(NodeRef node) {
        return node->OutEdgeEnd();
    }

    static PredecessorNodeIterator predecessor_begin(NodeRef node) {
        return node->InEdgeBegin();
    }

    static PredecessorNodeIterator predecessor_end(NodeRef node) {
        return node->InEdgeEnd();
    }
};

template<typename GraphT>
class LoopAnalysis {
public:
    typedef typename  GraphT::NodeType NodeT;
    typedef typename  GraphT::EdgeType EdgeT;
    typedef Wto<GraphT> WtoT;
    typedef Set<const EdgeT*> EdgeRefList;
    typedef Map<const NodeT*, EdgeRefList> NodeRefToEdgeListMap;
    typedef typename WtoT::WtoNestingT WtoNestingT;
private:
    GraphT *graph;
    WtoT wtoT;
    NodeRefToEdgeListMap headToEntryEdges;
    NodeRefToEdgeListMap headToBackEdges;

public:
    LoopAnalysis(GraphT *_graph, const NodeT *entry) : graph(_graph), wtoT(entry) {}

    virtual ~LoopAnalysis() = default;

    LoopAnalysis(const LoopAnalysis &) = delete;

    LoopAnalysis(const LoopAnalysis &&) = delete;

    LoopAnalysis operator=(const LoopAnalysis &) = delete;

    LoopAnalysis operator=(LoopAnalysis &&) = delete;

    virtual void run() {
        for (auto it = wtoT.headBegin(), eit = wtoT.headEnd(); it != eit; ++it) {
            const NodeT *head = *it;
            const WtoNestingT& cycle_nesting = wtoT.nesting(head);
            for (const auto &edge: head->getInEdges()) {
                const NodeT *pre = edge->getSrcNode();
                if (wtoT.nesting(pre) <= cycle_nesting) {
                    headToEntryEdges[head].insert(edge);
                } else {
                    headToBackEdges[head].insert(edge);
                }
            }
        }
    }

    const WtoT& wto() const {
        return wtoT;
    }

    bool isHead(const NodeT* node) const {
        return wtoT.isHead(node);
    }

    bool isHead(u32_t nodeId) const {
        return wtoT.isHead(graph->getGNode(nodeId));
    }

    EdgeRefList& getBackEdgeRefList(const NodeT* node) {
        assert(isHead(node) && "not head!");
        auto it = headToBackEdges.find(node);
        assert(it != headToBackEdges.end() && "head does not have back edge!");
        return it->second;
    }

    EdgeRefList& getEntryEdgeRefList(const NodeT* node) {
        assert(isHead(node) && "not head!");
        auto it = headToEntryEdges.find(node);
        assert(it != headToEntryEdges.end() && "head does not have entry edge!");
        return it->second;
    }

    EdgeRefList& getBackEdgeRefList(u32_t nodeId) {
        const NodeT* node = graph->getGNode(nodeId);
        assert(isHead(node) && "not head!");
        auto it = headToBackEdges.find(node);
        assert(it != headToBackEdges.end() && "head does not have back edge!");
        return it->second;
    }

    EdgeRefList& getEntryEdgeRefList(u32_t nodeId) {
        const NodeT* node = graph->getGNode(nodeId);
        assert(isHead(node) && "not head!");
        auto it = headToEntryEdges.find(node);
        assert(it != headToEntryEdges.end() && "head does not have entry edge!");
        return it->second;
    }

};
}


#endif //LOOPANALYSIS_LOOPANALYSIS_H

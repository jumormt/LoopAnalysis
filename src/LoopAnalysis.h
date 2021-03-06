//
// Created by Xiao on 7/3/2022.
//

#ifndef LOOPANALYSIS_LOOPANALYSIS_H
#define LOOPANALYSIS_LOOPANALYSIS_H

#include "ECG.h"
#include "WTO.hpp"

namespace SVF {

template<typename GraphT>
class LoopAnalysis {
public:
    typedef typename  GraphT::NodeType NodeT;
    typedef typename  GraphT::EdgeType EdgeT;
    typedef Wto<GraphT> WtoT;
    typedef Set<const EdgeT*> EdgeRefList;
    typedef Map<const NodeT*, EdgeRefList> NodeRefToEdgeListMap;
    typedef typename WtoT::WtoNestingT WtoNestingT;
    typedef typename WtoT::NodeRefList NodeRefList;
private:
    GraphT *graph;
    WtoT wtoT;
    NodeRefToEdgeListMap headToEntryEdges;
    NodeRefToEdgeListMap headToBackEdges;
    NodeRefToEdgeListMap tailToExitEdges;

public:
    LoopAnalysis(GraphT *_graph, const NodeT *entry) : graph(_graph), wtoT(entry) {}

    virtual ~LoopAnalysis() = default;

    LoopAnalysis(const LoopAnalysis &) = delete;

    LoopAnalysis(const LoopAnalysis &&) = delete;

    LoopAnalysis operator=(const LoopAnalysis &) = delete;

    LoopAnalysis operator=(LoopAnalysis &&) = delete;

    virtual void run();

    const WtoT& wto() const {
        return wtoT;
    }

    bool isHead(const NodeT* node) const {
        return wtoT.isHead(node);
    }

    bool isTail(const NodeT* node) const {
        return tailToExitEdges.find(node) != tailToExitEdges.end();
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

    const NodeRefList& getTails(u32_t nodeId) {
        const NodeT* node = graph->getGNode(nodeId);
        return getTails(node);
    }

    const NodeRefList& getTails(const NodeT* node) {
        return wtoT.getTails(node);
    }

    EdgeRefList& getExitEdgeRefList(u32_t nodeId) {
        const NodeT* node = graph->getGNode(nodeId);
        assert(isTail(node) && "not tail!");
        auto it = tailToExitEdges.find(node);
        assert(it != tailToExitEdges.end() && "head does not have entry edge!");
        return it->second;
    }

    EdgeRefList& getExitEdgeRefList(const NodeT* node) {
        assert(isTail(node) && "not tail!");
        auto it = tailToExitEdges.find(node);
        assert(it != tailToExitEdges.end() && "head does not have entry edge!");
        return it->second;
    }
};
}


#endif //LOOPANALYSIS_LOOPANALYSIS_H

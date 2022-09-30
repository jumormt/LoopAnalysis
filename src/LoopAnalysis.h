//
// Created by Xiao on 7/3/2022.
//

#ifndef LOOPANALYSIS_LOOPANALYSIS_H
#define LOOPANALYSIS_LOOPANALYSIS_H

#include "ECG.h"
#include "WTO.hpp"

namespace SVF {

template<typename GraphT, typename WtoT>
class LoopAnalysis {
public:
    typedef typename GraphT::NodeType NodeT;
    typedef typename GraphT::EdgeType EdgeT;
    typedef Set<const EdgeT *> EdgeRefSet;
    typedef Set<const NodeT *> NodeRefSet;
    typedef Map<const NodeT *, EdgeRefSet> NodeRefToEdgeRefSetMap;
    typedef typename WtoT::WtoNestingT WtoNestingT;
    typedef Map<const EdgeT *, NodeRefSet> EdgeRefToNodeRefSetMap;

private:
    GraphT *_graph;
    WtoT _wtoT;
    NodeRefToEdgeRefSetMap _headToEntryEdges;
    NodeRefToEdgeRefSetMap _headToBackEdges;
    NodeRefToEdgeRefSetMap _tailToExitEdges;
    NodeRefToEdgeRefSetMap _tailToInEdges;
    EdgeRefToNodeRefSetMap _inEdgeToHeads;
    EdgeRefToNodeRefSetMap _exitEdgeToHeads;
    EdgeRefSet _entryEdges;
    EdgeRefSet _backEdges;
    EdgeRefSet _inEdges;
    EdgeRefSet _exitEdges;

public:

    LoopAnalysis() {}

    LoopAnalysis(GraphT *_graph, const NodeT *entry) : _graph(_graph), _wtoT(entry) {}

    virtual ~LoopAnalysis() = default;

    LoopAnalysis(const LoopAnalysis &) = default;

    LoopAnalysis(LoopAnalysis &&) noexcept = default;

    LoopAnalysis &operator=(const LoopAnalysis &) = default;

    LoopAnalysis &operator=(LoopAnalysis &&) noexcept = default;

    virtual void run();

    const NodeRefToEdgeRefSetMap &getHeadToBackEdges() const {
        return _headToBackEdges;
    }

    const WtoT &wto() const {
        return _wtoT;
    }

    const GraphT *graph() const {
        return _graph;
    }

    bool isHead(const NodeT *node) const {
        return _wtoT.isHead(node);
    }

    bool isTail(const NodeT *node) const {
        return _tailToExitEdges.find(node) != _tailToExitEdges.end();
    }

    bool isEntryEdge(const EdgeT *edge) const {
        return _entryEdges.find(edge) != _entryEdges.end();
    }

    bool isBackEdge(const EdgeT *edge) const {
        return _backEdges.find(edge) != _backEdges.end();
    }

    bool isInEdge(const EdgeT *edge) const {
        return _inEdges.find(edge) != _inEdges.end();
    }

    bool isExitEdge(const EdgeT *edge) const {
        return _exitEdges.find(edge) != _exitEdges.end();
    }

    bool isHead(u32_t nodeId) const {
        return _wtoT.isHead(_graph->getGNode(nodeId));
    }

    NodeRefSet &getInEdgeHeads(const EdgeT *backEdge) {
        auto it = _inEdgeToHeads.find(backEdge);
        assert(it != _inEdgeToHeads.end() && "not in edge!");
        return it->second;
    }

    NodeRefSet &getExitEdgeHeads(const EdgeT *backEdge) {
        auto it = _exitEdgeToHeads.find(backEdge);
        assert(it != _exitEdgeToHeads.end() && "not exit edge!");
        return it->second;
    }

    EdgeRefSet &getBackEdgeRefList(const NodeT *node) {
        assert(isHead(node) && "not head!");
        auto it = _headToBackEdges.find(node);
        assert(it != _headToBackEdges.end() && "head does not have back edge!");
        return it->second;
    }

    EdgeRefSet &getEntryEdgeRefList(const NodeT *node) {
        assert(isHead(node) && "not head!");
        auto it = _headToEntryEdges.find(node);
        assert(it != _headToEntryEdges.end() && "head does not have entry edge!");
        return it->second;
    }

    EdgeRefSet &getBackEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isHead(node) && "not head!");
        auto it = _headToBackEdges.find(node);
        assert(it != _headToBackEdges.end() && "head does not have back edge!");
        return it->second;
    }

    EdgeRefSet &getEntryEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isHead(node) && "not head!");
        auto it = _headToEntryEdges.find(node);
        assert(it != _headToEntryEdges.end() && "head does not have entry edge!");
        return it->second;
    }

    const NodeRefSet &getTails(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        return getTails(node);
    }

    const NodeRefSet &getTails(const NodeT *node) {
        return _wtoT.getTails(node);
    }

    EdgeRefSet &getExitEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isTail(node) && "not tail!");
        auto it = _tailToExitEdges.find(node);
        assert(it != _tailToExitEdges.end() && "tail does not have exit edge!");
        return it->second;
    }

    EdgeRefSet &getExitEdgeRefList(const NodeT *node) {
        assert(isTail(node) && "not tail!");
        auto it = _tailToExitEdges.find(node);
        assert(it != _tailToExitEdges.end() && "tail does not have exit edge!");
        return it->second;
    }

    EdgeRefSet &getInEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isTail(node) && "not tail!");
        auto it = _tailToInEdges.find(node);
        assert(it != _tailToInEdges.end() && "tail does not have in edge!");
        return it->second;
    }

    EdgeRefSet &getInEdgeRefList(const NodeT *node) {
        assert(isTail(node) && "not tail!");
        auto it = _tailToInEdges.find(node);
        assert(it != _tailToInEdges.end() && "tail does not have in edge!");
        return it->second;
    }
};
}


#endif //LOOPANALYSIS_LOOPANALYSIS_H

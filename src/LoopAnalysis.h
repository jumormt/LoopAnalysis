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
    typedef typename GraphT::NodeType NodeT;
    typedef typename GraphT::EdgeType EdgeT;
    typedef Wto<GraphT> WtoT;
    typedef Set<const EdgeT *> EdgeRefList;
    typedef Map<const NodeT *, EdgeRefList> NodeRefToEdgeListMap;
    typedef typename WtoT::WtoNestingT WtoNestingT;
    typedef typename WtoT::NodeRefList NodeRefList;
private:
    GraphT *_graph;
    WtoT _wtoT;
    NodeRefToEdgeListMap _headToEntryEdges;
    NodeRefToEdgeListMap _headToBackEdges;
    NodeRefToEdgeListMap _tailToExitEdges;
    NodeRefToEdgeListMap _tailToInEdges;
    EdgeRefList _entryEdges;
    EdgeRefList _backEdges;
    EdgeRefList _inEdges;
    EdgeRefList _exitEdges;

public:
    LoopAnalysis(GraphT *_graph, const NodeT *entry) : _graph(_graph), _wtoT(entry) {}

    virtual ~LoopAnalysis() = default;

    LoopAnalysis(const LoopAnalysis &) = delete;

    LoopAnalysis(const LoopAnalysis &&) = delete;

    LoopAnalysis operator=(const LoopAnalysis &) = delete;

    LoopAnalysis operator=(LoopAnalysis &&) = delete;

    virtual void run();

    const WtoT &wto() const {
        return _wtoT;
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

    EdgeRefList &getBackEdgeRefList(const NodeT *node) {
        assert(isHead(node) && "not head!");
        auto it = _headToBackEdges.find(node);
        assert(it != _headToBackEdges.end() && "head does not have back edge!");
        return it->second;
    }

    EdgeRefList &getEntryEdgeRefList(const NodeT *node) {
        assert(isHead(node) && "not head!");
        auto it = _headToEntryEdges.find(node);
        assert(it != _headToEntryEdges.end() && "head does not have entry edge!");
        return it->second;
    }

    EdgeRefList &getBackEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isHead(node) && "not head!");
        auto it = _headToBackEdges.find(node);
        assert(it != _headToBackEdges.end() && "head does not have back edge!");
        return it->second;
    }

    EdgeRefList &getEntryEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isHead(node) && "not head!");
        auto it = _headToEntryEdges.find(node);
        assert(it != _headToEntryEdges.end() && "head does not have entry edge!");
        return it->second;
    }

    const NodeRefList &getTails(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        return getTails(node);
    }

    const NodeRefList &getTails(const NodeT *node) {
        return _wtoT.getTails(node);
    }

    EdgeRefList &getExitEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isTail(node) && "not tail!");
        auto it = _tailToExitEdges.find(node);
        assert(it != _tailToExitEdges.end() && "tail does not have exit edge!");
        return it->second;
    }

    EdgeRefList &getExitEdgeRefList(const NodeT *node) {
        assert(isTail(node) && "not tail!");
        auto it = _tailToExitEdges.find(node);
        assert(it != _tailToExitEdges.end() && "tail does not have exit edge!");
        return it->second;
    }

    EdgeRefList &getInEdgeRefList(u32_t nodeId) {
        const NodeT *node = _graph->getGNode(nodeId);
        assert(isTail(node) && "not tail!");
        auto it = _tailToInEdges.find(node);
        assert(it != _tailToInEdges.end() && "tail does not have in edge!");
        return it->second;
    }

    EdgeRefList &getInEdgeRefList(const NodeT *node) {
        assert(isTail(node) && "not tail!");
        auto it = _tailToInEdges.find(node);
        assert(it != _tailToInEdges.end() && "tail does not have in edge!");
        return it->second;
    }
};
}


#endif //LOOPANALYSIS_LOOPANALYSIS_H

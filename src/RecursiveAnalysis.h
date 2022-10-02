//
// Created by Xiao on 9/30/2022.
//

#ifndef LOOPANALYSIS_RECURSIVEANALYSIS_H
#define LOOPANALYSIS_RECURSIVEANALYSIS_H

#include "Graphs/ICFG.h"
#include "Graphs/PTACallGraph.h"
#include "LoopAnalysis.h"

namespace SVF {
class RecursiveAnalysis {

public:
    typedef SVF::LoopAnalysis<PTACallGraph, Wto<PTACallGraph>> CallLoopAnalysis;
    typedef typename ICFG::NodeType NodeT;
    typedef typename ICFG::EdgeType EdgeT;
    typedef Set<const EdgeT *> EdgeRefSet;
    typedef Map<const NodeT *, EdgeRefSet> NodeRefToEdgeSetMap;

private:
    CallLoopAnalysis _callLoopAnalysis;

    NodeRefToEdgeSetMap _headToEntryEdges;
    NodeRefToEdgeSetMap _headToBackEdges;
    NodeRefToEdgeSetMap _tailToExitEdges;
    NodeRefToEdgeSetMap _tailToInEdges;
    EdgeRefSet _entryEdges;
    EdgeRefSet _backEdges;
    EdgeRefSet _inEdges;
    EdgeRefSet _exitEdges;
public:
    RecursiveAnalysis(PTACallGraph *ptaCallGraph, const PTACallGraphNode *entry) : _callLoopAnalysis(ptaCallGraph,
                                                                                                     entry) {}

    virtual ~RecursiveAnalysis() = default;

    RecursiveAnalysis(const RecursiveAnalysis &) = default;

    RecursiveAnalysis(RecursiveAnalysis &&) noexcept = default;

    RecursiveAnalysis &operator=(const RecursiveAnalysis &) = default;

    RecursiveAnalysis &operator=(RecursiveAnalysis &&) noexcept = default;

    bool isHead(const NodeT* node) const {
        return _headToEntryEdges.find(node) != _headToEntryEdges.end();
    }

    bool isRecurEntryEdge(const EdgeT *edge) const {
        return _entryEdges.find(edge) != _entryEdges.end();
    }

    bool isRecurBackEdge(const EdgeT *edge) const {
        return _backEdges.find(edge) != _backEdges.end();
    }

    void run();


private:


}; // end class RecursiveAnalysis
} // end namespace SVF


#endif //LOOPANALYSIS_RECURSIVEANALYSIS_H

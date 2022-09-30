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
    typedef Set<const EdgeT *> EdgeRefList;
    typedef Map<const NodeT *, EdgeRefList> NodeRefToEdgeListMap;

private:
    CallLoopAnalysis _callLoopAnalysis;

    NodeRefToEdgeListMap _headToEntryEdges;
    NodeRefToEdgeListMap _headToBackEdges;
    NodeRefToEdgeListMap _tailToExitEdges;
    NodeRefToEdgeListMap _tailToInEdges;
    EdgeRefList _entryEdges;
    EdgeRefList _backEdges;
    EdgeRefList _inEdges;
    EdgeRefList _exitEdges;
public:
    RecursiveAnalysis(PTACallGraph *ptaCallGraph, const PTACallGraphNode *entry) : _callLoopAnalysis(ptaCallGraph,
                                                                                                     entry) {}

    virtual ~RecursiveAnalysis() = default;

    RecursiveAnalysis(const RecursiveAnalysis &) = default;

    RecursiveAnalysis(RecursiveAnalysis &&) noexcept = default;

    RecursiveAnalysis &operator=(const RecursiveAnalysis &) = default;

    RecursiveAnalysis &operator=(RecursiveAnalysis &&) noexcept = default;

    void run();


private:


}; // end class RecursiveAnalysis
} // end namespace SVF


#endif //LOOPANALYSIS_RECURSIVEANALYSIS_H

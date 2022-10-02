//
// Created by Xiao on 9/30/2022.
//

#include "RecursiveAnalysis.h"
#include "LoopAnalysis.cpp"

using namespace SVF;

void RecursiveAnalysis::run() {
    _callLoopAnalysis.run();
    const auto &headToBacks = _callLoopAnalysis.getHeadToBackEdges();
    const auto &headToEntrys = _callLoopAnalysis.getHeadToEntryEdges();
    ICFG *icfg = PAG::getPAG()->getICFG();
    const PTACallGraph* ptaCallGraph = _callLoopAnalysis.graph();
    for (const auto &item: headToBacks) {
        const ICFGNode *head = icfg->getFunEntryICFGNode(item.first->getFunction());
        const ICFGNode *revHead = icfg->getFunExitICFGNode(item.first->getFunction());
        auto it = headToEntrys.find(item.first);
        if(it == headToEntrys.end()) continue;
        assert(it != headToEntrys.end());
        for (const auto &entry: it->second) {
            const CallICFGNode *callNode = ptaCallGraph->getCallSite(entry->getCallSiteID());
            const RetICFGNode *retNode = callNode->getRetICFGNode();
            const ICFGEdge *callEdge = icfg->getICFGEdge(callNode, head, ICFGEdge::CallCF);
            const ICFGEdge *retEdge = icfg->getICFGEdge(revHead, retNode, ICFGEdge::RetCF);
            _headToEntryEdges[head].insert(callEdge);
            _headToEntryEdges[revHead].insert(retEdge);
            _entryEdges.insert(callEdge);
            _entryEdges.insert(retEdge);
        }

        for (const auto &back: item.second) {
            const CallICFGNode *callNode = ptaCallGraph->getCallSite(back->getCallSiteID());
            const RetICFGNode *retNode = callNode->getRetICFGNode();
            const ICFGEdge *callEdge = icfg->getICFGEdge(callNode, head, ICFGEdge::CallCF);
            const ICFGEdge *retEdge = icfg->getICFGEdge(revHead, retNode, ICFGEdge::RetCF);
            _headToBackEdges[head].insert(callEdge);
            _headToBackEdges[revHead].insert(retEdge);
            _backEdges.insert(callEdge);
            _backEdges.insert(retEdge);
        }
    }
}
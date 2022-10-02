//
// Created by chengxiao on 2/10/22.
//

#include "InterLoopFlattenHandler.h"
#include "Util/SVFUtil.h"
#include "LoopAnalysis.cpp"
#include "ICFGWrapper.h"
#include "WPA/Andersen.h"

using namespace SVF;
using namespace SVFUtil;

typedef SVF::LoopAnalysis<PTACallGraph, Wto<PTACallGraph>> CallLoopAnalysis;

void InterLoopFlattenHandler::runOnModule(SVF::SVFModule *svfModule) {
    SVFIR *svfir = PAG::getPAG();
    ICFG *icfg = svfir->getICFG();
    AndersenWaveDiff *ander = AndersenWaveDiff::createAndersenWaveDiff(svfir);
    PTACallGraph *ptaCallGraph = ander->getPTACallGraph();
    PTACallGraphNode *callEntry;
    for (const auto &func: *svfModule) {
        if (SVFUtil::isExtCall(func)) continue;
        if (func->getName() == "main")
            callEntry = ptaCallGraph->getCallGraphNode(func);
    }
    CallLoopAnalysis callLoopAnalysis(ptaCallGraph, callEntry);
    callLoopAnalysis.run();
    ICFGWrapper *icfgWrapper = ICFGWrapper::getICFGWrapper().get();
    const auto &headToBacks = callLoopAnalysis.getHeadToBackEdges();
    const auto &headToEntrys = callLoopAnalysis.getHeadToEntryEdges();
    for (const auto &item: headToBacks) {
        const ICFGNode *head = icfg->getFunEntryICFGNode(item.first->getFunction());
        ICFGNodeWrapper *headWrapper = icfgWrapper->getICFGNodeWrapper(head->getId());
        const ICFGNode *revHead = icfg->getFunExitICFGNode(item.first->getFunction());
        ICFGNodeWrapper *revHeadWrapper = icfgWrapper->getICFGNodeWrapper(revHead->getId());

        for (const auto &back: item.second) {
            const CallICFGNode *callNode = ptaCallGraph->getCallSite(back->getCallSiteID());
            ICFGNodeWrapper *callNodeWraper = icfgWrapper->getICFGNodeWrapper(callNode->getId());
            const RetICFGNode *retNode = callNode->getRetICFGNode();
            ICFGNodeWrapper *retNodeWraper = icfgWrapper->getICFGNodeWrapper(retNode->getId());
            // Prune the recursive call and return edge
            icfgWrapper->addICFGEdgeWrapper(new ICFGEdgeWrapper(callNodeWraper, retNodeWraper, nullptr));
            if (ICFGEdgeWrapper *edgeWrapper = icfgWrapper->hasICFGEdgeWrapper(callNodeWraper, headWrapper))
                icfgWrapper->removeICFGEdgeWrapper(edgeWrapper);
            if (ICFGEdgeWrapper *edgeWrapper = icfgWrapper->hasICFGEdgeWrapper(revHeadWrapper, retNodeWraper))
                icfgWrapper->removeICFGEdgeWrapper(edgeWrapper);

        }
    }
//    icfgWrapper->dump("ICFGWrapper");

}


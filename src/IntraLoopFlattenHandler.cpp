//
// Created by chengxiao on 2/10/22.
//

#include "IntraLoopFlattenHandler.h"
#include "Util/SVFUtil.h"
#include "LoopAnalysis.cpp"
#include "ICFGWrapper.h"
#include "ICFGWrapperWto.hpp"

using namespace SVF;
using namespace SVFUtil;
typedef LoopAnalysis<ICFGWrapper, ICFGWrapperWto> ICFGWrapperLoopAnalysis;


void IntraLoopFlattenHandler::runOnModule(SVFModule *svfModule) {
    ICFG *icfg = PAG::getPAG()->getICFG();
    ICFGWrapper *icfgWrapper = ICFGWrapper::getICFGWrapper().get();
    ICFGNodeWrapper *entry;
    Map<const SVFFunction *, ICFGWrapperLoopAnalysis> funcToICFGLoopAnalysis;
    for (const auto &func: *svfModule) {
        outs() << func->getName() << "\n";
        if (SVFUtil::isExtCall(func)) continue;
        entry = icfgWrapper->getICFGNodeWrapper(icfg->getFunEntryICFGNode(func)->getId());
        while (true) {
            ICFGWrapperLoopAnalysis loopAnalysis(icfgWrapper, entry);
            loopAnalysis.run();
            if (loopAnalysis.empty()) break;
            Set<ICFGEdgeWrapper *> edgesToRm;
            for (const auto &it: loopAnalysis.getHeadToBackEdges()) {
                Set<ICFGNodeWrapper *> srcNodes, tgtNodes;
                for (const auto &edge: it.second) {
                    edgesToRm.insert(const_cast<ICFGEdgeWrapper *>(edge));
                    srcNodes.insert(edge->getSrcNode());
                }
                for (const auto &tail: loopAnalysis.getTails(it.first)) {
                    for (const auto &edge: loopAnalysis.getExitEdgeRefList(tail)) {
                        edgesToRm.insert(const_cast<ICFGEdgeWrapper *>(edge));
                        tgtNodes.insert(edge->getDstNode());
                    }
                }

                for (const auto &srcNode: srcNodes) {
                    for (const auto &tgtNode: tgtNodes) {
                        if (ICFGEdgeWrapper *edgeWrapper = icfgWrapper->hasICFGEdgeWrapper(srcNode, tgtNode)) {
                            edgeWrapper->setICFGEdge(nullptr);
//                            edgesToRm.erase(edgeWrapper);
                        } else {
                            icfgWrapper->addICFGEdgeWrapper(new ICFGEdgeWrapper(srcNode, tgtNode, nullptr));
                        }
                    }
                }
            }
            for (const auto &edge: edgesToRm) {
                if (edge && edge->getSrcNode() && edge->getDstNode() &&
                    icfgWrapper->hasICFGEdgeWrapper(edge->getSrcNode(), edge->getDstNode()))
                    icfgWrapper->removeICFGEdgeWrapper(edge);
            }
        }
    }
    ICFGWrapper::getICFGWrapper()->dump("ICFGWrapper");
}
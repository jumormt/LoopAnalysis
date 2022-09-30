//
// Created by Xiao on 9/30/2022.
//

#include "RecursiveAnalysis.h"

using namespace SVF;

void RecursiveAnalysis::run() {
    _callLoopAnalysis.run();
    const auto &headToBacks = _callLoopAnalysis.getHeadToBackEdges();
    const PTACallGraph* ptaCallGraph = _callLoopAnalysis.graph();
    for (const auto &item: headToBacks) {
//        item.
    }
}
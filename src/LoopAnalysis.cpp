//
// Created by Xiao on 7/3/2022.
//

#include "LoopAnalysis.h"
using namespace SVF;


template<typename GraphT>
void LoopAnalysis<GraphT>::run() {
    for (auto it = wtoT.headBegin(), eit = wtoT.headEnd(); it != eit; ++it) {
        const NodeT *head = it->first;
        const WtoNestingT& cycle_nesting = wtoT.nesting(head);
        for (const auto &edge: head->getInEdges()) {
            const NodeT *pre = edge->getSrcNode();
            if (wtoT.nesting(pre) <= cycle_nesting) {
                headToEntryEdges[head].insert(edge);
            } else {
                headToBackEdges[head].insert(edge);
            }
            for (const auto &tail: wtoT.getTails(head)) {
                const WtoNestingT &tail_nesting = wtoT.nesting(tail);
                for (const auto &e: tail->getOutEdges()) {
                    const NodeT *out = e->getDstNode();
                    if (wtoT.nesting(out) < tail_nesting) {
                        tailToExitEdges[tail].insert(e);
                    }
                }
            }
        }
    }
}
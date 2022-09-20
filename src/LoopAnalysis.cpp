//
// Created by Xiao on 7/3/2022.
//

#include "LoopAnalysis.h"

using namespace SVF;


template<typename GraphT>
void LoopAnalysis<GraphT>::run() {
    for (auto it = _wtoT.headBegin(), eit = _wtoT.headEnd(); it != eit; ++it) {
        const NodeT *head = it->first;
        const WtoNestingT &cycle_nesting = _wtoT.nesting(head);
        for (const auto &edge: head->getInEdges()) {
            const NodeT *pre = edge->getSrcNode();
            if(!_wtoT.in_nesting_table(pre)) continue;
            if (_wtoT.nesting(pre) <= cycle_nesting) {
                _headToEntryEdges[head].insert(edge);
                _entryEdges.insert(edge);
            } else {
                _headToBackEdges[head].insert(edge);
                _backEdges.insert(edge);
            }
            for (const auto &tail: _wtoT.getTails(head)) {
                const WtoNestingT &tail_nesting = _wtoT.nesting(tail);
                for (const auto &e: tail->getOutEdges()) {
                    const NodeT *out = e->getDstNode();
                    if (out == head) {
                        _tailToInEdges[tail].insert(e);
                        _inEdges.insert(e);
                    } else {
                        if(!_wtoT.in_nesting_table(out)) continue;
                        if (_wtoT.nesting(out) < tail_nesting) {
                            _tailToExitEdges[tail].insert(e);
                            _exitEdges.insert(e);
                        } else {
                            _tailToInEdges[tail].insert(e);
                            _inEdges.insert(e);
                        }
                    }
                }
            }
        }
    }
}
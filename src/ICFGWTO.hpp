#include "WTO.hpp"

#include "Graphs/ICFG.h"

namespace SVF {

class ICFGWTO : public Wto<ICFG> {

public:
    /// \brief Compute the weak topological order of the given graph
    explicit ICFGWTO(const ICFGNode *entry) {
        visit(entry, _components);
        _dfn_table.clear();
        _stack.clear();
        build_nesting();
        build_tails();
    }

private:
    Dfn visit(const ICFGNode *vertex, Wto<SVF::ICFG>::WtoComponentRefList &partition) override {
        Dfn head(0);
        Dfn min(0);
        bool loop;

        push(vertex);
        _num += Dfn(1);
        head = _num;
        set_dfn(vertex, head);
        loop = false;
        for (auto it = vertex->OutEdgeBegin(), et = vertex->OutEdgeEnd(); it != et; ++it) {

            const NodeT *succ = (*it)->getDstNode();
            if (const CallICFGNode *callICFGNode = SVFUtil::dyn_cast<CallICFGNode>(vertex))
                succ = callICFGNode->getRetICFGNode();

            Dfn succ_dfn = dfn(succ);
            if (succ_dfn == Dfn(0)) {
                min = visit(succ, partition);
            } else {
                min = succ_dfn;
            }
            if (min <= head) {
                head = min;
                loop = true;
            }
        }
        if (head == dfn(vertex)) {
            set_dfn(vertex, UINT_MAX);
            const NodeT *element = pop();
            if (loop) {
                while (element != vertex) {
                    set_dfn(element, 0);
                    element = pop();
                }
                partition.push_front(component(vertex));
            } else {
                partition.push_front(newVertex(vertex));
            }
        }
        return head;
    }
};
}
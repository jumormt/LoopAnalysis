#include "WTO.hpp"

#include "ICFGWrapper.h"

namespace SVF {

class ICFGWrapperWto : public Wto<ICFGWrapper> {

public:
    /// \brief Compute the weak topological order of the given graph
    explicit ICFGWrapperWto() {}

    /// \brief Compute the weak topological order of the given graph
    explicit ICFGWrapperWto(const ICFGNodeWrapper *entry) {
        visit(entry, _components);
        _dfn_table.clear();
        _stack.clear();
        build_nesting();
        build_tails();
    }

private:
    Dfn visit(const ICFGNodeWrapper *vertex, Wto<SVF::ICFGWrapper>::WtoComponentRefList &partition) override {
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
            if (const CallICFGNode *callICFGNode = SVFUtil::dyn_cast<CallICFGNode>(vertex->getICFGNode()))
                succ = ICFGWrapper::getICFGWrapper()->getICFGNodeWrapper(callICFGNode->getRetICFGNode()->getId());
            if (SVFUtil::isa<FunExitICFGNode>(vertex->getICFGNode()))
                continue;
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

private:
    void build_tails() override {
        for (const auto &head: headRefToCycle) {
            NodeRefList tails;
            TailBuilderD builder(_nesting_table, tails, head.first, nesting(head.first));
            for (auto it = head.second->begin(), eit = head.second->end(); it != eit; ++it) {
                (*it)->accept(builder);
            }
            headRefToTails.emplace(head.first, tails);
        }
    }

    class TailBuilderD : public TailBuilder {
    public:
        explicit TailBuilderD(NestingTable &nesting_table, NodeRefList &tails, const NodeT *head,
                              const WtoNestingT &headNesting) : TailBuilder(nesting_table, tails, head, headNesting) {}

        void visit(const Wto<SVF::ICFGWrapper>::WtoVertexT &vertex) override {
            if (SVFUtil::isa<CallICFGNode>(vertex.node()->getICFGNode()) ||
                SVFUtil::isa<FunExitICFGNode>(vertex.node()->getICFGNode()))
                return;
            for (const auto &edge: vertex.node()->getOutEdges()) {
                const NodeT *succ = edge->getDstNode();
                const WtoNestingT &succNesting = nesting(succ);
                if (succ != _head && succNesting <= _headNesting) {
                    _tails.insert(vertex.node());
                }
            }
        }
    };
};
}
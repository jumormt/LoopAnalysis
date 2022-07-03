#define BOOST_TEST_MODULE loopTest
#define BOOST_TEST_DYN_LINK
#include "ECG.h"
#include "LoopAnalysis.h"
#include <boost/test/unit_test.hpp>

using namespace llvm;
using namespace std;
using namespace SVF;

typedef SVF::LoopAnalysis<ECG> ECGLoopAnalysis;


BOOST_AUTO_TEST_CASE(test01) {
    SVFUtil::outs() << "test1:\n";
    /*
        1->2->3->4->5
           \   /
            6
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {4, 6},
                       {6, 2}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test01");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    SVF::LoopAnalysis<ECG>::EdgeRefList &edgeSet = loopAnalysis.getEntryEdgeRefList(2);
    SVF::LoopAnalysis<ECG>::EdgeRefList &backEdgeSet = loopAnalysis.getBackEdgeRefList(2);
    const SVF::LoopAnalysis<ECG>::NodeRefList &tails = loopAnalysis.getTails(2);
    for (const auto &tail: tails) {
        SVF::LoopAnalysis<ECG>::EdgeRefList &exitEdges = loopAnalysis.getExitEdgeRefList(tail);
        continue;
    }
    BOOST_CHECK(loopAnalysis.isHead(2));
    delete ecg;
}

BOOST_AUTO_TEST_CASE(test02) {
    SVFUtil::outs() << "\ntest2:\n";
    /*
                7
                 \
        1->2->3->4->5
           \   /
            6
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {4, 6},
                       {6, 2},
                       {7, 4}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test02");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;
}

BOOST_AUTO_TEST_CASE(test03) {
    SVFUtil::outs() << "\ntest3:\n";
    /*
        1->2->3->4->5->6
           \     /
              7
             /
            8
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {5, 6},
                       {5, 7},
                       {7, 2},
                       {7, 8}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test03");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;

}

BOOST_AUTO_TEST_CASE(test04) {
    SVFUtil::outs() << "\ntest4:\n";
    /*
        1->2->3->4->5->6
           \     /
              7
            /^
          8
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {5, 6},
                       {5, 7},
                       {7, 2},
                       {8, 7}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test04");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;

}

BOOST_AUTO_TEST_CASE(test05) {
    SVFUtil::outs() << "\ntest5:\n";
    /*      8
          /  \
        1->2->3->4->5->6
           \     /
              7
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {5, 6},
                       {5, 7},
                       {7, 2},
                       {1, 8},
                       {8, 3}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test05");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;
}

BOOST_AUTO_TEST_CASE(test06) {
    SVFUtil::outs() << "\ntest6:\n";
    /*
                 8
             /     \
        1->2->3->4->5->6
           \     /
              7
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {5, 6},
                       {5, 7},
                       {7, 2},
                       {5, 8},
                       {8, 2}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test06");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    SVF::LoopAnalysis<ECG>::EdgeRefList &edgeSet = loopAnalysis.getEntryEdgeRefList(2);
    SVF::LoopAnalysis<ECG>::EdgeRefList &backEdgeSet = loopAnalysis.getBackEdgeRefList(2);
    const SVF::LoopAnalysis<ECG>::NodeRefList &tails = loopAnalysis.getTails(2);
    for (const auto &tail: tails) {
        SVF::LoopAnalysis<ECG>::EdgeRefList &exitEdges = loopAnalysis.getExitEdgeRefList(tail);
        continue;
    }
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;
}

BOOST_AUTO_TEST_CASE(test07) {
    SVFUtil::outs() << "\ntest7:\n";
    /*
                 8
             /     \
        1->2->3->4->5->6
           \     /
           |  7
           | /
           9

              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 1},
                       {1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {5, 6},
                       {5, 7},
                       {7, 2},
                       {5, 8},
                       {8, 2},
                       {2, 9},
                       {9, 7},
                       {4, 8}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test07");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    SVF::LoopAnalysis<ECG>::EdgeRefList &edgeSet = loopAnalysis.getEntryEdgeRefList(2);
    SVF::LoopAnalysis<ECG>::EdgeRefList &backEdgeSet = loopAnalysis.getBackEdgeRefList(2);
    const SVF::LoopAnalysis<ECG>::NodeRefList &tails = loopAnalysis.getTails(2);
    for (const auto &tail: tails) {
        SVF::LoopAnalysis<ECG>::EdgeRefList &exitEdges = loopAnalysis.getExitEdgeRefList(tail);
        continue;
    }
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;
}

BOOST_AUTO_TEST_CASE(test08) {
    SVFUtil::outs() << "\ntest8:\n";
    /*
                 8
             /     \
        1->2->3->4->5->6
           \     /
              7
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1, 2},
                       {2, 3},
                       {3, 4},
                       {4, 5},
                       {5, 6},
                       {5, 7},
                       {7, 2},
                       {5, 8},
                       {8, 2},
                       {3, 9},
                       {9, 2}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test08");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    SVF::LoopAnalysis<ECG>::EdgeRefList &edgeSet = loopAnalysis.getEntryEdgeRefList(2);
    SVF::LoopAnalysis<ECG>::EdgeRefList &backEdgeSet = loopAnalysis.getBackEdgeRefList(2);
    const SVF::LoopAnalysis<ECG>::NodeRefList &tails = loopAnalysis.getTails(2);
    for (const auto &tail: tails) {
        SVF::LoopAnalysis<ECG>::EdgeRefList &exitEdges = loopAnalysis.getExitEdgeRefList(tail);
        continue;
    }
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;
}

BOOST_AUTO_TEST_CASE(test09) {
    SVFUtil::outs() << "\ntest9:\n";
    /*
              8     10
            / 9  \/   \
        1->2->3->4->5->6
           \     /
              7
              */
    ECG *ecg = new ECG();
    NodePairVector nPV{{1,  2},
                       {2,  3},
                       {3,  4},
                       {4,  5},
                       {5,  6},
                       {5,  7},
                       {7,  2},
                       {5,  8},
                       {8,  2},
                       {3,  9},
                       {9,  2},
                       {6,  10},
                       {10, 4}};
    ecg->addECGEdgesFromVector(nPV);
    ecg->dump("test09");
    ECGLoopAnalysis loopAnalysis(ecg, ecg->getECGNode(1));
    loopAnalysis.run();
    outs() << loopAnalysis.wto();
    SVF::LoopAnalysis<ECG>::EdgeRefList &edgeSet = loopAnalysis.getEntryEdgeRefList(2);
    SVF::LoopAnalysis<ECG>::EdgeRefList &backEdgeSet = loopAnalysis.getBackEdgeRefList(2);
    const SVF::LoopAnalysis<ECG>::NodeRefList &tails = loopAnalysis.getTails(2);
    for (const auto &tail: tails) {
        SVF::LoopAnalysis<ECG>::EdgeRefList &exitEdges = loopAnalysis.getExitEdgeRefList(tail);
        continue;
    }
    BOOST_CHECK(loopAnalysis.isHead(2));

    delete ecg;
}
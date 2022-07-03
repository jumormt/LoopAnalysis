//
// Created by Xiao on 2022/6/15.
//

#include "SVF-FE/LLVMUtil.h"
#include "Graphs/SVFG.h"
#include "WPA/Andersen.h"
#include "SVF-FE/SVFIRBuilder.h"
#include "Util/Options.h"
#include "EC.hpp"

using namespace llvm;
using namespace std;
using namespace SVF;

typedef ECDetection<ICFG*> ICFGECD;
typedef SCCDetection<ECG *> ECGSCC;
typedef ECDetection<ECG*> ECD;

void test01() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();
    delete johnsonDetection;
    delete ecg;
    // assert(dfs(ecg->getECGNode(1), ecg->getECGNode(6)) == )
}

void test02() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;
    delete ecg;
}

void test03() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();
    delete johnsonDetection;
    delete ecg;

}

void test04() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;
    delete ecg;

}

void test05() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;
    delete ecg;
}

void test06() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;
    delete ecg;
}

void test07() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;
    delete ecg;
}

void test08() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;
    delete ecg;
}

void test09() {
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
    ECD *johnsonDetection = new ECD(ecg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;
    delete ecg;
}


int test10(int argc, char ** argv)
{

    int arg_num = 0;
    char **arg_value = new char*[argc];
    std::vector<std::string> moduleNameVec;
    SVFUtil::processArguments(argc, argv, arg_num, arg_value, moduleNameVec);
    cl::ParseCommandLineOptions(arg_num, arg_value,
                                "Whole Program Points-to Analysis\n");

    if (Options::WriteAnder == "ir_annotator")
    {
        LLVMModuleSet::getLLVMModuleSet()->preProcessBCs(moduleNameVec);
    }

    SVFModule* svfModule = LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);
    svfModule->buildSymbolTableInfo();

    /// Build Program Assignment Graph (SVFIR)
    SVFIRBuilder builder;
    SVFIR* pag = builder.build(svfModule);


    /// ICFG
    ICFG* icfg = pag->getICFG();

    ICFGECD *johnsonDetection = new ICFGECD(icfg);
    johnsonDetection->simpleCycle();

    delete johnsonDetection;

    AndersenWaveDiff::releaseAndersenWaveDiff();
    SVFIR::releaseSVFIR();

//    LLVMModuleSet::getLLVMModuleSet()->dumpModulesToFile(".svf.bc");
    SVF::LLVMModuleSet::releaseLLVMModuleSet();

    llvm::llvm_shutdown();
    return 0;
}

int main(int argc, char ** argv){
    test01();
    return 0;
}



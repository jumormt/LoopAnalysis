//
// Created by Xiao on 2022/6/15.
//

#include "SVF-FE/LLVMUtil.h"
#include "WPA/Andersen.h"
#include "SVF-FE/SVFIRBuilder.h"
#include "Util/Options.h"
#include "LoopAnalysis.cpp"
#include "ICFGWTO.hpp"

using namespace llvm;
using namespace std;
using namespace SVF;

static llvm::cl::opt<std::string> InputFilename(llvm::cl::Positional,
                                                llvm::cl::desc("<input bitcode>"), llvm::cl::init("-"));


typedef SVF::LoopAnalysis<ICFG, ICFGWTO> ICFGLoopAnalysis;

int main(int argc, char **argv) {

    int arg_num = 0;
    char **arg_value = new char *[argc];
    std::vector<std::string> moduleNameVec;
    LLVMUtil::processArguments(argc, argv, arg_num, arg_value, moduleNameVec);
    cl::ParseCommandLineOptions(arg_num, arg_value,
                                "Whole Program Points-to Analysis\n");

    if (Options::WriteAnder == "ir_annotator") {
        LLVMModuleSet::getLLVMModuleSet()->preProcessBCs(moduleNameVec);
    }

    SVFModule *svfModule = LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);
    svfModule->buildSymbolTableInfo();

    /// Build Program Assignment Graph (SVFIR)
    SVFIRBuilder builder;
    SVFIR *pag = builder.build(svfModule);


    /// ICFG
    ICFG *icfg = pag->getICFG();
    ICFGNode *entry;
    Map<const SVFFunction *, ICFGLoopAnalysis> funcToICFGLoopAnalysis;
    for (const auto &func: *svfModule) {
        if (SVFUtil::isExtCall(func)) continue;
        entry = icfg->getFunEntryICFGNode(func);
        ICFGLoopAnalysis loopAnalysis(icfg, entry);
        loopAnalysis.run();
        outs() << loopAnalysis.wto();
        funcToICFGLoopAnalysis[func] = std::move(loopAnalysis);
    }


    AndersenWaveDiff::releaseAndersenWaveDiff();
    SVFIR::releaseSVFIR();

//    LLVMModuleSet::getLLVMModuleSet()->dumpModulesToFile(".svf.bc");
    SVF::LLVMModuleSet::releaseLLVMModuleSet();

    llvm::llvm_shutdown();
    return 0;
}



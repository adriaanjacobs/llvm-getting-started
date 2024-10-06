#include "pass.h"

#include <llvm/IR/Instructions.h>

llvm::AnalysisKey MyAnalysis::Key;

MyAnalysis::Result MyAnalysis::run(llvm::Module& module, llvm::ModuleAnalysisManager& MAM) {

    // collect all store instructions in the program
    Result set;
    for (auto& func : module) {
        for (auto& bb : func) {
            for (auto& inst : bb) {
                if (llvm::isa<llvm::StoreInst>(&inst))
                    set.insert(&inst);
            }
        }
    }

    return set;
}

llvm::PreservedAnalyses MyInstrumentationPass::run(llvm::Module& module, llvm::ModuleAnalysisManager& passMAM) {
    auto& analysisResult = passMAM.getResult<MyAnalysis>(module);

    // we are lazy and say everything is invalidated
    return llvm::PreservedAnalyses::none();
}

void MyInstrumentationPass::registerAnalyses(llvm::ModuleAnalysisManager &MAM) {
    MAM.registerPass([] { return MyAnalysis{}; });
}

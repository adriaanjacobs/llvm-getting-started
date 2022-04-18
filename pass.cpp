#include "pass.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalVariable.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instruction.h>
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
    auto& unsafeAccesses = passMAM.getResult<MyAnalysis>(module);

    // we are lazy and say everything is invalidated
    return llvm::PreservedAnalyses::none();
}


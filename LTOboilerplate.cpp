#include "pass.h"

#include <llvm/Passes/PassPlugin.h>
#include <llvm/Passes/PassBuilder.h>

// This part is the new way of registering your pass
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "LLVMInstrumentLTO", LLVM_VERSION_STRING,
        [](llvm::PassBuilder &PB) {
            PB.registerFullLinkTimeOptimizationLastEPCallback([](llvm::ModulePassManager &MPM, llvm::OptimizationLevel Level) {
                llvm::outs() << "Registering pass for LateLTO!\n";
                MPM.addPass(MyInstrumentationPass{});
            });
            PB.registerAnalysisRegistrationCallback([] (llvm::ModuleAnalysisManager& MAM) {
                MAM.registerPass([] () { return MyAnalysis{}; });
            });
        }
    };
}


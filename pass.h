#pragma once

#include <llvm/IR/PassManager.h>

//===----------------------------------------------------------------------===//
/// This class implements an LLVM module analysis pass.
///
class MyAnalysis : public llvm::AnalysisInfoMixin<MyAnalysis> {
public:

    explicit MyAnalysis() = default;
    ~MyAnalysis() = default;
    // Provide a unique key, i.e., memory address to be used by the LLVM's pass
    // infrastructure.
    static llvm::AnalysisKey Key;
    friend llvm::AnalysisInfoMixin<MyAnalysis>;

    // Specify the result type of this analysis pass.
    // can be whatever type you want
    using Result = llvm::DenseSet<llvm::Instruction*>;

    // Analyze the bitcode/IR in the given LLVM module.
    Result run(llvm::Module &M, [[maybe_unused]] llvm::ModuleAnalysisManager &MAM);
};

//===----------------------------------------------------------------------===//
/// This class implements an LLVM module transformation pass.
class MyInstrumentationPass : public llvm::PassInfoMixin<MyInstrumentationPass> {
public:
    explicit MyInstrumentationPass() = default;
    ~MyInstrumentationPass() = default;

    // Transform the bitcode/IR in the given LLVM module.
    llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
};
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

namespace {
struct ProfilerPass : public FunctionPass {
  static char ID;
  ProfilerPass() : FunctionPass(ID) {}

  bool isFuncLogger(StringRef name) {
    return name == "EveryLogger";
  }

  virtual bool runOnFunction(Function &F) {
    if (isFuncLogger(F.getName())) {
      return false;
    }
    // Dump Function
    outs() << "In a function called " << F.getName() << "\n\n";

    for (auto &B : F) {
      for (auto &I : B) {
        // Dump Instructions
        outs() << "Instruction: " << (uint64_t)(&I) << "\n";
        I.print(outs(), true);
        outs() << "\n";
      }
      outs() << "\n";
    }

    // Prepare builder for IR modification
    LLVMContext &Ctx = F.getContext();
    IRBuilder<> builder(Ctx);
    Type *retType = Type::getVoidTy(Ctx);

    // Prepare EveryLogger function
    ArrayRef<Type *> EveryParamTypes = {builder.getInt8Ty()->getPointerTo()};
    FunctionType *EveryLogFuncType =
        FunctionType::get(retType, EveryParamTypes, false);
    FunctionCallee EveryLogFunc =
        F.getParent()->getOrInsertFunction("EveryLogger", EveryLogFuncType);

    // Insert loggers for call, binOpt and ret instructions
    for (auto &B : F) {
      for (auto &I : B) {
        Value *valueAddr =
            ConstantInt::get(builder.getInt64Ty(), (int64_t)(&I));

        if (auto *call = dyn_cast<CallInst>(&I)) {

          // Skip call function
          Function *callee = call->getCalledFunction();
          if (callee && isFuncLogger(callee->getName())) {
            continue;
          }
        }
        if (auto *phi = dyn_cast<PHINode>(&I)) {
            continue;
        }    

        auto *instr = dyn_cast<Instruction>(&I);
        // Insert before instruction
        builder.SetInsertPoint(instr);

        Value *InstrName = builder.CreateGlobalStringPtr(instr->getOpcodeName());
        Value *args[] = {InstrName};
        builder.CreateCall(EveryLogFunc, args);
      }
    }
    return true;
  }
};
} // namespace

char ProfilerPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerProfilerPass(const PassManagerBuilder &,
                                 legacy::PassManagerBase &PM) {
  PM.add(new ProfilerPass());
}
static RegisterStandardPasses
    RegisterProfilerPass(PassManagerBuilder::EP_OptimizerLast,
                   registerProfilerPass);

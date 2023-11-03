#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

void CreateModFunc(LLVMContext &context, Module *module, IRBuilder<> &builder) {
  
  // define int mod(int num, int mod)
  ArrayRef<Type *> FuncModParamTypes = {builder.getInt32Ty(), builder.getInt32Ty()};
  FunctionType *FuncModType = FunctionType::get(builder.getInt32Ty(), FuncModParamTypes, false);
  Function *ModFunc = 
      Function::Create(FuncModType, Function::ExternalLinkage, "mod", module);

  ModFunc->setDSOLocal(true);
//  ModFunc->addFnAttr(Attribute::LocalUnnamedAddr);

  BasicBlock *BB2 = BasicBlock::Create(context, "", ModFunc);
  builder.SetInsertPoint(BB2);

  Argument* val0 = ModFunc->getArg(0);
  Argument* val1 = ModFunc->getArg(1);

  Value* val3 = builder.CreateNSWAdd(val1, val0);
  Value* val4 = builder.CreateSRem(val3, val1);

  builder.CreateRet(val4);
}
// TODO: local_unnamed_addr
void CreateCalcNeighbFunc(LLVMContext &context, Module *module, IRBuilder<> &builder) {
  ArrayRef<Type *> FuncParamTypes = {builder.getInt32Ty(), builder.getInt32Ty()};
  FunctionType *FuncType = FunctionType::get(builder.getInt32Ty(), FuncParamTypes, false);
  Function *Func = 
      Function::Create(FuncType, Function::ExternalLinkage, "calc_neighb", module);

  Func->setDSOLocal(true);

  BasicBlock *BB2 = BasicBlock::Create(context, "", Func);
  builder.SetInsertPoint(BB2);

  Argument* val0 = Func->getArg(0);
  Argument* val1 = Func->getArg(1);

  Value* val3 = builder.CreateNSWAdd(val0, builder.getInt32(256));
  Value* val4 = builder.CreateSRem(val3, builder.getInt32(256));
  Value* val5 = builder.CreateShl(val4, builder.getInt32(9), "", false, true);
  Value* val6 = builder.CreateNSWAdd(val1, builder.getInt32(512));
  Value* val7 = builder.CreateSRem(val6, builder.getInt32(512));
  Value* val8 = builder.CreateNSWAdd(val5, val7);

  builder.CreateRet(val8);
}

//#include "llvm-project/clang/lib/CodeGen/CodeGenModule.h"

void DeclareSimPutPixelFunc(LLVMContext &context, Module *module, IRBuilder<> &builder) {

  ArrayRef<Type *> FuncParamTypes = {builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty()};
  FunctionType *FuncType =
      FunctionType::get(builder.getVoidTy(), FuncParamTypes, false);
  FunctionCallee DeclFunc = module->getOrInsertFunction("simPutPixel", FuncType);

//  setDSOLocal(DeclFunc); TODO
}

// declare dso_local void @simPutPixel(i32, i32, i32) local_unnamed_addr #5

int main() {

  LLVMContext context;
  // ; ModuleID = 'top'
  // source_filename = "top"
  Module *module = new Module("game_of_life.c", context);
  IRBuilder<> builder(context);

  CreateModFunc(context, module, builder);
  CreateCalcNeighbFunc(context, module, builder);

 // DeclareSimPutPixelFunc(context, module, builder);

  // declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1
/*
  // declare i32 @puts(i8*)
  std::vector<Type *> putsArgs;
  putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  ArrayRef<Type *> argsRef(putsArgs);
  FunctionType *putsType =
      FunctionType::get(builder.getInt32Ty(), argsRef, false);
  FunctionCallee putsFunc = module->getOrInsertFunction("puts", putsType);


  // @0 = private unnamed_addr constant [14 x i8] c"hello world!\0A\00", align 1
  Value *helloWorld = builder.CreateGlobalStringPtr("hello world!\n");

  // declare i32 @puts(i8*)
  std::vector<Type *> putsArgs;
  putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  ArrayRef<Type *> argsRef(putsArgs);
  FunctionType *putsType =
      FunctionType::get(builder.getInt32Ty(), argsRef, false);
  FunctionCallee putsFunc = module->getOrInsertFunction("puts", putsType);

  // %0 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @0,
  // i32 0, i32 0))
  builder.CreateCall(putsFunc, helloWorld);
  // ret i32 0
  builder.CreateRet(builder.getInt32(0));
*/
  // Dump LLVM IR
  module->print(outs(), nullptr);

  // Interpreter of LLVM IR
/*  outs() << "Running code...\n";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
  ee->finalizeObject();
  ArrayRef<GenericValue> noargs;
  GenericValue v = ee->runFunction(mainFunc, noargs);
  outs() << "Code was run.\n";
*/
  return 0;
}

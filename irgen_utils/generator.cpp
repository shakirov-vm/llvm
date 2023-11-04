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

#define FULL_SIZE 131072
#define FULL_MEM 524288

void CreateAppFunc(LLVMContext &context, Module *module, IRBuilder<> &builder) {
 // TODO: Invalid type: define dso_local i32 @app(i32 %0)
  FunctionType *FuncType = FunctionType::get(builder.getInt32Ty(), builder.getInt32Ty(), false);
  Function *Func = 
      Function::Create(FuncType, Function::ExternalLinkage, "app", module);

  Func->setDSOLocal(true);

  BasicBlock *BB0 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB5 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB15 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB23 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB25 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB26 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB30 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB33 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB36 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB44 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB45 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB46 = BasicBlock::Create(context, "", Func);

// TODO: align and more params
  ArrayType *FullArrType = ArrayType::get(builder.getInt32Ty(), FULL_SIZE);

  builder.SetInsertPoint(BB0);
  Value* val1 = builder.CreateAlloca(FullArrType);
  Value* val2 = builder.CreateBitCast(val1, builder.getInt8Ty()->getPointerTo());
  Value* val3 = builder.CreateAlloca(FullArrType);
  Value* val4 = builder.CreateBitCast(val3, builder.getInt8Ty()->getPointerTo());

  Type *LifetimeParam[] = {builder.getInt64Ty(), builder.getInt8Ty()->getPointerTo()};
  FunctionType *LifetimeType =
      FunctionType::get(builder.getVoidTy(), LifetimeParam, false);
  FunctionCallee LifetimeFunc = module->getOrInsertFunction("llvm.lifetime.start.p0i8", LifetimeType);

  Type *MemsetParam[] = {builder.getInt8Ty()->getPointerTo(), builder.getInt8Ty(), 
                         builder.getInt64Ty(), builder.getInt1Ty()};
  FunctionType *MemsetType =
      FunctionType::get(builder.getVoidTy(), MemsetParam, false);
  FunctionCallee MemsetFunc = module->getOrInsertFunction("llvm.memset.p0i8.i64", MemsetType);

  Value *LifetimeCall2[] = {builder.getInt64(FULL_MEM), val2};
  builder.CreateCall(LifetimeFunc, LifetimeCall2);

  Value *MemsetCall2[] = {val2, builder.getInt8(0), builder.getInt64(FULL_MEM), builder.getInt1(false)};
  builder.CreateCall(MemsetFunc, MemsetCall2);

  Value *LifetimeCall4[] = {builder.getInt64(FULL_MEM), val4};
  builder.CreateCall(LifetimeFunc, LifetimeCall4);

  Value *MemsetCall4[] = {val4, builder.getInt8(0), builder.getInt64(FULL_MEM), builder.getInt1(false)};
  builder.CreateCall(MemsetFunc, MemsetCall4);

  builder.CreateBr(BB5);
  builder.SetInsertPoint(BB5);

  PHINode* val6 = builder.CreatePHI(builder.getInt32Ty(), 2);
  Value* val7 = builder.CreateAnd(val6, builder.getInt32(3));
  Value* val8 = builder.CreateICmpEQ(val7, builder.getInt32(0));
  Value* val9 = builder.CreateNSWAdd(val6, builder.getInt32(2));
  Value* val10 = builder.CreateSelect(val8, val9, val6);
  Value* val11 = builder.CreateSExt(val10, builder.getInt64Ty());

  Value* idx_12[] = {builder.getInt64(0), val11};
  Value* val12 = GetElementPtrInst::CreateInBounds(FullArrType, val3, idx_12, "", BB5);
  builder.CreateStore(builder.getInt32(1), val12);
  Value* val13 = builder.CreateNSWAdd(val10, builder.getInt32(1));
  Value* val14 = builder.CreateICmpSLT(val10, builder.getInt32(3583));

  builder.CreateCondBr(val14, BB5, BB15);

  val6->addIncoming(builder.getInt32(1536), BB0);
  val6->addIncoming(val13, BB5);

  builder.SetInsertPoint(BB15);

  Value* idx_16[] = {builder.getInt64(0), builder.getInt64(0)};
  Value* val16 = GetElementPtrInst::CreateInBounds(FullArrType, val3, idx_16, "", BB15);

  Value* idx_17[] = {builder.getInt64(0), builder.getInt64(65792)};
  Value* val17 = GetElementPtrInst::CreateInBounds(FullArrType, val3, idx_17, "", BB15);

  builder.CreateStore(builder.getInt32(1), val17);

  Value* idx_18[] = {builder.getInt64(0), builder.getInt64(66305)};
  Value* val18 = GetElementPtrInst::CreateInBounds(FullArrType, val3, idx_18, "", BB15);

  builder.CreateStore(builder.getInt32(1), val18);

  Value* idx_19[] = {builder.getInt64(0), builder.getInt64(66815)};
  Value* val19 = GetElementPtrInst::CreateInBounds(FullArrType, val3, idx_19, "", BB15);

  builder.CreateStore(builder.getInt32(1), val19);
  
  Value* idx_20[] = {builder.getInt64(0), builder.getInt64(66816)};
  Value* val20 = GetElementPtrInst::CreateInBounds(FullArrType, val3, idx_20, "", BB15);

  builder.CreateStore(builder.getInt32(1), val20);
  
  Value* idx_21[] = {builder.getInt64(0), builder.getInt64(66817)};
  Value* val21 = GetElementPtrInst::CreateInBounds(FullArrType, val3, idx_21, "", BB15);

  builder.CreateStore(builder.getInt32(1), val21);
  
  Value* idx_22[] = {builder.getInt64(0), builder.getInt64(0)};
  Value* val22 = GetElementPtrInst::CreateInBounds(FullArrType, val1, idx_22, "", BB15);

  builder.CreateBr(BB23);
  builder.SetInsertPoint(BB23);

  PHINode* val24 = builder.CreatePHI(builder.getInt32Ty(), 2);

  Type *MemsetTwiceParam[] = {builder.getInt8Ty()->getPointerTo(), builder.getInt8Ty()->getPointerTo(), 
                         builder.getInt64Ty(), builder.getInt1Ty()};
  FunctionType *MemsetTwiceType =
      FunctionType::get(builder.getVoidTy(), MemsetTwiceParam, false);
  FunctionCallee MemsetTwiceFunc = module->getOrInsertFunction("llvm.memcpy.p0i8.p0i8.i64", MemsetTwiceType);

  Value *MemsetTwiceCall[] = {val2, val4, builder.getInt64(FULL_MEM), builder.getInt1(false)};
  builder.CreateCall(MemsetTwiceFunc, MemsetTwiceCall);

  Type *CalcFrameParam[] = {builder.getInt32Ty()->getPointerTo(), builder.getInt32Ty()->getPointerTo()};
  FunctionType *CalcFrameType =
      FunctionType::get(builder.getVoidTy(), CalcFrameParam, false);
  FunctionCallee CalcFrameFunc = module->getOrInsertFunction("calc_frame", CalcFrameType);

  Value *CalcFrameCall[] = {val22, val16};
  builder.CreateCall(CalcFrameFunc, CalcFrameCall);

  builder.CreateBr(BB26);
  builder.SetInsertPoint(BB25);

  Type *LifeTimeEndParam[] = {builder.getInt64Ty()->getPointerTo(), builder.getInt8Ty()->getPointerTo()};
  FunctionType *LifeTimeEndType =
      FunctionType::get(builder.getVoidTy(), LifeTimeEndParam, false);
  FunctionCallee LifeTimeEndFunc = module->getOrInsertFunction("llvm.lifetime.end.p0i8", LifeTimeEndType);

  Value *LifeTimeEndCall4[] = {builder.getInt64(FULL_MEM), val4};
  builder.CreateCall(LifeTimeEndFunc, LifeTimeEndCall4);

  Value *LifeTimeEndCall2[] = {builder.getInt64(FULL_MEM), val2};
  builder.CreateCall(LifeTimeEndFunc, LifeTimeEndCall2);

  builder.CreateRetVoid();
  builder.SetInsertPoint(BB26);

  PHINode* val27 = builder.CreatePHI(builder.getInt64Ty(), 2);
  Value* val28 = builder.CreateTrunc(val27, builder.getInt32Ty());
  Value* val29 = builder.CreateTrunc(val27, builder.getInt32Ty());

  builder.CreateBr(BB36);
  builder.SetInsertPoint(BB30);

  Type *SimFlushParam[] = {builder.getVoidTy()};
  FunctionType *SimFlushType =
      FunctionType::get(builder.getVoidTy(), SimFlushParam, false);
  FunctionCallee SimFlushFunc = module->getOrInsertFunction("simFlush", SimFlushType);

// TODO: what about tail call void (...)??
  builder.CreateCall(SimFlushFunc);
  Value* val31 = builder.CreateNSWAdd(val24, builder.getInt32(1)); // TODO: NUW
  Value* val32 = builder.CreateICmpEQ(val31, builder.getInt32(1000));

  builder.CreateCondBr(val32, BB25, BB23);
  builder.SetInsertPoint(BB33);

  Value* val34 = builder.CreateNSWAdd(val27, builder.getInt32(1)); // TODO: NUW
  Value* val35 = builder.CreateICmpEQ(val34, builder.getInt64(512));

  builder.CreateCondBr(val35, BB30, BB26);

  val24->addIncoming(builder.getInt32(0), BB15);
  val24->addIncoming(val31, BB30);

  val27->addIncoming(builder.getInt64(0), BB23);
  val27->addIncoming(val34, BB33);

  builder.SetInsertPoint(BB36);

  PHINode* val37 = builder.CreatePHI(builder.getInt64Ty(), 2);

  Value* val38 = builder.CreateShl(val37, builder.getInt64(9), "", true, true);
  Value* val39 = builder.CreateNSWAdd(val38, val27); // TODO: NUW

  Value* idx_40[] = {builder.getInt64(0), val39};
  Value* val40 = GetElementPtrInst::CreateInBounds(FullArrType, val1, idx_40, "", BB36);

  Value* val41 = builder.CreateLoad(builder.getInt32Ty(), val40);
  Value* val42 = builder.CreateICmpEQ(val41, builder.getInt32(1));
  Value* val43 = builder.CreateTrunc(val37, builder.getInt32Ty());

  builder.CreateCondBr(val42, BB44, BB45);

  Type *SimPutPixelParam[] = {builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty()};
  FunctionType *SimPutPixelType =
      FunctionType::get(builder.getVoidTy(), SimPutPixelParam, false);
  FunctionCallee SimPutPixelFunc = module->getOrInsertFunction("simPutPixel", SimPutPixelType);

  builder.SetInsertPoint(BB44);

// TODO: what about tail call void (...)??
  Value *SimPutPixelCall44[] = {val29, val43, builder.getInt32(16776960)};
  builder.CreateCall(SimPutPixelFunc, SimPutPixelCall44);

  builder.CreateBr(BB46);
  builder.SetInsertPoint(BB45);

// TODO: what about tail call void (...)??
  Value *SimPutPixelCall45[] = {val28, val43, builder.getInt32(0)};
  builder.CreateCall(SimPutPixelFunc, SimPutPixelCall45);

  builder.CreateBr(BB46);
  builder.SetInsertPoint(BB46);

  Value* val47 = builder.CreateNSWAdd(val37, builder.getInt64(1)); // TODO: NUW
  Value* val48 = builder.CreateICmpEQ(val47, builder.getInt64(256));

  builder.CreateCondBr(val48, BB33, BB36);

  val37->addIncoming(builder.getInt64(0), BB26);
  val37->addIncoming(val47, BB46);
}

void CreateGEPSnippet(IRBuilder<> &builder, Value *val0, Value **ret_val, BasicBlock *BB,
                                    Value *left_or, Value *right_or) {

  Value* val1 = builder.CreateOr(left_or, right_or);
  Value* val2 = builder.CreateZExt(val1, builder.getInt64Ty());

  Value* idx[] = {val2};
  Value* val3 = GetElementPtrInst::CreateInBounds(builder.getInt32Ty(), val0, idx, "", BB);
  Value* val4 = builder.CreateLoad(builder.getInt32Ty(), val3);
  *ret_val = builder.CreateICmpEQ(val4, builder.getInt32(1));
}

void CreateCalcFrameFunc(LLVMContext &context, Module *module, IRBuilder<> &builder) {
 // TODO: Invalid type: define dso_local

  ArrayRef<Type *> FuncParamTypes = {builder.getInt32Ty()->getPointerTo(), builder.getInt32Ty()->getPointerTo()};
  FunctionType *FuncType = FunctionType::get(builder.getVoidTy(), FuncParamTypes, false);
  Function *Func = 
      Function::Create(FuncType, Function::ExternalLinkage, "calc_frame", module);

  Func->setDSOLocal(true);

  BasicBlock *BB2 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB3 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB12 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB13 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB15 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB85 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB89 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB90 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB91 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB94 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB95 = BasicBlock::Create(context, "", Func);
  BasicBlock *BB96 = BasicBlock::Create(context, "", Func);

  builder.SetInsertPoint(BB2);

  Argument* val0 = Func->getArg(0);
  Argument* val1 = Func->getArg(1);
  builder.CreateBr(BB3);
  builder.SetInsertPoint(BB3);

  PHINode* val4 = builder.CreatePHI(builder.getInt64Ty(), 2);
  Value* val5 = builder.CreateTrunc(val4, builder.getInt32Ty());
  Value* val6 = builder.CreateAdd(val5, builder.getInt32(511));
  Value* val7 = builder.CreateAnd(val6, builder.getInt32(511));
  Value* val8 = builder.CreateNSWAdd(val4, builder.getInt64(1)); 
  Value* val9 = builder.CreateTrunc(val8, builder.getInt32Ty());
  Value* val10 = builder.CreateAnd(val9, builder.getInt32(511));
  Value* val11 = builder.CreateTrunc(val4, builder.getInt32Ty());

  builder.CreateBr(BB15);
  builder.SetInsertPoint(BB12);

  builder.CreateRetVoid();
  builder.SetInsertPoint(BB13);

  Value* val14 = builder.CreateICmpEQ(val8, builder.getInt64(512));
  builder.CreateCondBr(val14, BB12, BB3);
  val4->addIncoming(builder.getInt64(0), BB2);
  val4->addIncoming(val8, BB13);

  builder.SetInsertPoint(BB15);

  PHINode* val16 = builder.CreatePHI(builder.getInt64Ty(), 2);
  Value* val17 = builder.CreateShl(val16, builder.getInt64(9), "", false, false);
  Value* val18 = builder.CreateTrunc(val17, builder.getInt32Ty());
  Value* val19 = builder.CreateAdd(val18, builder.getInt32(130560));
  Value* val20 = builder.CreateAnd(val19, builder.getInt32(130560));
  
  Value* val25;
  CreateGEPSnippet(builder, val0, &val25, BB15, val20, val7);
  Value* val26 = builder.CreateZExt(val25, builder.getInt32Ty());

  Value* val31;
  CreateGEPSnippet(builder, val0, &val31, BB15, val20, val11);
  Value* val32 = builder.CreateSelect(val25, builder.getInt32(2), builder.getInt32(1));
  Value* val33 = builder.CreateSelect(val31, val32, val26);

  Value* val38;
  CreateGEPSnippet(builder, val0, &val38, BB15, val20, val10);
  Value* val39 = builder.CreateZExt(val38, builder.getInt32Ty());
  Value* val40 = builder.CreateNSWAdd(val33, val39);
  Value* val41 = builder.CreateTrunc(val17, builder.getInt32Ty());

  Value* val46;
  CreateGEPSnippet(builder, val0, &val46, BB15, val7, val41);
  Value* val47 = builder.CreateZExt(val46, builder.getInt32Ty());
  Value* val48 = builder.CreateNSWAdd(val40, val47);

  Value* val53;
  CreateGEPSnippet(builder, val0, &val53, BB15, val10, val41);
  Value* val54 = builder.CreateZExt(val53, builder.getInt32Ty());
  Value* val55 = builder.CreateNSWAdd(val48, val54);
  Value* val56 = builder.CreateNSWAdd(val16, builder.getInt64(1));
  Value* val57 = builder.CreateTrunc(val17, builder.getInt32Ty());
  Value* val58 = builder.CreateAdd(val57, builder.getInt32(512));
  Value* val59 = builder.CreateAnd(val58, builder.getInt32(130560));

  Value* val64;
  CreateGEPSnippet(builder, val0, &val64, BB15, val59, val7);
  Value* val65 = builder.CreateZExt(val64, builder.getInt32Ty());
  Value* val66 = builder.CreateNSWAdd(val55, val65);

  Value* val71;
  CreateGEPSnippet(builder, val0, &val71, BB15, val59, val11);
  Value* val72 = builder.CreateZExt(val71, builder.getInt32Ty());
  Value* val73 = builder.CreateNSWAdd(val66, val72);

  Value* val78;
  CreateGEPSnippet(builder, val0, &val78, BB15, val59, val10);
  Value* val79 = builder.CreateZExt(val78, builder.getInt32Ty());
  Value* val80 = builder.CreateNSWAdd(val73, val79);
  Value* val81 = builder.CreateNSWAdd(val17, val4);

  Value* idx_82[] = {val81};
  Value* val82 = GetElementPtrInst::CreateInBounds(builder.getInt32Ty(), val0, idx_82, "", BB15);
  Value* val83 = builder.CreateLoad(builder.getInt32Ty(), val82);
  Value* val84 = builder.CreateICmpEQ(val83, builder.getInt32(1));

  val16->addIncoming(builder.getInt64(0), BB3);
  val16->addIncoming(val56, BB96);

  builder.CreateCondBr(val84, BB85, BB91);
  builder.SetInsertPoint(BB85);

  Value* val86 = builder.CreateAnd(val80, builder.getInt32(-2));
  Value* val87 = builder.CreateICmpEQ(val86, builder.getInt32(2));

  Value* idx_88[] = {val81};
  Value* val88 = GetElementPtrInst::CreateInBounds(builder.getInt32Ty(), val1, idx_88, "", BB85);
  builder.CreateCondBr(val87, BB90, BB89);

  builder.SetInsertPoint(BB89);
  builder.CreateStore(builder.getInt32(0), val88);
  builder.CreateBr(BB96);

  builder.SetInsertPoint(BB90);
  builder.CreateStore(builder.getInt32(1), val88);
  builder.CreateBr(BB96);

  builder.SetInsertPoint(BB91);
  Value* val92 = builder.CreateICmpSGT(val80, builder.getInt32(2));

  Value* idx_93[] = {val81};
  Value* val93 = GetElementPtrInst::CreateInBounds(builder.getInt32Ty(), val1, idx_93, "", BB91);
  builder.CreateCondBr(val92, BB94, BB95);

  builder.SetInsertPoint(BB94);
  builder.CreateStore(builder.getInt32(1), val93);
  builder.CreateBr(BB96);

  builder.SetInsertPoint(BB95);
  builder.CreateStore(builder.getInt32(0), val93);
  builder.CreateBr(BB96);

  builder.SetInsertPoint(BB96);
  Value* val97 = builder.CreateICmpEQ(val56, builder.getInt64(256));
  builder.CreateCondBr(val97, BB13, BB15);
}
/*
; Function Attrs: nofree norecurse nounwind uwtable
define dso_local void @calc_frame(i32* nocapture readonly %0, i32* nocapture %1) local_unnamed_addr #2 {


}
*/

int main() {

  LLVMContext context;
  // ; ModuleID = 'top'
  // source_filename = "top"
  Module *module = new Module("game_of_life.c", context);
  IRBuilder<> builder(context);

  CreateModFunc(context, module, builder);
  CreateCalcNeighbFunc(context, module, builder);
  CreateCalcFrameFunc(context, module, builder);
  CreateAppFunc(context, module, builder);

//  DeclareSimPutPixelFunc(context, module, builder);

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

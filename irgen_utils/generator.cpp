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
}

/*

; Function Attrs: nounwind uwtable
define dso_local void @app() local_unnamed_addr #4 {
  %1 = alloca [131072 x i32], align 16
  %2 = bitcast [131072 x i32]* %1 to i8*
  %3 = alloca [131072 x i32], align 16
  %4 = bitcast [131072 x i32]* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 524288, i8* nonnull %2) #6
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 dereferenceable(524288) %2, i8 0, i64 524288, i1 false)
  call void @llvm.lifetime.start.p0i8(i64 524288, i8* nonnull %4) #6
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 dereferenceable(524288) %4, i8 0, i64 524288, i1 false)
  br label %5

5:                                                ; preds = %5, %0
  %6 = phi i32 [ 1536, %0 ], [ %13, %5 ]
  %7 = and i32 %6, 3
  %8 = icmp eq i32 %7, 0
  %9 = add nsw i32 %6, 2
  %10 = select i1 %8, i32 %9, i32 %6
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 %11
  store i32 1, i32* %12, align 4, !tbaa !2
  %13 = add nsw i32 %10, 1
  %14 = icmp slt i32 %10, 3583
  br i1 %14, label %5, label %15

15:                                               ; preds = %5
  %16 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 0
  %17 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 65792
  store i32 1, i32* %17, align 16, !tbaa !2
  %18 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 66305
  store i32 1, i32* %18, align 4, !tbaa !2
  %19 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 66815
  store i32 1, i32* %19, align 4, !tbaa !2
  %20 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 66816
  store i32 1, i32* %20, align 16, !tbaa !2
  %21 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 66817
  store i32 1, i32* %21, align 4, !tbaa !2
  %22 = getelementptr inbounds [131072 x i32], [131072 x i32]* %1, i64 0, i64 0
  br label %23

23:                                               ; preds = %30, %15
  %24 = phi i32 [ 0, %15 ], [ %31, %30 ]
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 16 dereferenceable(524288) %2, i8* nonnull align 16 dereferenceable(524288) %4, i64 524288, i1 false)
  call void @calc_frame(i32* nonnull %22, i32* nonnull %16)
  br label %26

25:                                               ; preds = %30
  call void @llvm.lifetime.end.p0i8(i64 524288, i8* nonnull %4) #6
  call void @llvm.lifetime.end.p0i8(i64 524288, i8* nonnull %2) #6
  ret void

26:                                               ; preds = %33, %23
  %27 = phi i64 [ 0, %23 ], [ %34, %33 ]
  %28 = trunc i64 %27 to i32
  %29 = trunc i64 %27 to i32
  br label %36

30:                                               ; preds = %33
  tail call void (...) @simFlush() #6
  %31 = add nuw nsw i32 %24, 1
  %32 = icmp eq i32 %31, 1000
  br i1 %32, label %25, label %23

33:                                               ; preds = %46
  %34 = add nuw nsw i64 %27, 1
  %35 = icmp eq i64 %34, 512
  br i1 %35, label %30, label %26

36:                                               ; preds = %46, %26
  %37 = phi i64 [ 0, %26 ], [ %47, %46 ]
  %38 = shl nuw nsw i64 %37, 9
  %39 = add nuw nsw i64 %38, %27
  %40 = getelementptr inbounds [131072 x i32], [131072 x i32]* %1, i64 0, i64 %39
  %41 = load i32, i32* %40, align 4, !tbaa !2
  %42 = icmp eq i32 %41, 1
  %43 = trunc i64 %37 to i32
  br i1 %42, label %44, label %45

44:                                               ; preds = %36
  tail call void @simPutPixel(i32 %29, i32 %43, i32 16776960) #6
  br label %46

45:                                               ; preds = %36
  tail call void @simPutPixel(i32 %28, i32 %43, i32 0) #6
  br label %46

46:                                               ; preds = %44, %45
  %47 = add nuw nsw i64 %37, 1
  %48 = icmp eq i64 %47, 256
  br i1 %48, label %33, label %36
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

  CreateAppFunc(context, module, builder);

  DeclareSimPutPixelFunc(context, module, builder);

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

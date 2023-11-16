
; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @mod(i32 %0, i32 %1) local_unnamed_addr #0 {
  %3 = add nsw i32 %1, %0
  %4 = srem i32 %3, %1
  ret i32 %4
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @calc_neighb(i32 %0, i32 %1) local_unnamed_addr #0 {
  %3 = add nsw i32 %0, 256
  %4 = srem i32 %3, 256
  %5 = shl nsw i32 %4, 9
  %6 = add nsw i32 %1, 512
  %7 = srem i32 %6, 512
  %8 = add nsw i32 %5, %7
  ret i32 %8
}

; Function Attrs: nofree norecurse nounwind uwtable
define dso_local void @calc_frame(i32* nocapture readonly %0, i32* nocapture %1) local_unnamed_addr #2 {
  br label %3

  MOV x2, 0

LABEL3:
3:                                                ; preds = %13, %2
  # x2
  %4 = phi i64 [ 0, %2 ], [ %8, %13 ]

  TRUNC_AA x3, x2, 511
  %5 = trunc i64 %4 to i32
  %6 = add i32 %5, 511
  %7 = and i32 %6, 511

  ADD x2, x2, 1
  %8 = add nuw nsw i64 %4, 1

  AND_TR x4, x0, 511
  %9 = trunc i64 %8 to i32
  %10 = and i32 %9, 511
  %11 = trunc i64 %4 to i32

  JMP LABEL15
  br label %15

12:                                               ; preds = %13
  RET
  ret void

13:                                               ; preds = %96

  
  ICMP_BR x3, 512, LABEL12, LABEL3
  %14 = icmp eq i64 %8, 512
  br i1 %14, label %12, label %3

LABEL15:
15:                                               ; preds = %96, %3
  %16 = phi i64 [ 0, %3 ], [ %56, %96 ]

  SHL
  %17 = shl i64 %16, 9

  TRUNC_AA x1, x2, 130560
  %18 = trunc i64 %17 to i32
  %19 = add i32 %18, 130560
  %20 = and i32 %19, 130560

  ORZE x1, x2, x3
  %21 = or i32 %20, %7
  %22 = zext i32 %21 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %23 = getelementptr inbounds i32, i32* %0, i64 %22
  %24 = load i32, i32* %23, align 4, !tbaa !2
  %25 = icmp eq i32 %24, 1

  ORZE x1, x2, x3
  %26 = zext i1 %25 to i32
  %27 = or i32 %20, %11

  %28 = zext i32 %27 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %29 = getelementptr inbounds i32, i32* %0, i64 %28
  %30 = load i32, i32* %29, align 4, !tbaa !2
  %31 = icmp eq i32 %30, 1

  %32 = select i1 %25, i32 2, i32 1
  %33 = select i1 %31, i32 %32, i32 %26

  ORZE x1, x2, x3
  %34 = or i32 %20, %10
  %35 = zext i32 %34 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %36 = getelementptr inbounds i32, i32* %0, i64 %35
  %37 = load i32, i32* %36, align 4, !tbaa !2
  %38 = icmp eq i32 %37, 1

  ZEA x1, x2, x3
  %39 = zext i1 %38 to i32
  %40 = add nuw nsw i32 %33, %39
  %41 = trunc i64 %17 to i32

  ORZE x1, x2, x3
  %42 = or i32 %7, %41
  %43 = zext i32 %42 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %44 = getelementptr inbounds i32, i32* %0, i64 %43
  %45 = load i32, i32* %44, align 4, !tbaa !2
  %46 = icmp eq i32 %45, 1

  ZEA x1, x2, x3
  %47 = zext i1 %46 to i32
  %48 = add nuw nsw i32 %40, %47

  ORZE x1, x2, x3
  %49 = or i32 %10, %41
  %50 = zext i32 %49 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %51 = getelementptr inbounds i32, i32* %0, i64 %50
  %52 = load i32, i32* %51, align 4, !tbaa !2
  %53 = icmp eq i32 %52, 1

  ZEA x1, x2, x3
  %54 = zext i1 %53 to i32
  %55 = add nuw nsw i32 %48, %54

  %56 = add nuw nsw i64 %16, 1
  %57 = trunc i64 %17 to i32
  %58 = add i32 %57, 512
  %59 = and i32 %58, 130560

  ORZE x1, x2, x3
  %60 = or i32 %59, %7
  %61 = zext i32 %60 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %62 = getelementptr inbounds i32, i32* %0, i64 %61
  %63 = load i32, i32* %62, align 4, !tbaa !2
  %64 = icmp eq i32 %63, 1

  %65 = zext i1 %64 to i32
  %66 = add nuw nsw i32 %55, %65

  ORZE x1, x2, x3
  %67 = or i32 %59, %11
  %68 = zext i32 %67 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %69 = getelementptr inbounds i32, i32* %0, i64 %68
  %70 = load i32, i32* %69, align 4, !tbaa !2
  %71 = icmp eq i32 %70, 1

  %72 = zext i1 %71 to i32
  %73 = add nuw nsw i32 %66, %72

  ORZE x1, x2, x3
  %74 = or i32 %59, %10
  %75 = zext i32 %74 to i64

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %76 = getelementptr inbounds i32, i32* %0, i64 %75
  %77 = load i32, i32* %76, align 4, !tbaa !2
  %78 = icmp eq i32 %77, 1

  %79 = zext i1 %78 to i32
  %80 = add nuw nsw i32 %73, %79
  %81 = add nuw nsw i64 %17, %4

  LOAD_AND_COMPARE_ONE x1, x2, x3
  %82 = getelementptr inbounds i32, i32* %0, i64 %81
  %83 = load i32, i32* %82, align 4, !tbaa !2
  %84 = icmp eq i32 %83, 1

  br i1 %84, label %85, label %91

85:                                               ; preds = %15
  %86 = and i32 %80, -2
  %87 = icmp eq i32 %86, 2
  %88 = getelementptr inbounds i32, i32* %1, i64 %81
  br i1 %87, label %90, label %89

89:                                               ; preds = %85
  store i32 0, i32* %88, align 4, !tbaa !2
  br label %96

90:                                               ; preds = %85
  store i32 1, i32* %88, align 4, !tbaa !2
  br label %96

91:                                               ; preds = %15
  %92 = icmp sgt i32 %80, 2
  %93 = getelementptr inbounds i32, i32* %1, i64 %81
  br i1 %92, label %94, label %95

94:                                               ; preds = %91
  store i32 1, i32* %93, align 4, !tbaa !2
  br label %96

95:                                               ; preds = %91
  store i32 0, i32* %93, align 4, !tbaa !2
  br label %96

96:                                               ; preds = %94, %95, %89, %90
  %97 = icmp eq i64 %56, 256
  br i1 %97, label %13, label %15
}

; Function Attrs: nofree norecurse nounwind uwtable writeonly
define dso_local void @init_game(i32* nocapture %0) local_unnamed_addr #3 {
  br label %8

2:                                                ; preds = %8
  %3 = getelementptr inbounds i32, i32* %0, i64 65792
  store i32 1, i32* %3, align 4, !tbaa !2
  %4 = getelementptr inbounds i32, i32* %0, i64 66305
  store i32 1, i32* %4, align 4, !tbaa !2
  %5 = getelementptr inbounds i32, i32* %0, i64 66815
  store i32 1, i32* %5, align 4, !tbaa !2
  %6 = getelementptr inbounds i32, i32* %0, i64 66816
  store i32 1, i32* %6, align 4, !tbaa !2
  %7 = getelementptr inbounds i32, i32* %0, i64 66817
  store i32 1, i32* %7, align 4, !tbaa !2
  ret void

8:                                                ; preds = %1, %8
  %9 = phi i32 [ 1536, %1 ], [ %16, %8 ]
  %10 = and i32 %9, 3
  %11 = icmp eq i32 %10, 0
  %12 = add nsw i32 %9, 2
  %13 = select i1 %11, i32 %12, i32 %9
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds i32, i32* %0, i64 %14
  store i32 1, i32* %15, align 4, !tbaa !2
  %16 = add nsw i32 %13, 1
  %17 = icmp slt i32 %13, 3583
  br i1 %17, label %8, label %2
}

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

  ADD_ICMP_BR x1, 1000, x2, x3
  %31 = add nuw nsw i32 %24, 1
  %32 = icmp eq i32 %31, 1000
  br i1 %32, label %25, label %23

33:                                               ; preds = %46

  ADD_ICMP_BR x1, 512, x2, x3
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

  ADD_ICMP_BR x1, 256, x2, x3
  %47 = add nuw nsw i64 %37, 1
  %48 = icmp eq i64 %47, 256
  br i1 %48, label %33, label %36
}

; ModuleID = 'game_of_life.c'
source_filename = "game_of_life.c"

define dso_local void @calc_frame(i32* %0, i32* %1) {
  br label %3

3:                                                ; preds = %13, %2
  %4 = phi i64 [ 0, %2 ], [ %8, %13 ]
  %5 = trunc i64 %4 to i32
  %6 = add i32 %5, 511
  %7 = and i32 %6, 511
  %8 = add nsw i64 %4, 1
  %9 = trunc i64 %8 to i32
  %10 = and i32 %9, 511
  %11 = trunc i64 %4 to i32
  br label %15

12:                                               ; preds = %13
  ret void

13:                                               ; preds = %96
  %14 = icmp eq i64 %8, 512
  br i1 %14, label %12, label %3

15:                                               ; preds = %96, %3
  %16 = phi i64 [ 0, %3 ], [ %56, %96 ]
  %17 = shl i64 %16, 9
  %18 = trunc i64 %17 to i32
  %19 = add i32 %18, 130560
  %20 = and i32 %19, 130560
  %21 = or i32 %20, %7
  %22 = zext i32 %21 to i64
  %23 = getelementptr inbounds i32, i32* %0, i64 %22
  %24 = load i32, i32* %23
  %25 = icmp eq i32 %24, 1
  %26 = zext i1 %25 to i32
  %27 = or i32 %20, %11
  %28 = zext i32 %27 to i64
  %29 = getelementptr inbounds i32, i32* %0, i64 %28
  %30 = load i32, i32* %29
  %31 = icmp eq i32 %30, 1
  %32 = select i1 %25, i32 2, i32 1
  %33 = select i1 %31, i32 %32, i32 %26
  %34 = or i32 %20, %10
  %35 = zext i32 %34 to i64
  %36 = getelementptr inbounds i32, i32* %0, i64 %35
  %37 = load i32, i32* %36
  %38 = icmp eq i32 %37, 1
  %39 = zext i1 %38 to i32
  %40 = add nsw i32 %33, %39
  %41 = trunc i64 %17 to i32
  %42 = or i32 %7, %41
  %43 = zext i32 %42 to i64
  %44 = getelementptr inbounds i32, i32* %0, i64 %43
  %45 = load i32, i32* %44
  %46 = icmp eq i32 %45, 1
  %47 = zext i1 %46 to i32
  %48 = add nsw i32 %40, %47
  %49 = or i32 %10, %41
  %50 = zext i32 %49 to i64
  %51 = getelementptr inbounds i32, i32* %0, i64 %50
  %52 = load i32, i32* %51
  %53 = icmp eq i32 %52, 1
  %54 = zext i1 %53 to i32
  %55 = add nsw i32 %48, %54
  %56 = add nsw i64 %16, 1
  %57 = trunc i64 %17 to i32
  %58 = add i32 %57, 512
  %59 = and i32 %58, 130560
  %60 = or i32 %59, %7
  %61 = zext i32 %60 to i64
  %62 = getelementptr inbounds i32, i32* %0, i64 %61
  %63 = load i32, i32* %62
  %64 = icmp eq i32 %63, 1
  %65 = zext i1 %64 to i32
  %66 = add nsw i32 %55, %65
  %67 = or i32 %59, %11
  %68 = zext i32 %67 to i64
  %69 = getelementptr inbounds i32, i32* %0, i64 %68
  %70 = load i32, i32* %69
  %71 = icmp eq i32 %70, 1
  %72 = zext i1 %71 to i32
  %73 = add nsw i32 %66, %72
  %74 = or i32 %59, %10
  %75 = zext i32 %74 to i64
  %76 = getelementptr inbounds i32, i32* %0, i64 %75
  %77 = load i32, i32* %76
  %78 = icmp eq i32 %77, 1
  %79 = zext i1 %78 to i32
  %80 = add nsw i32 %73, %79
  %81 = add nsw i64 %17, %4
  %82 = getelementptr inbounds i32, i32* %0, i64 %81
  %83 = load i32, i32* %82
  %84 = icmp eq i32 %83, 1
  br i1 %84, label %85, label %91

85:                                               ; preds = %15
  %86 = and i32 %80, -2
  %87 = icmp eq i32 %86, 2
  %88 = getelementptr inbounds i32, i32* %1, i64 %81
  br i1 %87, label %90, label %89

89:                                               ; preds = %85
  store i32 0, i32* %88
  br label %96

90:                                               ; preds = %85
  store i32 1, i32* %88
  br label %96

91:                                               ; preds = %15
  %92 = icmp sgt i32 %80, 2
  %93 = getelementptr inbounds i32, i32* %1, i64 %81
  br i1 %92, label %94, label %95

94:                                               ; preds = %91
  store i32 1, i32* %93
  br label %96

95:                                               ; preds = %91
  store i32 0, i32* %93
  br label %96

96:                                               ; preds = %95, %94, %90, %89
  %97 = icmp eq i64 %56, 256
  br i1 %97, label %13, label %15
}

define dso_local i32 @app(i32 %0) {
  %2 = alloca [131072 x i32]
  %3 = bitcast [131072 x i32]* %2 to i8*
  %4 = alloca [131072 x i32]
  %5 = bitcast [131072 x i32]* %4 to i8*
  call void @llvm.lifetime.start.p0i8(i64 524288, i8* %3)
  call void @llvm.memset.p0i8.i64(i8* %3, i8 0, i64 524288, i1 false)
  call void @llvm.lifetime.start.p0i8(i64 524288, i8* %5)
  call void @llvm.memset.p0i8.i64(i8* %5, i8 0, i64 524288, i1 false)
  br label %6

6:                                                ; preds = %6, %1
  %7 = phi i32 [ 1536, %1 ], [ %14, %6 ]
  %8 = and i32 %7, 3
  %9 = icmp eq i32 %8, 0
  %10 = add nsw i32 %7, 2
  %11 = select i1 %9, i32 %10, i32 %7
  %12 = sext i32 %11 to i64
  %13 = getelementptr inbounds [131072 x i32], [131072 x i32]* %4, i64 0, i64 %12
  store i32 1, i32* %13
  %14 = add nsw i32 %11, 1
  %15 = icmp slt i32 %11, 3583
  br i1 %15, label %6, label %16

16:                                               ; preds = %6
  %17 = getelementptr inbounds [131072 x i32], [131072 x i32]* %4, i64 0, i64 0
  %18 = getelementptr inbounds [131072 x i32], [131072 x i32]* %4, i64 0, i64 65792
  store i32 1, i32* %18
  %19 = getelementptr inbounds [131072 x i32], [131072 x i32]* %4, i64 0, i64 66305
  store i32 1, i32* %19
  %20 = getelementptr inbounds [131072 x i32], [131072 x i32]* %4, i64 0, i64 66815
  store i32 1, i32* %20
  %21 = getelementptr inbounds [131072 x i32], [131072 x i32]* %4, i64 0, i64 66816
  store i32 1, i32* %21
  %22 = getelementptr inbounds [131072 x i32], [131072 x i32]* %4, i64 0, i64 66817
  store i32 1, i32* %22
  %23 = getelementptr inbounds [131072 x i32], [131072 x i32]* %2, i64 0, i64 0
  br label %24

24:                                               ; preds = %31, %16
  %25 = phi i32 [ 0, %16 ], [ %32, %31 ]
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3, i8* %5, i64 524288, i1 false)
  call void @calc_frame(i32* %23, i32* %17)
  br label %27

26:                                               ; preds = %31
  call void @llvm.lifetime.end.p0i8(i64 524288, i8* %5)
  call void @llvm.lifetime.end.p0i8(i64 524288, i8* %3)
  ret void

27:                                               ; preds = %34, %24
  %28 = phi i64 [ 0, %24 ], [ %35, %34 ]
  %29 = trunc i64 %28 to i32
  %30 = trunc i64 %28 to i32
  br label %37

31:                                               ; preds = %34
  call void @simFlush()
  %32 = add nsw i32 %25, 1
  %33 = icmp eq i32 %32, 1000
  br i1 %33, label %26, label %24

34:                                               ; preds = %47
  %35 = add nsw i64 %28, i32 1
  %36 = icmp eq i64 %35, 512
  br i1 %36, label %31, label %27

37:                                               ; preds = %47, %27
  %38 = phi i64 [ 0, %27 ], [ %48, %47 ]
  %39 = shl nuw nsw i64 %38, 9
  %40 = add nsw i64 %39, %28
  %41 = getelementptr inbounds [131072 x i32], [131072 x i32]* %2, i64 0, i64 %40
  %42 = load i32, i32* %41
  %43 = icmp eq i32 %42, 1
  %44 = trunc i64 %38 to i32
  br i1 %43, label %45, label %46

45:                                               ; preds = %37
  call void @simPutPixel(i32 %30, i32 %44, i32 16776960)
  br label %47

46:                                               ; preds = %37
  call void @simPutPixel(i32 %29, i32 %44, i32 0)
  br label %47

47:                                               ; preds = %46, %45
  %48 = add nsw i64 %38, 1
  %49 = icmp eq i64 %48, 256
  br i1 %49, label %34, label %37
}

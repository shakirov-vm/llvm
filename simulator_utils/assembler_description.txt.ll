# First function arg in x0, second in x1

1) LOAD_AND_COMPARE_ONE [result_reg] [source_ptr_reg] [shift_reg]

  %29 = getelementptr inbounds i32, i32* %0, i64 %28
  %30 = load i32, i32* %29, align 4, !tbaa !2
  %31 = icmp eq i32 %30, 1

2) ORZE [result_reg] [or_left_reg] [or_right_reg]

  %27 = or i32 %20, %11
  %28 = zext i32 %27 to i64

3) TRUNC_AA [result_reg] [trunced_reg] {imm}

  %18 = trunc i64 %17 to i32
  %19 = add i32 %18, 130560
  %20 = and i32 %19, 130560

4) ZEA [result_reg] [extended_reg] [added_reg]

  %65 = zext i1 %64 to i32
  %66 = add nuw nsw i32 %55, %65

5) ADD_ICMP_BR [source_reg] {imm} (true_br_lab) (false_br_lab)

  %47 = add nuw nsw i64 %37, 1
  %48 = icmp eq i64 %47, 256
  br i1 %48, label %33, label %36

6) ICMP_BR [source_reg] {imm} (true_br_lab) (false_br_lab)

  %14 = icmp eq i64 %8, 512
  br i1 %14, label %12, label %3

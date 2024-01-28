# First function arg in x0, second in x1, third in x2
# 48 registers

1) LANCO [result_reg] [source_ptr_reg] [shift_reg]

  %29 = getelementptr inbounds i32, i32* %0, i64 %28
  %30 = load i32, i32* %29, align 4, !tbaa !2
  %31 = icmp eq i32 %30, 1

;2) ORZE [result_reg] [or_left_reg] [or_right_reg]

;  %27 = or i32 %20, %11
;  %28 = zext i32 %27 to i64

3) ADAN [result_reg] [source_reg] {imm}

  %19 = add i32 %18, 130560
  %20 = and i32 %19, 130560

;4) ZEA [result_reg] [extended_reg] [added_reg]

;  %65 = zext i1 %64 to i32
;  %66 = add nuw nsw i32 %55, %65

5) ADICBR [source_reg] {imm} (true_br_lab) (false_br_lab)

  %47 = add nuw nsw i64 %37, 1 ; source_reg don't change
  %48 = icmp eq i64 %47, 256
  br i1 %48, label %33, label %36

6) ADICBR_S [incr_reg], {imm}, (true_br_lab), (false_br_lab)
  %16 = add nsw i32 %13, 1      ; incr_reg += 1
  %17 = icmp slt i32 %13, 3583
  br i1 %17, label %8, label %2

7) ICBR [source_reg] {imm} (true_br_lab) (false_br_lab)

  %14 = icmp eq i64 %8, 512
  br i1 %14, label %12, label %3

; All ptr must be aligned to 4
8) GETPTRA [result_reg], [ptr_reg], [index_reg]
  %12 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 %11
;  [result_reg] = getelementptr inbounds [131072 x i32], [131072 x i32]* [ptr_reg], i64 0, i64 [index_reg]

8) GETPTRAI [result_reg], [ptr_reg], {index}
  %17 = getelementptr inbounds [131072 x i32], [131072 x i32]* %3, i64 0, i64 65792
;  [result_reg] = getelementptr inbounds [131072 x i32], [131072 x i32]* [ptr_reg], i64 0, i64 {index}

8) GETPTR [result_reg], [ptr_reg], [index_reg]
  %88 = getelementptr inbounds i32, i32* %1, i64 %81
;  [result_reg] = getelementptr inbounds i32, i32* [ptr_reg], i64 [index_reg]

9) MV [target_reg], [source_reg]
9) MV [target_reg], {imm}
10) ADD [target_reg], [source_reg_1], [source_reg_2]
11) ADDI [target_reg], [source_reg], {imm}
12) SREM [target_reg], [source_reg_1], [source_reg_2]
13) SREMI [target_reg], [source_reg], {imm}
14) SHLI [target_reg], [source_reg], {imm}
15) ANDI [target_reg], [source_reg], {imm}
16) OR [target_reg], [source_reg_1], [source_reg_2]
17) SELECT [target_reg], [switch_reg], [source_reg_1], [source_reg_2]
18) SELECTI [target_reg], [switch_reg], {imm_1}, {imm_2}
19) ICMPS [target_reg], [source_reg], {imm}
19) ICMPL [target_reg], [source_reg], {imm}
20) ICMPEQ [target_reg], [source_reg], {imm}
21) LOAD [target_reg], [source_ptr]
22) STORE [source_ptr], {store_imm}
23) ALLOCA [target_reg], {size_imm}
24) RET [result_reg]
25) BR (br_lab)
26) BRCOND [switch_reg], (true_br_lab), (false_br_lab)
27) GETPTR [result_reg], [ptr], [index]
28) SIMFLUSH
29) SIMPUTPIXEL
30) MEMCPY [dest_reg] [source_reg] [byte_size]

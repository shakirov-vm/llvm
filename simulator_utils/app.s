app 

  ALLOCA x4 131072
  ALLOCA x5 131072
  MVI x6 1536
  BR APP_LABEL6

APP_LABEL6

  ANDI x10 x6 3
  ICMPEQ x11 x10 0
  ADDI x12 x6 2
  SELECT x13 x11 x12 x6
  GETPTRA x14 x5 x13
  STORE x14 1
  ADDI x6 x13 1
  ICMPL x15 x13 3583
  BRCOND x15 APP_LABEL6 APP_LABEL16

APP_LABEL16

  GETPTRAI x16 x5 0
  GETPTRAI x17 x5 65792
  GETPTRAI x18 x5 66305
  GETPTRAI x19 x5 66815
  GETPTRAI x20 x5 66816
  GETPTRAI x21 x5 66817
  STORE x17 1
  STORE x18 1
  STORE x19 1
  STORE x20 1
  STORE x21 1
  GETPTRAI x22 x4 0
  MVI x7 0
  BR APP_LABEL24

APP_LABEL24

  MEMCPY x4 x5 524288
  PUSH x0
  PUSH x1
  MV x0 x22
  MV x1 x16
  CALL calc_frame
  POP x1
  POP x0
  MVI x8 0
  BR APP_LABEL27

APP_LABEL26

  EXIT

APP_LABEL27

  MVI x9 0
  BR APP_LABEL37

APP_LABEL31

  SIMFLUSH
  ADDI x7 x7 1
  ICBR x7 1000 APP_LABEL26 APP_LABEL24

APP_LABEL34

  ADDI x8 x8 1
  ICBR x8 512 APP_LABEL31 APP_LABEL27

APP_LABEL37

  SHLI x25 x9 9 
  ADD x25 x25 x8  
  GETPTRA x25 x4 x25
  LOAD x25 x25
  ICBR x25 1 APP_LABEL45 APP_LABEL46

APP_LABEL45

  PUSH x0
  PUSH x1
  PUSH x2
  MV x0 x8
  MV x1 x9
  MVI x2 16776960
  SIMPUTPIXEL
  POP x2
  POP x1
  POP x0
  BR APP_LABEL47

APP_LABEL46

  PUSH x0
  PUSH x1
  PUSH x2
  MV x0 x8
  MV x1 x9
  MVI x2 0
  SIMPUTPIXEL
  POP x2
  POP x1
  POP x0
  BR APP_LABEL47

APP_LABEL47

  ADDI x9 x9 1
  ICBR x9 256 APP_LABEL34 APP_LABEL37

calc_frame
  
  MVI x4 0
  BR LABEL3

LABEL3

  ADAN x10 x4 511
  ADDI x4 x4 1
  ANDI x11 x4 511
  MVI x5, 0
  BR LABEL15

LABEL12

  RET

LABEL13

  ICBR x4 512 LABEL12 LABEL3

LABEL15

  SHLI x12 x5 9
  ADAN x13 x12 130560
  ADDI x5 x5 1
  ADDI x14 x12 512
  ANDI x15 x14 130560
  ADD x16 x12 x4

  OR x17 x13 x10
  OR x18 x13 x4 
  OR x19 x13 x11
  OR x20 x10 x12
  OR x21 x11 x12
  OR x22 x15 x10
  OR x23 x15 x4
  OR x24 x15 x11

  LANCO x25 x0 x17
  LANCO x26 x0 x18
  LANCO x27 x0 x19
  LANCO x28 x0 x20
  LANCO x29 x0 x21
  LANCO x30 x0 x22
  LANCO x31 x0 x23
  LANCO x32 x0 x24
  LANCO x33 x0 x16

  SELECTI x34 x25 2 1
  SELECT x35 x26 x34 x25
  ADD x35 x35 x27
  ADD x35 x35 x28 
  ADD x35 x35 x29 
  ADD x35 x35 x30 
  ADD x35 x35 x31 
  ADD x35 x35 x32 
  BRCOND x33 LABEL85 LABEL91

LABEL85

  ANDI x36 x35 -2
  GETPTR x37 x1 x16
  ICBR x36 2 LABEL90 LABEL89

LABEL89

  STORE x37 0
  BR LABEL96

LABEL90

  STORE x37 1
  BR LABEL96

LABEL91

  GETPTR x38 x1 x16
  ICMPS x39 x35 2
  BRCOND x39 LABEL94 LABEL95

LABEL94

  STORE x38 1
  BR LABEL96

LABEL95

  STORE x38 0
  BR LABEL96

LABEL96

  ICBR x5 256 LABEL13 LABEL15


\ Bench test 4th
\ DIOS Forth 1.0 on PIC32 80MHz, 115200Bd - 10s

decimal
variable lines                         \ prime program from Forth

: test mod 0= ;
: pprint dup 5 .r lines @ dup 0= if cr drop 4 else 1 - then lines ! ;
: ptest dup 2/ 2
  do dup i test
     if 0 leave then
  1 +loop
  dup if pprint else drop then drop ;
: prime cr 4 lines ! do i ptest 1 +loop cr ;

: chart                                \ shows all previous tests
  ." Platform      OS       Forth       Compiler       Cell    Time" cr
  ." --------      --       -----       --------       ------  ----" cr
  ." Spectrum 48K  ZX-BASIC Artic Forth Z80 assembler  16 bit 53:50 mins" cr
  ." Sun Sp Ultra1 Solaris  4tH V3.2    GCC            32 bit  0:18 mins" cr
  ." RS/6000 250   AIX V3   4tH V3.2    XL C V1.3      32 bit  0:30 mins" cr
  ." P5/600        DOS V5   4tH V3.3    DJGPP V2.72    32 bit  0:05 mins" cr
  ." P5/600        Win 3.1  4tH V3.3    Turbo C++ V3.0 32 bit  0:06 mins" cr
  ." P5/166        DOS V5   4tH V3.2    DJGPP V2.72    32 bit  0:09 mins" cr
  ." P5/166        Win 3.1  4tH V3.2    Turbo C++ V3.0 32 bit  0:21 mins" cr
  ." 486/33        DOS V5   4tH V3.2    Turbo C V2.0   32 bit  2:45 mins" cr
  ." 486/33        DOS V5   4tH V3.2    DJGPP V2.72    32 bit  1:11 mins" cr
  ." 486/33        DOS V5   FPC V3.55   8086 assembler 16 bit  0:29 mins" cr
  ." 486/33        DOS V5   PFE V09.12  Unknown        32 bit  0:45 mins" cr
  ." 486/33        DOS V5   CForth      Turbo C V2.0   16 bit 11:26 mins" cr
  ." 486/66        DOS V5   NIB Forth   Unknown        16 bit 10:55 mins" cr
  ." 486/66        Win V3.1 AtLast      Turbo C++ V3.0 16 bit  5:08 mins" cr
  ." 486/66        Win V3.1 4tH V3.2    Turbo C++ V3.0 32 bit  1:23 mins" cr
  ." 486/66        DOS V5   CForth      Turbo C V2.0   16 bit  5:45 mins" cr
  ." 486/66        DOS V5   4tH V3.2    Turbo C V2.0   32 bit  1:22 mins" cr
  ." THIS COMPUTER Unknown  4tH V3.4    Unknown        32 bit  "
;

: do-sieve                             \ execute the sieve test
  coretim 9999 1 prime cr
  chart coretim swap - 40000000 / u. cr
;


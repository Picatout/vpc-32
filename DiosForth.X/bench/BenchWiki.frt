\ FORTH Benchmarks - http://atariwiki.strotmann.de/wiki/Wiki.jsp

\ DIOS Forth 1.0 - PIC32 80MHz
\ Int calc         0.08
\ Fib1             -
\ Fib2             0.75
\ Nesting 1M       1.40
\ Nesting 32M     44.84
\ Move             0.12
\ Sieve Prime      0.09
\ GCD1             0.44
\ GCD2             0.54
\ Takeuchi         0.05

decimal


\ ----- Integer calc -----
32000 constant intMax
variable intResult

: intcalc
  1 dup intResult dup >r !
  begin
    dup intMax <
  while
    dup negate r@ +! 1+
    dup r@ +! 1+
    r@ @ over * r@ ! 1+
    r@ @ over / r@ ! 1+
  repeat
  r> drop drop
;


\ ----- Fibonacci 1 -----
: fib1 ( n1 -- n2 )
    dup 2 < if drop 1 exit then
    dup  1- recurse
    swap 2 - recurse  + ;

: fib1-bench 1000 0 do i fib1 drop loop ;


\ ----- Fibonacci 2 -----
: fib2 ( n1 -- n2 )
   0 1 rot 0 do
      over + swap loop
   drop ;

: fib2-bench 1000 0 do i fib2 drop loop ;


 \ ----- Forth nesting (NEXT) Benchmark -----
 : bottom ;
 : 1st bottom bottom ;  : 2nd 1st 1st ;      : 3rd 2nd 2nd ;
 : 4th 3rd 3rd ;        : 5th 4th 4th ;      : 6th 5th 5th ;
 : 7th 6th 6th ;        : 8th 7th 7th ;      : 9th 8th 8th ;
 : 10th 9th 9th ;       : 11th 10th 10th ;   : 12th 11th 11th ;
 : 13th 12th 12th ;     : 14th 13th 13th ;   : 15th 14th 14th ;
 : 16th 15th 15th ;     : 17th 16th 16th ;   : 18th 17th 17th ;
 : 19th 18th 18th ;     : 20th 19th 19th ;   : 21th 20th 20th ;
 : 22th 21th 21th ;     : 23th 22th 22th ;   : 24th 23th 23th ;
 : 25th 24th 24th ;
 
 : 32million   cr ." 32 million operations " 25th ;
 :  1million   cr ."  1 million operations " 20th ;


\ ----- Forth Memory Move Benchmark -----
 8192 constant bufsize
 variable buf1 buf1 buf1 !
 variable buf2 buf2 buf2 ! bufsize allot
 
 : test-CMOVE 49 0 do buf1 @ buf2 @ bufsize move loop ;
 : test-CMOVE> 49 0 do buf2 @ buf1 @ bufsize move loop ;
 : test-MOVE> 49 0 do buf1 @ buf2 @ bufsize move loop ;
 : test-<MOVE 49 0 do buf2 @ buf1 @ bufsize move loop ;
 
 : move-bench test-CMOVE test-CMOVE> test-MOVE> test-<MOVE ;


\ ----- Sieve Benchmark -----
 8192 constant size
 variable flags 0 flags ! size allot

 : do-prime
   flags size 1 fill
   0 ( 0 count ) size 0
   do flags i + c@
     if i dup + 3 + dup i +
        begin dup size <
        while 0 over flags + c! over + repeat
        drop drop 1+
     then
  loop
  . ." Primes" cr ;


\ ----- gcd - greatest common divisor -----
 : gcd ( a b -- gcd )
   over if
     begin
       dup while
          2dup u> if swap then over -
     repeat drop else
     dup if nip else 2drop 1 then
   then ;

: gcd1-bench 100 0 do
      100 0 do j i gcd drop loop
      loop ;

: d0= ( d d -- f ) + 0= ;


\ ----- gcd2 runtime speed -----
: gcd2 ( a b -- gcd )
  2dup         d0= if 2drop 1 exit then
  dup           0= if  drop   exit then
  swap dup      0= if  drop   exit then
  begin 2dup -
  while 2dup < if over -
               else swap over - swap
               then
  repeat nip ;

: gcd2-bench 100 0 do
      100 0 do j i gcd2 drop loop
      loop ;


\ ----- takeuchi benchmark -----
 : 3dup 2 pick 2 pick 2 pick ;                                                        

 : tak ( x y z -- t )
   over 3 pick < negate if nip nip exit then
   3dup rot 1- -rot recurse >r
   3dup swap 1- -rot swap recurse >r
             1- -rot recurse
   r> swap r> -rot recurse ;

 : takbench ( -- )
   0 10000 0 do drop 18 12 6 tak loop drop ;


\ ----- All tests [ms] -----
: t1 coretim intcalc    coretim swap - 40000 / . ;
\ : t2 coretim fib1-bench coretim swap - 40000 / . ; \ no 1000
: t3 coretim fib2-bench coretim swap - 40000 / . ;
: t4 coretim 1million   coretim swap - 40000 / . ;
: t5 coretim 32million  coretim swap - 40000 / . ;
: t6 coretim move-bench coretim swap - 40000 / . ;
: t7 coretim do-prime   coretim swap - 40000 / . ;
: t8 coretim gcd1-bench coretim swap - 40000 / . ;
: t9 coretim gcd2-bench coretim swap - 40000 / . ;
: t0 coretim takbench   coretim swap - 40000 / . ;

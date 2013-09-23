\ Bench test GFORTH


\ 486DX2/66
\ relative       Win32-    NT       eforth       This-      
\ time     Gforth Forth Forth eforth  +opt   PFE Forth  TILE
\ sieve      1.00  2.16  1.78   2.16  1.32  2.46  4.96 13.37
\ bubble     1.00  1.93  2.07   2.18  1.29  2.21        5.70
\ matmul     1.00  1.92  1.76   1.90  0.96  2.06        5.32
\ fib        1.00  2.32  2.03   1.86  1.31  2.64  4.55  6.54 


\ DIOS Forth 1.0 on PIC32 80MHz
\  test1 fib   66 seconds
\  test2 sieve 56 seconds

\ sieve  fib
\ 16.29  10.5   PPC603 (Amiga + BlizzardPPC) 160Mhz; gcc-2.95.2
\ 30.0   33.0   PA-RISC 1.1 (HP 720) 50MHz 64K cache; gcc-2.6.3
\ 11.6   15.6   PA8000 (HP C160) 160MHz 64M RAM; gcc-2.7.2
\ 28.5   33.3   SuperSparc (Sparcstation 10) 40MHz; Solaris.5.5.1; gcc-2.7.1
\ 42.82	 48.30  i486 50MHz 256K cache; gcc-2.7.0
\ 25.87	 29.98  i486 66MHz 256K cache; gcc-2.95.1
\ 12.62	 14.97  AMD K6 166MHz 512K PB cache; gcc-2.7.2p


decimal

: fib ( n1 -- n2 )
    dup 2 < if
	drop 1
    else
	dup
	1- recurse
	swap 2 - recurse
	+
    then ;

: test1 34 fib drop 1 . ;


variable flags 8192 allot
variable eflag

: primes  ( -- n )  flags 8192 1 fill 0 3  eflag @ flags
  do   i c@
       if  dup i + dup eflag @ <
           if    eflag @ swap
                 do  0 i c! dup  +loop
           else  drop
           then  swap 1+ swap
       then  2 +
  loop  drop ;

: test2 coretim flags 8192 + eflag ! 0 1000 0 do primes nip loop drop
    coretim swap - 40000000 u/mod u. ;


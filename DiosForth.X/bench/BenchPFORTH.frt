\ Bench test 1-2 pForth

\ pForthV9 on PB5300, 100 MHz PPC 603 based Mac Powerbook
\  bench1  took 8.6 seconds
\  bench2  took 9.0 seconds

\ HForth on PB5300
\  bench1  took 1.1 seconds
\  bench2  took 3.6 seconds

\ DIOS Forth 1.0 on PIC32H 80MHz Duinomite
\  bench1  took 5.4 seconds
\  bench2  took 6.7 seconds
\ words: nop (9clk, 4.3 Mps), dup 1+ 1- drop (49 clk/4, 3.3 Mps),
\        coretim (17 clk, 2.3 Mps), do loop (17clk, 2.3 Mps)


decimal 

create data1 23 , 45 , 67 , 89 , 111 , 222 , 333 , 444 ,

: sum.cells ( addr num -- sum )
	0 swap \ sum
	0 do
		over \ get address
		i 4 * + @ +
	loop
	swap drop
;

: bench1 ( -- )
	200000 0
	do
		data1 8 sum.cells drop
	loop
;


: factorial ( n -- n! )
	dup 1 >
	if
		dup 1- recurse *
	else
		drop 1
	then
;

: bench2 ( -- )
	200000 0
	do
		10 factorial drop
	loop
; 



: test1 coretim bench1 coretim swap - 40000 / . ;  \ [ms]

: test2 coretim bench2 coretim swap - 40000 / . ;  \ [ms]


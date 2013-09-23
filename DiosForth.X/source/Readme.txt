
  DIOS Forth 
=====================================
ver.1.0 - 20.12.2012, (c) Lubos Pekny


  What is DIOS Forth?
----------------------- 
  DIOS Forth 1.0 is first, simple, version 32-bit Forth for PIC32mx795, module Duinomite mini.
Words can be compiled to RAM or flash. RAM for testing or often changed words, flash for embedded
applications. Words are from Forth-83, ANS Forth, not all implemented, writed in C MPLAB.

  DIOS is low level layer (Device Input Output Services), interrupt buffered routines for service
devices, pheripheries on chip PIC32. EMIT call directly UART TX, but is posible call DEFER or
DIOS services. Now are supported devices: timer1, uarts, I/O pins, AD converter, PWM.

  Maximal speed Forth virtual machine for 80MHz PIC32 is 4 Mps (as 4 millions forth words per second), 
empty DO LOOP is 2.3 Mps (millions cycles per second). For comparison with other systems to see benchmark
tests in directory \bench.

  Used memory: 16kB Duinomite boot, 43kB flash, 48kB RAM (12kB uarts, 16kB dictionary, 16kB RAM).
  Free memory: about 440 kB flash, 80kB RAM (PIC32mx795).

  Compiled with C MPLAB 8.83, C32 2.02, gcc 4.5.1. Tested with DUINOMITE-MINI (PIC32MX795F512H 80MHz),
uart2 115200 Bd, windows terminal Forfiter.exe 1.6 .

  DIOS Forth inspired by OpenBIOS (Patrick Mauritz, Stefan Reinauer), amforth (Matthias Trute), Forth-83,
ANS94 Forth.

General Public License (GPL) Version2, see Copying.txt.

www.forth.cz/Download/DIOSforth/DIOSforth.html, forth@email.cz



  DIOS structure
------------------------
    PIC32-->UART-I2C-SPI-CAN-USB-PIN-ADC-TIM-PWM-INT-DMA-NVM-RST-OSC-WDG-PWR-... 
interrupt    |                            |
C           BUF        ...               TIME      ...
            --------------------------------------------------------------------
C DevTab    init, release, open, close, read, write, ?read, ?write, ...
            --------------------------------------------------------------------
Forth       devcall

  For example, incoming data generate UART3 interrupt, data are stored to buffer. Forth call
?READ for test the presence of data: 0 2 COM 6 devcall (0-data, 2-UART3, COM-device, 6-?read).
Then read data from buffer: 0 2 COM 4 devcall (0-data, 2-UART3, COM-device, 4-read).
DEVCALL run indexed functions in table of device DevTab.

          Link        Functions   Name  Info
DevTab = {0          ,DevTabTIM1,"TIM1",0x01}    First 0-device
         {DevTab[0]  ,DevTabCOM, "COM", 0x01}
         ...
         ...
DevHead->{DevTab[i-1],DevTabPWM, "PWM", 0x01}    DevHead pointer to last i-device

DevTabCOM={init, release, open, close, read, write, ?read, ?write, ...}



  Forth structure
------------------------
  Virtual machine contains 17 functions defined in VMcore.h, VMcore.c. Forth words are defined in
VMword.h, VMword.c. Head of all primitive words is in array PRIMWORDS, is different from the new words.
Word FIND search new words in RAM first, then in flash and the end looks primitives.

Primitive words:
  [Link 16b][8b|Flags 3b|Length 5b][pointer C fce][pointer string]

  {0,pr|6,"(exit)",exitw}, {1,pr|7,"execute",executew}, ..., ..., {0xFF,pr,"",nop}

  Link=0 and Link=FF must be used. Other Links isn't used, must be 1..FE .


New words:
  LFA                                                   CFA      PFA
  [Flags 3b|Length 5b|RAM/flash 1b|Link 23b][Name+align][(colon)][....]

Head - pointer to LFA of newest word
Here - pointer to available memory for compiling
Heap - pointer to available RAM memory



  Nonstandard words
--------------------- 
ALLOT   - ( u -- ) alocate RAM memory [byte]

STICK   - ( 2 1 0 5 1 -- 2 5 0 ) overwrite u-position in data stack (inverse word pick)

CORETIM - ( -- u ) read core timer 32b, 40MHz

DEVCALL - ( .. u1 u2 -- .. ) call C function, data are in data stack
          u1-ptr to functions (DevTabCOM), u2-index of function (2-open,4-read,5-write)
          (        ADC 2 --       ) ADC 2 devcall = ADCopen()
          (      0 ADC 4 -- volt  ) TOS=ADCread(), volt overwrite 0
          (      1 COM 4 -- rx    ) TOS=COMread(uart2), rx overwrite 1
          (    7 1 COM 5 -- 7 1   ) COMwrite(uart2,7)
      hex ( 1 9000 PIN 5 -- 1 9000) RB15-LED1+RB12-LED2 ON
  decimal ( 1 2 400 50 PWM 5 -- 1 2 400 50 ) OC1-AUDIO, timer2, 400Hz, 50%

DEVHEAD - ( -- u ) pointer to last device

FLASH   - switch compiler to mode flash, copy FHERE to HERE
>FLASH  - flush buffer to flash
;FLASH  - end of mode flash, write HERE, HEAD, HEAP to last 4kB flash memory of 512kB

<BUILDS - with DOES> is for mode flash, becose CREATE DOES> can not be used for flash,
          DOES> overwrite CFA and PFA, <BUILDS reserves space for this.



  Printscreen
----------------

DUINOMITE  PIC32MX795F512H-80MHz
DIOS Forth 1.0      (c)2012 L.P.

> words
PWM ADC PIN COM TIM1 ccall devcall devhead ;flash >flash flash ms coretim words ver .s
dump \ #> #s # <# hold ( .( ." u.r .r u. . type spaces space cr defer! defer@ defer
to (to) value variable constant literal s" ; : postpone immediate does> <builds create
(create) [compile] compile ['] ' s, w, c, , ascii allot align link> >body cold quit
source refill interpret word parse number find count accept hex decimal bin ] [ bye
abort" abort endcase endof of case unloop leave recurse exit again repeat until while
begin else then if +loop loop ?do do >resolve >mark <resolve <mark #tib tib state pad
>in base bl within d< d> d<> d= u<= u>= u< u> 0> 0< 0= <= >= < > <> = not orl andl xor
or and invert negate 1- 1+ abs max min 2/ 2* << >> >>a */ u*/ mod / /mod u/mod m/mod
um/mod sqrt um* m* * d- d+ - + heap! heap head! head here! here move fill +! w! c! !
w@ c@ @ nop k j i ?key key ?emit emit rp@ sp@ rdepth! rdepth depth! depth r@ r> >r 2swap
swap -rot rot -roll roll stick pick 2over over nip ?dup 2dup dup 2drop drop (val)
(?branch) (branch) (+loop) (loop) (?do) (do) (defer) (does) (var) (con) (slit) (lit)
(colon) execute (exit) 
 ok
> 


  List of words
-----------------
	VMCORE
(exit)  execute  (colon)  (lit)  (slit)  (con)  (var)
(does)  (defer)  (do)  (?do)  (loop)  (+loop)  (branch)  (?branch)
(val)

	STACK
drop  2drop  dup  2dup  ?dup  nip  over  2over
pick  stick  roll  -roll  rot  -rot  swap  2swap
>r  r>  r@  depth  depth!  rdepth  rdepth!  sp@  rp@

	OTHER
emit  ?emit  key  ?key  i  j  k  nop 

	MEMORY
@  c@  w@  !  c!  w!  +!
fill  move  here  here!  head  head!  heap  heap! 

	ARITHMETIC
+  -  d+  d-  *  m*  um*  sqrt
um/mod   m/mod  u/mod  /mod  /  mod  u*/  */
>>a  >>  <<  2*  2/  min  max  abs
1+  1-  negate  invert  and  or  xor   

	LOGIC
andl  orl  not
=  <>  >  <  >=  <=  0=  0<  0>
u>  u<  u>=  u<=  d=  d<>  d>  d<
within

	High FORTH
	VARIABLE
bl  base  >in  pad  state  tib  #tib

	CTRLFLOW
<mark  <resolve  >mark  >resolve  do  ?do  loop  +loop  
if  then  else  begin  while  until  repeat  again
exit  recurse  leave  unloop  case  of  endof  endcase
abort  abort"  bye

	INTERPRETER
[  ]  bin  decimal  hex  accept  count  find
number  parse  word  interpret  refill  source  quit  cold  

	COMPILER
>body  link>  align  allot  ascii  ,  c,  w,
s,  '  [']  compile  [compile]  (create)  create  <builds
does>  immediate  postpone  :  ;  s"  literal  constant
variable  value  (to)  to  defer  defer@  defer!

	EMIT
cr  space  spaces  type  .  u.  .r  u.r
."  .(  (  hold  <#  #  #s  #>
\  dump  .s  ver

	VOCABULARY
words

	DEVICE
coretim  ms  flash  >flash  ;flash  devhead  devcall  ccall
TIM1  COM  PIN  ADC  PWM


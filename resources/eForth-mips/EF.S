# REF: http://www.offete.com/files/zeneForth.htm
# REF: https://getinfo.de/app/MIPS-eForth/id/BLCP%3ACN011970047
# REF: http://www.offete.com/eforth1.html
# cat ef.s
# cat: cannot open ef.s
# <104 weaver2 /people/ting> cd work
# Depend.awk            Makefile.log          ls/
# DoubleDefines         _c/                   polys
# Makefile              _c.1/                 progname*
# Makefile.BAK          _h/                   progname.awk*
# Makefile.bak          _o/                   we.93.Jul.19*
# Makefile.bak.930717   backup/               we.93.Jul.4*
# <105 weaver2 /ting/work> cd ls
# a.out*           eForth.s.14      ef.s.2           test*
# calling.c        eForth.s.15      ef.s.3           test.c
# calling.c.1      eForth.s.16      ef.s.4           test.c.BAK
# calling.o        eForth.s.17      ef.s.5           test.s
# calling.s        eForth.s.2       ef.s.6           test.x
# circuit.c        eForth.s.3       ef.s.7           trees.f
# circuit.f        eForth.s.5       ef.s.8           tty
# circuit.s        eForth.s.6       ef.s.9           tty:
# code.patterns    eForth.s.7       ef.s.BAK         ttyd1
# copy.of.test.c   eForth.s.8       efg.s            ttyd2
# core             eForth.s.9       files            ttyf1
# device.f         eForth.s.BAK     flower.f         ttyf2
# e.l              eForth.s.CKP     flower2.f        ttym1
# eForth.backup    ef.l             gl/              ttym2
# eForth.new       ef.o             gl.h@            walnut.f
# eForth.nl        ef.s             hi.f             win.f
# eForth.o         ef.s.1           man.f            yin.f
# eForth.s         ef.s.10          mandelbrot.f     yin1.f
# eForth.s.10      ef.s.11          plot.f           yin2.f
# eForth.s.11      ef.s.12          red.c            yin3.f
# eForth.s.12      ef.s.13          red.s            yin4.f
# eForth.s.13      ef.s.14          symbols
# <106 weaver2 /work/ls> ^@  cat ef.s

 #      Debugged through OVERT, where a segmentation fault
 #      occurred because o storing LAST into FORTH. 7/23/93 cht
 #      Change vocabulary data field to .data segment.
 #      Use scanf to get a line in QUERY.  scanf does not return
 #      character count.  sad.
 #      Try read() to get characters one at a time.
 #      7/30/93 interpreter works.  does not compile due to segmentation.
 #      8/3/93  Change to indirect threaded code.  separate code
 #              from colon definitions.  compiler works.
 #      8/6/93  Test read file hi.f.
 #      8/8/93  Test INCLUDE.  Works ine on hi.f.
 #      8/10/93 Full optimization
 #              Add dovar, docon, douse, dovov.
 #              Copied from eForth.s.17
 #              Add 0, 1, 2, 3 and replace 'DOLIT,0' and 'DOLIT,1'.
 #              Add GRAPHICS and APPLICATION vocabularies.
 #              Change all user variables, variables and vocs.
 #      8/12/93 Add floating point package.
 #      8/15/93 Add graphics package.  Initial entering word list.
 #      8/18/93 Add F<.  Failed to fix UM/MOD.
 #      8/19/93 Implement true UM/MOD and high level */MOD.
 #      8/20/93 Add low level Mandelbrot words.
 #      8/21/93 Mandelbrot debugged.  INCLUDE win.f and man.f files.
 #              Add input device interface.
 #      8/25/93 Add cpack, gconfig, polf, polfi, and RGBmode.
 #      8/26/93 Add lookat and perspective.
 #      9/08/93 Add 2*, 2/, pnt2i and pnti.
 #      9/09/93 Fix draw2i and move2i for fern drawing.

.extern UartPutch
.extern UartGetch
.extern UartReadln
.extern UartPrint
.extern debug_print
.extern UartWaitch

#define stdin    1
#define stdout   1

#define getchar UartGetch
#define putchar UartPutch
#define read    UartReadln
#define print   UartPrint
#define printf  debug_print
#define waitch  UartWaitch

        .verstamp       2 40
        .align  4, 0xFF
        .set    macro
        .set    noreorder

        COMPO=0X40
        IMEDD=0X80
        MASKK=0X1F7F7F7F
        CELLL=4
        BASEE=10
        VOCSS=8
        BKSPP=8
        LF=10
        CRR=13          # can''t input a CR
        ERR=27
        TIC=39

#     ax=$t0
#     bx=$t1
#     cx=$t2
#     dx=$t3

    DP=$s0  # vm data stack pointer
    RP=$s1  # vm return address stack pointer
    IP=$s2  # vm instruction pointer
    WP=$s3  # vm working pointer


        .text
        .globl  eForth
        .ent    eForth

eForth: sw      DP, SAVSP
        sw      $ra, SAVLINK
        la      DP, SPP-12
        sw      WP, 8(DP)
        sw      RP, 4(DP)
        sw      IP, 0(DP)
        subu    $sp, $sp, 64
        nop


        la      IP, COLD1

 # Test High Level Words
 #      la      IP, QUERY+8
 #      la      IP, IOTST+8

 #      addiu   DP, -8         # test 'find'
 #      la      $t0, FVOC
 #      sw      $t0, 0(DP)
 #      la      $t0, p6
 #      sw      $t0, 4(DP)
 #      la      IP, FIND+8

        la      RP, RPP-4
        sw      IP, RPP-4

 # Indirect threaded inner interpreter.

$next:
        la     $t0, TRACE
        lw     $t1, 0($t0)
        beqz   $t1, 1f
        nop
        jal    DBG
        nop
1:
        lw      $t0, 0(IP)      # get next word in list
        addiu   IP, IP, 4     # increment ip
        lw      $t1, 0($t0)       # get address in code field
        move    WP, $t0
        j       $t1              # indirect jump to next word
        addiu   WP, WP, 4     # set up wp to pfa
        
 # Call DBG in $next or in dolst to sinlge step Forth code.

DBG:    sw      DP, ULAST      # display sp, ip, wp
        sw      RP, ULAST+4    #         s0, s1, s2
        sw      IP, ULAST+8    #         rp, r0, r1
        sw      $ra, ULAST+12   # call from either $next or dolst
        sw      WP, ULAST+16
        la      $a0, msg9
        move    $a1, DP
        move    $a2, IP
        lw      $a3, 0(IP)
        jal     printf
        nop
        la      $a0, msg10
        lw      $a1, 0(DP)
        lw      $a2, 4(DP)
        lw      $a3, 8(DP)
        jal     printf
        nop
        la      $a0, msg8
        move    $a1, RP
        lw      $a2, 0(RP)
        lw      $a3, 4(RP)
        jal     printf
        nop
        li      $a0, stdin
        li      $a1, 0
        jal     waitch
        nop
        andi    $v0, $v0, 255
        xori    $v0,$v0,'q'
        bnez    $v0, 1f
        nop
        la      $t0, TRACE
        sw      $zero, 0($t0)
1:
        lw      WP, ULAST+16
        lw      $ra, ULAST+12
        lw      DP, ULAST
        lw      RP, ULAST+4
        lw      IP, ULAST+8
        j       $ra
        nop

BYE:    .word   BYE+4
        lw      $ra, SAVLINK
        la      DP, SPP-12
        lw      WP, 8(DP)
        lw      RP, 4(DP)
        lw      IP, 0(DP)
        lw      DP, SAVSP
        addu    $sp, $sp, 64
        j       $ra
        nop


DPRT: .word DPRT+4   # debug print
        jal DBG
        nop
        j $next
        nop

TRON:  .word  TRON+4  # trace on
        la $t0, TRACE
        li  $t1, 1
        j $next
        sw  $t1, 0($t0)

TROFF: .word  TROFF+4 # trace off
        la  $t0, TRACE
        j   $next
        sw  $zero, 0($t0)

QRX:    .word   QRX+4
        li      $a0, stdin      # UART device
        jal     getchar
        nop
        addiu   DP, DP, -4
        beq     $v0, $zero, $next
        sw      $zero, 0(DP)
        li      $t1, -1
        sw      $v0, 0(DP)
        addiu   DP, DP, -4
        j       $next
        sw      $t1, 0(DP)
        
TXSTO:  .word   TXSTO+4
        li      $a0, stdin
        lw      $a1, 0(DP)
        addiu   DP, DP, 4
        jal     putchar
        nop
        j       $next
        nop

# IOTST:        .word   dolst
# IOTS1:        .word   QRX, QBRAN, IOTS2
# IOTS2:        .word   TXSTO, BRAN, IOTS1

STOIO:  .word   STOIO+4
        j       $next
        nop

DOLIT:  .word   DOLIT+4
        lw      $t0, 0(IP)
        addiu   DP, DP, -4
        addiu   IP, IP, 4
        j       $next
        sw      $t0, 0(DP)

 # Colon word interpreter to lead the colon words

dolst:
DOLST:  addiu   RP, RP, -4    # push ip
        sw      IP, 0(RP)     # onto rp
        move    IP, WP        # move wp into ip
#        nop

 #      jal     DBG
 #      nop
 
        j       $next           # execute list in pf
        move IP, WP
#        nop

docon:  lw      WP, 0(WP)
dovar:  addiu   DP, DP, -4
        j       $next
        sw      WP, 0(DP)

douse:  addiu   DP, DP, -4
        lw      WP, 0(WP)
        la      $t0, UPP
        addu    $t0, $t0, WP
        j       $next
        sw      $t0, 0(DP)

dovoc:  la      $t0, CTXT
        j       $next
        sw      WP, 0($t0)

EXIT:   .word   EXIT+4
        lw      IP, 0(RP)     # get ip from rp
        j       $next           # return to calling list
        addiu   RP, RP, 4     # pop rp
        
EXECU:  .word   EXECU+4
        lw      $t0, 0(DP)      # get cfa from sp
        addiu   DP, DP, 4     # pop sp
        lw      $t1, 0($t0)       # get address in code field
        move    WP, $t0         # WP = t0
        j       $t1              # indirect jump to next word
        addiu   WP, WP, 4     # set up wp to pfa

DONXT:  .word   DONXT+4
        lw      $t0, 0(RP)
        nop
        beq     $t0, $zero, NEXT1
        nop
        addiu   $t0, $t0, -1
        sw      $t0, 0(RP)
        lw      IP, 0(IP)
        j       $next
        nop
NEXT1:  addiu   IP, IP, 4
        j       $next
        addiu   RP, RP, 4

        
QBRAN:  .word   QBRAN+4
        lw      $t0, 0(DP)
        addiu   DP, DP, 4
        beq     $t0, $zero, BRAN1
        nop
        addiu   IP, IP, 4
        j       $next
        nop
BRAN1:  lw      IP, 0(IP)
        j       $next
        nop

BRAN:   .word   BRAN+4
        lw      IP, 0(IP)
        j       $next
        nop

STORE:  .word   STORE+4
        lw      $t1, 0(DP)
        lw      $t0, 4(DP)
        addiu   DP, DP, 8
        j       $next
        sw      $t0, 0($t1)

AT:     .word   AT+4
        lw      $t1, 0(DP)
#        nop
        lw      $t0, 0($t1)
        j       $next
        sw      $t0, 0(DP)
        
CSTOR:  .word   CSTOR+4
        lw      $t1, 0(DP)
        lw      $t0, 4(DP)
        addiu   DP, DP, 8
        j       $next
        sb      $t0, 0($t1)

CAT:    .word   CAT+4
        lw      $t1, 0(DP)
        nop
        lbu     $t0, 0($t1)
        j       $next
        sw      $t0, 0(DP)
        
WSTOR:  .word   WSTOR+4
        lw      $t1, 0(DP)
        lw      $t0, 4(DP)
        addiu   DP, DP, 8
        j       $next
        sh      $t0, 0($t1)

WAT:    .word   WAT+4
        lw      $t1, 0(DP)
        nop
        lhu     $t0, 0($t1)
        j       $next
        sw      $t0, 0(DP)
        
RPAT:   .word   RPAT+4
        addiu   DP, DP, -4
        j       $next
        sw      RP, 0(DP)

RPSTO:  .word   RPSTO+4
        lw      $t0, 0(DP)
        addiu   DP, DP, 4
        j       $next
        or      RP, $t0, $zero
        
RFROM:  .word   RFROM+4
        lw      $t0, 0(RP)
        addiu   DP, DP, -4
        sw      $t0, 0(DP)
        j       $next
        addiu   RP, RP, 4

RAT:    .word   RAT+4
        lw      $t0, 0(RP)
        addiu   DP, DP, -4
        j       $next
        sw      $t0, 0(DP)

TOR:    .word   TOR+4
        lw      $t0, 0(DP)
        addiu   RP, RP, -4
        sw      $t0, 0(RP)
        j       $next
        addiu   DP, DP, 4

SPAT:   .word   SPAT+4
        or      $t0, DP, $zero
        addiu   DP, DP, -4
        j       $next
        sw      $t0, 0(DP)

SPSTO:  .word   SPSTO+4
        j       $next
        lw      DP, 0(DP)
        
DROP:   .word   DROP+4
        j       $next
        addiu   DP, DP, 4

DUPP:   .word   DUPP+4
        lw      $t0, 0(DP)
        addiu   DP, DP, -4
        j       $next
        sw      $t0, 0(DP)

SWAP:   .word   SWAP+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        sw      $t0, 4(DP)
        j       $next
        sw      $t1, 0(DP)

OVER:   .word   OVER+4
        lw      $t0, 4(DP)
        addiu   DP, DP, -4
        j       $next
        sw      $t0, 0(DP)

ZLESS:  .word   ZLESS+4
        lw      $t0, 0(DP)
        li      $t1, -1
        blt     $t0, $zero, $next
        sw      $t1, 0(DP)
        j       $next
        sw      $zero, 0(DP)
        
ANDD:   .word   ANDD+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        and     $t0, $t0, $t1
        j       $next
        sw      $t0, 0(DP)

ORR:    .word   ORR+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        or      $t0, $t0, $t1
        j       $next
        sw      $t0, 0(DP)

XORR:   .word   XORR+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        xor     $t0, $t0, $t1
        j       $next
        sw      $t0, 0(DP)

UPLUS:  .word   UPLUS+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        nop
        addu    $t0, $t0, $t1
        sltu    $t2, $t0, $t1
        sw      $t0, 4(DP)
        j       $next
        sw      $t2, 0(DP)

 ######  Following words are optimized for MIPS uP #####

QDUP:   .word   QDUP+4
        lw      $t0, 0(DP)
#        nop
        beq     $t0, $zero, $next
        nop
        addiu   DP, DP, -4
        j       $next
        sw      $t0, 0(DP)

ROT:    .word   ROT+4
        lw $t0, 0(DP)
        lw $t1, 4(DP)
        lw $t2, 8(DP)
        sw $t0, 4(DP)
        sw $t1, 8(DP)
        j       $next
        sw $t2, 0(DP)

DDROP:  .word   DDROP+4
        j       $next
        addiu DP, DP, 8

DDUP:   .word   DDUP+4
        lw $t0, 0(DP)
        lw $t1, 4(DP)
        addiu DP, DP, -8
        sw $t0, 0(DP)
        j $next
        sw $t1, 4(DP)

PLUS:   .word   PLUS+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        addu    $t0, $t0, $t1
        j       $next
        sw      $t0, 0(DP)


DPLUS:  .word   DPLUS+4
        lw      $t0, 12(DP)
        lw      $t1, 8(DP)
#        nop
        addu    $t0, $t0, $t1
        sltu    $t2, $t0, $t1
        sw      $t0, 12(DP)
        lw      $t0, 4(DP)
        lw      $t1, 0(DP)
        addiu   DP, DP, 8
        addu    $t0, $t0, $t1
        addu    $t0, $t0, $10
        j       $next
        sw      $t0, 0(DP)


INVER:  .word   INVER+4
        lw      $t0, 0(DP)
        li      $t1, -1
        xor     $t0, $t0, $t1
        j       $next
        sw      $t0, 0(DP)


NEGAT:  .word   NEGAT+4
        lw      $t0, 0(DP)
        nop
        subu    $t0, $zero, $t0
        j       $next
        sw      $t0, 0(DP)

SUBB:   .word   SUBB+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        subu    $t0, $t1, $t0
        j       $next
        sw      $t0, 0(DP)

ABSS:   .word   ABSS+4
        lw      $t0, 0(DP)
        nop
        bge     $t0, $zero, $next
        subu    $t0, $zero, $t0
        j       $next
        sw      $t0, 0(DP)


EQUAL:  .word   EQUAL+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        beq     $t0, $t1, EQUA1
        li      $t0, -1
        li      $t0, 0
EQUA1:  j       $next
        sw      $t0, 0(DP)


ULESS:  .word   ULESS+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        bltu    $t1, $t0, ULES1
        li      $t0, -1
        li      $t0, 0
ULES1:  j       $next
        sw      $t0, 0(DP)

  # signed compare of top two

LESS:   .word   LESS+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        blt     $t1, $t0, LESS1
        li      $t0, -1
        li      $t0, 0
LESS1:  j       $next
        sw      $t0, 0(DP)

  # return greater of top two

MAX:    .word   MAX+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        bgt     $t1, $t0, $next
        nop
        j       $next
        sw      $t0, 0(DP)

  # return smaller of top two

MIN:    .word   MIN+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        addiu   DP, DP, 4
        blt     $t1, $t0, $next
        nop
        j       $next
        sw      $t0, 0(DP)


  # unsigned divide of a double by a single, return mod and quotient

UMMOD:  .word   UMMOD+4
        lw      $t0, 0(DP)      # divisor
        lw      $t1, 4(DP)      # high
        lw      $t2, 8(DP)     # low
        bgeu    $t1, $t0, UMOD4   # overflow?
        addiu   DP, DP, 4
        li      $t3, 1          # shift 1 bit left
        li      $t5, 32         # process 32 bits
UMOD1:  sll     $t1, $t1, $11     # shift high
        bltz    $t2, UMOD2      # test low
        li      $t4, 1          # carry
        li      $t4, 0          # no carry
UMOD2:  addu    $t1, $t1, $12     # add carry to high
        bltu    $t1, $t0, UMOD3   # test high
        sll     $t2, $t2, $11   # shift low
        subu    $t1, $t1, $t0      # conditional subtract
        addu    $t2, $t2, $11   # add bit to quotient
UMOD3:  subu    $t5, $t5, 1
        bnez    $t5, UMOD1      # repeat 32 times
        nop
        sw      $t2, 0(DP)     # get quotient
        j       $next
        sw      $t1, 4(DP)      # get remainder
UMOD4:  sw      $zero, 4(DP)
        li      $t0, -1
        j       $next
        sw      $t0, 0(DP)

# signed floored divide of double by single, return mod and quotient
# upper 32 bits are ignored.  In effect a 32 bits divide.

 # MSMOD:       .word   MSMOD+4
        lw      $t0, 0(DP)
        lw      $t1, 8(DP)
        addiu   DP, DP, 4
        div     $t1, $t0
        mfhi    $t0
        mflo    $t1
        sw      $t0, 4(DP)
        j       $next
        sw      $t1, 0(DP)

# signed divide, return mod and quotient

SLMOD:  .word   SLMOD+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        nop
        div     $t1, $t0
        mfhi    $t0
        mflo    $t1
        sw      $t0, 4(DP)
        j       $next
        sw      $t1, 0(DP)

# signed divide, return mod only

MODD:   .word   MODD+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        nop
        div     $t1, $t0
        mfhi    $t0
        addiu   DP, DP, 4
        j       $next
        sw      $t0, 0(DP)

# signed divide, return quotient only

SLASH:  .word   SLASH+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        nop
        div     $t1, $t0
        mflo    $t1
        addiu   DP, DP, 4
        j       $next
        sw      $t1, 0(DP)

# unsigned multiply, return double product

UMSTA:  .word   UMSTA+4
        lw $t0, 0(DP)
        lw $t1, 4(DP)
        nop
        multu $t0, $t1
        mflo $t0
        mfhi $t1
        sw      $t0, 4(DP)
        j $next
        sw      $t1, 0(DP)

# signed multiply, return single product

STAR:   .word   STAR+4
        lw $t0, 0(DP)
        lw $t1, 4(DP)
        nop
        mult $t0, $t1
        mflo $t0
        sw      $t0, 4(DP)
        j $next
        addiu DP, DP, 4

# signed multiply, return double product

MSTAR:  .word   MSTAR+4
        lw $t0, 0(DP)
        lw $t1, 4(DP)
        nop
        mult $t0, $t1
        mflo $t0
        mfhi $t1
        sw      $t0, 4(DP)
        j $next
        sw      $t1, 0(DP)

CELLP:  .word   CELLP+4
        lw      $t0, 0(DP)
#        nop
        addiu   $t0, CELLL
        j       $next
        sw      $t0, 0(DP)

CELLM:  .word   CELLM+4
        lw      $t0, 0(DP)
        nop
        addiu   $t0, -(CELLL)
        j       $next
        sw      $t0, 0(DP)


CELLS:  .word   CELLS+4
        lw      $t0, 0(DP)
        li      $t1, CELLL
        mult    $t0, $t1
        mflo    $t0
        j       $next
        sw      $t0, 0(DP)

TWOST:  .word   TWOST+4
        lw      $t0, 0(DP)
        nop
        sll     $t0, $t0, 1
        j       $next
        sw      $t0, 0(DP)

TWOSL:  .word   TWOSL+4
        lw      $t0, 0(DP)
        nop
        sra     $t0, $t0, 1
        j       $next
        sw      $t0, 0(DP)

ALGND:  .word   ALGND+4
        lw      $t0, 0(DP)
        li      $t1, (CELLL-1)
        and     $t2, $t1, $t0
        beq     $t2, $zero, $next
        xor     $t0, $t0, $t2
        addiu   $t0, CELLL
        j       $next
        sw      $t0, 0(DP)

PICK:   .word   PICK+4
        lw      $t0, 0(DP)
        li      $t1, CELLL
        addiu   $t0, 1
        multu   $t0, $t1
        mflo    $t1
        addu    $t1, $t1, DP
        lw      $t0, 0($t1)
        j       $next
        sw      $t0, 0(DP)

CMOVE:  .word   CMOVE+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        lw      $t2, 8(DP)
        addiu   DP, 12
CMOV1:  beq     $t0, $zero, $next
        lb      $t3, 0($10)
        addiu   $t2, 1
        sb      $t3, 0($t1)
        addiu   $t0, -1
        j       CMOV1
        addiu   $t1, 1

MOVEE:  .word   MOVEE+4
        lw      $t0, 0(DP)
        lw      $t1, 4(DP)
        lw      $t2, 8(DP)
        addiu   DP, 12
MOV1:   beq     $t0, $zero, $next
        lw      $t3, 0($10)
        addiu   $t2, 4
        sw      IP, 0($t1)
        addiu   $t0, -1
        j       CMOV1
        addiu   $t1, 4


##### Mandelbrot plot #####

# MANDE:  .word   MANDE+4 # ( max a b -- n )
#         lw      $t6, 8(DP)
#         lw      $v0, 4(DP)
#         lw      $v1, 0(DP)
#         move    $a0, $2          # x
#         move    $a1, $3          # y
# MAND1:  mult    $a0, $a0          # x*x
#         mflo    $a2
#         mfhi    $a3
#         lui     $t4, 0x1000     # 4.0, limit
#         addiu   $t6, $t6, -1    # bump n
#         mult    $a1, $a1          # y*y
#         mflo    $t0
#         mfhi    $t1
#         addu    $t2, $t0, $a2     # xx+yy
#         sltu    $t3, $t2, $a2
#         addu    $t3, $t3, $a3
#         addu    $t3, $t3, $t1
#         bge     $t3, $t4, MAND2
#         sltu    $t5, $a2, $t0     # xx-yy
#         subu    $t3, $a2, $t0
#         subu    $t4, $a3, $t1
#         subu    $t4, $t4, $13
#         sll     $t4, $t4, 3     # xx-yy normalized
#         srl     $t3, $t3, 29
#         or      $t5, $t3, $12
#         mult    $a0, $a1          # xy
#         mflo    $11
#         mfhi    $12
#         addu    $a0, $t5, $2     # new x
#         sll     $t4, $t4, 4     # normalize 2xy
#         srl     $t3, $t3, 28
#         or      $t5, $t3, $12
#         bnez    $t6, MAND1
#         addu    $a1, $t5, $3     # new y
# MAND2:  addiu   DP, DP, 8
#         j       $next
#         sw      $t6, 0(DP)


##### floating point numbers #####


# FLOA:   .word   FLOA+4
#         l.s     $f0, 0(DP)
#         nop
#         cvt.s.w $f2, $f0
#         s.s     $f2, 0(DP)
#         j       $next
#         nop
#
# INT:    .word   INT+4
#         l.s     $f0, 0(DP)
#         nop
#         cvt.w.s $f2, $f0
#         s.s     $f2, 0(DP)
#         j       $next
#         nop

# FDOT:   .word   FDOT+4
#         la      $a0, msg6
#         l.s     $f0, 0(DP)
#         nop
#         cvt.d.s $f2, $f0
#         mfc1.d  $a2, $f2
#         jal     printf
#         nop
#         j       $next
#         addiu   DP, DP, 4
#
# FPLUS:  .word   FPLUS+4
#         l.s     $f0, 0(DP)
#         l.s     $f2, 4(DP)
#         nop
#         add.s   $f2, $f2, $f0
#         addiu   DP, DP, 4
#         j       $next
#         s.s     $f2, 0(DP)
#
# FSUB:   .word   FSUB+4
#         l.s     $f0, 0(DP)
#         l.s     $f2, 4(DP)
#         nop
#         sub.s   $f2, $f2, $f0
#         addiu   DP, DP, 4
#         j       $next
#         s.s     $f2, 0(DP)
#
# FMUL:   .word   FMUL+4
#         l.s     $f0, 0(DP)
#         l.s     $f2, 4(DP)
#         nop
#         mul.s   $f2, $f2, $f0
#         addiu   DP, DP, 4
#         j       $next
#         s.s     $f2, 0(DP)
#
# FDIV:   .word   FDIV+4
#         l.s     $f0, 0(DP)
#         l.s     $f2, 4(DP)
#         nop
#         div.s   $f2, $f2, $f0
#         addiu   DP, DP, 4
#         j       $next
#         s.s     $f2, 0(DP)

# PI:     .word   docon
#         .float  3.14159274
#
# ECON:   .word   docon
#         .float  2.718281813
#
# FZERO:  .word   docon
#         .float  0.0
#
# FONE:   .word   docon
#         .float  1.0
#
# FTWO:   .word   docon
#         .float  2.0
#
# FTEN:   .word   docon
#         .float  10.0
#
# FSIN:   .word   FSIN+4
#         l.s     $f12, 0(DP)
#         jal     sinf
#         nop
#         j       $next
#         s.s     $f0, 0(DP)
#
# FCOS:   .word   FCOS+4
#         l.s     $f12, 0(DP)
#         jal     cosf
#         nop
#         j       $next
#         s.s     $f0, 0(DP)
#
# FTAN:   .word   FTAN+4
#         l.s     $f12, 0(DP)
#         jal     tanf
#         nop
#         j       $next
#         s.s     $f0, 0(DP)
#
# FATAN:  .word   FATAN+4
#         l.s     $f12, 0(DP)
#         jal     atanf
#         nop
#         j       $next
#         s.s     $f0, 0(DP)
#
# FSQRT:  .word   FSQRT+4
#         l.s     $f12, 0(DP)
#         jal     sqrtf
#         nop
#         j       $next
#         s.s     $f0, 0(DP)
#
# FLESS:  .word   FLESS+4
#         l.s     $f0, 0(DP)
#         l.s     $f2, 4(DP)
#         li      $t0, -1
#         c.lt.s  $f2, $f0
#         addiu   DP, DP, 4
#         bc1t    FLES1
#         nop
#         li      $t0, 0
# FLES1:  j       $next
#         sw      $t0, 0(DP)

#         .word   FLOA, f01
# f00:    .ascii  "\005FLOAT"
#         .word   INT, f02
# f01:    .ascii  "\003INT"
#         .word   FPLUS, f03
# f02:    .ascii  "\002F+"
#         .word   FSUB, f04
# f03:    .ascii  "\002F-"
#         .word   FMUL, f05
# f04:    .ascii  "\002F*"
#         .word   FDIV, f06
# f05:    .ascii  "\002F/"
#         .word   FZERO, f07
# f06:    .ascii  "\0030.0"
#         .word   FONE, f08
# f07:    .ascii  "\0031.0"
#         .word   FTWO, f09
# f08:    .ascii  "\0032.0"
#         .word   FTEN, f10
# f09:    .ascii  "\00410.0"
#         .word   PI, f11
# f10:    .ascii  "\002PI"
#         .word   ECON, f12
# f11:    .ascii  "\001e"
#         .word   FSIN, f13
# f12:    .ascii  "\004FSIN"
#         .word   FCOS, f14
# f13:    .ascii  "\004FCOS"
#         .word   FTAN, f15
# f14:    .ascii  "\004FTAN"
#         .word   FATAN, f16
# f15:    .ascii  "\005FATAN"
#         .word   FSQRT, f17
# f16:    .ascii  "\005FSQRT"
#         .word   FDOT, f18
# f17:    .ascii  "\002F."
#         .word   FLESS, 0
# f18:    .ascii  "\002F<"
        .word   0,0,0,0


##### names for colors in color map loaded by greset #####


# BLACK:  .word   docon,0
# RED:    .word   docon,1
# GREEN:  .word   docon,2
# YELLOW: .word   docon,3
# BLUE:   .word   docon,4
# MAGENTA:        .word   docon,5
# CYAN:   .word   docon,6
# WHITE:  .word   docon,7

# GLXconfig *      GLXgetconfig( void *, long, GLXconfig * );
# void     acbuf( long, float );
# void     acsize( long );
# void     addtopup( long, String, ... );
# void     afunction( long, long );
# void     arc( Coord, Coord, Coord, Angle, Angle );
# void     arcf( Coord, Coord, Coord, Angle, Angle );
# void     arcfi( Icoord, Icoord, Icoord, Angle, Angle );
# ARCFI:  .word   ARCFI+4
#         lw      $a0, 0(DP)
#         nop
#         sw      $a0, 16($sp)
#         lw      $a0, 16(DP)
#         lw      $a1, 12(DP)
#         lw      $a2, 8(DP)
#         lw      $a3, 4(DP)
#         jal     arcfi
#         nop
#         j       $next
#         addiu   DP, DP, 20
#
#  # void     arcfs( Scoord, Scoord, Scoord, Angle, Angle );
#  # void     arci( Icoord, Icoord, Icoord, Angle, Angle );
# ARCI:   .word   ARCI+4
#         lw      $a0, 0(DP)
#         nop
#         sw      $a0, 16($sp)
#         lw      $a0, 16(DP)
#         lw      $a1, 12(DP)
#         lw      $a2, 8(DP)
#         lw      $a3, 4(DP)
#         jal     arci
#         nop
#         j       $next
#         addiu   DP, DP, 20
#
#  # void     arcs( Scoord, Scoord, Scoord, Angle, Angle );
#  # void     attachcursor( Device, Device );
#  # void     backbuffer( Boolean );
#  # void     backface( Boolean );
#  # void     bbox2( Screencoord, Screencoord, Coord, Coord, Coord, Coord );
#  # void     bbox2i( Screencoord, Screencoord, Icoord, Icoord, Icoord, Icoord );
#  # void     bbox2s( Screencoord, Screencoord, Scoord, Scoord, Scoord, Scoord );
#  # void     bgnclosedline( void );
# BCLIN:  .word   BCLIN+4
#         jal     bgnclosedline
#         nop
#         j       $next
#         nop
#
#  # void     bgncurve( void );
#  # void     bgnline( void );
# BLINE:  .word   BLINE+4
#         jal     bgnline
#         lw      $a0, 0(DP)
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     bgnpoint( void );
# BPNT:   .word   BPNT+4
#         jal     bgnpoint
#         nop
#         j       $next
#         nop
#
#  # void     bgnpolygon( void );
# BPOLY:  .word   BPOLY+4
#         jal     bgnpolygon
#         nop
#         j       $next
#         nop
#
#
#  # void     bgnqstrip( void );
#  # void     bgnsurface( void );
#  # void     bgntmesh( void );
# BMESH:  .word   BMESH+4
#         jal     bgntmesh
#         nop
#         j       $next
#         nop
#
#  # void     bgntrim( void );
#  # void     blankscreen( Boolean );
#  # void     blanktime( long );
#  # void     blendfunction( long, long );
#  # void     blink( short, Colorindex, short, short, short );
#  # long     blkqread( short[], short );
#  # void     c3f( const float[3] );
#  # void     c3i( const long[3] );
#  # void     c3s( const short[3] );
#  # void     c4f( const float[4] );
#  # void     c4i( const long[4] );
#  # void     c4s( const short[4] );
#  # void     callfunc( __PFV_, long, ... );          /* not recommended */
#  # void     callobj( Object );
#  # void     charstr( String );
# CHRST:  .word   CHRST+4
#         lw      $a0, 0(DP)
#         jal     charstr
#         nop
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     chunksize( long );
#  # void     circ( Coord, Coord, Coord );
#  # void     circf( Coord, Coord, Coord );
#  # void     circfi( Icoord, Icoord, Icoord );
# CIRFI:  .word   CIRFI+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         jal     circfi
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#
#  # void     circfs( Scoord, Scoord, Scoord );
#  # void     circi( Icoord, Icoord, Icoord );
# CIRCI:  .word   CIRCI+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         jal     circi
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     circs( Scoord, Scoord, Scoord );
#  # void     clear( void );
# CLEAR:  .word   CLEAR+4
#         jal     clear
#         nop
#         j       $next
#         nop
#
#  # void     clearhitcode( void );                   /* not recommended */
#  # void     clipplane( long, long, const float[] );
#  # void     clkoff( void );
#  # void     clkon( void );
#  # void     closeobj( void );
#  # void     cmode( void );
#  # void     cmov( Coord, Coord, Coord );
#  # void     cmov2( Coord, Coord );
#  # void     cmov2i( Icoord, Icoord );
# CMOV2:  .word   CMOV2+4
#         lw      $a0, 12(DP)
#         jal     cmov2i
#         lw      $a1, 8(DP)
#         j       $next
#         addiu   DP, DP, 8
#
#  # void     cmov2s( Scoord, Scoord );
#  # void     cmovi( Icoord, Icoord, Icoord );
# CMOVI:  .word   CMOVI+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         jal     cmovi
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     cmovs( Scoord, Scoord, Scoord );
#  # void     color( Colorindex );
# COLOR:  .word   COLOR+4
#         lw      $a0, 0(DP)
#         jal     color
#         nop
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     colorf( float );
#  # void     compactify( Object );
#  # void     concave( Boolean );
#  # void     cpack( unsigned long );
# CPACK:  .word   CPACK+4
#         lw      $a0, 0(DP)
#         jal     cpack
#         nop
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     crv( const Coord[4][3] );
#  # void     crvn( long, const Coord[][3] );
#  # void     curorigin( short, short, short );
#  # void     cursoff( void );
#  # void     curson( void );
#  # void     curstype( long );
#  # void     curvebasis( short );
#  # void     curveit( short );
#  # void     curveprecision( short );
#  # void     cyclemap( short, short, short );
#  # void     czclear( unsigned long, long );
#  # void     dbtext( char[8] );
#  # void     defbasis( short, const Matrix );
#  # void     defcursor( short, const unsigned short[128] );
#  # void     deflfont( short, short, const Lfontchar[], long, const unsigned short[] );
#  # void     deflinestyle( short, Linestyle );
#  # void     defpattern( short, short, const unsigned short[] );
#  # long     defpup( String, ... );
#  # void     defrasterfont( short, short, short, const Fontchar[], short, const unsigned short[] );
#  # void     delobj( Object );
#  # void     deltag( Tag );
#  # void     depthcue( Boolean );
#  # void     dglclose( long );
#  # long     dglopen( String, long );
#  # void     dither( long );
#  # long     dopup( long );
#  # void     doublebuffer( void );
#  # void     draw( Coord, Coord, Coord );
#  # void     draw2( Coord, Coord );
#  # void     draw2i( Icoord, Icoord );
# DRAW2:  .word   DRAW2+4
#         lw      $a0, 4(DP)
#         jal     draw2i
#         lw      $a1, 0(DP)
#         j       $next
#         addiu   DP, DP, 8
#
#  # void     draw2s( Scoord, Scoord );
#  # void     drawi( Icoord, Icoord, Icoord );
# DRAWI:  .word   DRAWI+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         jal     drawi
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     drawmode( long );
#  # void     draws( Scoord, Scoord, Scoord );
#  # void     editobj( Object );
#  # void     endclosedline( void );
# ECLIN:  .word   ECLIN+4
#         jal     endclosedline
#         nop
#         j       $next
#         nop
#
#  # void     endcurve( void );
#  # long     endfeedback( void * );
#  # void     endfullscrn( void );
#  # void     endline( void );
# ELINE:  .word   ELINE+4
#         jal     endline
#         nop
#         j       $next
#         nop
#
#  # long     endpick( short[] );
#  # void     endpoint( void );
# EPNT:   .word   EPNT+4
#         jal     endpoint
#         nop
#         j       $next
#         nop
#
#  # void     endpolygon( void );
# EPOLY:  .word   EPOLY+4
#         jal     endpolygon
#         nop
#         j       $next
#         nop
#
#  # void     endqstrip( void );
#  # long     endselect( short[] );
#  # void     endsurface( void );
#  # void     endtmesh( void );
# EMESH:  .word   EMESH+4
#         jal     endtmesh
#         nop
#         j       $next
#         nop
#
#  # void     endtrim( void );
#  # void     feedback( void *, long );
#  # void     finish( void );
#  # void     fogvertex( long, const float[] );
#  # void     font( short );
#  # void     foreground( void );
#  # void     freepup( long );
#  # void     frontbuffer( Boolean );
#  # void     frontface( Boolean );
#  # void     fudge( long, long );
#  # void     fullscrn( void );
#  # void     gammaramp( const short[256], const short[256], const short[256] );
#  # void     gbegin( void );
# GBEGN:  .word   GBEGN+4
#         jal     gbegin
#         nop
#         j       $next
#         nop
#
#  # void     gconfig( void );
# GCONF:  .word   GCONF+4
#         jal     gconfig
#         nop
#         j       $next
#         nop
#
#  # Object   genobj( void );
#  # Tag      gentag( void );
#  # long     getbackface( void );
#  # long     getbuffer( void );
#  # Boolean  getbutton( Device );
#  # Boolean  getcmmode( void );
#  # long     getcolor( void );
#  # void     getcpos( short *, short * );
#  # void     getcursor( short *, Colorindex *, Colorindex *, Boolean * );
#  # Boolean  getdcm( void );
#  # long     getdescender( void );
#  # void     getdev( long, const Device[], short[] );
# GTDEV:  .word   GTDEV+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         lw      $a2, 0(DP)
#         jal     getdev
#         nop
#         j       $next
#         addiu   DP, DP, 12
#
#  # long     getdisplaymode( void );
#  # long     getdrawmode( void );
#  # long     getfont( void );
#  # long     getgdesc( long );
# GDESC:  .word   GDESC+4
#         jal     getgdesc
#         lw      $a0, 0(DP)
#         j       $next
#         sw      $v0, 0(DP)
#
#  # void     getgpos( Coord *, Coord *, Coord *, Coord * );
#  # long     getheight( void );
#  # long     getlsrepeat( void );
#  # long     getlstyle( void );
#  # long     getlwidth( void );
#  # long     getmap( void );
#  # void     getmatrix( Matrix );
#  # void     getmcolor( Colorindex, short *, short *, short * );
#  # long     getmmode( void );
#  # long     getmonitor( void );
#  # void     getnurbsproperty( long, float * );
#  # Object   getopenobj( void );
#  # void     getorigin( long *, long * );
# GORIG:  .word   GORIG+4
#         lw      $a0, 4(DP)
#         lw      $a1, 0(DP)
#         jal     getorigin
#         addiu   DP, DP, 8
#         j       $next
#         nop
#
#  # long     getpattern( void );
#  # long     getplanes( void );
#  # void     getscrbox( long *, long *, long *, long * );
#  # void     getscrmask( Screencoord *, Screencoord *, Screencoord *, Screencoord * );
#  # void     getsize( long *, long * );
# GSIZE:  .word   GSIZE+4
#         lw      $a0, 4(DP)
#         lw      $a1, 0(DP)
#         jal     getsize
#         addiu   DP, DP, 8
#         j       $next
#         nop
#
#  # long     getsm( void );
#  # long     getvaluator( Device );
#  # long     getvideo( long );
#  # void     getviewport( Screencoord *, Screencoord *, Screencoord *, Screencoord * );
#  # long     getwritemask( void );
#  # long     getwscrn( void );
#  # Boolean  getzbuffer( void );
#  # void     gexit( void );
# GEXIT:  .word   GEXIT+4
#         jal     gexit
#         nop
#         j       $next
#         addu    $sp, 64
#
# GINIT:  .word   GINIT+4
#         jal     ginit
#         nop
#         j       $next
#         nop
#
#  # void     gflush( void );
#  # void     glcompat( long, long );
#  # long     GLXlink( void *, GLXconfig * );
#  # long     GLXunlink( void *, unsigned long );
#  # long     GLXwinset( void *, unsigned long );
#  # void     gRGBcolor( short *, short *, short * );
#  # void     gRGBcursor( short *, short *, short *, short *, short *, short *, short *, Boolean * ); /* obsolete */
#  # void     gRGBmask( short *, short *, short * );
#  # void     gselect( short[], long );
#  # void     gsync( void );
#  # long     gversion( char[12] );
#  # void     iconsize( long, long );
#  # void     icontitle( String );
#  # void     imakebackground( void );
#  # void     initnames( void );
#  # Boolean  isobj( Object );
#  # Boolean  isqueued( Device );
#  # Boolean  istag( Tag );
#  # void     keepaspect( long, long );
#  # void     lcharstr( long, Lstring );
#  # void     linesmooth( unsigned long );
#  # void     linewidth( short );
#  # void     linewidthf( float );
#  # void     lmbind( short, short );
#  # void     lmcolor( long );
#  # void     lmdef( short, short, short, const float[] );
#  # void     loadmatrix( const Matrix );
#  # void     loadname( short );
#  # void     logicop( long );
#  # void     lookat( Coord, Coord, Coord, Coord, Coord, Coord, Angle );
# LOKAT:  .word   LOKAT+4
#         l.s     $f12, 24(DP)
#         l.s     $f14, 20(DP)
#         lw      $a2, 16(DP)
#         lw      $a3, 12(DP)
#         l.s     $f4, 8(DP)
#         l.s     $f6, 4(DP)
#         lw      $a0, 0(DP)
#         s.s     $f4, 16($sp)
#         s.s     $f6, 20($sp)
#         jal     lookat
#         sw      $a0, 24($sp)
#         j       $next
#         addiu   DP, DP, 28
#
#  # long     lrectread( Screencoord, Screencoord, Screencoord, Screencoord, unsigned long[] );
#  # void     lrectwrite( Screencoord, Screencoord, Screencoord, Screencoord, const unsigned long[] );
#  # void     lRGBrange( short, short, short, short, short, short, long, long );
#  # void     lsetdepth( long, long );
#  # void     lshaderange( Colorindex, Colorindex, long, long );
#  # void     lsrepeat( long );
#  # long     lstrwidth( long, Lstring );
#  # void     makeobj( Object );
#  # void     maketag( Tag );
#  # void     mapcolor( Colorindex, short, short, short );
#  # void     mapw( Object, Screencoord, Screencoord, Coord *, Coord *, Coord *, Coord *, Coord *, Coord * );
#  # void     mapw2( Object, Screencoord, Screencoord, Coord *, Coord * );
#  # void     maxsize( long, long );
#  # void     minsize( long, long );
#  # void     mmode( short );
#  # void     move( Coord, Coord, Coord );
#  # void     move2( Coord, Coord );
#  # void     move2i( Icoord, Icoord );
# MOVE2:  .word   MOVE2+4
#         lw      $a0, 4(DP)
#         jal     move2i
#         lw      $a1, 0(DP)
#         j       $next
#         addiu   DP, DP, 8
#
#  # void     move2s( Scoord, Scoord );
#  # void     movei( Icoord, Icoord, Icoord );
# MOVEI:  .word   MOVEI+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         jal     movei
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     moves( Scoord, Scoord, Scoord );
#  # void     mswapbuffers( long );
#  # void     multimap( void );
#  # void     multmatrix( const Matrix );
#  # void     n3f( const float[3] );
#  # long     newpup( void );
#  # void     newtag( Tag, Tag, Offset );
#  # void     nmode( long );
#  # void     noborder( void );
#  # void     noise( Device, short );
#  # void     noport( void );
#  # void     nurbscurve( long, const double[], long, const double[], long, long );
#  # void     nurbssurface( long, const double[], long, const double[], long, long, const double[], long, long, long );
#  # void     objdelete( Tag, Tag );
#  # void     objinsert( Tag );
#  # void     objreplace( Tag );
#  # void     onemap( void );
#  # void     ortho( Coord, Coord, Coord, Coord, Coord, Coord );
# ORTHO:  .word   ORTHO+4
#         l.s     $f12, 20(DP)
#         l.s     $f14, 16(DP)
#         lw      $a2, 12(DP)
#         lw      $a3, 8(DP)
#         l.s     $f4, 4(DP)
#         l.s     $f6, 0(DP)
#         s.s     $f4, 16($sp)
#         jal     ortho
#         s.s     $f6, 20($sp)
#         j       $next
#         addiu   DP, DP, 24
#
#  # void     ortho2( Coord, Coord, Coord, Coord );
# ORTH2:  .word   ORTH2+4
#         l.s     $f12, 12(DP)
#         l.s     $f14, 8(DP)
#         lw      $a2, 4(DP)
#         jal     ortho2
#         lw      $a3, 0(DP)
#         j       $next
#         addiu   DP, DP, 16
#
#  # void     overlay( long );
#  # void     passthrough( short );
#  # void     patch( const Matrix, const Matrix, const Matrix );
#  # void     patchbasis( long, long );
#  # void     patchcurves( long, long );
#  # void     patchprecision( long, long );
#  # void     pclos( void );
#  # void     pdr( Coord, Coord, Coord );
#  # void     pdr2( Coord, Coord );
#  # void     pdr2i( Icoord, Icoord );
#  # void     pdr2s( Scoord, Scoord );
#  # void     pdri( Icoord, Icoord, Icoord );
#  # void     pdrs( Scoord, Scoord, Scoord );
#  # void     perspective( Angle, float, Coord, Coord );
# PERSP:  .word   PERSP+4
#         lw      $a0, 12(DP)
#         lw      $a1, 8(DP)
#         lw      $a2, 4(DP)
#         jal     perspective
#         lw      $a3, 0(DP)
#         j       $next
#         addiu   DP, DP, 16
#
#  # void     pick( short[], long );
#  # void     picksize( short, short );
#  # void     pixmode( long, long );
#  # void     pixmodef( long, float );
#  # void     pmv( Coord, Coord, Coord );
#  # void     pmv2( Coord, Coord );
#  # void     pmv2i( Icoord, Icoord );
#  # void     pmv2s( Scoord, Scoord );
#  # void     pmvi( Icoord, Icoord, Icoord );
#  # void     pmvs( Scoord, Scoord, Scoord );
#  # void     pnt( Coord, Coord, Coord );
#  # void     pnt2( Coord, Coord );
#  # void     pnt2i( Icoord, Icoord );
# PNT2I:  .word   PNT2I+4
#         lw      $a0, 12(DP)
#         jal     pnt2i
#         lw      $a1, 8(DP)
#         j       $next
#         addiu   DP, DP, 8
#
#  # void     pnt2s( Scoord, Scoord );
#  # void     pnti( Icoord, Icoord, Icoord );
# PNTI:   .word   PNTI+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         jal     pnti
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     pnts( Scoord, Scoord, Scoord );
#  # void     pntsize( short );
#  # void     pntsizef( float );
#  # void     pntsmooth( unsigned long );
#  # void     polarview( Coord, Angle, Angle, Angle );
#  # void     polf( long, const Coord[][3] );
# POLF:   .word   POLF+4
#         lw      $a0, 4(DP)
#         lw      $a1, 0(DP)
#         jal     polf
#         addiu   DP, DP, 8
#         j       $next
#         nop
#
#   # void     polf2( long, const Coord[][2] );
#  # void     polf2i( long, const Icoord[][2] );
# PLF2I:  .word   PLF2I+4
#         lw      $a0, 4(DP)
#         lw      $a1, 0(DP)
#         jal     polf2i
#         addiu   DP, DP, 8
#         j       $next
#         nop
#
#  # void     polf2s( long, const Scoord[][2] );
#  # void     polfi( long, const Icoord[][3] );
# POLFI:  .word   POLFI+4
#         lw      $a0, 4(DP)
#         lw      $a1, 0(DP)
#         jal     polfi
#         addiu   DP, DP, 8
#         j       $next
#         nop
#
#   # void     polfs( long, const Scoord[][3] );
#  # void     poly( long, const Coord[][3] );
#  # void     poly2( long, const Coord[][2] );
#  # void     poly2i( long, const Icoord[][2] );
# POLY2:  .word   POLY2+4
#         lw      $a0, 4(DP)
#         lw      $a1, 0(DP)
#         jal     poly2i
#         addiu   DP, DP, 8
#         j       $next
#         nop
#
#  # void     poly2s( long, const Scoord[][2] );
#  # void     polyi( long, const Icoord[][3] );
#  # void     polymode( long );
#  # void     polys( long, const Scoord[][3] );
#  # void     polysmooth( long );
#  # void     popattributes( void );
#  # void     popmatrix( void );
# POPMA:  .word   POPMA+4
#         jal     popmatrix
#         nop
#         j       $next
#         nop
#
#  # void     popname( void );
#  # void     popviewport( void );
#  # void     prefposition( long, long, long, long );
# PREPO:  .word   PREPO+4
#         lw      $a0, 12(DP)
#         lw      $a1, 8(DP)
#         lw      $a2, 4(DP)
#         jal     prefposition
#         lw      $a3, 0(DP)
#         j       $next
#         addiu   DP, DP, 16
#
#  # void     prefsize( long, long );
# PRESI:  .word   PRESI+4
#         lw      $a0, 4(DP)
#         jal     prefsize
#         lw      $a1, 0(DP)
#         j       $next
#         addiu   DP, DP, 8
#
#  # void     pushattributes( void );
#  # void     pushmatrix( void );
# PSHMA:  .word   PSHMA+4
#         jal     pushmatrix
#         nop
#         j       $next
#         nop
#
#  # void     pushname( short );
#  # void     pushviewport( void );
#  # void     pwlcurve( long, const double[], long, long );
#  # long     qcontrol( long, long, const short[], long, short[] );
#  # void     qdevice( Device );
# QDEVI:  .word   QDEVI+4
#         lw      $a0, 0(DP)
#         jal     qdevice
#         nop
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     qenter( Device, short );
#  # long     qgetfd( void );
#  # long     qread( short * );
# QREAD:  .word   QREAD+4
#         lw      $a0, 0(DP)
#         jal     qread
#         nop
#         j       $next
#         sw      $v0, 0(DP)
#
#  # void     qreset( void );
# QREST:  .word   QREST+4
#         jal     qreset
#         nop
#         j       $next
#         nop
#
#  # long     qtest( void );
#  # void     rcrv( const Coord[4][4] );
#  # void     rcrvn( long, const Coord[][4] );
#  # void     rdr( Coord, Coord, Coord );
#  # void     rdr2( Coord, Coord );
#  # void     rdr2i( Icoord, Icoord );
#  # void     rdr2s( Scoord, Scoord );
#  # void     rdri( Icoord, Icoord, Icoord );
#  # void     rdrs( Scoord, Scoord, Scoord );
#  # long     readdisplay( Screencoord, Screencoord, Screencoord, Screencoord, unsigned long[], unsigned long );
#  # long     readpixels( short, Colorindex[] );
#  # long     readRGB( short, RGBvalue[], RGBvalue[], RGBvalue[] );
#  # void     readsource( long );
#  # void     rect( Coord, Coord, Coord, Coord );
#  # void     rectcopy( Screencoord, Screencoord, Screencoord, Screencoord, Screencoord, Screencoord );
#  # void     rectf( Coord, Coord, Coord, Coord );
#  # void     rectfi( Icoord, Icoord, Icoord, Icoord );
# RCTFI:  .word   RCTFI+4
#         lw      $a0, 12(DP)
#         lw      $a1, 8(DP)
#         lw      $a2, 4(DP)
#         jal     rectfi
#         lw      $a3, 0(DP)
#         j       $next
#         addiu   DP, DP, 16
#
#  # void     rectfs( Scoord, Scoord, Scoord, Scoord );
#  # void     recti( Icoord, Icoord, Icoord, Icoord );
# RECTI:  .word   RECTI+4
#         lw      $a0, 12(DP)
#         lw      $a1, 8(DP)
#         lw      $a2, 4(DP)
#         jal     recti
#         lw      $a3, 0(DP)
#         j       $next
#         addiu   DP, DP, 16
#
#  # long     rectread( Screencoord, Screencoord, Screencoord, Screencoord, Colorindex[] );
#  # void     rects( Scoord, Scoord, Scoord, Scoord );
#  # void     rectwrite( Screencoord, Screencoord, Screencoord, Screencoord, const Colorindex[] );
#  # void     rectzoom( float, float );
#  # void     reshapeviewport( void );
#  # void     RGBcolor( short, short, short );
#  # void     RGBmode( void );
# RGBMD:  .word   RGBMD+4
#         jal     RGBmode
#         nop
#         j       $next
#         nop
#
#  # void     RGBwritemask( short, short, short );
#  # void     ringbell( void );
#  # void     rmv( Coord, Coord, Coord );
#  # void     rmv2( Coord, Coord );
#  # void     rmv2i( Icoord, Icoord );
#  # void     rmv2s( Scoord, Scoord );
#  # void     rmvi( Icoord, Icoord, Icoord );
#  # void     rmvs( Scoord, Scoord, Scoord );
#  # void     rot( float, char );
#  # void     rotate( Angle, char );
# ROTAT:  .word   ROTAT+4
#         lw      $a0, 4(DP)
#         jal     rotate
#         lw      $a1, 0(DP)
#         j       $next
#         addiu   DP, DP, 8
#
#  # void     rpatch( const Matrix, const Matrix, const Matrix, const Matrix );
#  # void     rpdr( Coord, Coord, Coord );
#  # void     rpdr2( Coord, Coord );
#  # void     rpdr2i( Icoord, Icoord );
#  # void     rpdr2s( Scoord, Scoord );
#  # void     rpdri( Icoord, Icoord, Icoord );
#  # void     rpdrs( Scoord, Scoord, Scoord );
#  # void     rpmv( Coord, Coord, Coord );
#  # void     rpmv2( Coord, Coord );
#  # void     rpmv2i( Icoord, Icoord );
#  # void     rpmv2s( Scoord, Scoord );
#  # void     rpmvi( Icoord, Icoord, Icoord );
#  # void     rpmvs( Scoord, Scoord, Scoord );
#  # void     sbox( Coord, Coord, Coord, Coord );
#  # void     sboxf( Coord, Coord, Coord, Coord );
#  # void     sboxfi( Icoord, Icoord, Icoord, Icoord );
# SBOXF:  .word   SBOXF+4
#         lw      $a0, 12(DP)
#         lw      $a1, 8(DP)
#         lw      $a2, 4(DP)
#         jal     sboxfi
#         lw      $a3, 0(DP)
#         j       $next
#         addiu   DP, DP, 16
#
#  # void     sboxfs( Scoord, Scoord, Scoord, Scoord );
#  # void     sboxi( Icoord, Icoord, Icoord, Icoord );
#  # void     sboxs( Scoord, Scoord, Scoord, Scoord );
#  # void     scale( float, float, float );
# SCALE:  .word   SCALE+4
#         l.s     $f12, 8(DP)
#         l.s     $f14, 4(DP)
#         jal     scale
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     sclear( unsigned long );
#  # void     scrbox( long );
#  # void     screenspace( void );
#  # void     scrmask( Screencoord, Screencoord, Screencoord, Screencoord );
#  # long     scrnattach( long );
#  # long     scrnselect( long );
#  # void     scrsubdivide( long, const float[] );
#  # void     setbell( Byte );
#  # void     setcursor( short, Colorindex, Colorindex );
#  # void     setdblights( unsigned long );
#  # void     setlinestyle( short );
#  # void     setmap( short );
#  # void     setmonitor( short );
#  # void     setnurbsproperty( long, float );
#  # void     setpattern( short );
#  # void     setpup( long, long, unsigned long );
#  # void     setshade( Colorindex );
#  # void     setvaluator( Device, short, short, short );
#  # void     setvideo( long, long );
#  # void     shademodel( long );
# SHDMD:  .word   SHDMD+4
#         jal     shademodel
#         lw      $a0, 0(DP)
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     singlebuffer( void );
# SLEEP:  .word   SLEEP+4
#         jal     sleep
#         lw      $a0, 0(DP)
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     splf( long, const Coord[][3], const Colorindex[] );
#  # void     splf2( long, const Coord[][2], const Colorindex[] );
#  # void     splf2i( long, const Icoord[][2], const Colorindex[] );
#  # void     splf2s( long, const Scoord[][2], const Colorindex[] );
#  # void     splfi( long, const Icoord[][3], const Colorindex[] );
#  # void     splfs( long, const Scoord[][3], const Colorindex[] );
#  # void     stencil( long, unsigned long, long, unsigned long, long, long, long );
#  # void     stensize( long );
#  # void     stepunit( long, long );
#  # long     strwidth( String );
#  # void     subpixel( Boolean );
#  # void     swapbuffers( void );
#  # void     swapinterval( short );
#  # void     swaptmesh( void );
# SMESH:  .word   SMESH+4
#         jal     swaptmesh
#         nop
#         j       $next
#         nop
#
#  # long     swinopen( long );
#  # void     swritemask( unsigned long );
#  # void     t2d( const double[2] );
#  # void     t2f( const float[2] );
#  # void     t2i( const long[2] );
#  # void     t2s( const short[2] );
#  # void     tevbind( long, long );
#  # void     tevdef( long, long, const float[] );
#  # void     texbind( long, long );
#  # void     texdef2d( long, long, long, long, const unsigned long[], long, const float[] );
#  # void     texgen( long, long, const float[] );
#  # void     tie( Device, Device, Device );
# TIE:    .word   TIE+4
#         lw      $a0, 8(DP)
#         lw      $a1, 4(DP)
#         lw      $a2, 0(DP)
#         jal     tie
#         nop
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     translate( Coord, Coord, Coord );
# TRANS:  .word   TRANS+4
#         l.s     $f12, 8(DP)
#         l.s     $f14, 4(DP)
#         jal     translate
#         lw      $a2, 0(DP)
#         j       $next
#         addiu   DP, DP, 12
#
#  # void     underlay( long );
#  # void     unqdevice( Device );
#  # void     v2d( const double[2] );
#  # void     v2f( const float[2] );
#  # void     v2i( const long[2] );
# V2I:    .word   V2I+4
#         jal     v2i
#         lw      $a0, 0(DP)
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     v2s( const short[2] );
#  # void     v3d( const double[3] );
#  # void     v3f( const float[3] );
#  # void     v3i( const long[3] );
# V3I:    .word   V3I+4
#         jal     v3i
#         lw      $a0, 0(DP)
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     v3s( const short[3] );
#  # void     v4d( const double[4] );
#  # void     v4f( const float[4] );
#  # void     v4i( const long[4] );
#  # void     v4s( const short[4] );
#  # void     videocmd( long );
#  # void     viewport( Screencoord, Screencoord, Screencoord, Screencoord );
#  # void     winclose( long );
#  # void     winconstraints( void );
#  # long     windepth( long );
#  # void     window( Coord, Coord, Coord, Coord, Coord, Coord );
#  # long     winget( void );
#  # void     winmove( long, long );
#  # long     winopen( String );
# WINOP:  .word   WINOP+4
#         la      $a0, WINDO+4
#         jal     winopen
#         nop
#         j       $next
#         sw      $v0, 0(DP)
#
#  # void     winpop( void );
#  # void     winposition( long, long, long, long );
#  # void     winpush( void );
#  # void     winset( long );
#  # void     wintitle( String );
#  # void     wmpack( unsigned long );
#  # void     writemask( Colorindex );
#  # void     writepixels( short, const Colorindex[] );
#  # void     writeRGB( short, const RGBvalue[], const RGBvalue[], const RGBvalue[] );
#  # void     zbuffer( Boolean );
# ZBUFF:  .word   ZBUFF+4
#         jal     zbuffer
#         lw      $a0, 0(DP)
#         j       $next
#         addiu   DP, DP, 4
#
#  # void     zclear( void );
# ZCLEA:  .word   ZCLEA+4
#         jal     zclear
#         nop
#         j       $next
#         nop
#
#  # void     zdraw( Boolean );
#  # void     zfunction( long );
#  # void     zwritemask( unsigned long );
#
# WINDO:  .word   dovar
#         .asciiz "window"
#
#         .word   BLACK, g01
# g00:    .ascii  "\005BLACK"
#         .word   RED, g02
# g01:    .ascii  "\003RED"
#         .word   GREEN, g03
# g02:    .ascii  "\005GREEN"
#         .word   YELLOW, g04
# g03:    .ascii  "\006YELLOW"
#         .word   BLUE, g05
# g04:    .ascii  "\004BLUE"
#         .word   MAGENTA, g06
# g05:    .ascii  "\007MAGENTA"
#         .word   CYAN, g07
# g06:    .ascii  "\004CYAN"
#         .word   WHITE, g08
# g07:    .ascii  "\005WHITE"
#         .word   BCLIN, g09
# g08:    .ascii  "\015bgnclosedline"
#         .word   BLINE, g10
# g09:    .ascii  "\007bgnline"
#         .word   BPNT, g11
# g10:    .ascii  "\010bgnpoint"
#         .word   BPOLY, g12
# g11:    .ascii  "\012bgnpolygon"
#         .word   BMESH, g13
# g12:    .ascii  "\010bgntmesh"
#         .word   CLEAR, g14
# g13:    .ascii  "\005clear"
#         .word   COLOR, g15
# g14:    .ascii  "\005color"
#         .word   CHRST, g15a
# g15:    .ascii  "\007charstr"
#         .word   CMOV2, g15b
# g15a:   .ascii  "\006cmov2i"
#         .word   CMOVI, g15c
# g15b:   .ascii  "\005cmovi"
#         .word   CPACK, g16
# g15c:   .ascii  "\005cpack"
#         .word   ECLIN, g17
# g16:    .ascii  "\015endclosedline"
#         .word   ELINE, g18
# g17:    .ascii  "\007endline"
#         .word   BCLIN, g19
# g18:    .ascii  "\015bgnclosedline"
#         .word   BLINE, g20
# g19:    .ascii  "\007bgnline"
#         .word   EPNT, g21
# g20:    .ascii  "\010endpoint"
#         .word   EPOLY, g22
# g21:    .ascii  "\012endpolygon"
#         .word   EMESH, g23
# g22:    .ascii  "\010endtmesh"
#         .word   GBEGN, g24
# g23:    .ascii  "\006gbegin"
#         .word   GDESC, g25
# g24:    .ascii  "\010getgdesc"
#         .word   GEXIT, g25a
# g25:    .ascii  "\005gexit"
#         .word   GINIT, g26
# g25a:   .ascii  "\005ginit"
#         .word   ORTHO, g27
# g26:    .ascii  "\005ortho"
#         .word   ORTH2, g28
# g27:    .ascii  "\006ortho2"
#         .word   POPMA, g28a
# g28:    .ascii  "\011popmatrix"
#         .word   PLF2I, g28b
# g28a:   .ascii  "\006polf2i"
#         .word   POLF, g28c
# g28b:   .ascii  "\004polf"
#         .word   POLFI, g28d
# g28c:   .ascii  "\005polfi"
#         .word   PNT2I, g28e
# g28d:   .ascii  "\005pnt2i"
#         .word   PNTI, g29
# g28e:   .ascii  "\004pnti"
#         .word   PREPO, g30
# g29:    .ascii  "\014prefposition"
#         .word   PRESI, g31
# g30:    .ascii  "\010prefsize"
#         .word   PSHMA, g32a
# g31:    .ascii  "\012pushmatrix"
#         .word   ROTAT, g32b
# g32a:   .ascii  "\006rotate"
#         .word   RCTFI, g32c
# g32b:   .ascii  "\006rectfi"
#         .word   RECTI, g33
# g32c:   .ascii  "\005recti"
#         .word   SCALE, g34
# g33:    .ascii  "\005scale"
#         .word   SHDMD, g35
# g34:    .ascii  "\012shademodel"
#         .word   SLEEP, g36
# g35:    .ascii  "\005sleep"
#         .word   SMESH, g37
# g36:    .ascii  "\011swaptmesh"
#         .word   TRANS, g38
# g37:    .ascii  "\011translate"
#         .word   V2I, g39
# g38:    .ascii  "\003v2i"
#         .word   V3I, g40
# g39:    .ascii  "\003v3i"
#         .word   WINOP, g41
# g40:    .ascii  "\007winopen"
#         .word   WINDO, g42
# g41:    .ascii  "\006window"
#         .word   ARCFI, g43
# g42:    .ascii  "\005arcfi"
#         .word   ARCI, g44
# g43:    .ascii  "\004arci"
#         .word   CIRFI, g45
# g44:    .ascii  "\006circfi"
#         .word   CIRCI, g46
# g45:    .ascii  "\005circi"
#         .word   DRAW2, g47
# g46:    .ascii  "\006draw2i"
#         .word   DRAWI, g48
# g47:    .ascii  "\005drawi"
#         .word   MOVE2, g49
# g48:    .ascii  "\006move2i"
#         .word   MOVEI, g50
# g49:    .ascii  "\005move1"
#         .word   POLY2, g51
# g50:    .ascii  "\006poly2i"
#         .word   MANDE, g52
# g51:    .ascii  "\006MANDEL"
#         .word   GTDEV, g53
# g52:    .ascii  "\006getdev"
#         .word   GORIG, g54
# g53:    .ascii  "\011getorigin"
#         .word   GSIZE, g55
# g54:    .ascii  "\007getsize"
#         .word   QDEVI, g56
# g55:    .ascii  "\007qdevice"
#         .word   QREAD, g56a
# g56:    .ascii  "\005qread"
#         .word   QREST, g57
# g56a:   .ascii  "\006qreset"
#         .word   TIE, g58
# g57:    .ascii  "\003tie"
#         .word   SBOXF, g59
# g58:    .ascii  "\006sboxfi"
#         .word   GCONF, g60
# g59:    .ascii  "\007gconfig"
#         .word   RGBMD, g61
# g60:    .ascii  "\007RGBmode"
#         .word   LOKAT, g62
# g61:    .ascii  "\006lookat"
#         .word   PERSP, g63
# g62:    .ascii  "\013perspective"
#         .word   ZBUFF, g64
# g63:    .ascii  "\007zbuffer"
#         .word   ZCLEA, 0
# g64:    .ascii  "\006zclear"
#         .word   0,0,0,0


        

 ##### open and read source file #####

 # OPEN ( file-buffer file-name-ptr -- bytes-read, 0 if error )

OPEN:   .word   OPEN+4
        lw      $a0, 0(DP)
        move    $a1, $zero
        jal     open
        nop
        la      $a0, msg5
        bne     $v0, -1, OPEN1
        nop
        jal     printf
        nop
        sw      $zero, 4(DP)
        j       $next
        addiu   DP, DP, 4
OPEN1:  move    $a0, $2
        lw      $a1, 4(DP)
        li      $a2, -1
        jal     read
        nop
        addiu   DP, DP, 4
        j       $next
        sw      $v0, 0(DP)

        .end eForth

######### high level words #############

    .section  .rodata   # put all high level words in data space


######### user variables ###############

UP:     .word   docon
.word   UPP

SZERO:  .word   douse
.word   16

RZERO:  .word   douse
.word   20

TQKEY:  .word   douse
.word   24

TEMIT:  .word   douse
.word   28

TEXPE:  .word   douse
.word   32

TTAP:   .word   douse
.word   36

TECHO:  .word   douse
.word   40

TPROM:  .word   douse
.word   44

BASE:   .word   douse
.word   48

TEMP:   .word   douse
.word   52

SPAN:   .word   douse
.word   56

INN:    .word   douse
.word   60

NTIB:   .word   douse
.word   64

CSP:    .word   douse
.word   72

TEVAL:  .word   douse
.word   76

TNUMB:  .word   douse
.word   80

HLD:    .word   douse
.word   84

HANDL:  .word   douse
.word   88

CNTXT:  .word   douse
.word   92

CRRNT:  .word   douse
.word   128

CP:     .word   douse
.word   136

NP:     .word   douse
.word   140

LAST:   .word   douse
.word   144


 ##### vocabulary words #####


FORTH:  .word   dovoc
        .word   LASTN
        .word   0

# GRAPH:  .word   dovoc
#         .word   g00
#         .word   FORTH+8
#
# FLTNG:  .word   dovoc
#         .word   f00
#         .word   GRAPH+8

# APPLI:  .word   dovoc
#        .word   LASTN
#        .word   FLTNG+8

ZERO:   .word   docon
        .word   0

ONE:    .word   docon
        .word   1

TWO:    .word   docon
        .word   2

THREE:  .word   docon
        .word   3

XXX:    .word   dovar
        .word   0


  ######  MISCELLANEOUS ######


  # return true if u is within the range of ul and uh
WITHI:  .word   dolst
.word OVER,SUBB,TOR
.word SUBB,RFROM,ULESS,EXIT

DNEGA:  .word   dolst
.word INVER,TOR,INVER
.word ONE,UPLUS
.word RFROM,PLUS,EXIT

MSMOD:  .word   dolst
        .word   DUPP,ZLESS,DUPP,TOR
        .word   QBRAN,MMOD1
        .word   NEGAT,TOR,DNEGA,RFROM
MMOD1:  .word   TOR,DUPP,ZLESS
        .word   QBRAN,MMOD2
        .word   RAT,PLUS
MMOD2:  .word   RFROM,UMMOD,RFROM
        .word   QBRAN,MMOD3
        .word   SWAP,NEGAT,SWAP
MMOD3:  .word   EXIT

   # return mod and quotient of n1 * n2 / n3
SSMOD:  .word   dolst
.word TOR,MSTAR,RFROM,MSMOD,EXIT

  # return quotient of n1 * n2 / n3
STASL:  .word   dolst
.word SSMOD,SWAP,DROP,EXIT

BLANK:  .word   dolst
.word DOLIT,32,EXIT

TCHAR:  .word   dolst
.word DOLIT,0x7F,ANDD,DUPP
.word BLANK,DOLIT,127,WITHI,INVER
.word QBRAN,TCHA1
.word DROP,DOLIT,95
TCHA1:
.word EXIT


DEPTH:  .word   dolst

.word SPAT,SZERO,AT,SWAP,SUBB
.word DOLIT,CELLL,SLASH,EXIT

PSTOR:  .word   dolst

.word SWAP,OVER,AT,PLUS
.word SWAP,STORE,EXIT

DSTOR:  .word   dolst
.word SWAP,OVER,STORE
.word CELLP,STORE,EXIT

DAT:    .word   dolst
.word DUPP, CELLP, AT
.word SWAP, AT, EXIT

COUNT:  .word   dolst
.word DUPP,ONE,PLUS
.word SWAP,CAT,EXIT

HERE:   .word   dolst
.word CP,AT,EXIT

PAD:    .word   dolst
.word HERE,DOLIT,80,PLUS,EXIT

TIB:    .word   dolst
.word NTIB,CELLP,AT,EXIT

ATEXE:  .word   dolst
.word AT,QDUP
.word QBRAN,EXE1
.word EXECU
EXE1:
.word EXIT


 ##### string words #####


FILL:   .word   dolst
.word SWAP,TOR,SWAP
.word BRAN,FILL2
FILL1:
.word DDUP,CSTOR,ONE,PLUS
FILL2:
.word DONXT,FILL1
.word DDROP,EXIT

DTRAI:  .word   dolst
.word TOR
.word BRAN,DTRA2
DTRA1:
.word BLANK,OVER,RAT,PLUS,CAT,LESS
.word QBRAN,DTRA2
.word RFROM,ONE,PLUS,EXIT
DTRA2:
.word DONXT,DTRA1
.word ZERO,EXIT

PACKS:  .word   dolst
.word ALGND,DUPP,TOR
.word OVER,DUPP,ZERO
.word DOLIT,CELLL,UMMOD,DROP
.word SUBB,OVER,PLUS
.word ZERO,SWAP,STORE
.word DDUP,CSTOR,ONE,PLUS
.word SWAP,CMOVE,RFROM,EXIT


 ##### number conversions #####


DIGIT:  .word   dolst
.word DOLIT,9,OVER,LESS
.word DOLIT,7,ANDD,PLUS
.word DOLIT,48,PLUS,EXIT

EXTRC:  .word   dolst
.word ZERO,SWAP,UMMOD
.word SWAP,DIGIT,EXIT

BDIGS:  .word   dolst
.word PAD,HLD,STORE,EXIT

HOLD:   .word   dolst
.word HLD,AT,ONE,SUBB
.word DUPP,HLD,STORE,CSTOR,EXIT

DIG:    .word   dolst
.word BASE,AT,EXTRC,HOLD,EXIT

DIGS:   .word   dolst
DIGS1:
.word DIG,DUPP
.word QBRAN,DIGS2
.word BRAN,DIGS1
DIGS2:
.word EXIT

SIGN:   .word   dolst
.word ZLESS
.word QBRAN,SIGN1
.word DOLIT,45,HOLD
SIGN1:
.word EXIT

EDIGS:  .word   dolst
.word DROP,HLD,AT
.word PAD,OVER,SUBB,EXIT

STRR:   .word   dolst
.word DUPP,TOR,ABSS
.word BDIGS,DIGS,RFROM
.word SIGN,EDIGS,EXIT

HEX:    .word   dolst
.word DOLIT,16,BASE,STORE,EXIT

DECIM:  .word   dolst
.word DOLIT,10,BASE,STORE,EXIT

DIGTQ:  .word   dolst
.word TOR,DOLIT,48,SUBB
.word DOLIT,9,OVER,LESS
.word QBRAN,DGTQ1
.word DOLIT,7,SUBB
.word DUPP,DOLIT,10,LESS,ORR
DGTQ1:
.word DUPP,RFROM,ULESS,EXIT

NUMBQ:  .word   dolst
.word BASE,AT,TOR,ZERO,OVER,COUNT
.word OVER,CAT,DOLIT,36,EQUAL
.word QBRAN,NUMQ1
.word HEX,SWAP,ONE,PLUS
.word SWAP,ONE,SUBB
NUMQ1:
.word OVER,CAT,DOLIT,45,EQUAL,TOR
.word SWAP,RAT,SUBB,SWAP,RAT,PLUS,QDUP
.word QBRAN,NUMQ6
.word ONE,SUBB,TOR
NUMQ2:
.word DUPP,TOR,CAT,BASE,AT,DIGTQ
.word QBRAN,NUMQ4
.word SWAP,BASE,AT,STAR,PLUS,RFROM
.word ONE,PLUS
.word DONXT,NUMQ2
.word RAT,SWAP,DROP
.word QBRAN,NUMQ3
.word NEGAT
NUMQ3:
.word SWAP
.word BRAN,NUMQ5
NUMQ4:
.word RFROM,RFROM,DDROP,DDROP,ZERO
NUMQ5:
.word DUPP
NUMQ6:
.word RFROM,DDROP
.word RFROM,BASE,STORE,EXIT


 ##### input output words #####


QKEY:   .word   dolst
.word   TQKEY, ATEXE, EXIT

KEY:    .word   dolst
KEY1:
.word   QKEY, QBRAN, KEY1, EXIT

EMIT:   .word   dolst
.word   TEMIT, ATEXE, EXIT

NUFQ:   .word   dolst
.word   QKEY, DUPP, QBRAN, NUFQ1
.word   DDROP, KEY, DOLIT, LF, EQUAL
NUFQ1:
.word   EXIT

PACE:   .word   dolst
.word   DOLIT, 11, EMIT, EXIT

SPACE:  .word   dolst
.word   BLANK, EMIT, EXIT

SPACS:  .word   dolst
.word   DOLIT, 0, MAX, TOR
.word   BRAN, CHAR2
CHAR1:
.word   SPACE
CHAR2:
.word   DONXT, CHAR1, EXIT

TYPEE:  .word   dolst
.word   TOR, BRAN, TYPE2
TYPE1:
.word   DUPP, CAT, EMIT, DOLIT, 1, PLUS
TYPE2:
.word   DONXT, TYPE1, DROP, EXIT

CR:     .word   dolst
 #      .word   DOLIT, CRR, EMIT
.word   DOLIT, LF, EMIT, EXIT


DOSTR:  .word   dolst
.word RFROM,RAT,RFROM,COUNT,PLUS
.word ALGND,TOR,SWAP,TOR,EXIT

STRQP:  .word   dolst
.word DOSTR,EXIT

DOTQP:  .word   dolst
.word DOSTR,COUNT,TYPEE,EXIT

DOTR:   .word   dolst
.word TOR,STRR,RFROM,OVER,SUBB
.word SPACS,TYPEE,EXIT

UDOTR:  .word   dolst
.word TOR,BDIGS,DIGS,EDIGS
.word RFROM,OVER,SUBB
.word SPACS,TYPEE,EXIT

UDOT:   .word   dolst
.word BDIGS,DIGS,EDIGS
.word SPACE,TYPEE,EXIT

DOT:    .word   dolst
.word BASE,AT,DOLIT,10,XORR
.word QBRAN,DOT1
.word UDOT,EXIT
DOT1:
.word STRR,SPACE,TYPEE,EXIT

QUEST:  .word   dolst
.word AT,DOT,EXIT


 ##### text interpreter #####


PARS:   .word   dolst
.word TEMP,STORE,OVER,TOR,DUPP
.word QBRAN,PARS8
.word ONE,SUBB,TEMP,AT,BLANK,EQUAL
.word QBRAN,PARS3
.word TOR
PARS1:
.word BLANK,OVER,CAT
.word SUBB,ZLESS,INVER
.word QBRAN,PARS2
.word ONE,PLUS
.word DONXT,PARS1
.word RFROM,DROP,ZERO,DUPP,EXIT
PARS2:
.word RFROM
PARS3:
.word OVER,SWAP
.word TOR
PARS4:
.word TEMP,AT,OVER,CAT,SUBB
.word TEMP,AT,BLANK,EQUAL
.word QBRAN,PARS5
.word ZLESS
PARS5:
.word QBRAN,PARS6
.word ONE,PLUS
.word DONXT,PARS4
.word DUPP,TOR
.word BRAN,PARS7
PARS6:
.word RFROM,DROP,DUPP
.word ONE,PLUS,TOR
PARS7:
.word OVER,SUBB
.word RFROM,RFROM,SUBB,EXIT
PARS8:
.word OVER,RFROM,SUBB,EXIT

PARSE:  .word   dolst
.word TOR,TIB,INN,AT,PLUS
.word NTIB,AT,INN,AT,SUBB
.word RFROM,PARS,INN,PSTOR,EXIT

DOTPR:  .word   dolst
.word DOLIT,41,PARSE,TYPEE,EXIT

PAREN:  .word   dolst
.word DOLIT,41,PARSE,DDROP,EXIT

BKSLA:  .word   dolst
.word NTIB,AT,INN,STORE,EXIT

CHAR:   .word   dolst
.word BLANK, PARSE, DROP, CAT, EXIT

TOKEN:  .word   dolst
.word BLANK,PARSE,DOLIT,31,MIN
.word NP,AT,OVER,SUBB,CELLM
.word PACKS,EXIT

WORDD:  .word   dolst
.word PARSE,HERE,PACKS,EXIT

NAMET:  .word   dolst
.word CELLM
.word CELLM,AT,EXIT

SAMEQ:  .word   dolst
.word TOR
.word BRAN,SAME2
SAME1:
.word OVER,RAT,CELLS,PLUS,AT
.word OVER,RAT,CELLS,PLUS,AT
.word SUBB,QDUP
.word QBRAN,SAME2
.word RFROM,DROP,EXIT
SAME2:
.word DONXT,SAME1
.word ZERO,EXIT

FIND:   .word   dolst  # (a va -- ca na | a F) \ search word in dictionary 'a' is target address, 'va' dict. head address
.word SWAP,DUPP,CAT
.word DOLIT,CELLL,SLASH,TEMP,STORE
.word DUPP,AT,TOR,CELLP,SWAP
FIND1:
.word AT,DUPP
.word QBRAN,FIND6
.word DUPP,AT,DOLIT,MASKK,ANDD,RAT,XORR
.word QBRAN,FIND2
.word CELLM
.word BRAN,FIND1
FIND2:
.word CELLP,TEMP,AT,SAMEQ
FIND3:
.word BRAN,FIND4
FIND6:
.word RFROM,DROP
.word SWAP,CELLM,SWAP,EXIT
FIND4:
.word QBRAN,FIND5
.word CELLM,CELLM
.word BRAN,FIND1
FIND5:
.word RFROM,DROP,SWAP,DROP
.word CELLM
.word DUPP,NAMET,SWAP,EXIT

NAMEQ:  .word   dolst
.word CNTXT,DUPP,DAT,XORR
.word QBRAN,NAMQ1
.word CELLM
NAMQ1:
.word TOR
NAMQ2:
.word RFROM,CELLP,DUPP,TOR
.word AT,QDUP
.word QBRAN,NAMQ3
.word FIND,QDUP
.word QBRAN,NAMQ2
.word RFROM,DROP,EXIT
NAMQ3:
.word RFROM,DROP
.word ZERO,EXIT

BKSP:   .word   dolst
.word TOR,OVER,RFROM,SWAP,OVER,XORR
.word QBRAN,BACK1
.word DOLIT,BKSPP,TECHO,ATEXE,ONE,SUBB
.word BLANK,TECHO,ATEXE
.word DOLIT,BKSPP,TECHO,ATEXE
BACK1:
.word EXIT

TAP:    .word   dolst
.word DUPP,TECHO,ATEXE
.word OVER,CSTOR,ONE,PLUS,EXIT

KTAP:   .word   dolst
.word DUPP,DOLIT,CRR,XORR
.word QBRAN,KTAP2
.word DOLIT,BKSPP,XORR
.word QBRAN,KTAP1
.word BLANK,TAP,EXIT
KTAP1:
.word BKSP,EXIT
KTAP2:
.word DROP,SWAP,DROP,DUPP,EXIT

ACCEP:  .word   dolst
.word OVER,PLUS,OVER
ACCP1:
.word DDUP,XORR
.word QBRAN,ACCP4
.word KEY,DUPP
.word BLANK,DOLIT,127,WITHI
.word QBRAN,ACCP2
.word TAP
.word BRAN,ACCP3
ACCP2:
.word TTAP,ATEXE
ACCP3:
.word BRAN,ACCP1
ACCP4:
.word DROP,OVER,SUBB,EXIT

EXPEC:  .word   dolst
.word TEXPE,ATEXE,SPAN,STORE,DROP,EXIT

QUERY:  .word   dolst
.word TIB,DOLIT,80, TEXPE,ATEXE,NTIB,STORE
.word DROP,ZERO,INN,STORE, EXIT


 ##### error checking words #####


CATCH:  .word   dolst
.word SPAT,TOR,HANDL,AT,TOR
.word RPAT,HANDL,STORE,EXECU
.word RFROM,HANDL,STORE
.word RFROM,DROP,ZERO,EXIT

THROW:  .word   dolst
.word HANDL,AT,RPSTO
.word RFROM,HANDL,STORE
.word RFROM,SWAP,TOR,SPSTO
.word DROP,RFROM,EXIT

NULLS:  .word   dovar
.word 0
.byte 99,111,121,111,116,101  
  # $align

ABORT:  .word   dolst
.word NULLS,THROW

ABORQ:  .word   dolst
.word QBRAN,ABOR1
.word DOSTR,THROW
ABOR1:
.word DOSTR,DROP,EXIT


   ###### text interpreter #####


INTER:  .word   dolst
.word NAMEQ,QDUP
.word QBRAN,INTE1
.word AT,DOLIT,0x40000000       #COMP
.word ANDD
.word ABORQ
.ascii "\X0d COMPILE ONLY"   
.word EXECU,EXIT
INTE1:
.word TNUMB,ATEXE
.word QBRAN,INTE2
.word EXIT
INTE2:
.word THROW

LBRAC:  .word   dolst
.word DOLIT,INTER,TEVAL,STORE,EXIT

DOTOK:  .word   dolst
.word DOLIT,INTER,TEVAL,AT,EQUAL
.word QBRAN,DOTO1
.word DOTQP
.ascii "\X03 ok"
DOTO1:
.word CR,EXIT

QSTAC:  .word   dolst
.word DEPTH,ZLESS
.word ABORQ
.ascii "\X0a underflow"
.word EXIT

EVAL:   .word   dolst
EVAL1:
.word TOKEN,DUPP,CAT
.word QBRAN,EVAL2
.word TEVAL,ATEXE,QSTAC
.word BRAN,EVAL1   
EVAL2:
.word DROP,TPROM,ATEXE,EXIT

PRESE:  .word   dolst
.word SZERO,AT,SPSTO
.word DOLIT,TIBB,NTIB,CELLP,STORE,EXIT

XIO:    .word   dolst
.word DOLIT,ACCEP,TEXPE,DSTOR
.word TECHO,DSTOR,EXIT

FILE:   .word   dolst
.word DOLIT,PACE,DOLIT,DROP
.word DOLIT,KTAP,XIO,EXIT

HAND:   .word   dolst
.word DOLIT,DOTOK,DOLIT,EMIT
.word DOLIT,KTAP,XIO,EXIT

ISLO:   .word   dovar
.word QRX,TXSTO

CONSO:  .word   dolst
.word ISLO,DAT,TQKEY,DSTOR
.word HAND,EXIT

QUIT:   .word   dolst
.word RZERO,AT,RPSTO
QUIT1:
.word LBRAC
QUIT2:
.word QUERY
.word DOLIT,EVAL,CATCH,QDUP
.word QBRAN,QUIT2
.word TPROM,AT,SWAP
.word CONSO,NULLS,OVER,XORR
.word QBRAN,QUIT3
.word SPACE,COUNT,TYPEE
.word DOTQP
.ascii "\X02 ?"
QUIT3:
.word DOLIT,DOTOK,XORR
.word QBRAN,QUIT4
.word DOLIT,ERR,EMIT
QUIT4:
.word PRESE
.word BRAN,QUIT1

TICK:   .word   dolst
.word TOKEN,NAMEQ
.word QBRAN,TICK1
.word EXIT
TICK1:
.word THROW


 ##### compiler words #####


ALLOT:  .word   dolst
.word CP,PSTOR,EXIT

COMMA:  .word   dolst
.word HERE,DUPP,CELLP
.word CP,STORE,STORE,EXIT

BCOMP:  .word   dolst
.word TICK,COMMA,EXIT

COMPI:  .word   dolst
.word   RFROM, DUPP, AT, COMMA
.word   CELLP, TOR, EXIT

LITER:  .word   dolst
.word   COMPI, DOLIT, COMMA, EXIT

STRCQ:  .word   dolst
.word DOLIT,34,WORDD
.word COUNT,PLUS,ALGND
.word CP,STORE,EXIT

RECUR:  .word   dolst
.word LAST,AT,NAMET,COMMA,EXIT

FOR:    .word   dolst
.word COMPI,TOR,HERE,EXIT

BEGIN:  .word   dolst
.word HERE,EXIT

NEXT:   .word   dolst
.word COMPI,DONXT,COMMA,EXIT

UNTIL:  .word   dolst
.word COMPI,QBRAN,COMMA,EXIT

AGAIN:  .word   dolst
.word COMPI,BRAN,COMMA,EXIT

IFF:    .word   dolst
.word COMPI,QBRAN,HERE
.word ZERO,COMMA,EXIT

AHEAD:  .word   dolst
.word COMPI,BRAN,HERE,ZERO,COMMA,EXIT

REPEA:  .word   dolst
.word AGAIN, HERE, SWAP, STORE, EXIT

THENN:  .word   dolst
.word HERE,SWAP,STORE,EXIT

AFT:    .word   dolst
.word DROP,AHEAD,BEGIN,SWAP,EXIT

ELSEE:  .word   dolst
.word AHEAD,SWAP,THENN,EXIT

WHILE:  .word   dolst
.word IFF,SWAP,EXIT

ABRTQ:  .word   dolst
.word COMPI,ABORQ,STRCQ,EXIT

STRQ:   .word   dolst
.word COMPI,STRQP,STRCQ,EXIT

DOTQ:   .word   dolst
.word COMPI,DOTQP,STRCQ,EXIT

UNIQU:  .word   dolst
.word DUPP,NAMEQ
.word QBRAN,UNIQ1
.word DOTQP
.ascii "\X07 reDef "  # "D$"
.word OVER,COUNT,TYPEE
UNIQ1:
.word DROP,EXIT

SNAME:  .word   dolst
.word DUPP,CAT
.word QBRAN,PNAM1
.word UNIQU
.word DUPP,LAST,STORE
.word HERE,ALGND,SWAP
.word CELLM
.word CRRNT,AT,AT,OVER,STORE
.word CELLM,DUPP,NP,STORE
.word STORE,EXIT
PNAM1:
.word STRQP
.ascii "\X05 name"   # "D$"
.word THROW

SCOMP:  .word   dolst
.word NAMEQ,QDUP
.word QBRAN,SCOM2
.word AT,DOLIT,0x80000000,ANDD
.word QBRAN,SCOM1
.word EXECU,EXIT
SCOM1:
.word COMMA,EXIT
SCOM2:
.word TNUMB,ATEXE
.word QBRAN,SCOM3
.word LITER,EXIT
SCOM3:
.word THROW

OVERT:  .word   dolst
.word LAST,AT,CRRNT,STORE,EXIT

SEMIS:  .word   dolst
.word COMPI,EXIT,LBRAC,OVERT,EXIT

RBRAC:  .word   dolst
.word DOLIT,SCOMP,TEVAL,STORE,EXIT

CALLC:  .word   dolst
.word DOLIT,CALLC,AT
.word COMMA
.word EXIT

COLON:  .word   dolst
.word TOKEN,SNAME,DOLIT,dolst
.word COMMA,RBRAC,EXIT

IMMED:  .word   dolst
.word DOLIT,0x80000000,LAST,AT,AT,ORR
.word LAST,AT,STORE,EXIT

USER:   .word   dolst
.word TOKEN,SNAME,OVERT
.word DOLIT,douse,COMMA,COMMA,EXIT

CREAT:  .word   dolst
.word TOKEN,SNAME,OVERT
.word DOLIT,dovar,COMMA
.word EXIT

VARIA:  .word   dolst
.word CREAT,ZERO,COMMA,EXIT

CONST:  .word   dolst
.word TOKEN,SNAME,OVERT
.word DOLIT,docon,COMMA,COMMA,EXIT


 ##### utility words #####


UTYPE:  .word   dolst
.word TOR
.word BRAN,UTYP2
UTYP1:
.word DUPP,CAT,TCHAR,EMIT
.word ONE,PLUS
UTYP2:
.word DONXT,UTYP1
.word DROP,EXIT

DMP:    .word   dolst
.word OVER,DOLIT,4,UDOTR,SPACE,TOR
.word BRAN,PDUM2
PDUM1:
.word DUPP,CAT,DOLIT,3,UDOTR
.word ONE,PLUS
PDUM2:
.word DONXT,PDUM1
.word EXIT

DUMP:   .word   dolst
.word BASE,AT,TOR,HEX
.word DOLIT,16,SLASH
.word TOR
DUMP1:
.word CR,DOLIT,16,DDUP,DMP
.word ROT,ROT
.word SPACE,SPACE,UTYPE
 #      .word NUFQ
 #      .word INVER
 #      .word QBRAN,DUMP2
.word DONXT,DUMP1
 #      .word BRAN,DUMP3
 #      DUMP2:
 #      .word RFROM,DROP
DUMP3:
.word DROP,RFROM,BASE,STORE
.word EXIT

DOTS:   .word   dolst
.word CR,DEPTH
.word TOR
.word BRAN,DOTS2
DOTS1:
.word RAT,PICK,DOT
DOTS2:
.word DONXT,DOTS1
.word DOTQP
.ascii "\X04 <sp"  # D$
.word EXIT

STCSP:  .word   dolst
.word SPAT,CSP,STORE,EXIT

QCSP:   .word   dolst
.word SPAT,CSP,AT, XORR, ABORQ
.ascii  "\X07 stacks"
.word   EXIT

TNAME:  .word   dolst
.word CRRNT
TNAM1:
.word CELLP,AT,QDUP
.word QBRAN,TNAM4
.word DDUP
TNAM2:
.word AT,DUPP
.word QBRAN,TNAM3
.word DDUP,NAMET,XORR
.word QBRAN,TNAM3
.word CELLM
.word BRAN,TNAM2
TNAM3:
.word SWAP,DROP,QDUP
.word QBRAN,TNAM1
.word SWAP,DROP,SWAP,DROP,EXIT
TNAM4:
.word DROP,ZERO,EXIT

DOTID:  .word   dolst
.word QDUP
.word QBRAN,DOTI1
.word COUNT,DOLIT,0x1F,ANDD
.word UTYPE,EXIT
DOTI1:
.word DOTQP
.ascii "\X09 {noName}"  # D$
.word EXIT

SEE:    .word   dolst
.word TICK
.word CR
SEE1:
.word CELLP,DUPP,AT
.word DOLIT,CALLC,AT,XORR
.word QBRAN,SEE5
.word DUPP,AT
.word TNAME
SEE2:
.word QDUP
.word QBRAN,SEE3
.word SPACE,DOTID
.word BRAN,SEE4
SEE3:
.word DUPP,AT,UDOT
SEE4:
 #      .word NUFQ
.word BRAN,SEE1
SEE5:
.word DROP,EXIT

WORDS:  .word   dolst
.word CR,CNTXT,AT
WORS1:
.word AT,QDUP
.word QBRAN,WORS2
.word DUPP,SPACE,DOTID
.word CELLM             
 #      .word NUFQ,QBRAN,WORS1
.word BRAN,WORS1
.word DROP
WORS2:
.word EXIT

 
 ##### include files #####


INCLU:  .word   dolst
.word TIB,NTIB,AT,PLUS          # end of text buffer
.word BLANK,WORDD,ONE,PLUS      # get file name
.word INN,DAT,TOR,TOR           # save >IN and #TIB
.word OPEN,QDUP                 # open and read file
.word QBRAN,INCL1
.word NTIB,AT,INN,STORE         # >IN points to beginning of file
.word NTIB,PSTOR                # #TIB points to end of file
.word EVAL                      # interpret file
INCL1:
.word RFROM,RFROM,INN,DSTOR     # restore >IN and #TIB
.word EXIT                      # done

.word dolst

 ##### cold start #####


VERSN:  .word   dolst
.word DOLIT,0x103,EXIT

HI:     .word   dolst
.word STOIO,CR
.word DOTQP
.ascii "\X0emips eForth v"
.word BASE,AT,HEX
.word VERSN,BDIGS,DIG,DIG
.word DOLIT,46,HOLD
.word DIGS,EDIGS,TYPEE
.word BASE,STORE,CR,EXIT

TBOOT:  .word   dovar
.word HI

DIAG:   .word   dolst
.word DOLIT,101,TXSTO
.word ZERO,ZLESS,DOLIT,-2,ZLESS,UPLUS,DROP
.word DOLIT,3,UPLUS,UPLUS,DROP,DOLIT,67,UPLUS,DROP,TXSTO
.word DOLIT,79,DOLIT,111,XORR,DOLIT,0xF0,ANDD
.word DOLIT,79,ORR,TXSTO
.word SPAT,SPAT,SWAP,OVER,XORR,DOLIT,CELLL,ANDD
.word DOLIT,114-CELLL,UPLUS,DROP
.word SWAP,DUPP,DUPP,SPSTO
.word TXSTO
.word ZERO
QB1:
.word QBRAN,QB2,DOLIT,63,TXSTO
QB2:
.word DOLIT,-1,QBRAN,QB3,DOLIT,116,TXSTO,BRAN,QB4
QB3:
.word DOLIT,33,TXSTO
QB4:
.word DOLIT,0xff,SPAT
.word DOLIT,0xff,OVER,DOLIT,1,UPLUS,DROP,CSTOR
.word DOLIT,104,OVER,AT,ANDD,OVER,STORE
.word DUPP,AT,OVER,DOLIT,1,UPLUS,DROP,CAT,ORR
.word SWAP,STORE,TXSTO,DOLIT,32,TXSTO

.word DOLIT,77,TOR,RPAT
.word DOLIT,63,TOR,RPSTO,RFROM
.word TXSTO
.word SZERO,DOLIT,UPP+16,XORR
.word QBRAN,UCHK1,DOLIT,63,TXSTO
UCHK1:
.word DOLIT,117,TXSTO
.word ZERO
.word DOLIT,0x1B,TOR
FOR1:
.word DOLIT,1,UPLUS,DROP,DONXT,FOR1
.word DOLIT,0x1C,XORR
.word QBRAN,NXTOK,DOLIT,63,TXSTO,EXIT
NXTOK:
.word DOLIT,108,TXSTO,EXIT

COLD:   .word   dolst
COLD1:
#       .word   DIAG
.word DOLIT,LASTN,LAST
.word STORE
.word PRESE
.word TBOOT,ATEXE
.word FORTH,CNTXT,AT, DUPP
.word CRRNT,DSTOR, OVERT
.word QUIT
.word BRAN,COLD1  
        .word   dolst   # stop SEE here



# name dictionary in FLASH
        .word   QRX, p1
p0:     .byte   3
        .ascii  "?RX"
        .word   TXSTO, p2
p1:     .byte   3
        .ascii  "TX!"
        .word   STOIO, p3
p2:     .byte   3
        .ascii  "!IO"
        .word   DOLIT, p4
p3:     .byte   5+COMPO
        .ascii  "doLIT"
        .word   DOLST, p5
p4:     .byte   6+COMPO
        .ascii  "doLIST"
        .word   EXIT, p6
p5:     .byte   4
        .ascii  "EXIT"
        .word   EXECU, p7
p6:     .byte   7
        .ascii  "EXECUTE"
        .word   DONXT, p8
p7:     .byte   4+COMPO
        .ascii  "next"    
        .word   QBRAN, p9
p8:     .byte   7+COMPO
        .ascii  "?branch"
        .word   BRAN, p10
p9:     .byte   6+COMPO
        .ascii  "branch"
        .word   STORE, p11
p10:    .byte   1,33
        .word   AT, p12
p11:    .byte   1
        .ascii  "@"
        .word   CSTOR, p13
p12:    .byte   2
        .ascii  "C!"
        .word   CAT, p11a
p13:    .byte   2
        .ascii  "C@"
        .word   WAT, p12a
p11a:   .byte   2
        .ascii  "W@"
        .word   WSTOR, p14
p12a:   .byte   2
        .ascii  "W!"
        .word   RPAT, p15
p14:    .byte   3
        .ascii  "RP@"
        .word   RPSTO, p16
p15:    .byte   3
        .ascii  "RP!"
        .word   RFROM, p17
p16:    .byte   2
        .ascii  "R>"
        .word   RAT, p18
p17:    .byte   2
        .ascii  "R@"
        .word   TOR, p19
p18:    .byte   2
        .ascii  ">R"
        .word   SPAT, p20
p19:    .byte   3
        .ascii  "SP@"
        .word   SPSTO, p21
p20:    .byte   3
        .ascii  "SP!"
        .word   DROP, p22
p21:    .byte   4
        .ascii  "DROP"
        .word   DUPP, p23
p22:    .byte   3
        .ascii  "DUP"
        .word   SWAP, p24
p23:    .byte   4
        .ascii  "SWAP"
        .word   OVER, p25
p24:    .byte   4
        .ascii  "OVER"
        .word   ZLESS, p26
p25:    .byte   2
        .ascii  "0<"
        .word   ANDD, p27
p26:    .byte   3
        .ascii  "AND"
        .word   ORR, p28
p27:    .byte   2
        .ascii  "OR"
        .word   XORR, p29
p28:    .byte   3
        .ascii  "XOR"
        .word   UPLUS, p31
p29:    .byte   3
        .ascii  "UM+"
        .word   UP, p33
p31:    .byte   2
        .ascii  "UP"
        .word   SZERO, p34
p33:    .byte   3
        .ascii  "SP0"
        .word   RZERO, p35
p34:    .byte   3
        .ascii  "RP0"
        .word   TQKEY, p36
p35:    .byte   5
        .ascii  "\'?KEY"
        .word   TEMIT, p37
p36:    .byte   5
        .ascii  "\'EMIT"
        .word   TEXPE, p38
p37:    .byte   7
        .ascii  "\'EXPECT"
        .word   TTAP, p39
p38:    .byte   4
        .ascii  "\'TAP"
        .word   TECHO, p40
p39:    .byte   5
        .ascii  "\'ECHO"
        .word   TPROM, p41
p40:    .byte   6
        .ascii  "PROMPT"
        .word   BASE, p42
p41:    .byte   4
        .ascii  "BASE"
        .word   TEMP, p43
p42:    .byte   3
        .ascii  "tmp"
        .word   SPAN, p44
p43:    .byte   4
        .ascii  "SPAN"
        .word   INN, p45
p44:    .byte   3
        .ascii  ">IN"
        .word   NTIB, p46
p45:    .byte   4
        .ascii  "\#TIB"
        .word   CSP, p47
p46:    .byte   3
        .ascii  "CSP"
        .word   TEVAL, p48
p47:    .byte   5
        .ascii  "\'EVAL"
        .word   TNUMB, p49
p48:    .byte   7
        .ascii  "\'NUMBER"
        .word   HLD, p50
p49:    .byte   3
        .ascii  "HLD"
        .word   HANDL, p51
p50:    .byte   7
        .ascii  "HANDLER"
        .word   CNTXT, p52
p51:    .byte   7
        .ascii  "CONTEXT"
        .word   CRRNT, p53
p52:    .byte   7
        .ascii  "CURRENT"
        .word   CP, p54
p53:    .byte   2
        .ascii  "CP"
        .word   NP, p55
p54:    .byte   2
        .ascii  "NP"
        .word   LAST, p57
p55:    .byte   4
        .ascii  "LAST"
        .word   FORTH, p58
p57:    .byte   5
        .ascii  "FORTH"
        .word   QDUP, p59
p58:    .byte   4
        .ascii  "?DUP"
        .word   ROT, p60
p59:    .byte   3
        .ascii  "ROT"
        .word   DDROP, p61
p60:    .byte   5
        .ascii  "2DROP"
        .word   DDUP, p62
p61:    .byte   4
        .ascii  "2DUP"
        .word   PLUS, p63
p62:    .byte   1
        .ascii  "+"
        .word   DPLUS, p64
p63:    .byte   2
        .ascii  "D+"
        .word   INVER, p65
p64:    .byte   3
        .ascii  "NOT"
        .word   NEGAT, p66
p65:    .byte   6
        .ascii  "NEGATE"
        .word   DNEGA, p67
p66:    .byte   7
        .ascii  "DNEGATE"
        .word   SUBB, p68
p67:    .byte   1
        .ascii  "-"
        .word   ABSS, p69
p68:    .byte   3
        .ascii  "ABS"
        .word   EQUAL, p70
p69:    .byte   1
        .ascii  "="
        .word   ULESS, p71
p70:    .byte   2
        .ascii  "U<"
        .word   LESS, p72
p71:    .byte   1
        .ascii  "<"
        .word   MAX, p73
p72:    .byte   3
        .ascii  "MAX"
        .word   MIN, p74
p73:    .byte   3
        .ascii  "MIN"
        .word   WITHI, p75
p74:    .byte   6
        .ascii  "WITHIN"
        .word   UMMOD, p76
p75:    .byte   6
        .ascii  "UM/MOD"
        .word   MSMOD, p77
p76:    .byte   5
        .ascii  "M/MOD"
        .word   SLMOD, p78
p77:    .byte   4
        .ascii  "/MOD"
        .word   MODD, p79
p78:    .byte   3
        .ascii  "MOD"
        .word   SLASH, p80
p79:    .byte   1
        .ascii  "/"
        .word   UMSTA, p81
p80:    .byte   3
        .ascii  "UM*"
        .word   STAR, p82
p81:    .byte   1
        .ascii  "*"
        .word   MSTAR, p83
p82:    .byte   2
        .ascii  "M*"
        .word   SSMOD, p84
p83:    .byte   5
        .ascii  "*/MOD"
        .word   STASL, p85
p84:    .byte   2
        .ascii  "*/"
        .word   CELLP, p86
p85:    .byte   5
        .ascii  "CELL+"
        .word   CELLM, p87
p86:    .byte   5
        .ascii  "CELL-"
        .word   CELLS, p88a
p87:    .byte   5
        .ascii  "CELLS"
        .word   TWOST, p88b
p88a:   .byte   2
        .ascii  "2*"
        .word   TWOSL, p88
p88b:   .byte   2
        .ascii  "2/"
        .word   ALGND, p89
p88:    .byte   7
        .ascii  "ALIGNED"
        .word   BLANK, p90
p89:    .byte   2
        .ascii  "BL"
        .word   TCHAR, p91
p90:    .byte   5
        .ascii  ">CHAR"
        .word   DEPTH, p92
p91:    .byte   5
        .ascii  "DEPTH"
        .word   PICK, p93
p92:    .byte   4
        .ascii  "PICK"
        .word   PSTOR, p94
p93:    .byte   2
        .ascii  "+!"
        .word   DSTOR, p95
p94:    .byte   2
        .ascii  "2!"
        .word   DAT, p96
p95:    .byte   2
        .ascii  "2@"
        .word   COUNT, p97
p96:    .byte   5
        .ascii  "COUNT"
        .word   HERE, p98
p97:    .byte   4
        .ascii  "HERE"
        .word   PAD, p99
p98:    .byte   3
        .ascii  "PAD"
        .word   TIB, p100
p99:    .byte   3
        .ascii  "TIB"
        .word   ATEXE, p101
p100:   .byte   8
        .ascii  "@EXECUTE"
        .word   CMOVE, p102
p101:   .byte   5
        .ascii  "CMOVE"
        .word   FILL, p103
p102:   .byte   4
        .ascii  "FILL"
        .word   DTRAI, p104
p103:   .byte   9
        .ascii  "-TRAILING"
        .word   PACKS, p105
p104:   .byte   5
        .ascii  "PACK$"
        .word   DIGIT, p106
p105:   .byte   5
        .ascii  "DIGIT"
        .word   EXTRC, p107
p106:   .byte   7
        .ascii  "EXTRACT"
        .word   BDIGS, p108
p107:   .byte   2
        .ascii  "<\#"
        .word   HOLD, p109
p108:   .byte   4
        .ascii  "HOLD"
        .word   DIG, p110
p109:   .byte   1
        .ascii  "\#"
        .word   DIGS, p111
p110:   .byte   2
        .ascii  "#S"
        .word   SIGN, p112
p111:   .byte   4
        .ascii  "SIGN"
        .word   EDIGS, p113
p112:   .byte   2
        .ascii  "\#>"
        .word   STRR, p114
p113:   .byte   3
        .ascii  "str"
        .word   HEX, p115
p114:   .byte   3
        .ascii  "HEX"
        .word   DECIM, p116
p115:   .byte   7
        .ascii  "DECIMAL"
        .word   DIGTQ, p117
p116:   .byte   6
        .ascii  "DIGIT?"
        .word   NUMBQ, p118
p117:   .byte   7
        .ascii  "NUMBER?"
        .word   QKEY, p119
p118:   .byte   4
        .ascii  "?KEY"
        .word   KEY, p120
p119:   .byte   3
        .ascii  "KEY"
        .word   EMIT, p121
p120:   .byte   4
        .ascii  "EMIT"
        .word   NUFQ, p122
p121:   .byte   4
        .ascii  "NUF?"
        .word   PACE, p123
p122:   .byte   4
        .ascii  "PACE"
        .word   SPACE, p124
p123:   .byte   5
        .ascii  "SPACE"
        .word   SPACS, p125
p124:   .byte   6
        .ascii  "SPACES"
        .word   TYPEE, p126
p125:   .byte   4
        .ascii  "TYPE"
        .word   CR, p127
p126:   .byte   2
        .ascii  "CR"
        .word   DOSTR, p128
p127:   .byte   3+COMPO
        .ascii  "do$"
        .word   STRQP, p128a
p128:   .byte   3+COMPO
        .ascii  "$\"|"
        .word   DOTQP, p129
p128a:  .byte   3+COMPO
        .ascii  ".\"|"
        .word   DOTR, p130
p129:   .byte   2
        .ascii  ".R"
        .word   UDOTR, p131
p130:   .byte   3
        .ascii  "U.R"
        .word   UDOT, p132
p131:   .byte   2
        .ascii  "U."
        .word   DOT, p133
p132:   .byte   1
        .ascii  "."
        .word   QUEST, p134
p133:   .byte   1
        .ascii  "?"
        .word   PARS, p135
p134:   .byte   5
        .ascii  "parse"
        .word   PARSE, p136
p135:   .byte   5
        .ascii  "PARSE"
        .word   DOTPR, p137
p136:   .byte   2+IMEDD
        .ascii  ".("
        .word   PAREN, p138
p137:   .byte   1+IMEDD
        .ascii  "("
        .word   BKSLA, p139
p138:   .byte   1+IMEDD
        .ascii  "\\"
        .word   CHAR, p140
p139:   .byte   4
        .ascii  "CHAR"
        .word   TOKEN, p141
p140:   .byte   5
        .ascii  "TOKEN"
        .word   WORDD, p142
p141:   .byte   4
        .ascii  "WORD"
        .word   NAMET, p143
p142:   .byte   5
        .ascii  "NAME>"
        .word   SAMEQ, p144
p143:   .byte   5
        .ascii  "SAME?"
        .word   FIND, p145
p144:   .byte   4
        .ascii  "find"
        .word   NAMEQ, p146
p145:   .byte   5
        .ascii  "NAME?"
        .word   BKSP, p147
p146:   .byte   2
        .ascii  "^H"
        .word   TAP, p148
p147:   .byte   3
        .ascii  "TAP"
        .word   KTAP, p149
p148:   .byte   4
        .ascii  "kTAP"
        .word   ACCEP, p150
p149:   .byte   6
        .ascii  "accept"
        .word   EXPEC, p151
p150:   .byte   6
        .ascii  "EXPECT"
        .word   QUERY, p152
p151:   .byte   5
        .ascii  "QUERY"
        .word   CATCH, p153
p152:   .byte   5
        .ascii  "CATCH"
        .word   THROW, p154
p153:   .byte   5
        .ascii  "THROW"
        .word   NULLS, p155
p154:   .byte   5
        .ascii  "NULL$"
        .word   ABORT, p156
p155:   .byte   5
        .ascii  "ABORT"
        .word   ABORQ, p157
p156:   .byte   6
        .ascii  "abort\""
        .word   INTER, p158
p157:   .byte   10
        .ascii  "$INTERPRET"
        .word   LBRAC, p159
p158:   .byte   1+IMEDD
        .ascii  "["
        .word   DOTOK, p160
p159:   .byte   3
        .ascii  ".OK"
        .word   QSTAC, p161
p160:   .byte   6
        .ascii  "?STACK"
        .word   EVAL, p162
p161:   .byte   4
        .ascii  "EVAL"
        .word   PRESE, p163
p162:   .byte   6
        .ascii  "PRESET"
        .word   XIO, p164
p163:   .byte   3+COMPO
        .ascii  "xio"
        .word   FILE, p164a
p164:   .byte   4
        .ascii  "FILE"
        .word   OPEN, p165
p164a:  .byte   4
        .ascii  "OPEN"
        .word   HAND, p166
p165:   .byte   4
        .ascii  "HAND"
        .word   ISLO, p167
p166:   .byte   3
        .ascii  "I/O"
        .word   CONSO, p168
p167:   .byte   7
        .ascii  "CONSOLE"
        .word   QUIT, p169
p168:   .byte   4
        .ascii  "QUIT"
        .word   TICK, p170
p169:   .byte   1
        .ascii  "\'"
        .word   ALLOT, p171
p170:   .byte   5
        .ascii  "ALLOT"
        .word   COMMA, p172
p171:   .byte   1
        .ascii  ","
        .word   BCOMP, p173
p172:   .byte   9+IMEDD+COMPO
        .ascii  "[COMPILE]"
        .word   COMPI, p174
p173:   .byte   7+COMPO+COMPO
        .ascii  "COMPILE"
        .word   LITER, p175
p174:   .byte   7+IMEDD+COMPO
        .ascii  "LITERAL"
        .word   STRCQ, p176
p175:   .byte   3+COMPO
        .ascii  "$,\""
        .word   RECUR, p177
p176:   .byte   7+IMEDD+COMPO
        .ascii  "RECURSE"
        .word   FOR, p178
p177:   .byte   3+IMEDD+COMPO
        .ascii  "FOR"
        .word   BEGIN, p179
p178:   .byte   5+IMEDD+COMPO
        .ascii  "BEGIN"
        .word   NEXT, p180
p179:   .byte   4+IMEDD+COMPO
        .ascii  "NEXT"
        .word   UNTIL, p181
p180:   .byte   5+IMEDD+COMPO
        .ascii  "UNTIL"
        .word   AGAIN, p182
p181:   .byte   5+IMEDD+COMPO
        .ascii  "AGAIN"
        .word   IFF, p183
p182:   .byte   2+IMEDD+COMPO
        .ascii  "IF"
        .word   AHEAD, p184
p183:   .byte   5+IMEDD+COMPO
        .ascii  "AHEAD"
        .word   REPEA, p185
p184:   .byte   6+IMEDD+COMPO
        .ascii  "REPEAT"
        .word   THENN, p186
p185:   .byte   4+IMEDD+COMPO
        .ascii  "THEN"
        .word   AFT, p187
p186:   .byte   3+IMEDD+COMPO
        .ascii  "AFT"
        .word   ELSEE, p188
p187:   .byte   4+IMEDD+COMPO
        .ascii  "ELSE"
        .word   WHILE, p189
p188:   .byte   5+IMEDD+COMPO
        .ascii  "WHILE"
        .word   ABRTQ, p190
p189:   .byte   6+IMEDD+COMPO
        .ascii  "ABORT\""
        .word   STRQ, p191
p190:   .byte   2+IMEDD+COMPO
        .ascii  "$\""
        .word   DOTQ, p192
p191:   .byte   2+IMEDD+COMPO
        .ascii  ".\""
        .word   UNIQU, p193
p192:   .byte   7
        .ascii  "?UNIQUE"
        .word   SNAME, p194
p193:   .byte   3
        .ascii  "$\,n"
        .word   SCOMP, p195
p194:   .byte   8
        .ascii  "$COMPILE"
        .word   OVERT, p196
p195:   .byte   5
        .ascii  "OVERT"
        .word   SEMIS, p197
p196:   .byte   1+IMEDD+COMPO
        .ascii  ";"
        .word   RBRAC, p199
p197:   .byte   1
        .ascii  "]"
        .word   COLON, p200
p199:   .byte   1
        .ascii  ":"
        .word   IMMED, p201
p200:   .byte   9
        .ascii  "IMMEDIATE"
        .word   USER, p202
p201:   .byte   4
        .ascii  "USER"
        .word   CREAT, p203
p202:   .byte   6
        .ascii  "CREATE"
        .word   VARIA, p204a
p203:   .byte   8
        .ascii  "VARIABLE"
        .word   CONST, p204
p204a:  .byte   8
        .ascii  "CONSTANT"
        .word   UTYPE, p205
p204:   .byte   5
        .ascii  "_TYPE"
        .word   DMP, p206
p205:   .byte   3
        .ascii  "dm+"
        .word   DUMP, p207
p206:   .byte   4
        .ascii  "DUMP"
        .word   DOTS, p208
p207:   .byte   2
        .ascii  ".S"
        .word   STCSP, p209
p208:   .byte   4
        .ascii  "!CSP"
        .word   QCSP, p210
p209:   .byte   4
        .ascii  "?CSP"
        .word   TNAME, p211
p210:   .byte   5
        .ascii  ">NAME"
        .word   DOTID, p212
p211:   .byte   3
        .ascii  ".ID"
        .word   SEE, p213
p212:   .byte   3
        .ascii  "SEE"
        .word   WORDS, p214a
p213:   .byte   5
        .ascii  "WORDS"
        .word   INCLU, p214
p214a:  .byte   7
        .ascii  "INCLUDE"
        .word   VERSN, p215
p214:   .byte   3
        .ascii  "VER"
        .word   HI, p216
p215:   .byte   2
        .ascii  "hi"
        .word   TBOOT, p217
p216:   .byte   5
        .ascii  "\'BOOT"
        .word   DIAG, p218
p217:   .byte   8
        .ascii  "DIAGNOSE"
        .word   COLD, p219
p218:   .byte   4
        .ascii  "COLD"
        .word   ZERO, p220
p219:   .byte   1
        .ascii  "0"
        .word   ONE, p221
p220:   .byte   1
        .ascii  "1"
        .word   TWO, p222
p221:   .byte   1
        .ascii  "2"
        .word   THREE, p227
 p222:   .byte   1
        .ascii  "3"
#        .word   GRAPH, p224
# p223:   .byte   8
#        .ascii  "GRAPHICS"
#        .word   FLTNG, p225
# p224:   .byte   8
#        .ascii  "FLOATING"
#        .word   APPLI, 226
# p225:   .byte   11
#        .ascii  "APPLICATION"
        .word   DPRT, p227
p226:   .byte   6
        .ascii  "DBGPRT"
        .word   TRON, p228
p227:   .byte 4
        .ascii  "TRON"
        .word   TROFF, 0
p228:   .byte 5
        .ascii "TROFF"
        .word   0
p229:   .word   0,0,0,0

msg1:   .asciiz "%s\n"
msg2:   .asciiz "%80c"
msg3:   .asciiz "ok>"
msg4:   .asciiz "hi.f\000"
msg5:   .asciiz "file read error\X0A\X00"
msg6:   .asciiz "%g\040"
msg7:   .asciiz "step 7\n"
msg8:   .asciiz "rp=%x r0=%x r1=%x\X0A\X00"
msg9:   .asciiz "sp=%x ip=%x wp=%x\X0A\X00"
msg10:  .asciiz "s0=%x s1=%x s2=%x\X0A\X00"


        .data
DATA0:  .space  512
SPP:    .word   0               # data stack
INBUF:  .space  512             # buffer for 'read'
RPP:    .word   0               # return stack
UZERO:
UPP:    .space  16              # user area
        .word   SPP     # 16    SZERO data pointer base
        .word   RPP     # 20    RZERO return pointer base
        .word   QRX     # 24    TQKEY
        .word   TXSTO   # 28    TEMIT
        .word   ACCEP   # 32    TEXPE
        .word   KTAP    # 36    TTAP
        .word   EMIT    # 40    TECHO
        .word   DOTOK   # 44    TPROM
        .word   BASEE   # 48    BASE
        .word   0, 0    # 52    TEMP, SPAN
TINN:   .word   0       # 60    INN
NTIBB:  .word   0       # 64    NTIB
        .word   TIBB    # 68
        .word   0       # 72    CSP
        .word   INTER   # 76    TEVAL
        .word   NUMBQ   # 80    TNUMB
        .word   0, 0    # 84    HLD, HANDL
CTXT:   .word   FORTH+4 # 92    CNTXT
        .word   0       # 96
        .word   0       # 100
        .word   0       # 104
        .word   0       # 108
        .space  16       # 112
CRNT:   .word   0, 0     # 128  CRRNT
        .word   CTOP     # 136  CP
        .word   NTOP     # 140  NP
        .word   LASTN    # 144  LAST
ULAST:  .word   0, 0, 0, 0   
        .word   0, 0, 0, 0   
FVOC:   .word   LASTN        
        .word   0, 0, 0, 0        # editor, assembly, application
SAVLINK:.word   0            
SAVSP:  .word   0            
TRACE:  .word   0
        
TIBB:
FBUFF:  .space 10000           # terminal input buffer and file buffer

CTOP:   .space 8192 # code dictionary space in RAM upwards


RAMN:            # name dictionary in RAM
NTOP:   .word   BYE, p0 # free name dictionary space downwards
LASTN:                  # name dictionary
        .byte   3
        .ascii  "BYE"




#        .end    eForth

# <107 weaver2 /work/ls>

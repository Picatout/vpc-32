/*  Nom: vm.S
 *  Auteur: Jacques Deschênes
 *  Description:
 *      machine viruelle à piles qui exécute du bytecode.
 *      le programme utilisant la VM doit l'appellé en utilisant le prototype de
 *      fonction suivant: void StackVM( char *ip, int* user);
 *         ip pointe vers le bytecode à exécuter
 *         user pointe vers un tableau de variables utilisé par le programme.
 */

#include <p32xxxx.h>
#include "vm.h"


.extern UartPutch
.extern UartGetch
.extern UartWaitch
.extern sys_tick
.extern print_integer
.extern print_float
.extern rnd
//math fonctions
.extern sinf
.extern cosf


#define _getch UartGetch
#define _putch UartPutch
#define _print UartPrint
#define _readln UartReadln
#define _key   UartWaitch


/*************** macros *************/

.macro _tpush
addiu dp,dp, CELL_SIZE
sw T, 0(dp)
.endm

.macro _tpop
lw T, 0(dp)
addiu dp,dp,-CELL_SIZE
.endm

.macro _rpush
addiu rp,rp,CELL_SIZE
sw R, 0(rp)
.endm

.macro _rpop
lw R, 0(rp)
addiu rp,rp,-CELL_SIZE
.endm

.macro _next
addiu ip,ip,1
.endm


/* utilisation des regitres MIPS */
ip=s0  /* pointeur d'instruction VM */
dp=s1  /* pointeur pile de données */
rp=s2  /* pointeur pile de contrôle */
w=s3  /* pointeur de travail */
T=s4  /* sommet de la pile des données */
R=s5  /* sommet de la pile des retours */
U=s6  /* pointeur variables utilisateur */
X=s7  /* compteur de boucle */



STDIO=1

DSTACK_SIZE=80
RSTACK_SIZE=64

SFR_CLEAR=4
SFR_SET=8
SFR_INV=12


.bss

# piles
dstack: .space DSTACK_SIZE
rstack: .space RSTACK_SIZE

.text
.align 2, 0xff
.set noreorder
.set macro


.global StackVM,  flash_code
# .global tokens

.ent StackVM


StackVM:  # initialisation
    addiu sp, sp, -40 // save registers
    sw  ra, 36(sp)
    sw  s0, 0(sp)
    sw  s1, 4(sp)
    sw  s2, 8(sp)
    sw  s3,12(sp)
    sw  s4,16(sp)
    sw  s5,20(sp)
    sw  s6,24(sp)
    sw  s7,28(sp)
    sw  s8,32(sp)
    move ip, a0   // pointeur bytecode à exécuter
    move U, a1    // pointeur tableau de variables
    lui w, RAM_SEG
    ori dp, w, dstack
    ori rp, w, rstack

next:
    lb    t0, 0(ip)
    addiu t1,t0, IBADOP
    bgtz  t1, badop
    sll   t0, t0, 2   
    addiu t0,t0,opcodes_table
    lui   t1, CODE_SEG
    or    t1,t1,t0
    lw    t1, 0(t1)
    jr    t1
    _next

bye:   # 'bye' ( -- ) sortie de la machine virtuelle
    b exit
    xor v0, zero,zero  # return 0

qrx: # '?key' ( -- ch -1 | 0 )
    ori a0,zero,STDIO
    jal _getch
    nop
    _tpush
    bltz v0, qrx_exit
    addiu t0,v0,1
    move T, v0
    addiu t0,zero,-1
    _tpush
qrx_exit:
    b next
    move T, t0

emit:   # 'emit' ( ch -- ) transmet charactère au terminal
    ori a0,zero,STDIO
    move  a1, T
    _tpop
    jal _putch
    andi a1,a1, 255
    b next
    nop

lit:    # 'lit' ( -- n) empile un entier
    jal fetch_int
    addiu dp,dp,CELL_SIZE
    sw T, 0(dp)
    b next
    move T,v0

clit:       # 'clit' ( -- byte ) empile un octet signe étendu sur 32 bits
    _tpush
    lb T, 0(ip)
    b next
    _next

wlit:       # 'wlit' ( -- w16  ) empile un mot de 16 bits
    _tpush
    lbu t0,0(ip)
    lb t1, 1(ip)
    addiu ip,ip,2
    sll  t1,t1, 8
    b next
    or  T,t0,t1

call:   # 'call' (R: -- addr_ret ) appelle sous-routine VM
    jal fetch_int
//    lui t2, CODE_SEG
//    lbu t0, 0(ip)
//fcall_1:
//    lbu t1, 1(ip)
//    addiu ip,ip,2
//    sll t1,t1,8
//    or t0,t0,t1
//    or t0,t0,t2
    _rpush
    move R, ip
    b next
    move ip, v0

//rcall:  # 'rcall' (R: -- addr_ret ) appelle sous-routine VM en mémoire RAM
//    lui t2, RAM_SEG
//    b fcall_1
//    lbu t0, 0(ip)

ret:        # ';' (R: addr_ret -- ) sortie de sous-routine
    move ip, R
    lw R, 0(rp)
    b next
    addiu rp,rp, -CELL_SIZE

bra:        # 'branch' ( -- ) branchement incondionnnel (adresse relative +127,-128)
    lb t0, 0(ip)
    _next
    b next
    addu ip, ip, t0   # branchement relatif à ip, intervale -128 à +127

qbra:       # '?bra' ( t|f -- ) branche si sommet pile est vrai (tos!=0)
    move t0, T
    _tpop
    beqz t0, next
    _next
    lb t0, -1(ip)
    b next
    addu ip,ip,t0

qbraz:      # '?braz' ( t|f -- ) branche si somme pile est faux (tos==0)
    move t0, T
    _tpop
    bne t0, zero, next
    _next
    lb t0, -1(ip)
    b next
    addu ip,ip,t0

exec:       # 'exec' ( addr -- ) appelle sous-routine à partir de la pile
    _rpush
    move R, ip
    move  ip, T
    addiu dp,dp, -CELL_SIZE
    b next
    lw T, 0(dp)

store:      # '!' ( n addr -- )  variable = n
    move t0, T
    lw t1, 0(dp)
    sb t1, 0(t0)   # l'adresse n'est pas forcément alignée donc on y va octet par octet
    srl t1,t1,8
    sb t1,1(t0)
    srl t1,t1,8
    sb t1,2(t0)
    srl t1,t1,8
    sb t1,3(t0)
    lw T, -CELL_SIZE(dp)
    b next
    addiu dp,dp,-2*CELL_SIZE

fetch:      # '@' ( addr -- n ) T= *addr
    move t0, T
    lbu t1, 0(t0)   # l'adresse n'est pas forcément alignée donc on y va octet par octet
    lbu t2, 1(t0)
    sll t2,t2,8
    or t1,t1,t2
    lbu t2, 2(t0)
    sll t2,t2,16
    or t1,t1,t2
    lbu t2, 3(t0)
    sll t2, 24
    b next
    or T,t1,t2

cstore:     # 'c!' ( char addr --  ) *addr = char
    lw t0, 0(dp)
    sb t0, 0(T)
    lw T, -CELL_SIZE(dp)
    b next
    addiu dp,dp,-2*CELL_SIZE

cfetch:     # 'c@' ( addr -- char )  T = (char)*addr
    lb t0, 0(T)
    b next
    move T, t0

rfetch:    # 'r@'  ( -- n R: n -- n )  empile le sommet de la pile des retours
    _tpush
    b next
    move T, R

ufetch:  # 'u@'  ( -- a-addr ) empile U
    _tpush
    b next
    move T, U

ustore:  # 'u!'  ( a-addr -- ) transfert T dans U
    move U, T
    lw T, 0(dp)
    b next
    addiu dp,dp,-CELL_SIZE

user: # 'user'  ( n -- a-addr) empile le pointeur de la variable
                             # utilisateur d'indice n.
      sll  T,T,2
      b next
      addu T, U, T


tor:        # '>r' ( n -- ) (R: -- n ) envoie T sur pile retour
    _rpush
    move R, T
    lw T, 0(dp)
    b next
    addiu dp, dp, -CELL_SIZE

rfrom:      # 'r>' ( -- n ) (R: n -- )  envoie R dans T
    _tpush
    move T, R
    lw  R, 0(rp)
    b next
    addiu rp,rp, -CELL_SIZE


dcnt:     # 'dcnt' ( -- n) nombre d'éléments sur la pile de données
    lui t0, RAM_SEG
    ori t0, t0,dstack
    subu t0,dp,t0
    sra t0,t0,2
    _tpush
    b next
    move T, t0

rcnt:     # 'rcnt' ( -- n ) nombre d'éléments sur la pile des retours
    _tpush
    lui t0, RAM_SEG
    ori t0,t0,rstack
    subu t0,rp,t0
    sra t0,t0,2
    b next
    move T, t0

ipfetch:        # 'ip@' ( -- ip ) empile la valeur courante du pointeur d''instructions
    _tpush
    b next
    move T, ip

ipstore:        # 'ip!' ( ip -- ) initialise le pointeur d''insturctions
    move ip, T
    lw T, 0(dp)
    b next
    addiu dp,dp, -CELL_SIZE

drop:       # 'drop' ( n -- )
    lw T, 0(dp)
    b next
    addiu dp,dp,-CELL_SIZE

twodrop:      # '2drop' ( n1 n2 -- )
    lw T, -CELL_SIZE(dp)
    b next
    addiu dp,dp,-2*CELL_SIZE

dup:        # 'dup' ( n -- n n )
    addiu dp,dp,CELL_SIZE
    b next
    sw T, 0(dp)

twodup:
    lw t0, 0(dp)
    addiu dp,dp,2*CELL_SIZE
    sw T, -CELL_SIZE(dp)
    b next
    sw t0, 0(dp)

qdup:       # ?dup ( n -- n n | n ) duplique n si n!=0
    beq T,zero, next
    nop
    addiu dp,dp,CELL_SIZE
    b next
    sw T, 0(dp)

swap:       # 'swap' (n2 n1 -- n1 n2 )
    move t0, T
    lw T, 0(dp)
    b next
    sw t0, 0(dp)

over:       # 'over' ( n2 n1 -- n2 n1 n2 )
    _tpush
    b next
    lw T, -CELL_SIZE(dp)

rot:
    move t0, T
    lw t1, 0(dp)
    lw T, -CELL_SIZE(dp)
    sw t0, 0(dp)
    b next
    sw t1, -CELL_SIZE(dp)

plus:       # '+'   ( n1 n2 -- n1+n2 )
    move t0, T
    _tpop
    b next
    addu T,T,t0

oneplus:      # '1+'  (n -- n+1 )
    b next
    addiu T, T, 1

plusstore:  # '+!'  ( n addr -- ) *addr  = *addr+n
    move w, T
    _tpop
    lw t0, 0(w)
    addu T, T, t0
    sw T, 0(w)
    lw T, 0(dp)
    b next
    addiu dp,dp,-CELL_SIZE

minus:      # '-' ( n1 n2 -- n1-n2 )
    move t0,T
    _tpop
    b next
    subu T,T,t0

oneminus:     # '1-' ( n -- n-1 )
    b next
    addiu T, T, -1

star:      # '*' ( n1 n2 -- n1*n2 ) multiplication signé, résultat simple précision.
    move t0, T
    _tpop
    b next
    mul T,t0,T

ustar:    # 'U*' ( u1 u2 -- u3 ) multiplication non signe, résultat simple précision
    move t0, T
    _tpop
    multu  t0, T
    b next
    mflo T

mstar:  # 'M*' ( n1 n2 -- d) multiplication signée avec résultat double précision.
    lw t0, 0(dp)
    mult t0,T
    mflo t0
    mfhi T
    b next
    sw t0, 0(dp)

umstar: # 'UM*' ( u1 u2 -- ud ) multiplication non signé avec résultat double précision.
    lw t0, 0(dp)
    multu t0,T
    mflo t0
    mfhi T
    b next
    sw t0, 0(dp)

slash:      # '/' ( n1 n2 -- n1/n2 )  division signée
    move t0, T
    _tpop
    div T,t0
    b next
    mflo T

uslash:     # 'U/'  (u1 u2 -- u3)  division non signée
    move t0, T
    _tpop
    divu T,t0
    b next
    mflo T

twostar:  # '2*'  ( n  -- 2*n ) multiply by 2 (shift left 1 bit)
    b next
    sll T,T,1

twoslash:  # '2/'  ( n -- n/2 )  divide by 2 ( arithmetic shift right 1 bit)
    b next
    sra T,T,1

divmod:     # '/mod'  ( n1 n2 -- n1%n2 n1/n2 )
    lw t0, 0(dp)
    div t0, T  # n1/n2
    mfhi t0    #  remainder
    sw t0, 0(dp)
    b next
    mflo T     # quotient

lshift:     # '<<' ( n1 u -- n1<<u )
    move t0, T
    _tpop
    b next
    sllv T, T, t0

rshift:     # '>>'  ( n1 u -- n1>>u )
    move t0, T
    _tpop
    b next
    srlv T, T, t0

modulo:     # 'mod' ( n1 n2 -- n1%n2 )
    move t0, T
    _tpop
    div T,t0
    b next
    mfhi T

/*
  # division non signée d'un double par un simple, retourne quotient et reste
  # les 32 bits les plus significatifs du double sont ignorés.
  #  Il s'agit dans les fait d'une division simple/simple.
umsmod:  # 'UM/MOD' ( ud1 u1 -- u2 u3 ) u2=reste, u3= quotient
        lw      t0, -CELL_SIZE(dp)
        divu    t0,T
        mfhi    t0
        mflo    T
        sw      t0, -CELL_SIZE(dp)
        b       next
        addiu   dp, dp, -CELL_SIZE
*/

# division non signée d'un double précision par un simple, retourne quotient et reste
# si le quotient > 32 bits retourne -1 -1.
umsmod: # 'UM/MOD (ud1 u1 -- u2 u3|-1 -1) u2=reste, u3=quotient
        lw      t0, -CELL_SIZE(dp)  # 32 bits poids faible
        lw      t1, 0(dp)           # 32 bits poids fort
        bgtz    t1, long_div
        nop
        divu    t0, T
        mfhi    t0              # reste
        mflo    T               # quotient
        sw      t0, -CELL_SIZE(dp)
        b       next
        addiu   dp, dp, -CELL_SIZE
1:      li      T, -1             # quotient > 32 bits
        sw      T, -CELL_SIZE(dp)
        b       next
        addiu   dp,dp,-CELL_SIZE
long_div:
        divu    t1,T
        mflo    t3      #quotient
        bgtz    t3, 1b
        mfhi    t1      #reste
        li      t2, 32  # compteur de boucle
2:      bltz    t3, 1b  # overflow
        sll     t3,t3,1
        subu    t4,t1,T
        bltz    t4,3f
        nop
        move    t1, t4
        ori     t3,t3,1
3:      addiu   t2,t2,-1  # décrémente le compteur de boucle
        beqz    t2, 5f    # division terminée si 0
        nop
        sll     t1,t1,1   # décale à gauche t1:t0
        bgez    t0, 4f
        nop
        ori     t1,t1,1
4:      b 2b
        sll     t0,t0,1
5:      move    T, t3                 # quotient
        sw      t1, -CELL_SIZE(dp)    # reste
        b       next
        addiu   dp,dp,-CELL_SIZE


  # division signée d'un double par un simple, retourne quotient et reste
  # les 32 bits les plus significatifs du double sont ignorés.
  #  Il s'agit dans les fait d'une division simple/simple.
msmod:   # 'M/MOD'  (d1 n1 - n2 n3 )  n2=reste, n3=quotient
        lw      t0, -CELL_SIZE(dp)
        div     t0, T
        mfhi    t0
        mflo    T
        sw      t0, -CELL_SIZE(dp)
        b       next
        addiu   dp,dp,-CELL_SIZE


min:        # 'min' (n1 n2 -- min )  conserve le plus petit
    move t0, T
    _tpop
    subu t1,T, t0
    bltz t1, next
    nop
    b next
    move T, t0

max:        # 'max' (n1 n2 -- max ) conserve le plus grand
    move t0, T
    _tpop
    subu t1,t0,T
    bltz t1, next
    nop
    b next
    move T, t0

abs:        # 'abs' ( n -- abs(n) )
    bgez T, next
    nop
    b next
    negu T

ltz:   # '0<'  ( n -- t|f )
    bltz T, next
    li T, -1
    b next
    li T,0

zequal: # '0=' ( n -- t|f ) vrai si n == 0 sinon faut
    beqz T, next
    li T, -1
    b next
    li T, 0

bitand:     # 'and' ( n1 n2 -- n1&n2)
    move t0, T
    _tpop
    b next
    and T,T,t0

bitor:     # 'or' ( n1 n2 -- n1|n2)
    move t0, T
    _tpop
    b next
    or T,T,t0

bitxor:     # 'xor' ( n1 n2 -- n1^n2)
    move t0, T
    _tpop
    b next
    xor T,T,t0

bitnot:     # 'not' ( n1 -- ~n1 )
    b next
    not T,T

equal:      # '='  ( n1 n2 -- t|f ) n1==n2?
    move t0, T
    _tpop
    beq t0,T, next
    li T, -1
    b next
    li T, 0

nequal:     # '<>' ( n1 n2 -- t|f ) n1<>n2?
    move t0, T
    _tpop
    bne t0,T, next
    li T, -1
    b next
    li T, 0

less:       # '<'  ( n1 n2 -- t|f ) n1<n2?
    move t0, T
    _tpop
    subu t0,T,t0
    bltz t0, next
    li T, -1
    b next
    li T, 0

greater:    # '>'  ( n1 n2 -- t|f ) n1>n2?
    move t0, T
    _tpop
    subu t0,T,t0
    bgtz t0, next
    li T, -1
    b next
    li T, 0

ltez:        # '<=' ( n1 n2 -- t|f ) n1<=n2?
    move t0, T
    _tpop
    subu t0,T,t0
    blez t0, next
    li T, -1
    b next
    li T, 0

gtez:        # '>=' ( n1 n2 -- t|f ) n1>=n2?
    move t0, T
    _tpop
    subu t0,T,t0
    bgez t0, next
    li T, -1
    b next
    li T, 0

key:        #  ( -- char ) attend réception caractère par terminal
    addiu a0, zero, STDIO
    xor a1, zero, zero
    jal _key
    _tpush
    b next
    move T, v0  # T=v0

sfr:        # ( reg -- sfr ) transforme la valeur de registre 16 bits en addresse SFR
    lui w, SFR_SEG
    andi T,T, 65535
    b next
    or  T,w,T

set:        # ( bits sfr -- ) mets à 1 les bits du SFR
    move w, T
    lw t1, 0(dp)
    addiu w, w,SFR_SET
    sw t1, 0(w)
    lw T, -CELL_SIZE(dp)
    b next
    addiu  dp, dp, -2*CELL_SIZE

clear:      # ( bits sfr -- ) met à 0 les bits du SFR
    move w, T
    lw t1, 0(dp)
    addiu w, w,SFR_CLEAR
    sw t1, 0(w)
    lw T, -CELL_SIZE(dp)
    b next
    addiu dp, dp, -2*CELL_SIZE

invert:     # ( bits sfr -- ) inverse la valeur des bits
    move w, T
    addiu w, w,SFR_INV
    lw t1, 0(dp)
    sw t1, 0(w)
    lw T, -CELL_SIZE(dp)
    b next
    addiu dp,dp,-2*CELL_SIZE

dotq:      # '."' ( -- ) ip pointe vers le texte, le premier octet est le compte.
    _tpush
    lb T, 0(ip)
    _next
1:
    beqz T, 2f
    addiu T,T,-1
    li  a0, STDIO
    lb a1, 0(ip)
    jal _putch
    _next
    b 1b
    nop
2:  lw T, 0(dp)
    b next
    addiu dp, dp, -CELL_SIZE

delay:  # 'delay' ( msec -- ) delais en millisecondes
    lui w, RAM_SEG
    ori w, w, sys_tick
    lw t0, 0(w)
    addu t0, T,t0
    lw T, 0(dp)
1:  lw t1, 0(w)
    bne t1, t0, 1b
    nop
    b next
    addiu dp, dp, -CELL_SIZE

ticks:  # 'ticks'  ( -- n ) empile le compteur sys_tick
    _tpush
    lui w, RAM_SEG
    ori w, w, sys_tick
    b next
    lw T, 0(w)

xstore:  # 'x!' ( n --  R: -- X )  X = n  initialise le compteur de boucle
    _rpush
    move R, X
    move X, T
    lw T, 0(dp)
    b next
    addiu dp,dp,-CELL_SIZE

xfetch:  # 'X@' ( -- X ) empile le compteur de boucle X
    _tpush
    b next
    move T, X

loop: #  'loop'  décrément X et boucle a l'adresse qui suit
    addiu X,X,-1   # si X > 0 sinon quitte la boucle en restaurant X à partir de R
    bne  X, zero, bra
    nop
    addiu ip,ip,1
    move X, R
    lw R, 0(rp)
    b next
    addiu  rp,rp,-CELL_SIZE

ploop: # '+loop' ( n -- ) soustrait n à X, si X>0 boucle sinon quitte la boucle
    subu X, X,T
    _tpop
    bgez X, bra
    nop
    addiu ip,ip,1
    move X, R
    lw R, 0(rp)
    b next
    addiu  rp,rp,-CELL_SIZE


pick: # ( xu...,x1,x0,u -- xu...,x1,x0,xu ) empile une copie du Uième élément de la pile
    move w, dp
    sll  T, T, 2
    subu w, dp,T
    b next
    lw T, 0(w)


badop:
    move v0,t0
exit:
    lw  s0,0(sp)
    lw  s1,4(sp)
    lw  s2,8(sp)
    lw  s3,12(sp)
    lw  s4,16(sp)
    lw  s5,20(sp)
    lw  s6,24(sp)
    lw  s7, 28(sp)
    lw  s8, 32(sp)
    lw  ra,36(sp)
    jr  ra
    addiu sp,sp,40


.end StackVM

.ent fetch_int
fetch_int:  # lit un entier little indian dans espace code VM, retourne dans v0
    lbu t0, 0(ip)
    lbu t1, 1(ip)
    sll  t1,t1, 8
    lbu t2, 2(ip)
    sll t2,t2, 16
    lb t3, 3(ip)
    sll t3,t3, 24
    or  t0,t0,t1
    or  t0,t0,t2
    or  v0,t0,t3
    j ra
    addiu ip,ip,CELL_SIZE
    

.end fetch_int



opcodes_table:
.word bye,qrx,emit,lit,call,ret,bra,qbra,exec,store,fetch,cstore
.word cfetch,rfetch,ufetch,ustore,tor,rfrom,drop,dup,swap,over,pick
.word plus,minus,star,ustar,mstar,umstar,slash,uslash,modulo,umsmod,msmod,ltz
.word zequal, bitand,bitor
.word bitxor,bitnot,key,sfr,set,clear,invert, clit, wlit, dotq, ticks, delay
.word qdup, oneplus, oneminus,plusstore, twostar, twoslash, twodrop, rot, twodup
.word  min, max, abs, lshift, rshift, divmod, qbraz, dcnt, rcnt
.word equal,nequal, less, greater,ltez,gtez, xfetch, xstore, loop, ploop,user




/*
* Copyright 2013, Jacques Deschênes
* This file is part of VPC-32.
*
*     VPC-32 is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     VPC-32 is distributed in the hope that it will be useful,
*     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with VPC-32.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * File:   vm.S
 * Author: Jacques Deschênes
 *
 * Created on 31 mai 2013
 *  machine viruelle à piles qui est utilisée comme
 *  modèle d'exécution par vpForth.
 */

#include <p32xxxx.h>
#include "opcodes.h"

.extern put_char
.extern get_key
.extern wait_key
.extern sys_tick
.extern print_int
.extern print_hex
.extern readline
.extern comm_channel

/*
RAM_SEG=0xA000
CODE_SEG=0x9D00
SFR_SEG=0xBF88
*/
/*
#define RAM_SEG  0xA000
#define CODE_SEG 0x9D00
#define SFR_SEG  0xBF88
*/

/* constantes */
FLASH_SIZE=2048
CELL_SIZE=4

RAM_SEG=0xA000
CODE_SEG=0x9D00
SFR_SEG=0xBF88


/* utilisation des regitres MIPS */
ip=s0  /* pointeur d'instruction VM */
dp=s1  /* pointeur pile de données */
rp=s2  /* pointeur pile de contrôle */
wp=s3 /* pointeur de travail */
topd=s4 /* sommet de la pile des arguments */
topr=s5 /* sommet de la pile de contrôle */

 # NOTE: les pointeurs de piles sont incrémentés avant l''empilement.
 #       et décrémenté après le dépilement.

    .macro _dpush  R   # empile le contenu de R
     addiu dp,dp, (CELL_SIZE)
     sw topd, 0(dp)
     addu topd,zero, \R
    .endm

    .macro _dpop  R=$0
    addu \R,zero, topd
    lw topd, 0(dp)
    addiu dp,dp,-(CELL_SIZE)
    .endm

    .macro _fetch_t R  # copie le contenu de topd dans R
    addu \R, topd, zero
    .endm

    .macro _store_t R  # met le contenu du registre R dans topd
    addu topd, \R, zero
    .endm



    .macro _fetch_n R, N  # copie le Nième élément de la pile dans R
    lw \R, -(\N-1)*4(dp)
    .endm

    .macro _store_n R, N  # copie R, N en N ième position de la pile
    sw \R, -(\N-1)*4(dp)
    .endm


    .macro _rpush   R   # empile R sur pile des retours
    addiu rp,rp,(CELL_SIZE)
    sw topr, 0(rp)
    addu topr, zero, \R  # move R -> topr
    .endm

    .macro _rpop  R  # dépile topr dans R
    addu \R, topr, zero
    lw topr, 0(rp)
    addiu rp,rp,-(CELL_SIZE)
    .endm

    .macro _fetch_r  R   # copie topr dans R
    addu \R, topr,zero
    .endm

    .macro _store_r R    # copie R dans topr
    addu topr, \R, zero
    .endm

    .macro _next        # next bytecode
    addiu ip,ip,1
    .endm

    .macro ld_bytecode R
    lb \R, 0(ip)
    .endm


    .macro _to_r    # >R
    _dpop wp
    _rpush wp
    .endm

    .macro _r_from  # R>
    _rpop t7
    _dpush t7
    .endm


    .macro _getch channel  # channel et un pointeur sur le device id.
    la a0, \channel
    jal get_key
    lb a0, 0(a0)
    .endm

    .macro _putch channel   # transmet le caractère qui est dans topd
    la a0, \channel
    lb a0, 0(a0)
    _dpop a1
    jal put_char
    nop
    .endm

    .macro ld_var R V  # charge le registre R avec le contenu du pointeur V
    la \R, \V
    lw \R, 0(\R)
    .endm

    DPRINT=1

    .macro _dbg_print N
    .if DPRINT
    jal dbg_print
    li a0, \N
    .endif
    .endm

    .macro _fn_enter N
    addiu sp, sp, -(\N)
    sw ra, (\N-4)(sp)
    .endm
    
    .macro _fn_exit N
    lw ra, (\N-4)(sp)
    j ra
    addiu sp,sp, \N
    .endm


STDIO=1

DSTACK_SIZE=128
RSTACK_SIZE=128

SFR_CLEAR=4
SFR_SET=8
SFR_INV=12

.data
.global base, dstack, rstack, here

base: .word 10

.section .bss
zsp: .word   dempty   # pile données position vide
zrp: .word   rempty   # pile retour position vide
here: .word  0
dempty:  .word 0    # pile vide
dstack:  .space DSTACK_SIZE
rempty:  .word 0    # pile vide
rstack: .space RSTACK_SIZE

.text
.set noreorder
.set macro
.global StackVM,  flash_code
# .global tokens
.ent StackVM


StackVM:
    _fn_enter 32
    sw  s0, 24(sp)
    sw  s1, 20(sp)
    sw  s2, 16(sp)
    sw  s3, 12(sp)
    sw  s4, 8(sp)
    sw  s5, 4(sp)
    addu ip, zero, a0  #buffer code passé en paramètre
    la dp, dempty
    la rp, rempty
    la wp, zsp
    sw dp, 0(wp)    // valeur de sp lorsque pile données vide.
    sw rp, 4(wp)    // valeur de rp lorsque pile retour vide.

vm:
    ld_bytecode t0
    _next
    addiu t1,t0, IBADOP
    bgtz  t1, badop
    sll   t0, t0, 2   
    la t1, opcodes_vectors
    addu  t1,t1,t0
    lw  t1, 0(t1)
    j   t1
    nop
end:   # 'bye' ( -- ) sortie de la machine virtuelle
    xor v0, zero,zero
    b exit
    nop

qrx: # '?key' ( -- ch -1 | 0 )
    _getch comm_channel
    _dpush v0
    beq v0, zero, vm
    nop
    addiu t0,zero,-1
    _dpush t0
    b vm
    nop

dot:  # '.'  ( n --  ) imprime entier n
    la wp, comm_channel
    lb a0,0(wp)
    _dpop a1
    jal print_int
    addiu a2,zero,1
    b vm
    nop

emit:   # 'emit' ( ch -- ) transmet charactère au terminal
    _putch comm_channel
    b vm
    nop

iemit:  # 'iemit' ( n -- ) transmet n comme 4 octet ordonné en little indian
    _fetch_t a1
    li t0, 4
    _dpush t0
    la wp, comm_channel
1:
    jal put_char
    lw a0,0(wp)
    addiu topd,-1
    beqz topd, 2f
    nop
    _fetch_n a1, 1
    srl a1,a1,8
    _store_n a1, 1
    b 1b
    nop
2:
    lw topd, -(CELL_SIZE)(dp)
    addiu dp, dp, -2*(CELL_SIZE)
    b vm
    nop

lit:    # 'lit' ( -- n) empile un entier
    jal fetch_int
    nop
    _dpush v0
    b vm
    nop

clit:       # 'clit' ( -- byte ) empile un octet signe étendu sur 32 bits
    ld_bytecode t0
    _next
    _dpush t0
    b vm
    nop

wlit:       # 'wlit' ( -- w16  ) empile un mot de 16 bits
    lbu t0, 0(ip)
    _next
    ld_bytecode t1
    _next
    sll  t1,t1, 8
    or  t0,t0,t1
    _dpush t0
    b vm
    nop

fcall:   # 'fcall' (R: -- addr_ret ) appelle sous-routine VM en mémoire flash
    lui t2, CODE_SEG
    lbu t0, 0(ip)
fcall_1:
    _next
    lbu t1, 0(ip)
    _next
    sll t1,t1,8
    or t0,t0,t1
    or t0,t0,t2
    _rpush ip
    b vm
    move ip, t0

rcall:  # 'rcall' (R: -- addr_ret ) appelle sous-routine VM en mémoire RAM
    lui t2, RAM_SEG
    lbu t0, 0(ip)
    b fcall_1
    nop

ret:        # ';' (R: addr_ret -- ) sortie de sous-routine
    la t1, zrp    # vérifie si la pile est vide, si c''est le cas exit.
    lw t1, 0(t1)
    beq t1,rp, exit # erreur accès pile retour vide
    nop
    _rpop ip
    b vm
    nop

bra:        # 'branch' ( -- ) branchement incondionnnel (adresse relative +127,-128)
    ld_bytecode t0
    _next
    b vm
    addu ip, ip, t0   # branchement relative intervale -128 à +127

qbra:       # '?bra' ( t|f -- ) branche si sommet pile est vrai (topd!=0)
    _dpop t0
    beqz t0, vm
    _next
    lb t0, -1(ip)
    b vm
    addu ip,ip,t0

qbraz:      # '?braz' ( t|f -- ) branche si sommet pile est faux (topd==0)
    _dpop t0
    bne t0, zero, vm
    _next
    lb t0, -1(ip)
    b vm
    addu ip,ip,t0

exec:       # 'exec' ( addr -- ) appelle sous-routine à partir de la pile
    _rpush ip
    _dpop ip
    b vm
    nop

store:      # '!' ( n addr -- )  variable = n
    _dpop t0
    _dpop t1
    sb t1, 0(t0)   # l''adresse n''est pas forcément alignée donc on y va octet par octet
    srl t1,t1,8
    sb t1,1(t0)
    srl t1,t1,8
    sb t1,2(t0)
    srl t1,t1,8
    sb t1,3(t0)
    b vm
    nop

fetch:      # '@' ( addr -- n ) topd = variable
    _fetch_t t0
    lbu t1, 0(t0)   # l''adresse n''est pas forcément alignée donc on y va octet par octet
    lbu t2, 1(t0)
    sll t2,t2,8
    or t1,t1,t2
    lbu t2, 2(t0)
    sll t2,t2,16
    or t1,t1,t2
    lbu t2, 3(t0)
    sll t2, 24
    or t0,t1,t2
    b vm
    _store_t t0

cstore:     # 'c!' ( char addr --  ) variable = char
    _dpop t0
    _dpop t1
    sb t1, 0(t0)
    b vm
    nop

cfetch:     # 'c@' ( addr -- char )  topd= char
    addu t0, topd, zero
    lb t0, 0(t0)
    b vm
    addu topd, t0, zero

rpfetch:    # 'rp@' ( -- rp ) topd = rp
    _dpush rp
    b vm
    nop

rpstore:    # 'rp!' ( rp -- ) rp = topd
    _dpop rp
    b vm
    nop

rfetch:     # 'r@' ( -- n ) (R: n -- n )
    _fetch_r t0
    _dpush t0
    b vm
    nop

tor:        # >r ( n -- ) (R: -- n )
    _to_r
    b vm
    nop

rfrom:      # 'r>' ( -- n ) (R: n -- )
    _r_from
    b vm
    nop

spfetch:    #  'sp@' ( -- dp )
    move t0, dp
    _dpush t0
    b vm
    nop

spstore:    # 'sp!' ( n -- )
    b vm
    move dp, topd

zerosp:     # 'zsp' ( -- n) position du bas de la pile données
    la t0, zsp
    lw t0, 0(t0)
    _dpush t0
    b vm
    nop

zerorp:     # 'zrp' ( -- n ) position du base de la pile retours
    la t0, zrp
    lw t0, 0(t0)
    _dpush t0
    b vm
    nop

ipfetch:        # 'ip@' ( -- ip ) empile la valeur courante du pointeur d''instructions
    _dpush ip
    b vm
    nop

ipstore:        # 'ip!' ( ip -- ) initialise le pointeur d''insturctions
    _dpop t0
    b vm
    move ip, t0

herefetch:  # 'here'  ( -- addr ) adresse de la variable here
    la t0, here
    lw t0, 0(t0)
    _dpush t0
    b vm
    nop

basefetch: # 'base' ( -- addr ) adresse de la variable here
    la t0, base
    lw t0, 0(t0)
    _dpush t0
    b vm
    nop

drop:       # 'drop' ( n -- )
    _dpop t0
    b vm
    nop

ddrop:      # '2drop' ( n1 n2 -- )
    lw topd, -4(dp)
    b vm
    addiu dp,dp,-8

dup:        # 'dup' ( n -- n n )
    move t0, topd
    _dpush t0
    b vm
    nop

ddup:       # '2dup' ( n1 n2 -- n1 n2 n1 n2 )
    _fetch_t t0
    _fetch_n t1, 2
    _dpush t1
    _dpush t0
    b vm
    nop

qdup:       # ?dup ( n -- n n | n ) duplique n si n!=0
    beqz topd, vm
    nop
    addiu dp, dp, (CELL_SIZE)
    b vm
    sw topd, 0(dp)

swap:       # 'swap' (n1 n2 -- n2 n1 )
    _fetch_t t0
    lw topd, 0(dp)
    b vm
    sw t0, 0(dp)

over:       # 'over' ( n1 n2 -- n1 n2 n1 )
    _fetch_n t0, 2
    addiu dp, dp, (CELL_SIZE)
    sw topd, 0(dp)
    b vm
    _store_t t0

rot:        # rot ( n2 n1 n0 -- n1 n0 n2 )
    _fetch_t t0
    _fetch_n topd, 2
    _fetch_n t1, 1
    _store_n t1, 2
    _store_n t0, 1 
    b vm
    nop

plus:       # '+'   ( n1 n2 -- n1+n2 )
    _dpop t0
    b vm
    addu topd,topd,t0

plus1:      # '+1'  (n -- n+1 )
    b vm
    addiu topd, topd, 1

plusstore:  # '+!'  ( n addr -- ) *addr  = *addr+n
    _dpop t0
    lw t1, 0(t0)
    add t1, topd, t1
    sw t1, 0(t0)
    _dpop t0
    b vm
    nop

minus:      # '-' ( n1 n2 -- n1-n2 )
    _dpop t0
    subu topd,topd,t0
    b vm
    nop

minus1:     # '-1' ( n -- n-1 )
    b vm
    addiu topd, topd, -1

multi:      # '*' ( n1 n2 -- n1*n2 )
    _dpop t0
    mul topd,topd,t0
    b vm
    nop

divi:      # '/' ( n1 n2 -- n1/n2 )
    _dpop t0
    div topd,t0
    b vm
    mflo topd

divmod:     # '/mod'  ( n1 n2 -- n1%n2 n1/n2 )
    _fetch_t t0
    lw t1, 0(dp)
    div t1, t0
    mflo topd
    mfhi t1
    sw t1, 0(dp)
    b vm
    nop

lshift:     # '<<' ( n1 u -- n1<<u )
    _dpop t0
    b vm
    sllv topd, topd, t0

rshift:     # '>>'  ( n1 u -- n1>>u )
    _dpop t0
    b vm
    srlv topd, topd, t0

modulo:     # 'mod' ( n1 n2 -- n1%n2 )
    _dpop t0
    div topd,t0
    b vm
    mfhi topd

min:        # 'min' (n1 n2 -- min )  conserve le plus petit
    _dpop t0
    subu t1,topd,t0
    bltz t1, vm
    nop
    b vm
    _store_t t0

max:        # 'max' (n1 n2 -- max ) conserve le plus grand
    _dpop t0
    subu t1,t0,topd
    bltz t1, vm
    nop
    b vm
    _store_t t0

abs:        # 'abs' ( n -- abs(n) )
    bltz topd, vm
    not topd, topd
    b vm
    nop

ltz:   # '0<'  ( n -- t|f )
    bltz topd, vm
    addiu topd,zero,-1
    b vm
    xor topd,topd,topd

bitand:     # 'and' ( n1 n2 -- n1&n2)
    _dpop t0
    b vm
    and topd,topd,t0

bitor:     # 'or' ( n1 n2 -- n1|n2)
    _dpop t0
    b vm
    or topd,topd,t0

bitxor:     # 'xor' ( n1 n2 -- n1^n2)
    _dpop t0
    b vm
    xor topd,topd,t0

bitnot:     # 'not' ( n1 -- ~n1 )
    b vm
    not topd, topd

equal:      # '='  ( n1 n2 -- t|f ) n1==n2?
    _dpop t0
    xor topd, topd, t0
    beqz topd, vm
    addiu topd, zero, -1
    b vm
    xor topd, topd ,topd

nequal:     # '<>' ( n1 n2 -- t|f ) n1<>n2?
    _dpop t0
    xor t0, topd, t0
    beqz t0, vm
    xor topd, topd, topd
    b vm
    addiu topd, zero, -1

less:       # '<'  ( n1 n2 -- t|f ) n1<n2?
    _dpop t0
    subu t0, topd, t0
    bltz t0, vm
    addiu topd, zero, -1
    b vm
    xor topd, topd, topd

greater:    # '>'  ( n1 n2 -- t|f ) n1>n2?
    _dpop t0
    subu t0, topd, t0
    bltz t0, vm
    xor topd, topd, topd
    b vm
    addiu topd, zero, -1

ltez:        # '<=' ( n1 n2 -- t|f ) n1<=n2?
    _dpop t0
    subu t0, topd, t0
    blez t0, vm
    addiu topd, zero, -1
    b vm
    xor topd, topd, topd

gtez:        # '>=' ( n1 n2 -- t|f ) n1>=n2?
    _dpop t0
    subu t0, topd, t0
    blez t0, vm
    xor topd, topd, topd
    b vm
    addiu topd, zero, -1

key:        #  ( -- char ) attend réception caractère par terminal
    _getch comm_channel
    _dpush v0
    b vm
    nop

sfr:        # ( reg -- sfr ) transforme la valeur de registre 16 bits en addresse SFR
    lui t0, SFR_SEG
    andi topd,topd, 65535
    b vm
    or  topd,topd,t0

set:        # ( bits sfr -- ) mets à 1 les bits du SFR
    _dpop t0
    addiu t0, t0,SFR_SET
    _dpop t1
    b vm
    sw t1, 0(t0)

clear:      # ( bits sfr -- ) met à 0 les bits du SFR
    _dpop t0
    addiu t0, t0, SFR_CLEAR
    _dpop t1
    b vm
    sw t1, 0(t0)

invert:     # ( bits sfr -- ) inverse la valeur des bits
    _dpop t0
    addiu t0, t0,SFR_INV
    _dpop t1
    b vm
    sw t1, 0(t0)

dotq:      # '."' ( -- ) ip pointe vers le texte, le premier octet est le compte.
    ld_bytecode t0
    _dpush t0
    _next
dotq_loop:
    beqz topd, dotq_exit
    addiu topd,topd,-1
    lui wp, RAM_SEG
    la a0, comm_channel
    lw a0, 0(a0)
    ld_bytecode a1
    jal put_char
    nop
    _next
    b dotq_loop
    nop
dotq_exit:
    _dpop t0
    b vm
    nop

readln:    # 'accept' ( buff max -- count ) lecture ligne du terminal
    la a0, comm_channel
    lw a0, 0(a0)
    _dpop a2
    _fetch_t a1
    jal readline
    nop
    b vm
    _store_t v0

type:   # 'type'  ( count buff -- ) envoie le texte dans buff au terminal
    la a0, comm_channel
    lw a0, 0(a0)
    _dpop wp
1:
    jal put_char
    lb a1,0(wp)
    addiu wp, wp, 1
    addiu topd, topd, -1
    beq topd, zero, 2f
    b 1b
2:
    _dpop t0
    b vm
    nop


delay:  # 'delay' ( msec -- ) delay en millisecondes
    la wp, sys_tick
    lw t0, 0(wp)
    addu topd, topd, t0
1:
    lw t0, 0(wp)
    subu t0,topd,t0
    bne t0,zero, 1b
    nop
    _dpop t0
    b vm
    nop

space:      # 'space' ( -- ) imprime un espace
    li t0,32
    _dpush t0
    _putch comm_channel
    b vm
    nop

spaces:     # 'spces' ( n -- ) imprime n espaces
    beqz topd, 1f
    li t0, 32
    _dpush t0
    _putch comm_channel
    addiu topd, topd, -1
    b spaces
    nop
1:
    _dpop
    b vm
    nop

crlf:
    li t0, '\r'
    _dpush t0
    _putch comm_channel
    b vm
    nop


badop:
    move v0,t0
exit:
    lw  s0,24(sp)
    lw  s1,20(sp)
    lw  s2,16(sp)
    lw  s3,12(sp)
    lw  s4,8(sp)
    lw  s5,4(sp)
    _fn_exit 32
.end StackVM

.ent fetch_int
fetch_int:  # lit un entier little indian dans espace code VM
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
    addiu ip,ip,CELL_SIZE
    j ra
    nop
.end fetch_int

.ent dbg_putch    # envoie un caratère vers UART2
dbg_putch:        # a0 contient le caractère
    la t6, U2STA
1:
    lw t5,0(t6)
    andi t5,t5, _U1STA_UTXBF_MASK
    bne t5, zero,  1b
    nop
    la t6, U2TXREG
    sw a0, 0(t6)
    j ra
    nop
.end dbg_putch

.ent dbg_print    # envoie une chaine de caractère asciiz vers UART2
                  # a0 contient le numéro du point de contrôle
dbg_print:
    _fn_enter 4
    _dpush a0
    la t7, dbg_msg
    _dpush t7
1:
    lb a0, 0(topd)
    andi a0, a0, 0x7f
    beqz a0, 2f
    nop
    jal dbg_putch
    nop
    addiu topd,topd,1
    b 1b
    nop
2:
    _dpop t7
    _dpop a0
    jal dbg_putch
    nop
    li a0, '\r'
    jal dbg_putch
    nop
    _fn_exit 4
.end dbg_print

dbg_msg:
.string "debug check point: "


opcodes_vectors:
.word end,qrx,emit,lit,fcall,ret,bra,qbra,exec,store,fetch,cstore
.word cfetch,rpfetch,rpstore,rfetch,tor,rfrom,spfetch,spstore,drop,dup,swap,over
.word plus,minus,multi,divi,modulo,ltz,bitand,bitor,bitxor,bitnot,key
.word sfr,set,clear,invert
.word clit, wlit
.word dotq, readln,rcall, type
.word delay
.word qdup, plus1, minus1,plusstore, ddrop, rot, ddup
.word  min, max, abs, lshift, rshift, divmod
.word qbraz, dot, zerosp, zerorp, herefetch,basefetch
.word equal,nequal, less, greater,ltez,gtez, iemit
.word space, spaces, crlf

flash_code: .space FLASH_SIZE

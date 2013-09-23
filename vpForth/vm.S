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

LEN_MASK=0x1F       # masque longueur nom 5 bits
BYTE_CODE=0x20      # bit indicateur mot en code machine
IMMEDIATE=0x40      # bit indicateur mot immédiat
COMPILE_ONLY=0x80   # bit indicateur mot compilateur

# offset entrée dictionnaire
CFA=0      # code byte
PFA=1      # pointeur des paramètres 2 octets
LNK=2      # lien dictionnaire 4 octets
LEN=6      # longueur du nom et bits attribus IMMEDIATE,COMPILE_ONLY, 1 octets
NAME=7     # nom , 1 à 31 octets


/* utilisation des regitres MIPS */
ip=s0  /* pointeur d'instruction VM */
dp=s1  /* pointeur pile de données */
rp=s2  /* pointeur pile de contrôle */
wp=s3 /* pointeur de travail */
topd=s4 /* sommet de la pile des arguments */
topr=s5 /* sommet de la pile de contrôle */


                        ############
                        #  MACROS  #
                        ############
        # NOTE: les pointeurs de piles sont incrémentés avant l''empilement.
        #       et décrémenté après le dépilement.
        #       le premier élément valide da la pile de donnée est à dstack+4
        #       le premier élément valide de la pile des retours est à rstack+4

    .macro _dpush  R   # empile le contenu de R
     addiu dp,dp, (CELL_SIZE)
     sw topd, 0(dp)
     addu topd,zero, \R
    .endm

    .macro _dpop  R=$0
    addu \R,zero, topd
    lw topd, 0(dp)
    addiu dp,dp, -(CELL_SIZE)
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
// .global base, dstack, rstack, here, ctib, tib
// .global sp_state, rp_state, ip_state
.global UP, sys_dict

  VAR_BASE=0
  VAR_TIB=4
  VAR_CTIB=8
  VAR_MAX=12
  VAR_HERE=16
  VAR_ZSP=20
  VAR_ZRP=24

 # variables utilisateur
UP:           #  nom   position
 # ---------------------------
.word 10        # BASE   0    (base numérique 10 par défaut)
tib:
.word tib       # TIB    4    (adresse terminal input buffer)
ctib:
.word ctib      # CTIB   8    (nombre de caractère reçus dans tib)
.word 80        # CMAX   12  ( nombre maximum de caractères acceptés dans TIB )
.word uspace    # HERE   16   (pointeur espace utilisateur libre)
zsp: .word   dempty   # pile données position vide
zrp: .word   rempty   # pile retour position vide

.section .bss
dempty:  .word 0    # pile vide
dstack:  .space DSTACK_SIZE
rempty:  .word 0    # pile vide
rstack: .space RSTACK_SIZE

.text
.set noreorder
.set macro
.global StackVM,  flash_code


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
    lbu t0, 0(ip)
    _next
    addiu t1,t0, IBADOP
    bgtz  t1, badop
    sll   t0, t0, 2   
    la t1, opcodes_vectors
    addu  t1,t1,t0
    lw  t1, 0(t1)
    j   t1
    move wp, ip

end:   # 'bye' ( -- ) sortie de la machine virtuelle
    b 1f
    xor v0, zero,zero
badop:
    move v0,t0
    addu topd,zero,zero
    addu topr,zero,zero
    la wp, dempty
    lw dp, 0(wp)
    lw rp, 4(wp)
1:
    lw  s0,24(sp)
    lw  s1,20(sp)
    lw  s2,16(sp)
    lw  s3,12(sp)
    lw  s4,8(sp)
    lw  s5,4(sp)
    _fn_exit 32

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

lit:    # 'lit' ( -- n) empile un entier
    jal fetch_int
    nop
    _dpush v0
    b vm
    nop

clit:       # 'clit' ( -- byte ) empile un octet signe étendu sur 32 bits
    lb t0, 0(ip)
    _next
    _dpush t0
    b vm
    nop

wlit:       # 'wlit' ( -- w16  ) empile un mot de 16 bits
    lbu t0, 0(ip)
    lb t1, 1(ip)
    addiu ip,ip,2
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
    lb t0, 0(ip)
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

sp0:     # 'zsp' ( -- n) position du bas de la pile données
    la wp, zsp
    lw t0, 0(wp)
    _dpush t0
    b vm
    nop

rp0:     # 'zrp' ( -- n ) position du base de la pile retours
    la wp, zrp
    lw t0, 0(wp)
    _dpush t0
    b vm
    nop

ipfetch:        # 'ip@' ( -- ip ) empile la valeur courante du pointeur d''instructions
    _dpush ip
    b vm
    nop

ipstore:        # 'ip!' ( ip -- ) initialise le pointeur d''insturctions
    _dpop ip
    b vm
    nop

drop:       # 'drop' ( n -- )
    lw topd, 0(dp)
    b vm
    addiu dp, dp, -(CELL_SIZE)

drop2:      # '2drop' ( n1 n2 -- )
    lw topd, -4(dp)
    b vm
    addiu dp,dp,-8

dup:        # 'dup' ( n -- n n )
    addiu dp, dp, CELL_SIZE
    b vm
    sw topd, 0(dp)


dup2:       # '2dup' ( n1 n2 -- n1 n2 n1 n2 )
    addiu dp, dp, 2*(CELL_SIZE)
    sw topd, -4(dp)
    lw t0, -8(dp)
    b vm
    sw t0, 0(dp)

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

minus1:     # '1-' ( n -- n-1 )
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

toggle:     # ( bits sfr -- ) inverse la valeur des bits d'un registre SFR
    _dpop t0
    addiu t0, t0,SFR_INV
    _dpop t1
    b vm
    sw t1, 0(t0)

dotq:      # '."' ( -- ) ip pointe vers le texte, le premier octet est le compte.
    lbu t0, 0(ip)
    _dpush t0
    _next
    la wp, comm_channel
dotq_loop:
    beqz topd, 1f
    addiu topd,topd,-1
    lw a0, 0(wp)
    lbu a1, 0(ip)
    jal put_char
    _next
    b dotq_loop
    nop
1:
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

crlf:       # emit un CR
    li t0, '\r'
    _dpush t0
    _putch comm_channel
    b vm
    nop

cellp:      #  ( n -- n+4)
    b vm
    addiu topd, topd, CELL_SIZE

cellm:      #  ( n -- n-4)
    b vm
    addiu topd, topd, -(CELL_SIZE)

cells:      # ( n -- n*4 )
    li t0, CELL_SIZE
    mult t0, topd
    b vm
    mflo topd

aligned:     # ( addr -- a-addr) alignement sur cellule
   li t0, ~(CELL_SIZE-1)
   and t1, t0, topd
   beq t1,topd, vm   # déjà aligné
   nop
   b vm
   addiu topd, t1, CELL_SIZE


douser:     # 'user' ( vname -- adr )  empile la valeur d''une variable système
    la t0, UP
    addu t0,t0,topd
    b vm
    lw  topd, 0(t0)



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

.section .rodata

dbg_msg:
.string "debug check point: "

# ************************************
# ******* mots de haut niveau ********
# ************************************
here:       # 'here' ( -- adr)  retourne l''adresse position libre dans la mémoire utilisateur
.byte ICLIT, VAR_HERE, ISYSF, IRET

pad:      # 'pad'  ( -- adr)  addresse du scratch pad
.byte   ICLIT, VAR_HERE, ISYSF, ICLIT, 80, IADD, IRET

dots:       # '.s'  ( -- )  imprime le contenu de la pile de données
.byte ISPACE,ICLIT,'(',IEMIT,ISPFETCH,IZSP,ISUB
.byte ICLIT,CELL_SIZE,IDIV,IDUP,IDOT,ICLIT,')',IEMIT,ISPACE
.byte ITOR,IZSP,ICELLP,ICELLP,IRFROM,IDUP,IQBRAZ,8,I1MINUS
.byte ITOR,IDUP,IFETCH,IDOT,ISPACE,IBRA, -13,IDDROP,IRET

dotr:       # '.r'  ( -- ) imprime le contenu de la pile des retours
.byte ISPACE,ICLIT,'(',IEMIT,IRPFETCH,IZRP,ISUB
.byte ICLIT,CELL_SIZE,IDIV,IDUP,IDOT,ICLIT,')',IEMIT,ISPACE
.byte ITOR,IZRP,ICELLP,ICELLP,IRFROM,IDUP,IQBRAZ,8,I1MINUS
.byte ITOR,IDUP,IFETCH,IDOT,ISPACE,IBRA, -13,IDDROP,IRET


# **********************************
# adresse des mots en code machine *
# **********************************
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
.word qbraz, dot, sp0, rp0
.word equal,nequal, less, greater,ltez,gtez
.word space, spaces, crlf, cellp, cellm, user

# ************** dictionnaire système *********************
sys_dict:
# primitives en code machines
bc0:
.byte IBYE              # cfa
.short 0                # pfa
.word bc1               # lnk
.byte 3                 # name_len
.ascii "BYE"            # name
bc1:
 .byte IQRX
 .short 0
 .word bc2
 .byte 4
 .ascii "?QRX"
bc2:
  .byte IEMIT
  .short 0
  .word bc3
  .byte 4
  .ascii "EMIT"
bc3:
  .byte ILIT
  .short 0
  .word bc4
  .byte 3
  .ascii "LIT"
bc4
  .byte IFCALL
  .short 0
  .word bc5
  .byte 5
  .ascii "FCALL"
bc5:
  .byte IRET
  .short 0
  .word bc6
  .byte 4
  .ascii "EXIT"
bc6:
  .byte IBRA
  .short 0
  .word bc7
  .byte 6
  .ascii "BRANCH"
bc7:
  .byte IQBRA
  .short 0
  .word bc8
  .byte 7
  .ascii "?BRANCH"
bc8:
  .byte IEXEC
  .short 0
  .word bc9
  .byte  7
  .ascii "EXECUTE"
bc9:
  .byte ISTORE
  .short 0
  .word bc10
  .byte 1
  .ascii "!"
bc10:
  .byte IFETCH
  .short 0
  .word bc11
  .byte 1
  .ascii "@"
bc11:
  .byte ICSTORE
  .short 0
  .word bc12
  .byte 2
  .ascii "C!"
bc12:
  .byte ICFETCH
  .short 0
  .word bc13
  .byte 2
  .ascii "C@"
bc13:
  .byte IRPFETCH
  .short 0
  .word bc14
  .byte 3
  .ascii "RP@"
bc14:
  .byte IRPSTORE
  .short 0
  .word bc15
  .byte 3
  .ascii "RP!"
bc15:
  .byte IRFETCH
  .short 0
  .word bc16
  .byte 2
  .ascii "R@"
bc16:
  .byte ITOR
  .short 0
  .word bc17
  .byte 2
  .ascii ">R"
bc17:
  .byte IRFROM
  .short 0
  .word bc18
  .byte 2
  .ascii "R>"b
bc18:
  .byte ISPFETCH
  .short 0
  .word bc19
  .byte 3
  .ascii "SP@"
bc19:
  .byte ISPSTORE
  .short 0
  .word bc20
  .byte 3
  .ascii "SP!"
bc20:
  .byte IDROP
  .short 0
  .word bc21
  .byte 4
  .ascii "DROP"
bc21:
  .byte IDUP
  .short 0
  .word bc22
  .byte 3
  .ascii "DUP"
bc22:
  .byte ISWAP
  .short 0
  .word bc23
  .byte 4
  .ascii "SWAP"
 bc23:
  .byte IOVER
  .short 0
  .word bc24
  .byte 4
  .ascii "OVER"
bc24:
  .byte  IADD
  .short 0
  .word  bc25
  .byte  1
  .ascii "+"
bc25:
  .byte  ISUB
  .short 0
  .word  bc26
  .byte  1
  .ascii "-"
bc26:
  .byte  IMUL
  .short 0
  .word  bc27
  .byte  1
  .ascii "*"
bc27:
  .byte  IDIV
  .short 0
  .word  bc28
  .byte  1
  .ascii "/"
bc28:
  .byte  IMOD
  .short 0
  .word  bc29
  .byte  3
  .ascii "MOD"
bc29:
  .byte  ILTZ
  .short 0
  .word  bc30
  .byte  2
  .ascii "0<"
bc30:
  .byte   IAND
  .short  0
  .word   bc31
  .byte   3
  .ascii  "AND"
bc31:
  .byte  IOR
  .short 0
  .word  bc32
  .byte  2
  .ascii "OR"
bc32:
  .byte   IXOR
  .short  0
  .word   bc33
  .byte   3
  .ascii  "XOR"
bc33:
  .byte   INOT
  .short  0
  .word   bc34
  .byte   3
  .ascii  "NOT"
bc34:
  .byte   IKEY
  .short   0
  .word   bc35
  .byte    3
  .ascii  "KEY"
bc35:
  .byte   ISFR
  .short  0
  .word   bc36
  .byte   3
  .ascii  "SFR"
bc36:
  .byte   ISET
  .short  0
  .word   bc37
  .byte   3
  .ascii  "SET"
bc37:
  .byte ICLR
  .short  0
  .word  bc38
  .byte  5
  .ascii "CLEAR"
bc38:
  .byte   ITGL
  .short  0
  .word   bc39
  .byte   6
  .ascii  "TOGGLE"
bc39:
  .byte   ICLIT
  .short   0
  .word   bc40
  .byte   4
  .ascii  "CLIT"
bc40:
  .byte   IWLIT
  .short  0
  .word   bc41
  .byte   4
  .ascii  "WLIT"
bc41:
  .byte   IDOTQ
  .short  0
  .word   bc42
  .byte   2
  .ascii  ".\""
bc42:
  .byte   IACCEPT
  .short  0
  .word   bc43
  .byte   8
  .ascii  "ACCEPT"
bc43:
  .byte   IRCALL
  .short   0
  .word    bc44
  .byte   5
  .ascii  "RCALL"
bc44:
  .byte   ITYPE
  .short  0
  .word   bc45
  .byte   4
  .ascii  "TYPE"
bc45:
  .byte   IDELAY
  .short  0
  .word   bc46
  .byte   4
  .ascii  "DELAY"
bc46:
  .byte   IQDUP
  .short  0
  .word   bc47
  .byte   4
  .ascii  "?DUP"
bc47:
  .byte   I1PLUS
  .short  0
  .word   bc48
  .byte   2
  .ascii  "1+"
bc48:
  .byte  I1MINUS
  .short 0
  .word  bc49
  .byte  2
  .ascii "1-"
bc49:
  .byte   IPLUSSTR
  .short  0
  .word   bc50
  .byte   2
  .ascii  "+!"
bc50:
  .byte   I2DROP
  .short  0
  .word   bc51
  .byte   5
  .ascii  "2DROP"
bc51:
  .byte  IROT
  .short 0
  .word  bc52
  .byte 3
  .ascii "ROT"
bc52:
  .byte  I2DUP
  .short  0
  .word   bc53
  .byte   4
  .ascii  "2DUP"
bc53:
  .byte   IMIN
  .short  0
  .word   bc54
  .byte   3
  .ascii  "MIN"
bc54:
  .byte  IMAX
  .short  0
  .word   bc55
  .byte   3
  .ascii  "MAX"
bc55:
  .byte   IABS
  .short  0
  .word   bc56
  .byte   3
  .ascii  "ABS"
bc56:
  .byte   ILSHIFT
  .short  0
  .word   bc57
  .byte   2
  .ascii  "<<"
bc57:
  .byte  IRSHIFT
  .short  0
  .word   bc58
  .byte   2
  .ascii  ">>"
bc58:
  .byte   IDIVMOD
  .short  0
  .word   bc59
  .byte   4
  .ascii  "/MOD"
bc59:
  .byte  IQBRAZ
  .short 0
  .word  bc60
  .byte  5
  .ascii "?BRANCH0"
bc60:
  .byte   IDOT
  .short  0
  .word   bc61
  .byte   1
  .ascii  "."
bc61:
  .byte   ISP0
  .short  0
  .word   bc62
  .byte   3
  .ascii  "SP0"
bc62:
  .byte  IRP0
  .short  0
  .word  bc63
  .byte  3
  .ascii "RP0"
bc63:
  .byte   IEQUAL
  .short   0
  .word    bc64
  .byte    1
  .ascii   "="
bc64:
  .byte   INEQUAL
  .short  0
  .word   bc65
  .byte   2
  .ascii  "<>"
bc65:
  .byte   ILESS
  .short  0
  .word   bc66
  .byte   1
  .ascii  "<"
bc66:
  .byte   IGREATER
  .short  0
  .word   bc67
  .byte   1
  .ascii  ">"
bc67:
  .byte   ILTE
  .short  0
  .word   bc68
  .byte   2
  .ascii  "<="
bc68:
  .byte  IGTE
  .short 0
  .word  bc69
  .byte  2
  .ascii ">="
bc69:
  .byte   ISPACE
  .short  0
  .word   bc70
  .byte   4
  .ascii  "SPACE"
bc70:
  .byte   ISPACES
  .short  0
  .word   bc71
  .byte   5
  .ascii  "SPACES"
bc71:
  .byte   ICRLF
  .short  0
  .word   bc72
  .byte   2
  .ascii  "CR"
bc72:
  .byte   ICELLP
  .short  0
  .word   bc73
  .byte   5
  .ascii  "CELL+"
bc73:
  .byte  ICELLM
  .short  0
  .word   bc74
  .byte   5
  .ascii  "CELL-"
bc74:
  .byte  ICELLS
  .short  0
  .word  bc75
  .byte  5
  .ascii "CELLS"
bc75:
  .byte IALIGNED
  .short 0
  .word bc76
  .byte 7
  .ascii "ALIGNED"
bc76:
  .byte   IUSER
  .short  0
  .word   f0
  .byte   4
  .ascii  "USER"

# mots forth de haut niveau
f0:
 .byte IFCALL
 .short (here & 0xffff)
 .word f1
 .short 4
 .ascii "HERE"
f1:
 .byte IFCALL
 .short (pad & 0xffff)
 .word f2
 .short 3
 .ascii "PAD"
f2:
flast:
 .byte 0
 .short 0
 .word 0
 .short 0
 .ascii



# flash_code: .space FLASH_SIZE

.bss
uspace:  .space FREE_RAM   # user space

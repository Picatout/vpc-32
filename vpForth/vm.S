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

STDIO=1

DSTACK_SIZE=64
RSTACK_SIZE=64

SFR_CLEAR=4
SFR_SET=8
SFR_INV=12

.data
.global base, dstack, rstack, here

base: .word 10

.section .bss
dstack: .space DSTACK_SIZE
rstack: .space RSTACK_SIZE
zsp: .word   0   # pile données position vide
zrp: .word   0   # pile retour position vide
here: .word  0

.text
.set noreorder
.global StackVM,  flash_code
# .global tokens
.ent StackVM


StackVM:
    addiu sp, sp, -16
    sw  ra, 12(sp)
    sw  s0, 8(sp)
    sw  s1, 4(sp)
    sw  s2, 0(sp)
    addu ip, zero, a0
    lui dp, RAM_SEG
    ori dp, dp, dstack
    _dpop   # pile vide pointeur sous le premier élément
    lui rp, RAM_SEG
    ori rp, rp, rstack
    _rpop
    lui ptr, RAM_SEG
    ori ptr,ptr, zsp
    sw dp, 0(ptr)    // valeur de sp lorsque pile données vide.
    sw rp, 4(ptr)    // valeur de rp lorsque pile retour vide.
vm:
    lb   t0, _bytecode
    _next
    addiu t1,t0, IBADOP
    bgtz  t1, badop
    sll   t0, t0, 2   
    lui   t1, CODE_SEG
    addiu  t0,t0,opcodes_vectors
    or    t1,t1,t0
    lw  t1, 0(t1)
    j     t1
    nop
end:   # 'bye' ( -- ) sortie de la machine virtuelle
    xor v0, zero,zero
    b exit
    nop
qrx: # '?key' ( -- ch -1 | 0 )
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0,0(ptr)
    jal get_key
    nop
    _dpush
    bltz v0, qrx_exit
    addiu t0,v0,1
    sw v0,_tos
    addiu t0,zero,-1
    _dpush
qrx_exit:
    b vm
    sw t0,_tos
dot:  # '.'  ( n --  ) imprime entier n
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0,0(ptr)
    lw a1, _tos
    _dpop
    jal print_int
    ori a2,zero,1
    b vm
    nop
emit:   # 'emit' ( ch -- ) transmet charactère au terminal
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0,0(ptr)
    lw  a1,_tos
    _dpop
    jal put_char
    andi a1,a1, 255
    b vm
    nop
iemit:  # 'iemit' ( n -- ) transmet n comme 4 octet ordonné en little indian
    lw t0, _tos
    _dpop
    ori t1,zero,4
1:
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0,0(ptr)
    jal put_char
    andi a1, t0, 255
    addiu t1,-1
    beqz t1, vm
    srl t0,t0,8
    b 1b
    nop
lit:    # 'lit' ( -- n) empile un entier
    jal fetch_int
    _dpush
    b vm
    sw  v0, _tos
clit:       # 'clit' ( -- byte ) empile un octet signe étendu sur 32 bits
    lb t0,_bytecode
    _next
    _dpush
    b vm
    sw t0, _tos
wlit:       # 'wlit' ( -- w16  ) empile un mot de 16 bits
    lbu t0,_bytecode
    _next
    lb t1, _bytecode
    _next
    sll  t1,t1, 8
    or  t0,t0,t1
    _dpush
    b vm
    sw t0, _tos
fcall:   # 'fcall' (R: -- addr_ret ) appelle sous-routine VM en mémoire flash
    lui t2, CODE_SEG
    lbu t0, _bytecode
fcall_1:
    _next
    lbu t1, _bytecode
    _next
    sll t1,t1,8
    or t0,t0,t1
    or t0,t0,t2
    _rpush
    sw ip,0(rp)
    b vm
    move ip, t0
rcall:  # 'rcall' (R: -- addr_ret ) appelle sous-routine VM en mémoire RAM
    lui t2, RAM_SEG
    b fcall_1
    lbu t0, _bytecode
ret:        # ';' (R: addr_ret -- ) sortie de sous-routine
    lui t1, RAM_SEG # vérifie si la pile est vide, si c''est le cas exit.
    ori t1,t1,zrp
    lw t1, 0(t1)
    lw t0, 0(rp)
    move ip, t0
    bne t1,t0, vm
    _rpop
    b exit
    nop
bra:        # 'branch' ( -- ) branchement incondionnnel (adresse relative +127,-128)
    lb t0, _bytecode
    _next
    b vm
    addu ip, ip, t0   # branchement relative intervale -128 à +127
qbra:       # '?bra' ( t|f -- ) branche si sommet pile est vrai (tos!=0)
    lw t0, _tos
    _dpop
    beqz t0, vm
    _next
    lb t0, -1(ip)
    b vm
    addu ip,ip,t0
qbraz:      # '?braz' ( t|f -- ) branche si somme pile est faux (tos==0)
    lw t0, _tos
    _dpop
    bne t0, zero, vm
    _next
    lb t0, -1(ip)
    b vm
    addu ip,ip,t0
exec:       # 'exec' ( addr -- ) appelle sous-routine à partir de la pile
    _rpush
    sw  ip, 0(rp)
    lw  ip, _tos
    b vm
    _dpop
store:      # '!' ( n addr -- )  variable = n
    lw t0, _tos
    lw t1, -CELL_SIZE(dp)
    sb t1, 0(t0)   # l'adresse n'est pas forcément alignée donc on y va octet par octet
    srl t1,t1,8
    sb t1,1(t0)
    srl t1,t1,8
    sb t1,2(t0)
    srl t1,t1,8
    sb t1,3(t0)
    b vm
    addiu dp,dp,-8
fetch:      # '@' ( addr -- n ) tos = variable
    lw t0, _tos
    lbu t1, 0(t0)   # l'adresse n'est pas forcément alignée donc on y va octet par octet
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
    sw t0, _tos
cstore:     # 'c!' ( char addr --  ) variable = char
    lw t0, _tos
    lw t1, -CELL_SIZE(dp)
    sb t1, 0(t0)
    b vm
    addiu dp,dp,-8
cfetch:     # 'c@' ( addr -- char )  tos= char
    lw t0, _tos
    lb t0, 0(t0)
    b vm
    sw t0, _tos
rpfetch:    # 'rp@' ( -- rp ) tos = rp
    _dpush
    b vm
    sw  rp, _tos
rpstore:    # 'rp!' ( rp -- ) rp = tos
    lw rp, _tos
    b vm
    _dpop
rfetch:     # 'r@' ( -- n ) (R: n -- n )
    _dpush
    lw t0, 0(rp)
    b vm
    sw t0, _tos
tor:        # '>r' ( n -- ) (R: -- n )
    lw t0, _tos
    _rpush
    sw t0, 0(rp)
    b vm
    _dpop
rfrom:      # 'r>' ( -- n ) (R: n -- )
    _dpush
    lw t0, 0(rp)
    sw t0, _tos
    b vm
    _rpop
spfetch:    #  'sp@' ( -- sp )
    move t0, dp
    _dpush
    b vm
    sw t0, _tos
spstore:    # 'sp!' ( n -- )
    b vm
    lw dp, _tos
zerosp:     # 'zsp' ( -- n) position du bas de la pile données
    lui ptr, RAM_SEG
    ori ptr,ptr,zsp
    lw t0, 0(ptr)
    _dpush
    b vm
    sw t0, _tos
zerorp:     # 'zrp' ( -- n ) position du base de la pile retours
    lui ptr, RAM_SEG
    ori ptr,ptr,zrp
    lw t0, 0(ptr)
    _dpush
    b vm
    sw t0, _tos
ipfetch:        # 'ip@' ( -- ip ) empile la valeur courante du pointeur d''instructions
    _dpush
    b vm
    sw ip, _tos
ipstore:        # 'ip!' ( ip -- ) initialise le pointeur d''insturctions
    lw t0, _tos
    _dpop
    b vm
    move ip, t0
herefetch:  # 'here'  ( -- addr ) adresse de la variable here
    lui ptr, RAM_SEG
    ori ptr, ptr, here
    lw t0, 0(ptr)
    _dpush
    b vm
    sw t0, _tos
basefetch: # 'base' ( -- addr ) adresse de la variable here
    lui ptr, RAM_SEG
    ori ptr, ptr, base
    lw t0, 0(ptr)
    _dpush
    b vm
    sw t0, _tos
drop:       # 'drop' ( n -- )
    b vm
    _dpop
ddrop:      # '2drop' ( n1 n2 -- )
    b vm
    addiu dp,dp,-8
dup:        # 'dup' ( n -- n n )
    lw t0, _tos
    _dpush
    b vm
    sw t0, _tos
ddup:
    lw t0, _tos
    lw t1, -CELL_SIZE(dp)
    addiu dp,dp,8
    sw t1, -CELL_SIZE(dp)
    b vm
    sw t0, _tos
qdup:       # ?dup ( n -- n n | n ) duplique n si n!=0
    lw t0, _tos
    beqz t0, vm
    nop
    _dpush
    b vm
    sw t0, _tos
swap:       # 'swap' (n1 n2 -- n2 n1 )
    lw t0, _tos
    lw t1, -CELL_SIZE(dp)
    sw t0, -CELL_SIZE(dp)
    b vm
    sw t1, _tos
over:       # 'over' ( n1 n2 -- n1 n2 n1 )
    lw t0, -CELL_SIZE(dp)
    _dpush
    b vm
    sw t0,_tos
rot:
    lw t0, _tos
    lw t1, -CELL_SIZE(dp)
    lw t2, -8(dp)
    sw t0, -CELL_SIZE(dp)
    sw t1, -8(dp)
    b vm
    sw t2, _tos
plus:       # '+'   ( n1 n2 -- n1+n2 )
    lw t0, _tos
    _dpop
    lw t1, _tos
    addu t0,t1,t0
    b vm
    sw t0, _tos
plus1:      # '+1'  (n -- n+1 )
    lw t0, _tos
    addiu t0, t0, 1
    b vm
    sw t0, _tos
plusstore:  # '+!'  ( n addr -- ) *addr  = *addr+n
    lw t1, _tos
    lw t0, 0(t1)
    lw t2, -CELL_SIZE(dp)
    add t0, t0, t2
    sw t0, 0(t1)
    b vm
    addiu dp,dp,-8 # 2drop
minus:      # '-' ( n1 n2 -- n1-n2 )
    lw t0, _tos
    _dpop
    lw t1, _tos
    subu t0,t1,t0
    b vm
    sw t0, _tos
minus1:     # '-1' ( n -- n-1 )
    lw t0, _tos
    addiu t0, t0, -1
    b vm
    sw t0, _tos
multi:      # '*' ( n1 n2 -- n1*n2 )
    lw t0, _tos
    _dpop
    lw t1, _tos
    mul t0,t1,t0
    b vm
    sw t0, _tos
divi:      # '/' ( n1 n2 -- n1/n2 )
    lw t0, _tos
    _dpop
    lw t1, _tos
    div t1,t0
    mflo t0
    b vm
    sw t0, _tos
divmod:     # '/mod'  ( n1 n2 -- n1%n2 n1/n2 )
    lw t0, _tos
    lw t1, -CELL_SIZE(dp)
    div t1, t0
    mfhi t1
    mflo t0
    sw t1, -CELL_SIZE(dp)
    b vm
    sw t0, _tos
lshift:     # '<<' ( n1 u -- n1<<u )
    lw t0, _tos
    _dpop
    lw t1, _tos
    sllv t0, t1, t0
    b vm
    sw t0, _tos
rshift:     # '>>'  ( n1 u -- n1>>u )
    lw t0, _tos
    _dpop
    lw t1, _tos
    srlv t0, t1, t0
    b vm
    sw t0, _tos
modulo:     # 'mod' ( n1 n2 -- n1%n2 )
    lw t0, _tos
    _dpop
    lw t1, _tos
    div t1,t0
    mfhi t0
    b vm
    sw t0, _tos
min:        # 'min' (n1 n2 -- min )  conserve le plus petit
    lw t0, _tos
    _dpop
    lw t1, _tos
    subu t1,t1,t0
    bltz t1, vm
    nop
    b vm
    sw t0, _tos
max:        # 'max' (n1 n2 -- max ) conserve le plus grand
    lw t0, _tos
    _dpop
    lw t1, _tos
    subu t1,t0,t1
    bltz t1, vm
    nop
    b vm
    sw t0, _tos
abs:        # 'abs' ( n -- abs(n) )
    lw t0, _tos
    bgez t0, vm
    not t0
    b vm
    sw t0, _tos
ltz:   # '0<'  ( n -- t|f )
    lw t0, _tos
    addiu t1, zero,-1
    bltz t0, vm
    sw t1, _tos
    xor t0,t0,t0
    b vm
    sw t0, _tos
bitand:     # 'and' ( n1 n2 -- n1&n2)
    lw t0, _tos
    _dpop
    lw t1, _tos
    and t0,t1,t0
    b vm
    sw t0, _tos
bitor:     # 'or' ( n1 n2 -- n1|n2)
    lw t0, _tos
    _dpop
    lw t1, _tos
    or t0,t1,t0
    b vm
    sw t0, _tos
bitxor:     # 'xor' ( n1 n2 -- n1^n2)
    lw t0, _tos
    _dpop
    lw t1, _tos
    xor t0,t1,t0
    b vm
    sw t0, _tos
bitnot:     # 'not' ( n1 -- ~n1 )
    lw t0, _tos
    not t0,t0
    b vm
    sw t0, _tos
equal:      # '='  ( n1 n2 -- t|f ) n1==n2?
    lw t2, _tos
    _dpop
    lw t1, _tos
    addiu t0,zero,-1
    beql t1,t2, vm   # branch on equal likely n''exécutre l''instruction dans
    sw t0, _tos      # le delay slot seulement s''il y a branchement, pratique!!
    addu t0,zero,zero
    b vm
    sw t0, _tos
nequal:     # '<>' ( n1 n2 -- t|f ) n1<>n2?
    lw t2, _tos
    _dpop
    lw t1, _tos
    addiu t0,zero,-1
    bnel t1,t2, vm
    sw t0, _tos
    addu t0,zero,zero
    b vm
    sw t0, _tos
less:       # '<'  ( n1 n2 -- t|f ) n1<n2?
    lw t2, _tos
    _dpop
    lw t1, _tos
    addiu t0,zero,-1
    subu t1,t1,t2
    bltzl t1, vm
    sw t0, _tos
    addu t0,zero,zero
    b vm
    sw t0, _tos
greater:    # '>'  ( n1 n2 -- t|f ) n1>n2?
    lw t2, _tos
    _dpop
    lw t1, _tos
    addiu t0, zero,-1
    subu t1,t1,t2
    bgtzl t1, vm
    sw t0, _tos
    addu t0,zero,zero
    b vm
    sw t0, _tos
ltez:        # '<=' ( n1 n2 -- t|f ) n1<=n2?
    lw t2, _tos
    _dpop
    lw t1, _tos
    addiu t0, zero,-1
    subu t1,t1,t2
    blezl t1, vm
    sw t0, _tos
    addu t0,zero,zero
    b vm
    sw t0, _tos
gtez:        # '>=' ( n1 n2 -- t|f ) n1>=n2?
    lw t2, _tos
    _dpop
    lw t1, _tos
    addiu t0, zero,-1
    subu t1,t1,t2
    bgezl t1, vm
    sw t0, _tos
    addu t0,zero,zero
    b vm
    sw t0, _tos
key:        #  ( -- char ) attend réception caractère par terminal
    addiu a0, zero, STDIO
    xor a1, zero, zero
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0, 0(ptr)
    jal get_key
    _dpush
    b vm
    sw v0, _tos
sfr:        # ( reg -- sfr ) transforme la valeur de registre 16 bits en addresse SFR
    lui t0, SFR_SEG
    lw t1, _tos
    andi t1,t1, 65535
    or  t0,t0,t1
    b vm
    sw t0, _tos
set:        # ( bits sfr -- ) mets à 1 les bits du SFR
    lw t0, _tos
    _dpop
    addiu t0, t0,SFR_SET
    lw t1, _tos
    _dpop
    b vm
    sw t1, 0(t0)
clear:      # ( bits sfr -- ) met à 0 les bits du SFR
    lw t0, _tos
    _dpop
    addiu t0, t0,SFR_CLEAR
    lw t1, _tos
    _dpop
    b vm
    sw t1, 0(t0)
invert:     # ( bits sfr -- ) inverse la valeur des bits
    lw t0, _tos
    _dpop
    addiu t0, t0,SFR_INV
    lw t1, _tos
    _dpop
    b vm
    sw t1, 0(t0)
dotq:      # '."' ( -- ) ip pointe vers le texte, le premier octet est le compte.
    lb t0, _bytecode
    _next
    _dpush
    sw t0, _tos
dotq_loop:
    lw t0, _tos
    beqz t0, dotq_exit
    addiu t0,t0,-1
    sw t0, _tos
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0, 0(ptr)
    lb a1, _bytecode
    jal put_char
    _next
    b dotq_loop
    nop
dotq_exit:
    b vm
    _dpop
readln:    # 'accept' ( buff max -- count ) lecture ligne du terminal
    ori a0, zero, STDIO
    lw  a2, _tos
    _dpop
    lw  a1, _tos
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0, 0(ptr)
    jal readline
    nop
    b vm
    sw v0, _tos
type:   # 'type'  ( buff count -- ) envoie le texte dans buff au terminal
    ori a0,zero, STDIO
    lw a2, _tos
    _dpop
    lw a1, _tos
    lui ptr, RAM_SEG
    ori ptr, ptr, comm_channel
    lw a0, 0(ptr)
    jal print
    _dpop
    b vm
    nop
delay:  # 'delay' ( msec -- ) delay en millisecondes
    lui ptr, RAM_SEG
    ori ptr, ptr, sys_tick
    lw t0, 0(ptr)
    lw t1, _tos
    _dpop
    bgtz t1, 1f
    nop
    andi t1,t1,0xFF
1:
    addu t0,t1,t0
dly_loop:
    lw t1, 0(ptr)
    subu t1, t1,t0
    bltz t1, dly_loop
    nop
    b vm
    nop
badop:
    move v0,t0
exit:
    lw  s0,8(sp)
    lw  s1,4(sp)
    lw  s2,0(sp)
    lw  ra,12(sp)
    jr  ra
    addiu sp,sp,16
.end StackVM

.ent fetch_int
fetch_int:  # lit un entier little indian dans espace code VM
    lbu t0, _bytecode
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

flash_code: .space FLASH_SIZE

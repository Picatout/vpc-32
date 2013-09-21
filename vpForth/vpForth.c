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
 * File:   vpForth.c
 * Author: Jacques Deschênes
 *
 * Created on 6 juin 2013
 */

#include "../hardware/HardwareProfile.h"
#include "../hardware/serial_comm.h"
#include "../console.h"


#include "vpForth.h"
#include "opcodes.h"

#define TIB_SIZE 80
#define PAD_SIZE 80
#define USER_NAMES_SIZE 100
#define XT_NULL 0
#define COMPILE 1
#define IMMEDIATE 0
#define RESERVED 0
#define CELL_SIZE 4
#define HASH_BASE 68


// data types
typedef unsigned char BYTE ;
typedef unsigned short WORD;
typedef unsigned int name_hash;
typedef char *code_ptr;
typedef int(*compfct)();

typedef struct{
    name_hash name;
    code_ptr cfa;
} dict_entry_t;


// variables
unsigned char ram_code[RAM_SPACE];
char state=IMMEDIATE;
int cstack[6]; // pile utilisé par le compilateur (control flow stack)
char cp=-1; //pointeur pour la pile cstack
char error=0;
code_ptr imm_code, mark;

const name_hash vm_tokens[]= { // vm tokens hash
156436, // BYE
9629672, // ?KEY
11525779, // EMIT
201603, // LIT
801952535, // FCALL
229075, // RET
155956, // BRA
9588916, // ?BRA
11576354, // EXEC
0, // !
31, // @
2312, // C!
2343, // C@
229803, // RP@
229772, // RP!
3363, // R@
2021, // >R
3361, // R>
234427, // SP@
234396, // SP!
11234871, // DROP
165423, // DUP
15973519, // SWAP
14711441, // OVER
10, // +
12, // -
9, // *
14, // /
206619, // MOD
1047, // 0<
151063, // AND
3177, // OR
257497, // XOR
211259, // NOT
196712, // KEY
233765, // SFR
233699, // SET
740656049, // CLEAR
188073, // INV
10892291, // CLIT
17180931, // WLIT
885, // ."
1059157367, // READL
1058529047, // RCALL
16298208, // TYPE
759868776, // DELAY
9598383, // ?DUP
1098, // 1+
1100, // 1-
680, // +!
374718263, // 2drop
229755, // rot
5510767, // 2dup
206221, // min
205687, // max
150262, // abs
1863, // <<
2001, // >>
4608667, // /mod
652046345, // ?braz
13, // .
267015, // zsp
266947, // zrp
12432680, // here
10527660, // base
28, // =
1865, // <>
27, // <
29, // >
1864, // <=
2000, // >=
866780819, // iemit
1083997420, // ispace
1084006818, // ispces
2361, // cr
738488102, // cell+
738488104, // cell-
1059157367, // readl
15983975, // sys@
15983944, // sys!
} ;


// mots systèmes enrigistrés en mémoire flash

const char dots[]={ISPACE,ICLIT,'(',IEMIT,ISPFETCH,IZSP,ISUB,
                   ICLIT,CELL_SIZE,IDIV,IDUP,IDOT,ICLIT,')',IEMIT,ISPACE,
                   ITOR,IZSP,ICELLP,ICELLP,IRFROM,IDUP,IQBRAZ,8,I1MINUS,
                   ITOR,IDUP,IFETCH,IDOT,ISPACE,IBRA, -13,IDDROP,IRET};

const char dotr[]={ISPACE,ICLIT,'(',IEMIT,IRPFETCH,IZRP,ISUB,
                   ICLIT,CELL_SIZE,IDIV,IDUP,IDOT,ICLIT,')',IEMIT,ISPACE,
                   ITOR,IZRP,ICELLP,ICELLP,IRFROM,IDUP,IQBRAZ,8,I1MINUS,
                   ITOR,IDUP,IFETCH,IDOT,ISPACE,IBRA, -13,IDDROP,IRET};

//void print_integer(int n);
void compile_if();
void compile_then();
void compile_else();
void compile_repeat();
void compile_again();
void compile_while();
void compile_do();
void compile_loop();
void compile_ploop();
void compile_begin();
void compile_until();
void compile_var();
void compile_const();
int compile_token(int);
void column();
void semi_column();


dict_entry_t system_dict[]={
    {-25,(code_ptr)column}, // :
    {-26,(code_ptr)semi_column}, // ;
    {-247297, (code_ptr)compile_var}, // var
    {-741642187, (code_ptr)compile_const},  // const
    {-2757, (code_ptr)compile_if}, // if
    {-16218861, (code_ptr)compile_then}, // then
    {-11521820, (code_ptr)compile_else}, // else
    {-15576879, (code_ptr)compile_repeat},  //rept
    {-696303181, (code_ptr)compile_again}, // again
    {-1167045072, (code_ptr)compile_while}, // while
    {-2426, (code_ptr)compile_do}, // do
    {-13736455, (code_ptr)compile_loop}, // loop
    {-227550215, (code_ptr)compile_ploop}, // +loop
    {-717083437, (code_ptr)compile_begin}, // begin
    {-1126219579, (code_ptr)compile_until}, // until
    {934,(code_ptr)dots}, // .s
    {933,(code_ptr)dotr}, // .r
};
#define SYSTEM_COUNT 20



dict_entry_t user_dict[USER_NAMES_SIZE];
char free_slot=0;

char tib[TIB_SIZE];
char pad[PAD_SIZE];

code_ptr cip;

char ctib;
char first, last,current;


name_hash hash(){// hash le token courant.
    name_hash nh;
    int i;
    char c;
    nh=0;
    for (i=first;i<first+5 && tib[i]!=0;i++){
        c = tib[i] & 127;
        if (c>='a' && c<='z') c -= 32;
        else if (c>'z') c -= 26;
        nh *= HASH_BASE;
        nh += c-33;
    }
    return nh;
}//hash()

void hash2str(name_hash hash, char *name){
    int i,d;
    char str[6];
    str[5]=0;
    i=4;
    while (hash){
        d=hash % HASH_BASE;
        hash /= HASH_BASE;
        d += 33;
        if (d>='a') d += 26;
        str[i--]=d;
    }
    if (i==4) str[4]=33; else i++;
    strcpy(name,&str[i]);
}//hashs2str()


void upper(){
    int i;
    for (i=first;i<=last;i++) if (tib[i]>='a' && tib[i]<='z') tib[i] -= 32;
}// upper()

int word(int c){
    int i;
    i=current;
    while  (i<ctib && tib[i]==c ) i++;
    first=i;
    while (i<ctib && tib[i]!=c) i++;
    last = i-1;
    if (tib[i]==c){tib[i]=0; i++;}
    current=i;
}// word()


int try_user(name_hash nh){
    int i;
    code_ptr cptr;
    if (state)cptr=here; else cptr=cip;
    for(i=free_slot-1;i>-1;i--){
        if (user_dict[i].name==nh){
            *cptr++=IRCALL;
            *cptr++=(int)user_dict[i].cfa;
            *cptr++=(int)user_dict[i].cfa>>8;
            if (state)here=cptr;else cip=cptr;
            break;
        }
    }
    if (i==-1) return 0; else return 1;
}// try_user()

int try_system(name_hash nh){
    int i;
    code_ptr cptr;
   compfct f;
   if (state) cptr=here;else cptr=cip;
   for(i=SYSTEM_COUNT-1;i>-1;i--){
        if (abs(system_dict[i].name)==nh){
            if ((int)system_dict[i].name<0){
                f=(compfct)system_dict[i].cfa;
                f();
                if (error) return 0;
            }else{
                *cptr++=IFCALL;
                *cptr++=(int)system_dict[i].cfa;
                *cptr++=(int)system_dict[i].cfa>>8;
                if (state) here=cptr;else cip=cptr;
            }
            return 1;
        }
    }
    return 0;
}// try_system()


int try_token(name_hash nh){ // recherche le mot dans VM_TOKENS
    int i;
    for (i=TOK_COUNT-1;i>-1;i--){
        if (nh==vm_tokens[i]){
            return compile_token(i);
        }
    }
    return 0;
}//try_token()


int parse_int(int *n){
    int i,base,sign;
    upper();
    *n=0;
    base=10;
    sign=1;
    i=first;
    switch (tib[i]){
        case '$':
            base=16;
            i++;
            break;
        case '-':
            sign=-1;
            i++;
            break;
        case '+':
            i++;
            break;
    }
    for (;i<=last;i++){
        *n = *n*base;
        if (tib[i]>='0' && tib[i]<='9') *n += tib[i]-'0';
        else if (base==16 && tib[i]>='A' && tib[i]<='F') *n += tib[i]-'A'+10;
        else{
            error=1;
            break;
        }
    }
    *n = *n * sign;
    return !error;
}//parse_int()


int try_integer(){
    int n;

    code_ptr cptr;
    if (state) cptr=here;else cptr=cip;
    if (parse_int(&n)){
        if (n<128 && n > -129){
            *cptr++=ICLIT;
            *cptr++=n;
        }else if (abs(n)<65536){
            *cptr++=IWLIT;
            *cptr++=n;
            *cptr++=n>>8;
        }else{
            *cptr++=ILIT;
            *cptr++=n;
            *cptr++=n>>8;
            *cptr++=n>>16;
            *cptr++=n>>24;
        }
       if (state)here=cptr;else cip=cptr;
    }
    return !error;
}// try_integer()

void column(){ // ':' doit être le premier mot dans tib
    name_hash nh;

    if (state || first>0){error=1;return;}
    word(SPC);
    nh=hash();
    state=1;
    mark=here;
    user_dict[free_slot].name=nh;
    user_dict[free_slot].cfa=here;
}//column()

void semi_column(){// ';' doit-être le dernier mot dans tib
    if (!state){error=1;return;}
    *here++=IRET;
    cip=here+256;
    state=IMMEDIATE;
    free_slot++;
}//semi_column()

void compile_var(){ // var, ne doit pas être utilisé à l'intérieur d'une définition
    name_hash nh;
    code_ptr cfa,var;
    if (state) {error=1;return;}
    state=COMPILE;
    word(SPC);
    if (last<first){error=1;return;}
    nh=hash();
    cfa=here;
    var = cfa+6;
    *here++=ILIT;
    *here++=(int)var;
    *here++=(int)var>>8;
    *here++=(int)var>>16;
    *here++=(int)var>>24;
    *here++=IRET;
    here +=CELL_SIZE;
    user_dict[free_slot].name=nh;
    user_dict[free_slot].cfa=cfa;
    free_slot++;
}// compile_var()

void compile_const(){ // const, ne doit pas être utilisé à l'intérieur d'une définition
    name_hash nh;
    code_ptr cfa;
    int n;
    if (state) {error=1;return;}
    state=COMPILE;
    word(SPC);
    if (last<first){error=1;return;}
    nh=hash();
    cfa=here;
    cip=here+256;
    *here++=*cip++;
    *here++=*cip++;
    if (*(cip-2)==IWLIT){
        *here++=*cip++;
    }else if (*cip-2==ILIT){
        *here++=*cip++;
        *here++=*cip++;
        *here++=*cip++;
    }
    *here++=IRET;
    user_dict[free_slot].name=nh;
    user_dict[free_slot].cfa=cfa;
    free_slot++;
}// compile_const()

void compile_if(){ //if
    code_ptr cptr;
    if (state) cptr=here;else cptr=cip;
    *cptr++=IQBRAZ;
    cstack[++cp]=(int)cptr;
    cptr++; // saute  addresse de ?braz
    if (state)here=cptr;else cip=cptr;
}// compile_if()

void compile_then(){ //then
    char *there;
    code_ptr cptr;
    if (state) cptr=here;else cptr=cip;
    there=(char *)cstack[cp--];
    *there=cptr-there-1;
    if (state)here=cptr;else cip=cptr;
}// compile_then()

void compile_else(){ //else
    char *there;
    code_ptr cptr;
    if (state) cptr=here;else cptr=cip;
    there=(char*)cstack[cp];
    *there=cptr-there+1;
    *cptr++=IBRA;
    cstack[cp]=(int)cptr;
    cptr++;
    if (state)here=cptr;else cip=cptr;
}//compile_else(){

void compile_do(){ // 'do'
    code_ptr cptr;
    if (state)cptr=here;else cptr=cip;
    *cptr++=ITOR;
    *cptr++=ITOR;
    cstack[++cp]=(int)cptr;
    if (state) here=cptr; else cip=cptr;
}//compile_do()

void compile_loop(){ //loop
    code_ptr there, cptr;

    if (state) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IRFROM;
    *cptr++=I1PLUS;
    *cptr++=IDUP;
    *cptr++=IRFETCH;
    *cptr++=ISWAP;
    *cptr++=ITOR;
    *cptr++=ISUB;
    *cptr++=IQBRA;
    *cptr++=there-cptr-1;
    *cptr++=IRFROM;
    *cptr++=IRFROM;
    *cptr++=IDDROP;
    if (state) here=cptr;else cip=cptr;
}//compile_loop()


void compile_ploop(){//+loop
    code_ptr there, cptr;

    if (state) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IRFROM;
    *cptr++=IADD;
    *cptr++=IDUP;
    *cptr++=IRFETCH;
    *cptr++=ISWAP;
    *cptr++=ITOR;
    *cptr++=ISUB;
    *cptr++=IQBRA;
    *cptr++=there-cptr-1;
    *cptr++=IRFROM;
    *cptr++=IRFROM;
    *cptr++=IDDROP;
    if (state) here=cptr;else cip=cptr;
}//compile_ploop()

void compile_begin(){//begin
    if (state) 
      cstack[++cp]=(int)here;
    else
      cstack[++cp]=(int)cip;
}//compile_begin()

void compile_again(){//again
    code_ptr there,cptr;
    if (state) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IBRA;
    *cptr++=there-cptr-1;
    if (state) here=cptr;else cip=cptr;
}// compile_again()

void compile_while(){ // while
    code_ptr there,cptr;
    if (state) cptr=here;else cptr=cip;
    *cptr++=IQBRAZ;
    cstack[++cp]=(int)cptr;
    cptr++;
    if (state) here=cptr;else cip=cptr;
}// compile_while()

void compile_repeat(){//rept
    code_ptr there,therew,cptr;
    if (state) cptr=here;else cptr=cip;
    therew=(code_ptr)cstack[cp--]; // while référence avant
    there=(code_ptr)cstack[cp--]; // begin référence avant
    *cptr++=IBRA;
    *cptr++=there-cptr-1; // saut arrière vers BEGIN
    *therew=cptr-there-1; // saut avant du while après rept
    if (state) here=cptr;else cip=cptr;
}//compile_repeat()

void compile_until(){// until
    code_ptr there, cptr;
    if (state) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IQBRAZ;
    *cptr++=there-cptr-1;
    if (state) here=cptr;else cip=cptr;
}// compile_until()

int compile_token(int code){
    int i;
    code_ptr cptr;
    if (state) cptr=here;else cptr=cip;
    if (code==ILIT || code==ICLIT || code == IWLIT){
        word(SPC);
        upper();
        try_integer();
    } else if (code==IDOTQ){
        for(i=current;i<ctib && tib[i]!='"';i++);
        first=current;
        last=i-1;
        if (tib[i]=='"') i++;
        current=i;
        i=last-first+1;
        if (i>0){
            *cptr++=IDOTQ;
            *cptr++=i;
            for (i=first;i<=last;i++) *cptr++=tib[i];
        }
    } else if (code==IFCALL || code== IRCALL || code==IBRA|| code==IQBRA){
        *cptr++=code;
        word(SPC);
        upper();
        if (parse_int(&i)){
            *cptr++=i;
            if (code==IFCALL || code==IRCALL) *cptr++= (i>>8);
        }
    }else {
        *cptr++=code;
    }
    if (state) here=cptr;else cip=cptr;
    return !error;
}// compile_token()


//void print_integer(int n){
//    int i;
//    char sign;
//    if (n<0){sign='-';n=abs(n);}else sign=' ';
//    pad[11]=0;
//    i=10;
//    while (n){
//       pad[i--]= '0'+ n%10;
//       n /=10;
//    }
//    if (i==10) pad[i]='0'; else (sign=='-')?pad[i]=sign:i++;
//    UartPrint(STDOUT,&pad[i]);
//    UartPrint(STDOUT," ");
//}// print_integer()

void compile_run(){ // analyse le contenu de TIB
    char mot[7];
    int code;
    imm_code=here+256;
    cip=imm_code;
    current=0;
    name_hash nh;
    state=IMMEDIATE;
    error=0;
    while (!error && current<ctib){
        word(SPC);
        nh = hash();
        if (!(try_user(nh)||try_system(nh)||try_token(nh)||try_integer())){
            hash2str(nh,mot);
            print(comm_channel, "mot inconnu '");
            print(comm_channel,mot);
            print(comm_channel,"', position: ");
            print_int(comm_channel, first+1,1);
        }
    }//while (current<ctib)
    if (!(error || state)){
        *cip=IBYE;
        code = StackVM((const unsigned char*)imm_code,WARM_START);
        if (code){
            UartPrint(STDOUT,"Erreur opcode VM: ");
            print_int(SERIAL_CON,code,2);
            UartPutch(STDOUT,'\r');
        }
    }
}// compile_run()


void vpForth(){ // démarrage système forth en mode interpréteur
    here=(unsigned char *)&ram_code;
//    comm_channel=SERIAL_CON;
    print(comm_channel,SYSTEM_NAME);
    print(comm_channel,SYSTEM_VERSION);
    while (1){
        if ((ctib=readline(comm_channel, &tib[0],TIB_SIZE-1))){
            cursor_right();
            compile_run();
        }
        print(comm_channel, " ok\r");
    }// while(1)
}//tForth()

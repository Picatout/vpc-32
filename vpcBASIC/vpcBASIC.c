/*
 *    système vpcBASIC exécutant sur la VM.
 */
#include <math.h>
#include "../hardware/serial_comm/serial_comm.h"
#include "../hardware/HardwareProfile.h"
#include "vpcBASIC.h"
#include "vm.h"

#define NAME_LEN 7  // longueur maximale des mots
#define F_COMPILE 128 // mot compilant
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
typedef char *code_ptr;
typedef int(*compfct)();

typedef struct{
    code_ptr cfa;
    unsigned char flags;  //indicateurs et longueur nom
    char name[NAME_LEN];
} dict_entry_t;


// variable systèmes
// indices dans user[]
#define BASE 0
#define HERE 1
#define STATE 2

const char *keywords[]={
"LET",
"READ",
"INPUT",
"PRINT",
"DATA",
"RESTORE",
"IF",
"THEN",
"FOR",
"TO",
"STEP",
"NEXT",
"DIM",
"GOTO",
"GOSUB",
"ON",
"RETURN",
"END",
"STOP",
"DEF",
"FNEND",
"REM",
"CHANGE",
};

const char *functions[]={
"SIN",  // sine
"COS",  // cosine
"TAN",  // tangent
"COT",  // cotagent
"ATN",  // arctagent
"EXP",  // e^X
"LOG",  // natural logarigth
"ABS",  // absolute
"SQR",  // square root
"INT",  // integer part
"RND",  // random number [0-1[
"SGN",  // sign of number
"NUM",  // items count last input
};

// pointeurs
char *here; // pointeur espace code

// variables
int user[16]; // variables utilisateurs et système
unsigned char *ram_code;
int cstack[6]; // pile utilisé par le compilateur (control flow stack)
char cp=-1; //pointeur pour la pile cstack
char error=0;
code_ptr imm_code, mark;
code_ptr cip; // pointeur vers tampon d'exécution immédiate

/*
const char *vm_tokens[]= { // mnémoniques bytecodes de la VM  index=bytecode
    "BYE",
    "?KEY",
    "EMIT",
    "LIT",
    "FCALL",
    "RET",
    "BRA",
    "?BRA",
    "EXEC",
    "!",
    "@",
    "C!",
    "C@",
    "R@",
    "U@",
    "U!",
    ">R"
    "R>",
    "DROP",
    "DUP",
    "SWAP",
    "OVER",
    "PICK",
    "+",
    "-",
    "*",
    "/",
    "MOD",
    "0<",
    "0=",
    "AND",
    "OR",
    "XOR",
    "INVERT",
    "KEY",
    "SFR",
    "SET",
    "CLEAR",
    "TOGGLE",
    "CLIT",
    "WLIT",
    ".\"",
    "RCALL",
    "TICKS",
    "DELAY",
    "?DUP",
    "1+",
    "1-",
    "+!",
    "2*",
    "2/",
    "2drop",
    "rot",
    "2dup",
    "min",
    "max",
    "abs",
    "<<",
    ">>",
    "/mod",
    "?braz",
    ".",
    "dcnt",
    "rcnt",
    "=",
    "<>",
    "<",
    ">",
    "<=",
    ">=",
    "X@",
    "X!",
    "XLOOP",
    "+XLOOP",
    "USER"
} ;
*/

// mots système enregistrés en mémoire flash
const char space[]={ICLIT,32,IEMIT,IRET};
const char spaces[]={IXSTORE,ICLIT,32,IEMIT,IXLOOP,IRET};
const char cr[]={ICLIT,13,IEMIT,IRET};
const char phere[]={ICLIT,HERE,IUSER,IFETCH,IRET}; // empile pointeur here
const char base[]={ICLIT,BASE,IUSER,IRET}; // empile adresse de la variable base
const char state[]={ICLIT,STATE,IUSER,IRET};// empile l'adresse de la variable state

// registres spéciaux PIC32MX110F016B
const char portb[]={IWLIT,0x20,0x61,ISFR,IRET};
const char trisb[]={IWLIT,0x10,0x61,ISFR,IRET};
const char anselb[]={IWLIT,0,0x61,ISFR,IRET};
const char latb[]={IWLIT,0x30,0x61,ISFR,IRET};

/*
const char dots[]={ICLIT,SPC,IEMIT,ICLIT,'(',IEMIT,ICLIT,SPC,IEMIT,IDCNT,
                   IDUP,IDOT,ICLIT,')',IEMIT,ICLIT,SPC,IEMIT,IDUP,IQBRAZ,9,IDUP,ITOR,
                   IMINUS1,IPICK,IDOT,IRFROM,IMINUS1,IBRA,-12,IDROP,IRET};
*/

void print_integer(int n);
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


dict_entry_t system_dict[]={ // les mots compilant on len >127 
    {(code_ptr)column,129,":"}, // :
    {(code_ptr)semi_column,129,";"}, // ;
    {(code_ptr)compile_var,131,"VAR"}, // var
    {(code_ptr)compile_const,132,"CONST"},  // const
    {(code_ptr)compile_if,130,"IF"}, // if
    {(code_ptr)compile_then,132,"THEN"}, // then
    {(code_ptr)compile_else,132,"ELSE"}, // else
    {(code_ptr)compile_repeat,134,"REPEAT"},  //repeat
    {(code_ptr)compile_again,133,"AGAIN"}, // again
    {(code_ptr)compile_while,133,"WHILE"}, // while
    {(code_ptr)compile_do,130,"DO"}, // do
    {(code_ptr)compile_loop,132,"LOOP"}, // loop
    {(code_ptr)compile_ploop,133,"+LOOP"}, // +loop
    {(code_ptr)compile_begin,133,"BEGIN"}, // begin
    {(code_ptr)compile_until,133,"UNTIL"}, // until
//    {(code_ptr)dots,2,".S"}, // .s
    {(code_ptr)space,5,"SPACE"},  // 'space'
    {(code_ptr)spaces,6,"SPACES"}, // 'spces'
    {(code_ptr)cr,2,"CR"},  // 'cr'
    {(code_ptr)trisb,5,"TRISB"},  // 'trisb'
    {(code_ptr)portb,5,"PORTB"},   // 'portb'
    {(code_ptr)latb,4,"LATB"}, // 'latb'
    {(code_ptr)anselb,6,"ANSELB"}, // 'anselb'
    {(code_ptr)base,4,"BASE"},
    {(code_ptr)phere,4,"HERE"},
    {(code_ptr)state,5,"STATE"},
};
#define SYSTEM_COUNT 26



dict_entry_t user_dict[USER_NAMES_SIZE];
char free_slot=0;

char tib[TIB_SIZE];
char pad[PAD_SIZE];


char ctib;
char first, last,current;


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

int cp_name(char dest[NAME_LEN]){
    int i,j,len;
    upper();
    len=last-first+1;
    j=first;
    for(i=0;i<len;i++){
        dest[i]=tib[j++];
    }
    return len;
}//cp_name

int is_name(dict_entry_t entry){
    int i,j;
    i=first;
    j=0;
    for (i=first;i<=last;i++){
        if (tib[i]!=entry.name[j++])return 0;
    }
    return 1;
}//is_name()

int try_user(){
    int i,len;
    code_ptr cptr;
    len=last-first+1;
    if (user[STATE])cptr=here; else cptr=cip;
    for(i=free_slot-1;i>-1;i--){
        if (len==(user_dict[i].flags&15) && is_name(user_dict[i])){
            *cptr++=IRCALL;
            *cptr++=(int)user_dict[i].cfa;
            *cptr++=(int)user_dict[i].cfa>>8;
            if (user[STATE])here=cptr;else cip=cptr;
            break;
        }
    }
    if (i==-1) return 0; else return 1;
}// try_user()

int try_system(){
    int i,len;
    code_ptr cptr;
   compfct f;
   len=last-first+1;
   if (user[STATE]) cptr=here;else cptr=cip;
   for(i=SYSTEM_COUNT-1;i>-1;i--){
        if (len==(system_dict[i].flags&15) && is_name(system_dict[i])){
            if ((int)system_dict[i].name<0){
                f=(compfct)system_dict[i].cfa;
                f();
                if (error) return 0;
            }else{
                *cptr++=IFCALL;
                *cptr++=(int)system_dict[i].cfa;
                *cptr++=(int)system_dict[i].cfa>>8;
                if (user[STATE]) here=cptr;else cip=cptr;
            }
            return 1;
        }
    }
    return 0;
}// try_system()

typedef struct{
    unsigned char len;
    char mnemo[];
} token_t;

extern token_t vm_tokens[TOK_COUNT];

int try_token(){ // recherche le mot dans vm_tokens
    int i,j,len;
    len=last-first+1;
    for (i=TOK_COUNT-1;i>-1;i--){
        if (vm_tokens[i].len==len){
            for(j=0;j<len;j++)
                if (vm_tokens[i].mnemo[j]!=tib[first+j]) break;
            if (j=len) return 1;
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
    if (tib[i]=='$'){base=16;i++;} else if (tib[i]=='-'){ sign=-1;i++;}
    for (;i<=last;i++){
        *n = *n*base;
        if (tib[i]>='0' && tib[i]<='9') *n += tib[i]-'0';
        else if (base==16 && tib[i]>='A' && tib[i]<='F') *n += tib[i]-'A'+10;
        else{
            error=1;
            //UartPrint(STDOUT,"mot inconnu: ");
            UartPrint(STDOUT,&tib[first]);
            UartPrint(STDOUT," ?\n");
            break;
        }
    }
    *n = *n * sign;
    return !error;
}//parse_int()


int try_integer(){
    int n;

    code_ptr cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    if (parse_int(&n)){
        if (n>-128 && n <128){
            *cptr++=ICLIT;
            *cptr++=n;
        }else if (n>-32768 && n<32767){
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
       if (user[STATE])here=cptr;else cip=cptr;
    }
    return !error;
}// try_integer()

void column(){ 
    if (user[STATE] ){error=1;return;}
    word(SPC);
    mark=here;
    user_dict[free_slot].flags=cp_name(user_dict[free_slot].name);
    user_dict[free_slot].cfa=here;
    user[STATE]=COMPILE;
}//column()

void semi_column(){// ';' doit-être le dernier mot dans tib
    if (!user[STATE]){error=1;return;}
    *here++=IRET;
    imm_code=here+256;
    cip=imm_code;
    user[STATE]=IMMEDIATE;
    free_slot++;
}//semi_column()

void compile_var(){ // var, ne doit pas être utilisé à l'intérieur d'une définition
    code_ptr cfa,var;
    if (user[STATE]) {error=1;return;}
    word(SPC);
    if (last<first){error=1;return;}

    cfa=here;
    var = cfa+6;
    *here++=ILIT;
    *here++=(int)var;
    *here++=(int)var>>8;
    *here++=(int)var>>16;
    *here++=(int)var>>24;
    *here++=IRET;
    here +=CELL_SIZE;
    user_dict[free_slot].flags=cp_name(user_dict[free_slot].name);
    user_dict[free_slot].cfa=cfa;
    free_slot++;
}// compile_var()

void compile_const(){ // const, ne doit pas être utilisé à l'intérieur d'une définition
    code_ptr cfa;
    int n;
    if (user[STATE]) {error=1;return;}
    word(SPC);
    if (last<first){
        error=1;
        return;
    }
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
    user_dict[free_slot].flags=cp_name(user_dict[free_slot].name);
    user_dict[free_slot].cfa=cfa;
    free_slot++;
}// compile_const()

void compile_if(){ //if
    code_ptr cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    *cptr++=IQBRAZ;
    cstack[++cp]=(int)cptr;
    cptr++; // saute  addresse de ?braz
    if (user[STATE])here=cptr;else cip=cptr;
}// compile_if()

void compile_then(){ //then
    char *there;
    code_ptr cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    there=(char *)cstack[cp--];
    *there=cptr-there-1;
    if (user[STATE])here=cptr;else cip=cptr;
}// compile_then()

void compile_else(){ //else
    char *there;
    code_ptr cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    there=(char*)cstack[cp];
    *there=cptr-there+1;
    *cptr++=IBRA;
    cstack[cp]=(int)cptr;
    cptr++;
    if (user[STATE])here=cptr;else cip=cptr;
}//compile_else(){

void compile_do(){ // 'do'
    code_ptr cptr;
    if (user[STATE])cptr=here;else cptr=cip;
    *cptr++=ISUB;
    *cptr++=IXSTORE;
    cstack[++cp]=(int)cptr;
    if (user[STATE]) here=cptr; else cip=cptr;
}//compile_do()

void compile_loop(){ //loop
    code_ptr there, cptr;

    if (user[STATE]) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IXLOOP;
    *cptr++=there-cptr-1;
    if (user[STATE]) here=cptr;else cip=cptr;
}//compile_loop()


void compile_ploop(){//+loop
    code_ptr there, cptr;

    if (user[STATE]) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IPXLOOP;
    *cptr++=there-cptr-1;
    if (user[STATE]) here=cptr;else cip=cptr;
}//compile_ploop()

void compile_begin(){//begin
    if (user[STATE])
      cstack[++cp]=(int)here;
    else
      cstack[++cp]=(int)cip;
}//compile_begin()

void compile_again(){//again
    code_ptr there,cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IBRA;
    *cptr++=there-cptr-1;
    if (user[STATE]) here=cptr;else cip=cptr;
}// compile_again()

void compile_while(){ // while
    code_ptr there,cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    *cptr++=IQBRAZ;
    cstack[++cp]=(int)cptr;
    cptr++;
    if (user[STATE]) here=cptr;else cip=cptr;
}// compile_while()

void compile_repeat(){//rept
    code_ptr there,therew,cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    therew=(code_ptr)cstack[cp--]; // while référence avant
    there=(code_ptr)cstack[cp--]; // begin référence avant
    *cptr++=IBRA;
    *cptr++=there-cptr-1; // saut arrière vers BEGIN
    *therew=cptr-there-1; // saut avant du while après rept
    if (user[STATE]) here=cptr;else cip=cptr;
}//compile_repeat()

void compile_until(){// until
    code_ptr there, cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
    there=(code_ptr)cstack[cp--];
    *cptr++=IQBRAZ;
    *cptr++=there-cptr-1;
    if (user[STATE]) here=cptr;else cip=cptr;
}// compile_until()

int compile_token(int code){
    int i;
    code_ptr cptr;
    if (user[STATE]) cptr=here;else cptr=cip;
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
    if (user[STATE]) here=cptr;else cip=cptr;
    return !error;
}// compile_token()


void print_integer(int n){
    int i;
    char sign;
    if (n<0){sign='-';n=abs(n);}else sign=' ';
    pad[11]=0;
    i=10;
    while (n){
       pad[i--]= '0'+ n%10;
       n /=10;
    }
    if (i==10) pad[i]='0'; else (sign=='-')?pad[i]=sign:i++;
    UartPrint(STDOUT,&pad[i]);
    UartPrint(STDOUT," ");
}// print_integer()

void compile_run(){ // analyse le contenu de TIB
    imm_code=here+256;
    cip=imm_code;
    current=0;
    error=0;
    while (!error && current<ctib){
        word(SPC);
        upper();
        if (!(try_user()||try_system()||try_token()||try_integer())){
        }
    }//while (current<ctib)
    if (!(error || user[STATE])){
        *cip=IEND;
        StackVM((const unsigned char*)imm_code,user);
    }else if (error && user[STATE]){
        here=mark;
        user[STATE]=IMMEDIATE;
    }
    user[HERE]=(unsigned int)here;
}// compile_run()

//#define SIM

#ifdef SIM
const char test[]="32 emit";
#endif
void tForth(){ // démarrage système forth en mode interpréteur
    ram_code=malloc(free_heap());
    user[BASE]=10;
    here=(char*)ram_code;
    user[HERE]=(unsigned int)here;
    user[STATE]=IMMEDIATE;

#ifdef SIM

        int i;
        ctib=7;
        for (i=0;i<ctib;i++) tib[i]=test[i];
        tib[i]=0;
#endif
    while (1){
#ifndef SIM
        if ((ctib=UartReadln(STDIN,tib,TIB_SIZE-1)))compile_run();
#else
        compile_run();
#endif
        UartPrint(STDOUT," ok\r");
    }// while(1)
}//tForth()

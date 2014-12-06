/*
 *    système vpcBASIC exécutant sur la VM.
 */
#include <math.h>
#include <stdint.h>
#include "../hardware/serial_comm/serial_comm.h"
#include "../hardware/HardwareProfile.h"
#include "../hardware/spiram/spiram.h"
#include "../console.h"
#include "vpcBASIC.h"
#include "vm.h"

#define NAME_LEN 32  // longueur maximale des mots
#define COMPILE 1
#define IMMEDIATE 0

// data types
typedef unsigned char BYTE ;
typedef unsigned short WORD;

typedef enum eError {ERR_NONE,ERR_SYNTAX} error_t;

typedef enum eToken_id {NONE,NUMBER,SYMBOL,ADDOP, MULOP,POW,LPAREN,RPARENT,
        COMMA,SEMICOLUMN,STRING,APOSTROPH} token_t;

typedef struct{
    uint16_t const_base; // début bloc de constantes
    uint16_t const_read; // position du pointeur READ
    uint16_t str_base;   // début block chaine de caractères
    uint16_t str_read;   // poisition du pointeur READ
    uint16_t var_base;   // début des variables
    uint16_t var_count;  // nombre de variables
} prog_header_t;

// blocs SPIRAM utilisés pour le code source et la compilation
#define SRC_BASE (0x10000)
#define BIN_BASE (0x18000)
#define VAR_BASE (0x1A000)
#define CONST_BASE (0x1C00)
#define STR_BASE (0x1E000)

// déclarations vpcBASIC
#define STATEMENTS_COUNT (19)
static const char *statements[STATEMENTS_COUNT]={
"CHANGE",
"DATA",
"DEF",
"DIM",
"END",
"FOR",
"GOSUB",
"GOTO",
"IF",
"INPUT",
"LET",
"ON",
"PRINT",
"READ",
"REM",
"RESTORE",
"RETURN",
"STOP",
};

typedef enum eStatement {CHANGE,DATA,DEF,DIM,END,FOR,GOSUB,GOTO,IF,INPUT,LET,ON,PRINT,
        READ,REM,RESTORE,RETURN,STOP} eStatement_t;

// fonctions vpcBASIC internes
#define FUNC_COUNT (13)
static const char *functions[FUNC_COUNT]={
"ABS",  // absolut
"ATN",  // arctangente
"COS",  // cosinus
"COT",  // cotangente
"EXP",  // e^X
"INT",  // partie entière
"LOG",  // logarigthe naturel
"NUM",  // nombre d'items lus lors du dernier READ,INPUT
"RND",  // nombre aléatoire entre [0-1[
"SGN",  // signe du nombre
"SIN",  // sinus
"SQR",  // racine carrée
"TAN",  // tangente
};

typedef enum eBasicFunc {ABS,ATN,COS,COT,EXP,INTEG,LOG,NUM,RND,SGN,SIN,SQR,TAN} eBasicFunc_t;

token_t tok_id;
char tok_value[33];
prog_header_t prog_header;
error_t error;
unsigned state;

unsigned char *ram_code; // espace code
char *here; // pointeur espace code

char src_line[CHAR_PER_LINE+1];
unsigned inp; //progression de next_token() dans src_line

src_error(){
    error=ERR_SYNTAX;
}

float hex_to_float(char *hex){
    uint32_t h;
    h=0;
    while (*hex){
        h= h*16 + *hex<='9'?(*hex)-'0':(*hex)-'A'+10;
        hex++;
    }
    return (float)((int32_t)h);
}

float bin_to_float(char *bin){
    uint32_t b;
    b=0;
    while (*bin){
        b = b<<1 + *bin=='1'?1:0;
        bin++;
    }
    return (float)((int32_t)b);
}

float dec_to_float(char *dec){
    uint32_t d,x;
    float f;
    BOOL eneg=FALSE;

    f=0.0;
    d=1;
    while (*dec && *dec!='.' && *dec!='E'){
        f = f*10 + (*dec)-'0';
        dec++;
    }
    if (*dec=='.'){
        dec++;
        while (*dec && *dec!='E'){
            d *=10;
            f = f + ((*dec)-'0')/d;
        }
    }
    if (*dec=='E'){
        dec++;
        if (*dec=='+' || (eneg=(*dec=='-'))){
            dec++;
        }
        x=0;
        while (*dec){
            x= x*10 + (*dec)-'0';
            dec++;
        }
        f *= eneg?pow(10,-x):pow(10,x);
    }
    return f;
}

float number_to_float(char *number){
    if (number[0]=='&' && number[1]=='H')
        return hex_to_float(&number[2]);
    if (number[0]=='&' && number[1]=='B')
        return bin_to_float(&number[2]);
    return dec_to_float(number);
}

// compile les expressions
void expression(){

}

// fonctions compilant
// les déclarations
void compile_dim(){

}

void compile_let(){

}

void compile_if(){

}

void compile_goto(){

}

void compile_gosub(){

}

void compile_for(){

}

void compile_read(){

}

void compile_input(){

}

void compile_print(){

}

void compile_data(){

}

void compile_restore(){

}

void compile_on(){

}

void compile_return(){

}

void compile_def(){

}

void compile_change(){

}

void compile_stop(){

}

void compile_end(){

}

void compile_function(){

}

void compile_comma(){

}

void semi_column(){

}


void upper(char *str){
    while (*str) if (*str>='a' && *str<='z') *str -= 32;
}// upper()

void skip_white(){
    while(src_line[inp]==' ' || src_line[inp]=='\t') inp++;
}

unsigned parse_string(){
    BOOL quote=FALSE;
    BOOL escape=FALSE;
    unsigned i=0;

    while (src_line[inp] && !quote){
        switch(src_line[inp]){
        case '\\':
            if (!escape) escape=TRUE;else{tok_value[i++]=src_line[inp];escape=FALSE;}
            break;
        case 'n':
            if (!escape) tok_value[i++]=src_line[inp];else{tok_value[i++]='\n';escape=FALSE;}
            break;
        case '"':
            if (!escape) quote=TRUE;else{tok_value[i++]=src_line[inp];escape=FALSE;}
            break;
        default:
            if ((src_line[inp]<32)||src_line[inp]>127) src_error();else{tok_value[i++]=src_line[inp];}
            break;
        }//switch
        inp++;
    }//while
    if (quote){tok_value[i]=0;inp--;} else src_error();
    return i;
}

#define TOK_START (0)
#define TOK_END (9)
static int next_token(){
    unsigned state=TOK_START;
    unsigned i;

    i=inp;
    skip_white();
    tok_id=NONE;
    tok_value[0]=0;
    while (!error && (state<TOK_END) && src_line[inp]){
        switch(state){
        case 0:
            switch(src_line[inp]){
            case '+':
            case '-':
                    tok_id=ADDOP;
                    tok_value[i++]=src_line[inp];
                    state=TOK_END;
                    break;
            case '*':
            case '/':
                    tok_id=MULOP;
                    tok_value[i++]=src_line[inp];
                    state=TOK_END;
                    break;
            case '^':
                    tok_id=POW;
                    tok_value[i++]=src_line[inp];
                    state=TOK_END;
                    break;
            case ',':
                    tok_id=COMMA;
                    tok_value[i++]=src_line[inp];
                    state=TOK_END;
                    break;
            case ';':
                    tok_id=SEMICOLUMN;
                    tok_value[i++]=src_line[inp];
                    state=TOK_END;
                    break;
            case '"':
                    inp++;
                    i=parse_string();
                    tok_id=STRING;
                    state=TOK_END;
                    break;
            case '&':
                    tok_value[i++]=src_line[inp];
                    state=1; // nombre hexadécimal
                    break;
            default:
                    if (isalpha(src_line[inp])||(src_line[inp]=='_')){
                        tok_id=SYMBOL;
                        tok_value[i++]=toupper(src_line[inp]);
                        state=8; // symbole alphanumérique
                    }else if (isdigit(src_line[inp])){
                        tok_id=NUMBER;
                        tok_value[i++]=src_line[inp];
                        state=4; // nombre décimal
                    }else{
                        src_error();
                    }
            }//switch
            break;
        case 1: // nombre hexadécimal &H ou binaire &B
            if (src_line[inp]=='H'){
                tok_id=NUMBER;
                state=2;
            }else if (src_line[inp]=='B'){
                tok_id=NUMBER;
                state=3;
            }else{
                src_error();
            }
            break;
        case 2:    // nombre hexadécimal
            if (isxdigit(src_line[inp])){
                 tok_value[i++]=toupper(src_line[inp]);
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                state=TOK_END;
            }
            break;
        case 3: // nombre binaire
            if ((src_line[inp]=='1') || (src_line[inp]=='0')){
                tok_value[i++]=src_line[inp];
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                state=TOK_END;
            }
            break;
        case 4: // nombre décimal
            if (isdigit(src_line[inp])){
                tok_value[i++]=src_line[inp];
            }else if (src_line[inp]=='.'){
                tok_value[i++]=src_line[inp];
                state=5;
            }else if (toupper(src_line[inp])=='E'){
                tok_value[i++]='E';
                state=6;
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                state=TOK_END;
            }
            break;
        case 5: //après le '.'
            if (isdigit(src_line[inp])){
                tok_value[i++]=src_line[inp];
            }else if (toupper(src_line[inp])=='E'){
                tok_value[i++]='E';
                state=6;
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                state=TOK_END;
            }
            break;
        case 6: // après le 'E'
            if (isdigit(src_line[inp])||src_line[inp]=='+'||src_line[inp]=='-'){
                tok_value[i++]=src_line[inp];
                state=7;
            }else{
                src_error();
            }
            break;
        case 7://après signe ou 1ier décimal
            if (isdigit(src_line[inp])){
                tok_value[i++]=src_line[inp];
            }else if (!isdigit(tok_value[i-1])){
                src_error();
            }else{
                inp--;
                state=TOK_END;
            }
            break;
        case 8: // symbole alphanumérique
            if (isalnum(src_line[inp]) || src_line[inp]=='_'){
                    tok_value[i++]=toupper(src_line[inp]);
            }else{
                inp--;
                state=TOK_END;
            }
            break;
        }//switch
        inp++;
    }//while
    tok_value[i]=0;

}// word()

// cherche un chaîne dans un liste.
// paramètres:
//  s chaine recherchée
//  list liste de chaines
//  size  nombre d'items dans la liste
// renvoie:
//   -1 si pas trouvé dans la liste
//   l'index si trouvé.
int search_list(char *s,const char *list[], int size){
    int i,j;
    char *target;
    const char *item;
    BOOL match;
     for (i=0;i<size;i++){
         target=s;
         match=FALSE;
         item = list[i];
#if defined _DEBUG_
         UartPrint(STDOUT,item);
         UartPutch(STDOUT,'\r');
#endif
         j=0;
         while (*target && item[j]){
             if (!(*target++==item[j++])) break;
         }
         if (!*target){
             match=TRUE;
             break;
         }
     }
    return match?i:-1;
}//f()




float parse_number(char *nstr){

}//parse_number()



void statement(){
    next_token();

}

void print_result(){

}

void run(){

}

void compile_run(){ // analyse le contenu de TIB
    error=ERR_NONE;
    if (src_line[0]==' ' || isdigit(src_line[0])) state=COMPILE; else state=IMMEDIATE;
    statement();
    if (state=IMMEDIATE) run();
}// compile_run()


BOOL bRun;

#define CMD_COUNT (4)

static const char *cmd_list[CMD_COUNT]={
    "RUN",
    "LOAD",
    "SAVE",
    "BYE"
};

BOOL command(){
    int i;
    inp=0;
    next_token();
#if defined _DEBUG_
    UartPrint(STDOUT, tok_value);
    UartPutch(STDOUT,'\r');
#endif
    if ((i=search_list(tok_value,cmd_list,CMD_COUNT))==-1)
        return FALSE;
    switch(i){
        case 0: //RUN
            break;
        case 1: // LOAD
            break;
        case 2: // SAVE
            break;
        case 3: // BYE
            bRun=FALSE;
            break;
    }//switch
    return TRUE;
}

//#define SIM

#ifdef SIM
const char test[]="32 emit";
#endif
void vpcBasic(){ // démarrage système forth en mode interpréteur
    ram_code=malloc(free_heap());
    bRun=TRUE;
#ifdef SIM

        int i;
        ctib=7;
        for (i=0;i<ctib;i++) tib[i]=test[i];
        tib[i]=0;
#endif
    while (bRun){
#ifndef SIM
        print(comm_channel,">");
        readline(comm_channel,src_line,CHAR_PER_LINE);
        //src_line[strlen(src_line)-1]=0;
        if (!command()) compile_run();
#else
        compile_run();
#endif
    }// while(1)
}//tForth()

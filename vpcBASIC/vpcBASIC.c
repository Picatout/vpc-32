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

typedef enum eError {ERR_NONE,ERR_SYNTAX,ERR_DUP_ID} error_t;

typedef enum eToken_id {NONE,NUMBER,SYMBOL,ADDOP, MULOP,POW,LPAREN,RPARENT,
        COMMA,SEMICOLUMN,STRING,APOSTROPH,RELOP} token_t;

typedef enum sSymbol_class {LABEL,VARIABLE,FUNCTION} eSymbol_t;

typedef struct symbol_struct{
    char name[NAME_LEN];
    eSymbol_t class;
    unsigned addr;
}symb_t;

typedef struct{
    uint16_t const_base; // début bloc de constantes
    uint16_t const_read; // position du pointeur READ
    uint16_t str_base;   // début block chaine de caractères
    uint16_t str_read;   // poisition du pointeur READ
    uint16_t var_base;   // début des variables
    uint16_t var_count;  // nombre de variables
} prog_header_t;

// blocs SPIRAM utilisés pour le code source et la compilation
#define SRC_BASE (0x10000)    // texte source
#define SYMB_BASE (0x1A000)   // table des symboles
#define CONST_BASE (0x1C00)   // liste des constantes numériques DATA
#define STR_BASE (0x1E000)    // liste des chaînes DATA

// déclarations vpcBASIC
#define STATEMENTS_COUNT (18)
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

unsigned src_end; // fin du code source dans la SPIRAM
unsigned symb_free; // position libre table des symboles dans la SPIRAM
unsigned const_free; // position libre table des constantes DATA
unsigned str_free;   // position libre table des chaînes DATA
unsigned ram_free; // RAM libre
unsigned char *ram_code; // espace code
char *here; // pointeur espace code
char *vars; // pointeur des variables


char src_line[CHAR_PER_LINE+1];
unsigned inp; //progression de next_token() dans src_line

src_error(error_t e){
    char msg[56];

    error=e;
    switch(e){
    case ERR_SYNTAX:
        sprintf(msg,"syntax error at %d\r",inp-strlen(tok_value));
        print(comm_channel,msg);
        break;
    case ERR_DUP_ID:
        sprintf(msg,"duplicate identifier at %d\r",inp-strlen(tok_value));
        print(comm_channel,msg);
        break;
    }
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
            if ((src_line[inp]<32)||src_line[inp]>127) src_error(ERR_SYNTAX);else{tok_value[i++]=src_line[inp];}
            break;
        }//switch
        inp++;
    }//while
    if (quote){tok_value[i]=0;inp--;} else src_error(ERR_SYNTAX);
    return i;
}

#define TOK_START (0)
#define TOK_END (9)
static int next_token(){
    unsigned pstate=TOK_START;
    unsigned i=0;

    
    skip_white();
    tok_id=NONE;
    tok_value[0]=0;
    while (!error && (pstate<TOK_END) && src_line[inp]){
        switch(pstate){
        case 0:
            switch(src_line[inp]){
            case '+':
            case '-':
                    tok_id=ADDOP;
                    tok_value[i++]=src_line[inp];
                    pstate=TOK_END;
                    break;
            case '*':
            case '/':
                    tok_id=MULOP;
                    tok_value[i++]=src_line[inp];
                    pstate=TOK_END;
                    break;
            case '^':
                    tok_id=POW;
                    tok_value[i++]=src_line[inp];
                    pstate=TOK_END;
                    break;
            case ',':
                    tok_id=COMMA;
                    tok_value[i++]=src_line[inp];
                    pstate=TOK_END;
                    break;
            case ';':
                    tok_id=SEMICOLUMN;
                    tok_value[i++]=src_line[inp];
                    pstate=TOK_END;
                    break;
            case '"':
                    inp++;
                    i=parse_string();
                    tok_id=STRING;
                    pstate=TOK_END;
                    break;
            case '=':
                    tok_value[i++]=src_line[inp];
                    tok_id=RELOP;
                    pstate=TOK_END;
            case '>':
                    tok_id=RELOP;
                    tok_value[i++]=src_line[inp];
                    if (src_line[inp+1]=='='){
                        tok_value[i++]=src_line[++inp];
                    }
                    pstate=TOK_END;
                    break;
            case '<':
                    tok_id=RELOP;
                    tok_value[i++]=src_line[inp];
                    if (src_line[inp+1]=='='||src_line[inp+1]=='>'){
                        tok_value[i++]=src_line[++inp];
                    }
                    pstate=TOK_END;
                    break;
            case '&':
                    tok_value[i++]=src_line[inp];
                    pstate=1; // nombre hexadécimal
                    break;
            default:
                    if (isalpha(src_line[inp])||(src_line[inp]=='_')){
                        tok_id=SYMBOL;
                        tok_value[i++]=toupper(src_line[inp]);
                        pstate=8; // symbole alphanumérique
                    }else if (isdigit(src_line[inp])){
                        tok_id=NUMBER;
                        tok_value[i++]=src_line[inp];
                        pstate=4; // nombre décimal
                    }else{
                        src_error(ERR_SYNTAX);
                    }
            }//switch
            break;
        case 1: // nombre hexadécimal &H ou binaire &B
            if (src_line[inp]=='H'){
                tok_id=NUMBER;
                pstate=2;
            }else if (src_line[inp]=='B'){
                tok_id=NUMBER;
                pstate=3;
            }else{
                src_error(ERR_SYNTAX);
            }
            break;
        case 2:    // nombre hexadécimal
            if (isxdigit(src_line[inp])){
                 tok_value[i++]=toupper(src_line[inp]);
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                pstate=TOK_END;
            }
            break;
        case 3: // nombre binaire
            if ((src_line[inp]=='1') || (src_line[inp]=='0')){
                tok_value[i++]=src_line[inp];
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                pstate=TOK_END;
            }
            break;
        case 4: // nombre décimal
            if (isdigit(src_line[inp])){
                tok_value[i++]=src_line[inp];
            }else if (src_line[inp]=='.'){
                tok_value[i++]=src_line[inp];
                pstate=5;
            }else if (toupper(src_line[inp])=='E'){
                tok_value[i++]='E';
                pstate=6;
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                pstate=TOK_END;
            }
            break;
        case 5: //après le '.'
            if (isdigit(src_line[inp])){
                tok_value[i++]=src_line[inp];
            }else if (toupper(src_line[inp])=='E'){
                tok_value[i++]='E';
                pstate=6;
            }else if (src_line[inp]==' '){
                //ignore espace
            }else{
                inp--;
                pstate=TOK_END;
            }
            break;
        case 6: // après le 'E'
            if (isdigit(src_line[inp])||src_line[inp]=='+'||src_line[inp]=='-'){
                tok_value[i++]=src_line[inp];
                pstate=7;
            }else{
                src_error(ERR_SYNTAX);
            }
            break;
        case 7://après signe ou 1ier décimal
            if (isdigit(src_line[inp])){
                tok_value[i++]=src_line[inp];
            }else if (!isdigit(tok_value[i-1])){
                src_error(ERR_SYNTAX);
            }else{
                inp--;
                pstate=TOK_END;
            }
            break;
        case 8: // symbole alphanumérique
            if (isalnum(src_line[inp]) || src_line[inp]=='_'){
                    tok_value[i++]=toupper(src_line[inp]);
            }else{
                inp--;
                pstate=TOK_END;
            }
            break;
        }//switch
        inp++;
    }//while
    tok_value[i]=0;
#if defined _DEBUG_
    UartPrint(STDOUT,tok_value);
    UartPutch(STDOUT,'\r');
#endif
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
         j=0;
         while (*target && item[j]){
             if (!(*target==item[j])) break;
             target++;
             j++;
         }
         if (!*target && !item[j]){
             match=TRUE;
             break;
         }
     }
    return match?i:-1;
}//f()




float parse_number(char *nstr){

}//parse_number()


unsigned search_symbol(char *name, symb_t *s){
    unsigned i=SYMB_BASE;

    while (i<symb_free){
        sram_read_block(i,(char*)s,sizeof(symb_t));
        if (!strcmp(s->name,name)) return i;
        i += sizeof(symb_t);
    }
    return -1;
}

BOOL add_symbol(char *name, eSymbol_t class, unsigned addr){
    unsigned i;
    symb_t s;
    
    i=search_symbol(name, &s);
    if (i==-1){
        s.class=class;
        strcpy(s.name,name);
        s.addr=addr;
        sram_write_block(symb_free,(char*)&s,sizeof(symb_t));
        symb_free  +=sizeof(symb_t);
        return TRUE;
    }
    print(comm_channel,"Duplicate symbol\r");
    return FALSE;
}

void statement(){
    int i=-1;
    char name[NAME_LEN];
    unsigned addr;
    eSymbol_t class;

    name[0]=0;
    next_token();
    if (!(tok_id==NUMBER || tok_id==SYMBOL)){
        src_error(ERR_SYNTAX);
        return;
    }
    if (tok_id==SYMBOL){
        i=search_list(tok_value,statements,STATEMENTS_COUNT);
    }
    if (tok_id==NUMBER || i==-1){
        strcpy(name,tok_value);
        addr=(unsigned)here;
        class=LABEL;
        next_token();
        if (tok_id!=SYMBOL || (i=search_list(tok_value,statements,STATEMENTS_COUNT))==-1){
            src_error(ERR_SYNTAX);
            return;
        }
        //add_symbol(name,LABEL,(unsigned)here);
    }
    switch (i){
        case CHANGE:
            compile_change();
            break;
        case DATA:
            compile_data();
            break;
        case DEF:
            compile_def();
            break;
        case DIM:
            compile_dim();
            break;
        case END:
            compile_end();
            break;
        case FOR:
            compile_for();
            break;
        case GOSUB:
            compile_gosub();
            break;
        case GOTO:
            compile_goto();
            break;
        case IF:
            compile_if();
            break;
        case INPUT:
            compile_input();
            break;
        case LET:
            compile_let();
            break;
        case ON:
            compile_on();
            break;
        case PRINT:
            compile_print();
            break;
        case READ:
            compile_read();
            break;
        case REM:
            break;
        case RESTORE:
            compile_restore();
            break;
        case RETURN:
            compile_return();
            break;
        case STOP:
            compile_stop();
            break;
        default:
            src_error(ERR_SYNTAX);
            return;
    }
    if (error) return;
success: // si déclaration BASIC est correcte on la sauvegarde dans la SPIRAM
    if (name[0])add_symbol(name,class,addr);
    sram_write_string(src_end,src_line);
    src_end += strlen(src_line)+1;
}


void run(){

}

void compile_run(){ // analyse le contenu de TIB
    error=ERR_NONE;
    if (src_line[0]==' ' || isdigit(src_line[0])) state=COMPILE; else state=IMMEDIATE;
    if (src_end>=SYMB_BASE){
        print(comm_channel,"source memory filled.");
        return;
    }
    statement();
    if (state=IMMEDIATE) run();
}// compile_run()


void list_source(){
    char *in,*out;
    unsigned addr=SRC_BASE;
    unsigned line_no=0;

    in = malloc(80);
    out = malloc(80);
    while (addr<src_end){
        addr+=sram_read_string(addr,in,CHAR_PER_LINE+1)+1;
        line_no++;
        sprintf(out,"%4d %s",line_no,in);
//        print_int(comm_channel, line_no,0);
        println(comm_channel,out);
    }
    free(in);
    free(out);
}


BOOL bRun;

#define CMD_COUNT (6)

static const char *cmd_list[CMD_COUNT]={
    "RUN",
    "LIST",
    "LOAD",
    "SAVE",
    "BYE",
    "CLEAR"
};

BOOL command(){
    int i;
    inp=0;
    next_token();
    if ((i=search_list(tok_value,cmd_list,CMD_COUNT))==-1){
        inp=0;
        return FALSE;
    }
    switch(i){
        case 0: //RUN
            break;
        case 1: // LIST
            list_source();
            break;
        case 2: // LOAD
            break;
        case 3: // SAVE
            break;
        case 4: // BYE
            bRun=FALSE;
            break;
        case 5: // CLEAR
            src_end=SRC_BASE;
            break;
    }//switch
    return TRUE;
}


void vpcBasic(){ 
    ram_free=free_heap()-1024;
    print(comm_channel,"code space: ");
    print_int(comm_channel,ram_free,0);
    crlf();
    ram_code=malloc(ram_free);
    here=ram_code;
    vars=ram_code+ram_free;
    bRun=TRUE;
    src_end=SRC_BASE;
    while (bRun){
        print(comm_channel,">");
        readline(comm_channel,src_line,CHAR_PER_LINE);
        if (strlen(src_line) && !command()) compile_run();
    }// while(1)
    free(ram_code);
}//vpcBasic()

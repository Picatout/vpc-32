#include <stdio.h>

#define HASH_BASE 68

typedef unsigned int name_hash;

name_hash hash( char *name){// hash le token courant.
    name_hash nh;
    int i;
    char c;
    nh=0;
    for (i=0;i<5 && *name!=0;i++){
        c = *name++ & 127;
        if (c>='a' && c<='z') c -= 32;
        else if (c>'z') c -= 26;
        nh *= HASH_BASE;
        nh += c-33;
    }
    return nh;
}//hash()

int parse_opt(int argc, char *argv[]){
	if (argc<3 || argv[1][0]!='-'){
		puts("USAGE: hash -t|-m nom [,nom]*");
		exit(1);
	}
	if (argv[1][1]=='m') return 1; else return 0;
}// parse_opt()

void main(int argc, char *argv[]){
	int i, opt;
	opt=parse_opt(argc,argv);
	for (i=2;i<argc;i++)
		if (opt)
	       printf("{%d, (code_ptr)%s},\n",hash(argv[i]), argv[i]);
		else
		   printf("%d, // %s\n", hash(argv[i]), argv[i]);
}


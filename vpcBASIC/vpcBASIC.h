/* 
 * File:   vpcBASIC.h
 * Author: Jacques Desch�nes
 *
 * Created on 30 mai 2013, 10:54
 */

#ifndef TINYFORTH_H
#define TINYFORTH_H
#include "vm.h"

#define SYSTEM_NAME "vpcBASIC "
#define SYSTEM_VERSION "0.01\r"

// index des variables
#define _BASE 0   // base num�rique
#define _HERE 1   // position libre dans dictionnaire ram_code[]
#define _INP  2   // pointeur tampon saisie interpr�teur
#define _HP   3   // pointeur HOLD
#define _PAD  4   // tampon pour convertir les nombres en cha�ne de caract�res.

extern StackVM(const unsigned char*, unsigned int*);
#endif	/* TINYFORTH_H */


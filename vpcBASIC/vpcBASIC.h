/* 
 * File:   vpcBASIC.h
 * Author: Jacques Deschênes
 *
 * Created on 30 mai 2013, 10:54
 */

#ifndef TINYFORTH_H
#define TINYFORTH_H
#include "vm.h"

#define SYSTEM_NAME "vpcBASIC "
#define SYSTEM_VERSION "0.01\r"

// index des variables
#define _BASE 0   // base numérique
#define _HERE 1   // position libre dans dictionnaire ram_code[]
#define _INP  2   // pointeur tampon saisie interpréteur
#define _HP   3   // pointeur HOLD
#define _PAD  4   // tampon pour convertir les nombres en chaîne de caractères.

extern StackVM(const unsigned char*, unsigned int*);
#endif	/* TINYFORTH_H */


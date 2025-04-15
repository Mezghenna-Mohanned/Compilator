#ifndef TS_H
#define TS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"

#define MAX_TYPE_LENGTH 50

typedef struct constant {
    char type[MAX_TYPE_LENGTH];
    union {
        int   i;
        float f;
        char *s;
    } valeur;
} constant;

typedef struct listeD {
    char *entite;
    char *type;
    int is_const;
    union {
        int   i;
        float f;
        char *s;
    } valeur;
    struct listeD *suivant;
} listeD;

typedef struct listeT {
    char *entite;
    char *type;
    int   taille;
    struct listeT *suivant;
} listeT;

typedef struct listeKW {
    char *mot;
    struct listeKW *suivant;
} listeKW;

typedef struct listeOP {
    char *op;
    struct listeOP *suivant;
} listeOP;


extern listeD  *TS;          /* variables & consts */
extern listeT  *TStab;       /* arrays             */
extern listeKW *TSkw;        /* keywords           */
extern listeOP *TSarith;     /* + – * /            */
extern listeOP *TScomp;      /* < > == …           */
extern listeOP *TSlogic;     /* AND OR !           */

#endif
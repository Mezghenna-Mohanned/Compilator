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
        int i;
        float f;
        char *s;
    } valeur;
} constant;

typedef struct listeD {
    char *entite;
    char *type;
    int is_const; /* 1 if it's a constant, 0 otherwise */
    union {
        int i;
        float f;
        char *s;
    } valeur;
    struct listeD *suivant;
} listeD;

typedef struct listeT {
    char *entite;
    char *type;
    int taille;
    union {
        int i;
        float f;
        char *s;
    } valeur;
    struct listeT *suivant;
} listeT;

extern listeD *TS;    /* Variables and constants */
extern listeT *TStab; /* Arrays */

#endif

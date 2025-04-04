#include "semantique.h"

/* Our global tables */
listeD *TS = NULL;
listeT *TStab = NULL;

/* 
 * Insert a variable/constant into TS (not an array).
 * Return -1 if already declared, else 0.
 */
int insererTableVar(char entite[], char type[], void *valeur) {
    // Check if 'entite' is already in TS
    listeD *p = TS;
    while (p) {
        if (strcmp(p->entite, entite) == 0) {
            return -1; /* already declared */
        }
        p = p->suivant;
    }
    // Insert at head
    listeD *newNode = (listeD *)malloc(sizeof(listeD));
    newNode->entite = strdup(entite);
    newNode->type   = strdup(type);
    newNode->is_const = 0; 
    // For now, we won't copy 'valeur' unless you do an initial value assignment
    newNode->valeur.i = 0; 
    newNode->suivant = TS;
    TS = newNode;
    return 0;
}

/*
 * Insert an array in TStab. Return -1 if declared, else 0.
 */
int insererTableTab(char entite[], char type[], int taille, void *valeur) {
    listeT *p = TStab;
    while (p) {
        if (strcmp(p->entite, entite) == 0) {
            return -1; 
        }
        p = p->suivant;
    }
    listeT *newNode = (listeT *)malloc(sizeof(listeT));
    newNode->entite = strdup(entite);
    newNode->type   = strdup(type);
    newNode->taille = taille;
    newNode->suivant = TStab;
    TStab = newNode;
    return 0;
}

/*
 * Look up a variable/constant in TS by name.
 */
listeD *chercherTS(char nom[]) {
    listeD *p = TS;
    while (p) {
        if (strcmp(p->entite, nom) == 0) {
            return p;
        }
        p = p->suivant;
    }
    return NULL;
}

/*
 * Look up an array in TStab by name; return its size or -1 if not found.
 */
int chercherTailleTab(char tete[]) {
    listeT *p = TStab;
    while (p) {
        if (strcmp(p->entite, tete) == 0) {
            return p->taille;
        }
        p = p->suivant;
    }
    return -1;
}

/* Functions used by lexical.l for debugging/logging */
void insererKeyword(const char* kw) {
    printf("Keyword: %s\n", kw);
}
void insererOperator(const char* op) {
    printf("Operator: %s\n", op);
}
void insererCompareOp(const char* op) {
    printf("CompareOp: %s\n", op);
}
void insererLogicOp(const char* op) {
    printf("LogicOp: %s\n", op);
}

/*
 * Display all symbols in TS and TStab, for debugging.
 */
void afficherToutesLesTablesSymboles(void) {
    printf("----- Table des symboles (Variables / Const) -----\n");
    listeD *p = TS;
    while (p) {
        printf("  %s : %s%s\n", p->entite, p->type, p->is_const ? " [const]" : "");
        p = p->suivant;
    }
    printf("----- Table des tableaux -----\n");
    listeT *q = TStab;
    while (q) {
        printf("  %s : %s[%d]\n", q->entite, q->type, q->taille);
        q = q->suivant;
    }
}

void afficherTStab(void) {
    printf("Affichage table des tableaux:\n");
    listeT *q = TStab;
    while (q) {
        printf("  %s : %s[%d]\n", q->entite, q->type, q->taille);
        q = q->suivant;
    }
}

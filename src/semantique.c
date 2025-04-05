#include "semantique.h"

listeD *TS = NULL;
listeT *TStab = NULL;

int insererTableVar(char entite[], char type[], void *valeur) {
    
    listeD *p = TS;
    while (p) {
        if (strcmp(p->entite, entite) == 0) {
            return -1;
        }
        p = p->suivant;
    }
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

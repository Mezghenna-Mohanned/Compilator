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
    newNode->valeur.i = 0;
    newNode->suivant = TS;
    TS = newNode;
    return 0;
}

int insererTableConst(char entite[], char type[], void *valeur) {
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
    newNode->is_const = 1;
    if (strcmp(type, "Int") == 0) {
        newNode->valeur.i = *((int *)valeur);
    } else if (strcmp(type, "Float") == 0) {
        newNode->valeur.f = *((float *)valeur);
    }
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
    printf("%sKeyword:%s %s\n", YELLOW, RESET, kw);
}
void insererOperator(const char* op) {
    printf("%sOperator:%s %s\n", YELLOW, RESET, op);
}
void insererCompareOp(const char* op) {
    printf("%sCompareOp:%s %s\n", YELLOW, RESET, op);
}
void insererLogicOp(const char* op) {
    printf("%sLogicOp:%s %s\n", YELLOW, RESET, op);
}

void afficherToutesLesTablesSymboles(void) {
    printf("%s+----------------------+------------+--------------+\n", YELLOW);
    printf("| %-20s | %-10s | %-12s |\n", "Identifier", "Type", "Properties");
    printf("+----------------------+------------+--------------+%s\n", RESET);
    listeD *p = TS;
    while (p) {
        printf("%s| %-20s | %-10s | %-12s |%s\n", CYAN, p->entite, p->type, (p->is_const ? "const" : "variable"), RESET);
        p = p->suivant;
    }
    printf("%s+----------------------+------------+--------------+\n\n%s", YELLOW, RESET);

    printf("%s+----------------------+------------+---------+\n", YELLOW);
    printf("| %-20s | %-10s | %-7s |\n", "Array", "Type", "Size");
    printf("+----------------------+------------+---------+%s\n", RESET);
    listeT *q = TStab;
    while (q) {
        printf("%s| %-20s | %-10s | %-7d |%s\n", CYAN, q->entite, q->type, q->taille, RESET);
        q = q->suivant;
    }
    printf("%s+----------------------+------------+---------+\n%s\n", YELLOW, RESET);
}

void afficherTStab(void) {
    printf("%s+----------------------+------------+---------+\n", YELLOW);
    printf("| %-20s | %-10s | %-7s |\n", "Array", "Type", "Size");
    printf("+----------------------+------------+---------+%s\n", RESET);
    listeT *q = TStab;
    while (q) {
        printf("%s| %-20s | %-10s | %-7d |%s\n", CYAN, q->entite, q->type, q->taille, RESET);
        q = q->suivant;
    }
    printf("%s+----------------------+------------+---------+\n%s\n", YELLOW, RESET);
}

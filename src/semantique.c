#include "semantique.h"

listeD  *TS      = NULL;
listeT  *TStab   = NULL;
listeKW *TSkw    = NULL;
listeOP *TSarith = NULL;
listeOP *TScomp  = NULL;
listeOP *TSlogic = NULL;

static int dejaDansOP(listeOP *l, const char *lex) {
    for ( ; l; l = l->suivant)
        if (strcmp(l->op, lex) == 0) return 1;
    return 0;
}
static int dejaDansKW(listeKW *l, const char *lex) {
    for ( ; l; l = l->suivant)
        if (strcmp(l->mot, lex) == 0) return 1;
    return 0;
}

int insererTableVar(char entite[], char type[], void *valeur)
{
    for (listeD *p = TS; p; p = p->suivant)
        if (strcmp(p->entite, entite) == 0) return -1;

    listeD *n = malloc(sizeof *n);
    n->entite   = strdup(entite);
    n->type     = strdup(type);
    n->is_const = 0;
    n->valeur.i = 0;
    n->suivant  = TS;
    TS = n;
    return 0;
}

int insererTableConst(char entite[], char type[], void *valeur)
{
    for (listeD *p = TS; p; p = p->suivant)
        if (strcmp(p->entite, entite) == 0) return -1;

    listeD *n  = malloc(sizeof *n);
    n->entite   = strdup(entite);
    n->type     = strdup(type);
    n->is_const = 1;
    if (strcmp(type, "Int") == 0)   n->valeur.i = *(int *)valeur;
    if (strcmp(type, "Float") == 0) n->valeur.f = *(float *)valeur;
    n->suivant  = TS;
    TS = n;
    return 0;
}

int insererTableTab(char entite[], char type[], int taille, void *valeur)
{
    (void)valeur;
    for (listeT *p = TStab; p; p = p->suivant)
        if (strcmp(p->entite, entite) == 0) return -1;

    listeT *n  = malloc(sizeof *n);
    n->entite  = strdup(entite);
    n->type    = strdup(type);
    n->taille  = taille;
    n->suivant = TStab;
    TStab = n;
    return 0;
}

listeD *chercherTS(char nom[])
{
    for (listeD *p = TS; p; p = p->suivant)
        if (strcmp(p->entite, nom) == 0) return p;
    return NULL;
}

int chercherTailleTab(char tete[])
{
    for (listeT *p = TStab; p; p = p->suivant)
        if (strcmp(p->entite, tete) == 0) return p->taille;
    return -1;
}

void insererKeyword(const char *kw)
{
    if (!dejaDansKW(TSkw, kw)) {
        listeKW *n = malloc(sizeof *n);
        n->mot   = strdup(kw);
        n->suivant = TSkw;
        TSkw = n;
    }
    printf("%sKeyword:%s %s\n", YELLOW, RESET, kw);
}

void insererOperator(const char *op)
{
    if (!dejaDansOP(TSarith, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op   = strdup(op);
        n->suivant = TSarith;
        TSarith = n;
    }
    printf("%sOperator:%s %s\n", YELLOW, RESET, op);
}

void insererCompareOp(const char *op)
{
    if (!dejaDansOP(TScomp, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op   = strdup(op);
        n->suivant = TScomp;
        TScomp = n;
    }
    printf("%sCompareOp:%s %s\n", YELLOW, RESET, op);
}

void insererLogicOp(const char *op)
{
    if (!dejaDansOP(TSlogic, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op   = strdup(op);
        n->suivant = TSlogic;
        TSlogic = n;
    }
    printf("%sLogicOp:%s %s\n", YELLOW, RESET, op);
}

static void afficherKW(void)
{
    puts("\nKeywords encountered:");
    puts("+----------------------+");
    for (listeKW *p = TSkw; p; p = p->suivant)
        printf("| %-20s |\n", p->mot);
    puts("+----------------------+");
}

static void afficherOP(const char *titre, listeOP *l)
{
    printf("\n%s:\n", titre);
    puts("+----------------------+");
    for (listeOP *p = l; p; p = p->suivant)
        printf("| %-20s |\n", p->op);
    puts("+----------------------+");
}

void afficherToutesLesTablesSymboles(void)
{
    printf("%s+----------------------+------------+--------------+\n", YELLOW);
    printf("| %-20s | %-10s | %-12s |\n", "Identifier", "Type", "Properties");
    printf("+----------------------+------------+--------------+%s\n", RESET);
    for (listeD *p = TS; p; p = p->suivant)
        printf("%s| %-20s | %-10s | %-12s |%s\n", CYAN,
               p->entite, p->type ? p->type : "--",
               p->is_const ? "const" : "variable", RESET);
    printf("%s+----------------------+------------+--------------+\n\n%s", YELLOW, RESET);

    printf("%s+----------------------+------------+---------+\n", YELLOW);
    printf("| %-20s | %-10s | %-7s |\n", "Array", "Type", "Size");
    printf("+----------------------+------------+---------+%s\n", RESET);
    for (listeT *q = TStab; q; q = q->suivant)
        printf("%s| %-20s | %-10s | %-7d |%s\n", CYAN,
               q->entite, q->type, q->taille, RESET);
    printf("%s+----------------------+------------+---------+\n%s", YELLOW, RESET);

    afficherKW();
    afficherOP("Arithmetic operators", TSarith);
    afficherOP("Comparison operators", TScomp);
    afficherOP("Logical operators",    TSlogic);
}

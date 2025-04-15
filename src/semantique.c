#include "semantique.h"
#include <stdio.h>

listeD  *TS      = NULL;   /* variables & constantes                      */
listeT  *TStab   = NULL;   /* tableaux                                    */
listeKW *TSkw    = NULL;   /* mots‑clés                                   */
listeOP *TSarith = NULL;   /* +  -  *  /                                  */
listeOP *TScomp  = NULL;   /* <  >  ==  !=  <=  >=                        */
listeOP *TSlogic = NULL;   /* AND  OR  !                                  */

static int dejaDansOP(listeOP *l, const char *lex)
{
    for (; l; l = l->suivant)
        if (strcmp(l->op, lex) == 0) return 1;
    return 0;
}
static int dejaDansKW(listeKW *l, const char *lex)
{
    for (; l; l = l->suivant)
        if (strcmp(l->mot, lex) == 0) return 1;
    return 0;
}

int insererTableVar(char entite[], char type[], void *valeur)
{
    (void)valeur;
    for (listeD *p = TS; p; p = p->suivant)
        if (!strcmp(p->entite, entite)) return -1;

    listeD *n  = malloc(sizeof *n);
    n->entite   = strdup(entite);
    n->type     = strdup(type);
    n->is_const = 0;

    if (!strcmp(type, "String"))
        n->valeur.s = malloc(100);  // Allocate string space
    else
        n->valeur.i = 0;  // Default for int/float

    n->suivant  = TS;
    TS = n;
    return 0;
}

int insererTableConst(char entite[], char type[], void *valeur)
{
    for (listeD *p = TS; p; p = p->suivant)
        if (!strcmp(p->entite, entite)) return -1;

    listeD *n  = malloc(sizeof *n);
    n->entite   = strdup(entite);
    n->type     = strdup(type);
    n->is_const = 1;

    if (!strcmp(type, "Int"))   n->valeur.i = *(int *)valeur;
    if (!strcmp(type, "Float")) n->valeur.f = *(float *)valeur;
    if (!strcmp(type, "String")) n->valeur.s = strdup((char *)valeur);

    n->suivant  = TS;
    TS = n;
    return 0;
}

int insererTableTab(char entite[], char type[], int taille, void *valeur)
{
    (void)valeur;
    for (listeT *p = TStab; p; p = p->suivant)
        if (!strcmp(p->entite, entite)) return -1;

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
        if (!strcmp(p->entite, nom)) return p;
    return NULL;
}

int chercherTailleTab(char tete[])
{
    for (listeT *p = TStab; p; p = p->suivant)
        if (!strcmp(p->entite, tete)) return p->taille;
    return -1;
}

void insererKeyword(const char *kw)
{
    if (!dejaDansKW(TSkw, kw)) {
        listeKW *n = malloc(sizeof *n);
        n->mot     = strdup(kw);
        n->suivant = TSkw;
        TSkw = n;
    }
}

void insererOperator(const char *op)
{
    if (!dejaDansOP(TSarith, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op      = strdup(op);
        n->suivant = TSarith;
        TSarith = n;
    }
}

void insererCompareOp(const char *op)
{
    if (!dejaDansOP(TScomp, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op      = strdup(op);
        n->suivant = TScomp;
        TScomp = n;
    }
}

void insererLogicOp(const char *op)
{
    if (!dejaDansOP(TSlogic, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op      = strdup(op);
        n->suivant = TSlogic;
        TSlogic = n;
    }
}

static void ligne_sep(int len)
{
    for (int i = 0; i < len; ++i) putchar('-');
    putchar('\n');
}

static void afficherKW(void)
{
    puts("\n/************** Table des symboles mots cles **************/");
    ligne_sep(31);
    printf("| %-20s | %-8s |\n", "NomEntite", "CodeEntite");
    ligne_sep(31);
    for (listeKW *p = TSkw; p; p = p->suivant)
        printf("| %-20s | %-8s |\n", p->mot, "Mot cle");
    ligne_sep(31);
}

static void afficherOP(const char *titre, const char *code, listeOP *l)
{
    printf("\n/************** %s **************/\n", titre);
    ligne_sep(31);
    printf("| %-20s | %-8s |\n", "NomEntite", "CodeEntite");
    ligne_sep(31);
    for (listeOP *p = l; p; p = p->suivant)
        printf("| %-20s | %-8s |\n", p->op, code);
    ligne_sep(31);
}

void afficherToutesLesTablesSymboles(void)
{
    printf("%s+----------------------+------------+--------------+\n", YELLOW);
    printf("| %-20s | %-10s | %-12s |\n",
           "Identifier", "Type", "Properties");
    printf("+----------------------+------------+--------------+%s\n", RESET);
    for (listeD *p = TS; p; p = p->suivant)
        printf("%s| %-20s | %-10s | %-12s |%s\n", CYAN,
               p->entite, p->type ? p->type : "--",
               p->is_const ? "const" : "variable", RESET);
    printf("%s+----------------------+------------+--------------+\n\n%s",
           YELLOW, RESET);

    printf("%s+----------------------+------------+---------+\n", YELLOW);
    printf("| %-20s | %-10s | %-7s |\n", "Array", "Type", "Size");
    printf("+----------------------+------------+---------+%s\n", RESET);
    for (listeT *q = TStab; q; q = q->suivant)
        printf("%s| %-20s | %-10s | %-7d |%s\n", CYAN,
               q->entite, q->type, q->taille, RESET);
    printf("%s+----------------------+------------+---------+\n%s",
           YELLOW, RESET);

    afficherKW();
    afficherOP("Table des symboles operateurs arithmetiques",
               "Op arith", TSarith);
    afficherOP("Table des symboles operateurs de comparaison",
               "Op comp",  TScomp);
    afficherOP("Table des symboles operateurs logiques",
               "Op log",   TSlogic);
}

int errorCount = 0;


void finalizeAndPrintTS(void)
{
    if (errorCount > 0)
    {
        printf("\n%s%d errors found. Symbol Table:\n", RED, errorCount);
    }
    else
    {
        printf("\n%sNo errors. Symbol Table:\n", GREEN);
    }
    afficherToutesLesTablesSymboles();
}

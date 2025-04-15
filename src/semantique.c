#include "semantique.h"
#include <stdio.h>

listeD  *TS      = NULL;
listeT  *TStab   = NULL;
listeKW *TSkw    = NULL;
listeOP *TSarith = NULL;
listeOP *TScomp  = NULL;
listeOP *TSlogic = NULL;

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
        n->valeur.s = malloc(100);
    else
        n->valeur.i = 0;

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
    printf("%sOperator:%s %s\n", YELLOW, RESET, op);
}

void insererCompareOp(const char *op)
{
    if (!dejaDansOP(TScomp, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op      = strdup(op);
        n->suivant = TScomp;
        TScomp = n;
    }
    printf("%sCompareOp:%s %s\n", YELLOW, RESET, op);
}

void insererLogicOp(const char *op)
{
    if (!dejaDansOP(TSlogic, op)) {
        listeOP *n = malloc(sizeof *n);
        n->op      = strdup(op);
        n->suivant = TSlogic;
        TSlogic = n;
    }
    printf("%sLogicOp:%s %s\n", YELLOW, RESET, op);
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

void lire(char *idf)
{
    listeD *var = chercherTS(idf);
    if (!var) {
        yyerror("Variable non déclarée pour input()");
        return;
    }
    freopen("CON", "r", stdin);

    if (strcmp(var->type, "Int") == 0) {
        printf(">> Saisir un entier (%s): ", idf);
        scanf("%d", &var->valeur.i);
    } else if (strcmp(var->type, "Float") == 0) {
        printf(">> Saisir un réel (%s): ", idf);
        scanf("%f", &var->valeur.f);
    } else if (strcmp(var->type, "String") == 0) {
        printf(">> Saisir un texte (%s): ", idf);
        scanf(" %[^\n]", var->valeur.s);
    } else {
        yyerror("Type non supporté dans input()");
    }
}

void afficher(constant *val)
{
    if (!val) {
        yyerror("Expression invalide dans output()");
        return;
    }

    if (strcmp(val->type, "Int") == 0) {
        printf("%d\n", val->valeur.i);
    } else if (strcmp(val->type, "Float") == 0) {
        printf("%f\n", val->valeur.f);
    } else if (strcmp(val->type, "String") == 0) {
        printf("%s\n", val->valeur.s);
    } else {
        yyerror("Type non supporté dans output()");
    }
}
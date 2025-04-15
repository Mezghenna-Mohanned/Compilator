/*======================================================================
 *  semantique.h  –  interface des services sémantiques + helpers
 *  (version étendue : tables Mots‑clés & Opérateurs)
 *====================================================================*/
#ifndef SEMANTIQUE_H
#define SEMANTIQUE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ts.h"     /* contient les nouvelles listes TSkw / TSarith … */
#include "color.h"  /* pour l’affichage coloré */

/* ------------------------------------------------------------------ */
/*  Fonctions exportées vers Bison / Flex                              */
/* ------------------------------------------------------------------ */
extern void   yyerror(const char *s);

/* --- Tables des identificateurs, constantes et tableaux ----------- */
extern int    insererTableVar   (char entite[], char type[], void *valeur);
extern int    insererTableConst (char entite[], char type[], void *valeur);
extern int    insererTableTab   (char entite[], char type[],
                                 int taille, void *valeur);
extern listeD *chercherTS       (char nom[]);
extern int    chercherTailleTab (char tete[]);

/* --- Nouvelles tables : Mots‑clés & Opérateurs -------------------- */
void insererKeyword   (const char *kw);   /* ex. MainPrgm, Var …      */
void insererOperator  (const char *op);   /* +  -  *  /               */
void insererCompareOp (const char *op);   /* <  >  ==  !=  <=  >=     */
void insererLogicOp   (const char *op);   /* AND  OR  !               */

static inline void insertionTS_et_verif_double_declaration(listeD *l,char type[])
{
    listeD *var;
    for (var = l; var != NULL; var = var->suivant)
        if (insererTableVar(var->entite, type, &(var->valeur)) == -1)
            yyerror("Double déclaration");
}

static inline void insertionTStab_et_verif_double_declaration(listeT *l,char type[])
{
    listeT *var;
    for (var = l; var != NULL; var = var->suivant)
        if (insererTableTab(var->entite, type, var->taille,
                            NULL) == -1) // Replace with NULL or appropriate logic
            yyerror("Double déclaration");
}

static inline listeD *creationVarlist1(char *tete, listeD *l)
{
    listeD *new_var = (listeD *)malloc(sizeof(listeD));
    new_var->entite  = strdup(tete);
    new_var->suivant = l;
    new_var->type    = NULL;
    new_var->is_const= 0;
    return new_var;
}

static inline listeT *creationVarlist2(char *tete, int taille, listeT *l)
{
    listeT *new_var = (listeT *)malloc(sizeof(listeT));
    new_var->entite  = strdup(tete);
    new_var->taille  = taille;
    new_var->suivant = l;
    new_var->type    = NULL;
    return new_var;
}

static inline listeD *constDeclaration(constant *p, char *tete, int hasValue)
{
    listeD *new_var = (listeD *)malloc(sizeof(listeD));
    new_var->entite  = strdup(tete);
    if (hasValue && p != NULL) {
        new_var->type = strdup(p->type);
        if (strcmp(p->type, "Int") == 0)
            new_var->valeur.i = p->valeur.i;
        else if (strcmp(p->type, "Float") == 0)
            new_var->valeur.f = p->valeur.f;
    } else
        new_var->type = NULL;

    new_var->is_const = 1;
    new_var->suivant  = NULL;
    return new_var;
}

static inline void gestionErreurAssig(constant *p, char *tete)
{
    listeD *var = chercherTS(tete);
    if (var == NULL) {
        yyerror("Identificateur non déclaré");
    } else if (var->is_const == 1) {
        yyerror("Tentative de modification d'une constante");
    } else {
        if (var->type && strcmp(var->type, p->type) == 0) {
            if (strcmp(var->type, "Int") == 0)
                var->valeur.i = p->valeur.i;
            else if (strcmp(var->type, "Float") == 0)
                var->valeur.f = p->valeur.f;
        } else {
            if (var->type && strcmp(var->type, "Int") == 0 &&
                strcmp(p->type, "Float") == 0)
                yyerror("Incompatibilité de types (Float -> Int)");
            else if (var->type && strcmp(var->type, "Float") == 0 &&
                     strcmp(p->type, "Int") == 0)
                var->valeur.f = (float)p->valeur.i;
            else
                yyerror("Incompatibilité de types");
        }
    }
    free(p);
}

static inline void gestion_taille_tableau(char *tete, int indice)
{
    int taille = chercherTailleTab(tete);
    if (taille == -1)
        yyerror("Identificateur (tableau) non déclaré");
    else if (indice < 0 || indice >= taille)
        yyerror("Dépassement de la taille du tableau");
}

static inline constant *gestionErreurType(int op,
                                          constant *exp1,
                                          constant *exp2)
{
    constant *res = (constant *)malloc(sizeof(constant));
    if (!res) {
        yyerror("Erreur d'allocation mémoire");
        return NULL;
    }
    strcpy(res->type, "");

    if (!strcmp(exp1->type, "Int") && !strcmp(exp2->type, "Int")) {
        strcpy(res->type, "Int");
        int left  = exp1->valeur.i;
        int right = exp2->valeur.i;
        switch (op) {
            case 1: res->valeur.i = left + right; break;
            case 2: res->valeur.i = left - right; break;
            case 3: res->valeur.i = left * right; break;
            case 4:
                if (right == 0) {
                    yyerror("Division par zéro (entier)");
                    free(res); return NULL;
                }
                res->valeur.i = left / right; break;
            default:
                yyerror("Operateur arithmetique inconnu");
                free(res); return NULL;
        }
    } else if ((!strcmp(exp1->type, "Float") && !strcmp(exp2->type, "Float")) || 
               (!strcmp(exp1->type, "Int")   && !strcmp(exp2->type, "Float")) ||
               (!strcmp(exp1->type, "Float") && !strcmp(exp2->type, "Int"))) {
        strcpy(res->type, "Float");
        float left  = !strcmp(exp1->type, "Int") ? (float)exp1->valeur.i
                                                 : exp1->valeur.f;
        float right = !strcmp(exp2->type, "Int") ? (float)exp2->valeur.i
                                                 : exp2->valeur.f;
        switch (op) {
            case 1: res->valeur.f = left + right; break;
            case 2: res->valeur.f = left - right; break;
            case 3: res->valeur.f = left * right; break;
            case 4:
                if (right == 0.0f) {
                    yyerror("Division par zero (réel)");
                    free(res); return NULL;
                }
                res->valeur.f = left / right; break;
            default:
                yyerror("Operateur arithmetique inconnu");
                free(res); return NULL;
        }
    } else {
        yyerror("Operation arithmetique non supportee pour ces types");
        free(res); return NULL;
    }

    return res;
}

static inline void gestionIncompatibilite(constant *exp1, constant *exp2)
{
    if (!exp1 || !exp2)
        yyerror("Expression invalide dans la condition");
    else if ((strcmp(exp1->type, "Int")   && strcmp(exp1->type, "Float")) ||
             (strcmp(exp2->type, "Int")   && strcmp(exp2->type, "Float")))
        yyerror("Comparaison non supportée pour ces types");
}

static inline void gestionIncompatibiliteEQ_NEQ(constant *exp1,
                                                constant *exp2)
{
    if (!exp1 || !exp2) {
        yyerror("Expression invalide dans la condition"); return;
    }
    if (!((!strcmp(exp1->type, "Int")   || !strcmp(exp1->type, "Float")) &&
          (!strcmp(exp2->type, "Int")   || !strcmp(exp2->type, "Float"))))
        yyerror("Types incompatibles pour la comparaison d'égalité");
}

static inline void gestion_io_statement(int isArray, char *tete, int index)
{
    if (!isArray) {
        if (!chercherTS(tete))
            yyerror("Variable non déclarée pour I/O");
    } else {
        int taille = chercherTailleTab(tete);
        if (taille == -1)
            yyerror("Tableau non déclaré pour I/O");
        else if (index < 0 || index >= taille)
            yyerror("Indice hors limite pour I/O");
    }
}

static inline constant *gestionIDF(char *tete)
{
    listeD *var = chercherTS(tete);
    constant *variable = (constant *)malloc(sizeof(constant));
    if (!variable) {
        yyerror("Erreur d'allocation mémoire pour variable");
        return NULL;
    }
    if (!var)
        yyerror("Identificateur non déclaré");
    else {
        strcpy(variable->type, var->type ? var->type : "");
        if (!strcmp(var->type, "Int"))
            variable->valeur.i = var->valeur.i;
        else if (!strcmp(var->type, "Float"))
            variable->valeur.f = var->valeur.f;
    }
    return variable;
}

static inline void gererTaille(char *teteTableau, char *idxName)
{
    listeD *var = chercherTS(idxName);
    if (!var)
        yyerror("Index non déclaré");
    else if (strcmp(var->type, "Int"))
        yyerror("Index du tableau n'est pas de type entier (Int)");
    else if (chercherTailleTab(teteTableau) < var->valeur.i || var->valeur.i < 0)
        yyerror("Indice hors limite");
}

#endif /* SEMANTIQUE_H */

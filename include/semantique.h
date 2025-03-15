#ifndef SEMANTIQUE_H
#define SEMANTIQUE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "color.h"
extern void yyerror(const char *s);
extern int insererTableau(char entite[], char type[], int taille, void *valeur);
extern int insererTS(char entite[], char type[], int is_const, void *valeur);
extern listeD *chercherTS(char nom[]);
extern int rechercherTaille(char tete[]);

void insererMotCle(const char* mot);
void insererOperateurNoAsso(const char* op);
void insererOperateurLogique(const char* op);
void insererOperateurArithmetique(const char* op);

extern void gestionIncompatibiliteEQ_NEQ(constant *exp1, constant *exp2);
extern void gestionErreurAssig(constant *p, char tete[]);
extern constant *gestionIDF(char tete[]);

static inline void insertionTS_et_verifications_double_declarations(listeD *l, char type[]) {
    listeD *var;
    for (var = l; var != NULL; var = var->suivant) {
        if (insererTS(var->entite, type, var->is_const, &(var->valeur)) == -1) {
            yyerror("Double déclaration");
        }
    }
}


void insertionTStab_et_verifications_double_declarations(listeT *l, char type[]) {
    listeT *var;
    for (var = l; var != NULL; var = var->suivant) {
        if (insererTableau(var->entite, type, var->taille, &(var->valeur)) == -1) {
            yyerror("Double déclaration");
        }
    }
}

listeD *creationVarlist1(char *tete, listeD *l) {
    listeD *new_var = (listeD *)malloc(sizeof(listeD));
    new_var->entite = strdup(tete);
    new_var->is_const = 0;
    new_var->suivant = l;
    new_var->type = NULL;
    return new_var;
}

listeT *creationVarlist2(char *tete, int taille, listeT *l) {
    listeT *new_var = (listeT *)malloc(sizeof(listeT));
    new_var->entite = strdup(tete);
    new_var->taille = taille;
    new_var->suivant = l;
    new_var->type = NULL;
    return new_var;
}

listeD *constDeclaration(constant *p, char *tete, int j) {
    listeD *new_var = (listeD *)malloc(sizeof(listeD));
    new_var->entite = strdup(tete);
    if (j == 1 && p != NULL) {
        if (strcmp(p->type, "NUM") == 0) {
            new_var->valeur.i = p->valeur.i;
            new_var->type = strdup("NUM");
        } else if (strcmp(p->type, "REAL") == 0) {
            new_var->valeur.f = p->valeur.f;
            new_var->type = strdup("REAL");
        } else if (strcmp(p->type, "TEXT") == 0) {
            new_var->valeur.s = strdup(p->valeur.s);
            new_var->type = strdup("TEXT");
        }
    } else {
        new_var->type = NULL;
    }
    new_var->is_const = 1;
    new_var->suivant = NULL;
    return new_var;
}

void gestionErreurAssig(constant *p, char *tete) {
    listeD *var = chercherTS(tete);
    if (var == NULL) {
        yyerror("Variable non déclarée");
    } else if (var->is_const == 1) {
        yyerror("Tentative de modification d'une constante");
    } else {
        if (var->type && strcmp(var->type, p->type) == 0) {
            if (strcmp(var->type, "NUM") == 0) {
                var->valeur.i = p->valeur.i;
            } else if (strcmp(var->type, "REAL") == 0) {
                var->valeur.f = p->valeur.f;
            } else if (strcmp(var->type, "TEXT") == 0) {
                free(var->valeur.s);
                var->valeur.s = strdup(p->valeur.s);
            }
        } else {
            if (var->type && strcmp(var->type, "NUM") == 0 && strcmp(p->type, "REAL") == 0) {
                yyerror("Incompatibilité de types dans l'affectation");
            } else if (var->type && strcmp(var->type, "REAL") == 0 && strcmp(p->type, "NUM") == 0) {
                var->valeur.f = (float)p->valeur.i;
            } else if ((var->type && strcmp(var->type, "TEXT") == 0 && (strcmp(p->type, "NUM") == 0 || strcmp(p->type, "REAL") == 0))
                    || (strcmp(p->type, "TEXT") == 0 && (var->type && (strcmp(var->type, "NUM") == 0 || strcmp(var->type, "REAL") == 0)))) {
                yyerror("Incompatibilité de types dans l'affectation à cause du type TEXT");
            }
        }
    }
    free(p);
}

void gestion_taille_tableau(char *tete, int tailleT) {
    int taille = rechercherTaille(tete);
    if (taille == -1) {
        yyerror("Variable non déclarée");
    } else if (tailleT >= taille || tailleT < 0) {
        yyerror("Indice hors limites");
    }
}

constant *gestionErreurType(int i, constant *exp1, constant *exp2) {
    constant *var = (constant *)malloc(sizeof(constant));
    if (var == NULL) {
        yyerror("Erreur d'allocation mémoire");
        return NULL;
    }
    switch (i) {
        case 1: 
        case 2: 
        case 3: 
        case 4:
            if (strcmp(exp1->type, exp2->type) == 0) {
                if (strcmp(exp1->type, "NUM") == 0) {
                    strcpy(var->type, "NUM");
                    if (i == 1) var->valeur.i = exp1->valeur.i + exp2->valeur.i;
                    else if (i == 2) var->valeur.i = exp1->valeur.i - exp2->valeur.i;
                    else if (i == 3) var->valeur.i = exp1->valeur.i * exp2->valeur.i;
                    else if (i == 4) {
                        if (exp2->valeur.i == 0) {
                            yyerror("Division par zéro");
                        } else {
                            var->valeur.i = exp1->valeur.i / exp2->valeur.i;
                        }
                    }
                } else if (strcmp(exp1->type, "REAL") == 0) {
                    strcpy(var->type, "REAL");
                    if (i == 1) var->valeur.f = exp1->valeur.f + exp2->valeur.f;
                    else if (i == 2) var->valeur.f = exp1->valeur.f - exp2->valeur.f;
                    else if (i == 3) var->valeur.f = exp1->valeur.f * exp2->valeur.f;
                    else if (i == 4) {
                        if (exp2->valeur.f == 0.0) {
                            yyerror("Division par zéro");
                        } else {
                            var->valeur.f = exp1->valeur.f / exp2->valeur.f;
                        }
                    }
                } else {
                    yyerror("Opération non supportée pour le type TEXT");
                }
            } else if ((strcmp(exp1->type, "NUM") == 0 && strcmp(exp2->type, "REAL") == 0)
                    || (strcmp(exp1->type, "REAL") == 0 && strcmp(exp2->type, "NUM") == 0)) {
                float val1 = (strcmp(exp1->type, "NUM") == 0) ? (float)exp1->valeur.i : exp1->valeur.f;
                float val2 = (strcmp(exp2->type, "NUM") == 0) ? (float)exp2->valeur.i : exp2->valeur.f;
                strcpy(var->type, "REAL");
                if (i == 1) var->valeur.f = val1 + val2;
                else if (i == 2) var->valeur.f = val1 - val2;
                else if (i == 3) var->valeur.f = val1 * val2;
                else if (i == 4) {
                    if (val2 == 0.0) {
                        yyerror("Division par zéro");
                        return var;
                    } else {
                        var->valeur.f = val1 / val2;
                    }
                }
            } else if (strcmp(exp1->type, "TEXT") == 0 || strcmp(exp2->type, "TEXT") == 0) {
                yyerror("Impossible de faire une opération sur un type TEXT");
            }
            break;
        default:
            yyerror("Opération non reconnue");
    }
    return var;
}

void gestionIncompatibilite(constant *exp1, constant *exp2) {
    if (exp1 == NULL || exp2 == NULL) {
        yyerror("Expression invalide dans la condition");
    } else if ((strcmp(exp1->type, "TEXT") == 0 || strcmp(exp2->type, "TEXT") == 0)) {
        yyerror("Comparaison non supportée pour le type TEXT");
    }
}

void gestionIncompatibiliteEQ_NEQ(constant *exp1, constant *exp2) {
    if (exp1 == NULL || exp2 == NULL) {
        yyerror("Expression invalide dans la condition");
    } else if (strcmp(exp1->type, exp2->type) != 0) {
        yyerror("Types incompatibles pour la comparaison d'égalité");
    }
}

void gestion_io_statement(int i, char *tete, int taille) {
    if (i == 0) {
        if (chercherTS(tete) == NULL) {
            yyerror("Variable non déclarée");
        }
    }
    if (i == 1) {
        if (rechercherTaille(tete) == (-1)) {
            yyerror("Variable non déclarée");
        } else if (rechercherTaille(tete) < taille) {
            yyerror("Indice hors limites");
        }
    }
}

constant *gestionIDF(char *tete) {
    listeD *var = chercherTS(tete);
    constant *variable = (constant *)malloc(sizeof(constant));
    if (var == NULL) {
        yyerror("Variable non déclarée");
    } else {
        strcpy(variable->type, var->type);
        if (strcmp(var->type, "NUM") == 0) {
            variable->valeur.i = var->valeur.i;
        } else if (strcmp(var->type, "REAL") == 0) {
            variable->valeur.f = var->valeur.f;
        } else if (strcmp(var->type, "TEXT") == 0) {
            if (var->valeur.s) {
                size_t len = strlen(var->valeur.s);
                variable->valeur.s = (char *)malloc(len + 1);
                if (variable->valeur.s) {
                    strcpy(variable->valeur.s, var->valeur.s);
                } else {
                    yyerror("Erreur d'allocation mémoire");
                }
            } else {
                variable->valeur.s = NULL;
            }
        }
    }
    return variable;
}

void gererTaille(char *teteTableau, char *tete) {
    listeD *var = chercherTS(tete);
    if (var == NULL) {
        yyerror("Indice du tableau non déclarée");
    } else if (strcmp(var->type, "NUM") != 0) {
        yyerror("Indice du tableau n'est pas de type entier");
    } else if (rechercherTaille(teteTableau) < var->valeur.i) {
        yyerror("Indice hors limite");
    }
}

#endif
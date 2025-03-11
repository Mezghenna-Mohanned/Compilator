#include "semantique.h"
#include "ts.h"

struct listeD *TS = NULL;
struct listeT *TStab = NULL;

int insererTS(char entite[], char type[], int is_const, void *valeur) {
    struct listeD *new_node = (struct listeD *)malloc(sizeof(struct listeD));
    if (!new_node) {
        yyerror("Memory allocation error");
        return -1;
    }

    strcpy(new_node->entite, entite);
    strcpy(new_node->type, type);
    new_node->is_const = is_const;
    if (strcmp(type, "NUM") == 0) {
        new_node->valeur.i = ((struct constant *)valeur)->valeur.i;
    } else if (strcmp(type, "REAL") == 0) {
        new_node->valeur.f = ((struct constant *)valeur)->valeur.f;
    } else if (strcmp(type, "TEXT") == 0) {
        new_node->valeur.s = strdup(((struct constant *)valeur)->valeur.s);
    }
    new_node->suivant = TS;
    TS = new_node;
    return 0;
}

int insererTableau(char entite[], char type[], int taille, void *valeur) {
    struct listeT *new_node = (struct listeT *)malloc(sizeof(struct listeT));
    if (!new_node) {
        yyerror("Memory allocation error");
        return -1;
    }

    strcpy(new_node->entite, entite);
    strcpy(new_node->type, type);
    new_node->taille = taille;
    if (strcmp(type, "NUM") == 0) {
        new_node->valeur.i = ((struct constant *)valeur)->valeur.i;
    } else if (strcmp(type, "REAL") == 0) {
        new_node->valeur.f = ((struct constant *)valeur)->valeur.f;
    } else if (strcmp(type, "TEXT") == 0) {
        new_node->valeur.s = strdup(((struct constant *)valeur)->valeur.s);
    }
    new_node->suivant = TStab;
    TStab = new_node;
    return 0;
}

struct listeD *chercherTS(char nom[]) {
    struct listeD *current = TS;
    while (current != NULL) {
        if (strcmp(current->entite, nom) == 0) {
            return current;
        }
        current = current->suivant;
    }
    return NULL;
}

int rechercherTaille(char tete[]) {
    struct listeT *current = TStab;
    while (current != NULL) {
        if (strcmp(current->entite, tete) == 0) {
            return current->taille;
        }
        current = current->suivant;
    }
    return -1;
}

void inserstionTS_et_verifications_double_declarations(listeD *i, char type[], int isconst) {
    struct listeD *var;
    if (isconst == 0) {
        for (var = i; var != NULL; var = var->suivant) {
            if (insererTS(var->entite, type, 0, &(var->valeur)) == -1) {
                yyerror("Double déclaration");
            }
        }
    } else {
        for (var = i; var != NULL; var = var->suivant) {
            if (insererTS(var->entite, type, 1, &(var->valeur)) == -1) {
                yyerror("Double déclaration");
            }
        }
    }
}

void inserstionTStab_et_verifications_double_declarations(listeT *i, char type[]) {
    struct listeT *var;
    for (var = i; var != NULL; var = var->suivant) {
        if (insererTableau(var->entite, type, var->taille, &(var->valeur)) == -1) {
            yyerror("Double déclaration");
        }
    }
}

struct listeD *creationVarlist1(char tete[], listeD *i) {
    struct listeD *new_var = (struct listeD *)malloc(sizeof(struct listeD));
    strcpy(new_var->entite, tete);
    new_var->is_const = 0;
    new_var->suivant = i;
    return new_var;
}

struct listeT *creationVarlist2(char tete[], int taille, listeT *i) {
    struct listeT *new_var = (struct listeT *)malloc(sizeof(struct listeT));
    strcpy(new_var->entite, tete);
    new_var->taille = taille;
    new_var->suivant = i;
    return new_var;
}

void gestionErreurAssig(constant *p, char tete[]) {
    struct listeD *var = chercherTS(tete);
    if (var == NULL) {
        yyerror("Variable non déclarée");
    } else if (var->is_const == 1) {
        yyerror("Tentative de modification d'une constante");
    } else {
        if (strcmp(var->type, p->type) == 0) {
            if (strcmp(var->type, "NUM") == 0) {
                var->valeur.i = p->valeur.i;
            } else if (strcmp(var->type, "REAL") == 0) {
                var->valeur.f = p->valeur.f;
            } else if (strcmp(var->type, "TEXT") == 0) {
                free(var->valeur.s);
                var->valeur.s = strdup(p->valeur.s);
            }
        } else {
            if (strcmp(var->type, "NUM") == 0 && strcmp(p->type, "REAL") == 0) {
                yyerror("Incompatibilité de types dans l'affectation");
            } else if (strcmp(var->type, "REAL") == 0 && strcmp(p->type, "NUM") == 0) {
                var->valeur.f = (float)p->valeur.i;
            } else if ((strcmp(var->type, "TEXT") == 0 && (strcmp(p->type, "NUM") == 0 || strcmp(p->type, "REAL") == 0))
                    || (strcmp(p->type, "TEXT") == 0 && (strcmp(var->type, "NUM") == 0 || strcmp(var->type, "REAL") == 0))) {
                yyerror("Incompatibilité de types dans l'affectation à cause du type TEXT");
            }
        }
    }
    free(p);
}

void gestion_taille_tableau(char tete[], int tailleT) {
    int taille = rechercherTaille(tete);
    if (taille == -1) {
        yyerror("Variable non déclarée");
    } else if (tailleT >= taille || tailleT < 0) {
        yyerror("Indice hors limites");
    }
}

struct constant *gestionErreurType(int i, constant *exp1, constant *exp2) {
    struct constant *var = (struct constant *)malloc(sizeof(struct constant));
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

void gestionIncompatiblite(constant *exp1, constant *exp2) {
    if (exp1 == NULL || exp1 == NULL) {
        yyerror("Expression invalide dans la condition");
    } else if ((strcmp(exp1->type, "TEXT") == 0 || strcmp(exp2->type, "TEXT") == 0)
            || (strcmp(exp1->type, "TEXT") == 0 && strcmp(exp2->type, "TEXT") == 0)) {
        yyerror("Comparaison non supportée pour le type TEXT");
    }
}

void gestionIcompatibilitéEQ_NEQ(constant *exp1, constant *exp2) {
    if (exp1 == NULL || exp2 == NULL) {
        yyerror("Expression invalide dans la condition");
    } else if (strcmp(exp1->type, exp2->type) != 0) {
        yyerror("Types incompatibles pour la comparaison d'égalité");
    }
}

void gestion_io_statemnt(int i, char tete[], int taille) {
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

struct constant *gestionIDF(char tete[]) {
    struct listeD *var = chercherTS(tete);
    struct constant *variable = (struct constant *)malloc(sizeof(struct constant));
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

void gererTaille(char teteTableau[], char tete[]) {
    struct listeD *var = chercherTS(tete);
    if (var == NULL) {
        yyerror("Indice du tableau non déclarée");
    } else if (strcmp(var->type, "NUM") != 0) {
        yyerror("Indice du tableau n'est pas de type entier");
    } else if (rechercherTaille(teteTableau) < var->valeur.i) {
        yyerror("Indice hors limite");
    }
}

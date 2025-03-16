#include "semantique.h"

listeD *TS = NULL;
listeT *TStab = NULL;

int insererTS(char entite[], char type[], int is_const, void *valeur) {
    return 0;
}

int insererTableau(char entite[], char type[], int taille, void *valeur) {
    return 0;
}

listeD *chercherTS(char nom[]) {
    return NULL;
}

int rechercherTaille(char tete[]) {
    return -1;
}

void insererMotCle(const char* mot) {
    printf("Mot clé inséré: %s\n", mot);
}

void insererOperateurNoAsso(const char* op) {
    printf("Opérateur non associatif inséré: %s\n", op);
}

void insererOperateurLogique(const char* op) {
    printf("Opérateur logique inséré: %s\n", op);
}

void insererOperateurArithmetique(const char* op) {
    printf("Opérateur arithmétique inséré: %s\n", op);
}


void afficherToutesLesTablesSymboles(void) {
    printf("Symbol tables (TS and TStab) are not implemented yet.\n");
}

void afficherTStab(void) {
    printf("Tableau symbolique non implémenté.\n");
}

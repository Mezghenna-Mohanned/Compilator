#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listeD {
    char *entite;
    char *type;
    int is_const;
    union {          // YAAAW HEDI MCHI UNION TE3 BISON
        int i;
        float f;
        char *s;
    } valeur;
    struct listeD *suivant;
} listeD;

typedef struct listeT {
    char *entite;
    char *type;
    int taille;
    union {
        int i;
        float f;
        char *s;
    } valeur;
    struct listeT *suivant;
} listeT;

extern listeD *TS;
extern listeT *TStab;

typedef struct liste {
    char *entite;
    struct liste *suivant;
} liste;

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"

void afficherListe(const char *titre, liste *tete) {
    printf(CYAN "╭─────────────────────────────────────────────╮\n" RESET);
    printf(CYAN "│ " GREEN "%-43s" CYAN " │\n", titre);
    printf(CYAN "├─────────────────────────────────────────────┤\n" RESET);
    liste *current = tete;
    while (current != NULL) {
        printf(CYAN "│ " YELLOW "%-43s" CYAN " │\n", current->entite);
        current = current->suivant;
    }
    printf(CYAN "╰─────────────────────────────────────────────╯\n\n" RESET);
}

void afficherToutesLesTablesSymboles() {
    printf("\n" CYAN "╔════════════════════════════════════════════════════════════════════════╗\n" RESET);
    printf(CYAN "║" GREEN "                TABLE DES SYMBOLES PRINCIPALE                         " CYAN "║\n" RESET);
    printf(CYAN "╠═════════════════════╦════════════╦════════════╦══════════════════════════════╣\n" RESET);
    printf(CYAN "║ " YELLOW "Entité" RESET "              ║ " YELLOW "Type" RESET "      ║ " YELLOW "Constante" RESET "  ║ " YELLOW "Valeur" RESET "                     ║\n");
    printf(CYAN "╠═════════════════════╬════════════╬════════════╬══════════════════════════════╣\n" RESET);
    
    listeD *current = TS;
    while (current != NULL) {
        printf(CYAN "║ %-18s ║ %-10s ║ %-10s ║ ", 
               current->entite, 
               current->type, 
               current->is_const ? "Oui" : "Non");
        if (strcmp(current->type, "NUM") == 0) {
            printf("%-28d ║\n", current->valeur.i);
        } else if (strcmp(current->type, "REAL") == 0) {
            printf("%-28.2f ║\n", current->valeur.f);
        } else if (strcmp(current->type, "TEXT") == 0) {
            printf("%-28s ║\n", current->valeur.s);
        } else {
            printf("%-28s ║\n", "Inconnu");
        }
        current = current->suivant;
    }
    printf(CYAN "╚═════════════════════╩════════════╩════════════╩══════════════════════════════╝\n\n" RESET);

    printf(CYAN "╔════════════════════════════════════════════════╗\n" RESET);
    printf(CYAN "║" GREEN "              TABLE DES TABLEAUX              " CYAN "║\n" RESET);
    printf(CYAN "╠═════════════════════╦════════════╦════════════╦═══════════╣\n" RESET);
    printf(CYAN "║ " YELLOW "Entité" RESET "              ║ " YELLOW "Type" RESET "      ║ " YELLOW "Taille" RESET "   ║ " YELLOW "Valeur" RESET "   ║\n");
    printf(CYAN "╠═════════════════════╬════════════╬════════════╬═══════════╣\n" RESET);
    
    listeT *currentT = TStab;
    while (currentT != NULL) {
        printf(CYAN "║ %-18s ║ %-10s ║ %-10d ║ ", 
               currentT->entite, 
               currentT->type, 
               currentT->taille);
        if (strcmp(currentT->type, "NUM") == 0) {
            printf("%-9d ║\n", currentT->valeur.i);
        } else if (strcmp(currentT->type, "REAL") == 0) {
            printf("%-9.2f ║\n", currentT->valeur.f);
        } else if (strcmp(currentT->type, "TEXT") == 0) {
            printf("%-9s ║\n", currentT->valeur.s);
        } else {
            printf("%-9s ║\n", "Inconnu");
        }
        currentT = currentT->suivant;
    }
    printf(CYAN "╚═════════════════════╩════════════╩════════════╩═══════════╝\n" RESET);
}

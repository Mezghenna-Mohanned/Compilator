%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nb_ligne = 1;
int col = 1;
extern int yylex();

%}

lettre [a-zA-Z]
chiffre [0-9]
idf {lettre}({lettre}|{chiffre}|_)*

cstflt [+-]?([0-9]*\.[0-9]+|\.[0-9]+|[0-9]+\.[0-9]*)
cstint [+-]?[0-9]+

%%

"MainPrgm" { printf("TOKEN: MAINPRGM\n"); }
"Var" { printf("TOKEN: VAR\n"); }
"BeginPg" { printf("TOKEN: BEGINPG\n"); }
"EndPg" { printf("TOKEN: ENDPG\n"); }

"let" { printf("TOKEN: LET\n"); }
"@define" { printf("TOKEN: DEFINE\n"); }
"Const" { printf("TOKEN: CONST\n"); }

"if" { printf("TOKEN: IF\n"); }
"then" { printf("TOKEN: THEN\n"); }
"else" { printf("TOKEN: ELSE\n"); }
"do" { printf("TOKEN: DO\n"); }
"while" { printf("TOKEN: WHILE\n"); }
"for" { printf("TOKEN: FOR\n"); }
"from" { printf("TOKEN: FROM\n"); }
"to" { printf("TOKEN: TO\n"); }
"step" { printf("TOKEN: STEP\n"); }

"input" { printf("TOKEN: INPUT\n"); }
"output" { printf("TOKEN: OUTPUT\n"); }

"Int" { printf("TOKEN: INT\n"); }
"Float" { printf("TOKEN: FLOAT\n"); }

"+" { printf("TOKEN: +\n"); }
"-" { printf("TOKEN: -\n"); }
"*" { printf("TOKEN: *\n"); }
"/" { printf("TOKEN: /\n"); }

"AND" { printf("TOKEN: AND\n"); }
"OR" { printf("TOKEN: OR\n"); }
"!" { printf("TOKEN: NOT\n"); }

"<" { printf("TOKEN: <\n"); }
">" { printf("TOKEN: >\n"); }
"<=" { printf("TOKEN: LE\n"); }
">=" { printf("TOKEN: GE\n"); }
"==" { printf("TOKEN: EQ\n"); }
"!=" { printf("TOKEN: NEQ\n"); }

":=" { printf("TOKEN: ASSIGN\n"); }

"{" { printf("TOKEN: {\n"); }
"}" { printf("TOKEN: }\n"); }
"(" { printf("TOKEN: (\n"); }
")" { printf("TOKEN: )\n"); }
"[" { printf("TOKEN: [\n"); }
"]" { printf("TOKEN: ]\n"); }
";" { printf("TOKEN: ;\n"); }
":" { printf("TOKEN: :\n"); }
"," { printf("TOKEN: ,\n"); }

{idf} {
    if (strlen(yytext) > 14 || yytext[strlen(yytext)-1] == '_' || strstr(yytext, "__")) {
        printf("Erreur lexicale : identifiant invalide '%s' à la ligne %d, colonne %d\n", yytext, nb_ligne, col);
    } else {
        printf("TOKEN: IDF (%s)\n", yytext);
    }
}

{cstint} {
    int val = atoi(yytext);
    if (val < -32768 || val > 32767) {
        printf("Erreur lexicale : entier hors limite [-32768, 32767] à la ligne %d, colonne %d\n", nb_ligne, col);
    } else {
        printf("TOKEN: CSTINT (%s)\n", yytext);
    }
}

{cstflt} { printf("TOKEN: CSTFLT (%s)\n", yytext); }

"<!-"[^>]*"-!>" { printf("TOKEN: COMMENT\n"); }
"{--"([^}]|"-"[^}])*"--}" { printf("TOKEN: COMMENT\n"); }

\"[^\"]*\" { printf("TOKEN: TEXT (%s)\n", yytext); }

"=" { printf("TOKEN: =\n"); }

\n { nb_ligne++; col = 1; }
[ \t] { col++; }

. {
    printf("Erreur lexicale : caractère inconnu '%s' à la ligne %d, colonne %d\n", yytext, nb_ligne, col);
}

%%



int main() {
    printf("=== Démarrage de l'analyse lexicale ===\n");
    while (yylex() != 0);
    printf("=== Fin de l'analyse lexicale ===\n");
    return 0;
}


int yywrap() { return 1; }


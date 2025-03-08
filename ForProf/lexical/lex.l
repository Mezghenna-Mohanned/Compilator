%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntaxique/syntax.tab.h"   /* Include Bison header with token definitions */

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

"MainPrgm"        { printf("TOKEN: MAINPRGM\n"); return MAINPRGM; }
"Var"             { printf("TOKEN: VAR\n"); return VAR; }
"BeginPg"         { printf("TOKEN: BEGINPG\n"); return BEGINPG; }
"EndPg"           { printf("TOKEN: ENDPG\n"); return ENDPG; }

"let"             { printf("TOKEN: LET\n"); return LET; }
"@define"         { printf("TOKEN: DEFINE\n"); return DEFINE; }
"Const"           { printf("TOKEN: CONST\n"); return CONST; }

"if"              { printf("TOKEN: IF\n"); return IF; }
"then"            { printf("TOKEN: THEN\n"); return THEN; }
"else"            { printf("TOKEN: ELSE\n"); return ELSE; }
"do"              { printf("TOKEN: DO\n"); return DO; }
"while"           { printf("TOKEN: WHILE\n"); return WHILE; }
"for"             { printf("TOKEN: FOR\n"); return FOR; }
"from"            { printf("TOKEN: FROM\n"); return FROM; }
"to"              { printf("TOKEN: TO\n"); return TO; }
"step"            { printf("TOKEN: STEP\n"); return STEP; }

"input"           { printf("TOKEN: INPUT\n"); return INPUT; }
"output"          { printf("TOKEN: OUTPUT\n"); return OUTPUT; }

"Int"             { printf("TOKEN: INT\n"); return INT_T; }
"Float"           { printf("TOKEN: FLOAT\n"); return FLOAT_T; }

"+"               { printf("TOKEN: +\n"); return '+'; }
"-"               { printf("TOKEN: -\n"); return '-'; }
"*"               { printf("TOKEN: *\n"); return '*'; }
"/"               { printf("TOKEN: /\n"); return '/'; }

"AND"             { printf("TOKEN: AND\n"); return AND; }
"OR"              { printf("TOKEN: OR\n"); return OR; }
"!"               { printf("TOKEN: NOT\n"); return NOT; }

"<="              { printf("TOKEN: LE\n"); return LE; }
">="              { printf("TOKEN: GE\n"); return GE; }
"=="              { printf("TOKEN: EQ\n"); return EQ; }
"!="              { printf("TOKEN: NEQ\n"); return NEQ; }
"<"               { printf("TOKEN: <\n"); return '<'; }
">"               { printf("TOKEN: >\n"); return '>'; }

":="              { printf("TOKEN: ASSIGN\n"); return ASSIGN; }

"{"               { printf("TOKEN: {\n"); return '{'; }
"}"               { printf("TOKEN: }\n"); return '}'; }
"("               { printf("TOKEN: (\n"); return '('; }
")"               { printf("TOKEN: )\n"); return ')'; }
"["               { printf("TOKEN: [\n"); return '['; }
"]"               { printf("TOKEN: ]\n"); return ']'; }
";"               { printf("TOKEN: ;\n"); return ';'; }
":"               { printf("TOKEN: :\n"); return ':'; }
","               { printf("TOKEN: ,\n"); return ','; }

{idf} {
    if (strlen(yytext) > 14 || yytext[strlen(yytext)-1] == '_' || strstr(yytext, "__")) {
        printf("Erreur lexicale : identifiant invalide '%s' à la ligne %d, colonne %d\n", yytext, nb_ligne, col);
        return 0;
    } else {
        printf("TOKEN: IDF (%s)\n", yytext);
        return IDF;
    }
}

{cstint} {
    int val = atoi(yytext);
    if (val < -32768 || val > 32767) {
        printf("Erreur lexicale : entier hors limite [-32768, 32767] à la ligne %d, colonne %d\n", nb_ligne, col);
        return 0;
    } else {
        printf("TOKEN: CSTINT (%s)\n", yytext);
        return CSTINT;
    }
}

{cstflt} { printf("TOKEN: CSTFLT (%s)\n", yytext); return CSTFLT; }

"<!-"[^>]*"-!>" { printf("TOKEN: COMMENT\n"); return COMMENT; }
"{--"([^}]|"-"[^}])*"--}" { printf("TOKEN: COMMENT\n"); return COMMENT; }

\"[^\"]*\" { printf("TOKEN: TEXT (%s)\n", yytext); return TEXT; }

\n { nb_ligne++; col = 1; }
[ \t] { col++; }

. {
    printf("Erreur lexicale : caractère inconnu '%s' à la ligne %d, colonne %d\n", yytext, nb_ligne, col);
    return 0;
}

%%

int yywrap() { return 1; }

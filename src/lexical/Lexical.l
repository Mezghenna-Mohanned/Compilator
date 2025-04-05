%{
#include "syntax.tab.h"
#include "semantique.h"
extern YYSTYPE yylval;
int nb_ligne = 1;
int col = 1;
%}


letter        [a-zA-Z]
digit         [0-9]
tail          ({letter}|{digit}|_){0,13}
good_idf      {letter}{tail}

int_pat       [0-9]+
float_pat     [0-9]*\.[0-9]+

assign_op     ":="

%%

"MainPrgm"          { insererKeyword("MainPrgm"); return MAINPRGM; }
"Var"               { insererKeyword("Var");       return VAR; }
"BeginPg"           { insererKeyword("BeginPg");   return BEGINPG; }
"EndPg"             { insererKeyword("EndPg");     return ENDPG; }
"let"               { insererKeyword("let");       return LET; }
"Int"               { return INT_TYPE; }
"Float"             { return FLOAT_TYPE; }


"@define"           { insererKeyword("@define");   return DEFINE; }
"Const"             { insererKeyword("Const");     return CONSTTK; }

"if"                { insererKeyword("if");        return IF; }
"then"              { insererKeyword("then");      return THEN; }
"else"              { insererKeyword("else");      return ELSE; }

"do"                { insererKeyword("do");        return DO; }
"while"             { insererKeyword("while");     return WHILE; }

"for"               { insererKeyword("for");       return FOR; }
"from"              { insererKeyword("from");      return FROM; }
"to"                { insererKeyword("to");        return TO; }
"step"              { insererKeyword("step");      return STEP; }

"input"             { insererKeyword("input");     return INPUT; }
"output"            { insererKeyword("output");    return OUTPUT; }

"+"   { insererOperator("+"); return '+'; }
"-"   { insererOperator("-"); return '-'; }
"*"   { insererOperator("*"); return '*'; }
"/"   { insererOperator("/"); return '/'; }


"<="  { insererCompareOp("<="); return LE; }
">="  { insererCompareOp(">="); return GE; }
"=="  { insererCompareOp("=="); return EQ; }
"!="  { insererCompareOp("!="); return NEQ; }
"<"   { insererCompareOp("<");  return '<'; }
">"   { insererCompareOp(">");  return '>'; }

"AND" { insererLogicOp("AND"); return AND; }
"OR"  { insererLogicOp("OR");  return OR; }
"!"   { insererLogicOp("!");   return NOT; }


{assign_op}         { return ASSIGN; }

"("    { return '('; }
")"    { return ')'; }
"{"    { return '{'; }
"}"    { return '}'; }
"["    { return '['; }
"]"    { return ']'; }
";"    { return ';'; }
":"    { return ':'; }
","    { return ','; }

"="    { return '='; }


"<!-".*"-!>" {
}
"{--"([^-]|-+[^-])*"--}" {
    char *p;
    for (p = yytext; *p; p++) {
        if (*p == '\n') { nb_ligne++; col = 1; } 
        else { col++; }
    }
}

"\\(""?[+-]?{int_pat}\\)" {
    int val = atoi(yytext+1);
    yylval.entier = val;
    return INTCST;
}

"\\(""?[+-]?{float_pat}\\)" {
    float val = atof(yytext+1);
    yylval.reel = val;
    return FLOATCST;
}

{int_pat} {
    int val = atoi(yytext);
    yylval.entier = val;
    return INTCST;
}

{float_pat} {
    float val = atof(yytext);
    yylval.reel = val;
    return FLOATCST;
}

{good_idf} {
    if ((yytext[strlen(yytext)-1] == '_') || (strstr(yytext,"__")!=NULL)) {
        fprintf(stderr,"Erreur lexicale (identifiant invalide) '%s' ligne %d, col %d\n", yytext, nb_ligne, col);
        return -1;
    }
    yylval.str = strdup(yytext);
    return IDF;
}

[\t ]+  { col += yyleng; }

\n      { nb_ligne++; col=1; }

. {
    fprintf(stderr,"Caractère invalide '%s' ligne %d, col %d\n", yytext, nb_ligne, col);
    return -1;
}

%%

int yywrap() { return 1; }

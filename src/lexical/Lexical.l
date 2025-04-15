%{
#include "syntax.tab.h"
#include "semantique.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

extern YYSTYPE yylval;

int nb_ligne = 1;
int col      = 1;
%}

letter              [a-zA-Z]
digit               [0-9]
tail                ({letter}|{digit}|_){0,13}
good_idf            {letter}{tail}

sign                [\+\-]

int_pat             {digit}+
float_pat           {digit}*\.{digit}+

signed_int_pat      \({sign}{int_pat}\)
signed_float_pat    \({sign}{int_pat}\.{int_pat}\)

assign_op           ":="

%%

"MainPrgm"          { insererKeyword("MainPrgm"); return MAINPRGM; }
"Var"               { insererKeyword("Var");       return VAR;      }
"BeginPg"           { insererKeyword("BeginPg");   return BEGINPG;  }
"EndPg"             { insererKeyword("EndPg");     return ENDPG;    }
"let"               { insererKeyword("let");       return LET;      }
"Int"               { return INT_TYPE;  }
"Float"             { return FLOAT_TYPE; }

"@define"           { insererKeyword("@define");   return DEFINE;   }
"Const"             { insererKeyword("Const");     return CONSTTK;  }

"if"                { insererKeyword("if");        return IF;   }
"then"              { insererKeyword("then");      return THEN; }
"else"              { insererKeyword("else");      return ELSE; }

"do"                { insererKeyword("do");        return DO;    }
"while"             { insererKeyword("while");     return WHILE; }

"for"               { insererKeyword("for");       return FOR;   }
"from"              { insererKeyword("from");      return FROM;  }
"to"                { insererKeyword("to");        return TO;    }
"step"              { insererKeyword("step");      return STEP;  }

"input"             { insererKeyword("input");     return INPUT;  }
"output"            { insererKeyword("output");    return OUTPUT; }

"+"                 { insererOperator("+");  return '+'; }
"-"                 { insererOperator("-");  return '-'; }
"*"                 { insererOperator("*");  return '*'; }
"/"                 { insererOperator("/");  return '/'; }

"<="                { insererCompareOp("<="); return LE;  }
">="                { insererCompareOp(">="); return GE;  }
"=="                { insererCompareOp("=="); return EQ;  }
"!="                { insererCompareOp("!="); return NEQ; }
"<"                 { insererCompareOp("<");  return '<'; }
">"                 { insererCompareOp(">");  return '>'; }

"AND"               { insererLogicOp("AND"); return AND; }
"OR"                { insererLogicOp("OR");  return OR;  }
"!"                 { insererLogicOp("!");   return NOT; }

{assign_op}         { return ASSIGN; }

"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"["                 { return '['; }
"]"                 { return ']'; }
";"                 { return ';'; }
":"                 { return ':'; }
","                 { return ','; }
"="                 { return '=';  }

{signed_float_pat}  {
                       int sign = (yytext[1] == '-') ? -1 : 1;
                       float val = atof(yytext + 2) * sign;  /* skip "(+" or "(-" */
                       yylval.reel = val;
                       return FLOATCST;
                    }

{signed_int_pat}    {
                       int sign = (yytext[1] == '-') ? -1 : 1;
                       int val  = atoi(yytext + 2) * sign;
                       yylval.entier = val;
                       return INTCST;
                    }

{float_pat}         { yylval.reel   = atof(yytext); return FLOATCST; }
{int_pat}           { yylval.entier = atoi(yytext); return INTCST;   }

{good_idf}          {
                       int err = 0;
                       size_t len = strlen(yytext);

                       /* règle 1 : longueur > 14 */
                       if (len > 14) {
                           fprintf(stderr,
                             "Identificateur >14 caractères  '%s'  ligne %d, col %d\n",
                             yytext, nb_ligne, col);
                           err = 1;
                       }
                       /* règle 2 : premier caractère pas minuscule */
                       else if (!(yytext[0] >= 'a' && yytext[0] <= 'z')) {
                           fprintf(stderr,
                             "Identificateur doit commencer par une lettre minuscule  '%s'  ligne %d, col %d\n",
                             yytext, nb_ligne, col);
                           err = 1;
                       }
                       /* règle 3 : contient une majuscule */
                       else {
                           for (size_t i = 0; i < len; ++i)
                               if (isupper((unsigned char)yytext[i])) {
                                   fprintf(stderr,
                                     "Identificateur contient des majuscules  '%s'  ligne %d, col %d\n",
                                     yytext, nb_ligne, col);
                                   err = 1;
                                   break;
                               }
                       }
                       /* règle 4 : finit par '_' */
                       if (!err && yytext[len-1] == '_') {
                           fprintf(stderr,
                             "Identificateur se termine par '_'  '%s'  ligne %d, col %d\n",
                             yytext, nb_ligne, col);
                           err = 1;
                       }
                       /* règle 5 : '__' consécutifs */
                       if (!err && strstr(yytext,"__")) {
                           fprintf(stderr,
                             "Identificateur contient '__' consécutifs  '%s'  ligne %d, col %d\n",
                             yytext, nb_ligne, col);
                           err = 1;
                       }

                       if (err) {
                           col += yyleng;
                           return -1;
                       }

                       yylval.str = strdup(yytext);
                       col += yyleng;
                       return IDF;
                    }

"<!-".*"-!>"        {}

"{--"([^-]|-+[^-])*"--}" {
                       for (char *p = yytext; *p; ++p)
                           (*p == '\n') ? (nb_ligne++, col = 1) : (col++);
                    }

[\t ]+              { col += yyleng; }
\n                  { nb_ligne++; col = 1; }

.                   {
                       fprintf(stderr,
                               "Caractère invalide '%s' ligne %d, col %d\n",
                               yytext, nb_ligne, col);
                       col += yyleng;
                       return -1;
                    }

%%

int yywrap() { return 1; }

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex(void);
extern int nb_ligne;
extern int col;
extern int yynerrs;
extern char *yytext;
void yyerror(const char *s);

#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"
#define GREEN   "\033[0;32m"
%}

/* Our semantic values: integers, floats, or strings */
%union {
    int entier;
    float reel;
    char *str;
}

/* Token declarations that must match those returned by lex.l */
%token MAINPRGM VAR BEGINPG ENDPG
%token LET DEFINE CONST
%token IF THEN ELSE DO WHILE FOR FROM TO STEP
%token INPUT OUTPUT
%token INT_T FLOAT_T

%token <str> ASSIGN   /* for ":=" */
%token <str> EQ       /* for "==" */
%token <str> NEQ      /* for "!=" */
%token <str> LE       /* for "<=" */
%token <str> GE       /* for ">=" */
%token <str> AND      /* for "AND" */
%token <str> OR       /* for "OR" */
%token <str> NOT      /* for "!" */

%token <entier> CSTINT
%token <reel> CSTFLT
%token <str> IDF
%token <str> TEXT
%token COMMENT

/* Precedence and associativity */
%left OR
%left AND
%nonassoc '<' '>' LE GE EQ NEQ
%left '+' '-'
%left '*' '/'

/* Declare nonterminals that carry a semantic value as a char* */
%type <str> type constant_value expression

%start program

%%

/* Program structure */
program:
    MAINPRGM IDF ';' declarations BEGINPG block ENDPG ';'
    ;

/* Declarations: variables and constant declarations */
declarations:
      declarations declaration
    | /* empty */
    ;

declaration:
      VAR declaration_list ';'
    | DEFINE const_declaration ';'
    ;

declaration_list:
      declaration_list ',' var_decl
    | var_decl
    ;

var_decl:
      LET IDF ':' type                { /* No semantic action yet */ }
    | LET IDF ':' array_decl           { /* No semantic action yet */ }
    ;

array_decl:
      '[' type ';' CSTINT ']'          { /* No semantic action yet */ }
    ;

const_declaration:
      CONST IDF ':' type ASSIGN constant_value  { /* No semantic action yet */ }
    ;

/* Type: Int or Float */
type:
      INT_T      { $$ = strdup("Int"); }
    | FLOAT_T    { $$ = strdup("Float"); }
    ;

/* Constant value: return a string representation */
constant_value:
      CSTINT {
           $$ = (char *)malloc(32);
           sprintf($$, "%d", $1);
       }
    | CSTFLT {
           $$ = (char *)malloc(32);
           sprintf($$, "%f", $1);
       }
    | TEXT {
           $$ = strdup($1);
       }
    ;

/* Block of statements */
block:
      '{' statements '}'
    ;

/* Statements: a sequence of statements */
statements:
      statements statement
    | /* empty */
    ;

/* Statement: assignment, conditional, loop, or I/O */
statement:
      assignment ';'
    | conditional
    | loop
    | io_statement ';'
    ;

/* Assignment: simple assignment and array assignment */
assignment:
      IDF ASSIGN expression { /* No semantic action yet */ }
    | IDF '[' CSTINT ']' ASSIGN expression { /* No semantic action yet */ }
    | IDF '[' expression ']' ASSIGN expression { /* No semantic action yet */ }
    ;

/* Expression: arithmetic expressions and identifiers/constants */
expression:
      expression '+' expression { /* No semantic action yet */ }
    | expression '-' expression { /* No semantic action yet */ }
    | expression '*' expression { /* No semantic action yet */ }
    | expression '/' expression { /* No semantic action yet */ }
    | '(' expression ')'         { $$ = $2; }
    | constant_value             { $$ = $1; }
    | IDF                        { $$ = strdup($1); }
    ;

/* Conditional statement */
conditional:
      IF '(' expression ')' THEN block ELSE block
    | IF '(' expression ')' THEN block
    ;

/* Loop statements: do-while and for-loop */
loop:
      DO block WHILE '(' expression ')'
    | FOR IDF FROM expression TO expression STEP expression block
    ;

/* I/O statements */
io_statement:
      OUTPUT '(' io_args ')'
    | INPUT '(' IDF ')'
    ;

/* I/O arguments: one or more expressions */
io_args:
      io_args ',' expression
    | expression
    ;

%%

void yyerror(const char *s) {
    printf("\n%sErreur: ligne %d, colonne %d: %s%s\n", YELLOW, nb_ligne, col, s, RESET);
}

int main() {
    int result = yyparse();
    if (result == 0 && yynerrs == 0) {
        printf("\n\n%sAnalyse syntaxique réussie%s\n\n", GREEN, RESET);
    } else {
        printf("\n\nNombre total d'erreurs: %d\n\n", yynerrs);
    }
    return result;
}

%{
#include "include/semantique.h"
extern int insererTableConst(char entite[], char type[], void *valeur);
extern int yylex(void);
extern int nb_ligne;
extern int col;
extern int yynerrs;
extern char *yytext;
extern void yyerror(const char *s);
extern void afficherTStab(void);
extern void afficherToutesLesTablesSymboles(void);
extern void gererTaille(char*, char*);

void lire(char *idf);
void afficher(constant *val);

%}

/* We need ts.h for 'listeD', 'listeT', 'constant' in %union */
%code requires {
  #include "include/ts.h"
}

/* The union for Bison symbols */
%union {
    int    entier;
    float  reel;
    char  *str;
    listeD *symbole;
    listeT *symbol;
    constant *con;
}

/* Tokens from lexical.l */
%token MAINPRGM VAR BEGINPG ENDPG
%token LET DEFINE CONSTTK
%token IF THEN ELSE DO WHILE FOR FROM TO STEP
%token INPUT OUTPUT

%token <entier> INTCST
%token <reel>   FLOATCST
%token <str>    IDF
%token <str> STRING

%token INT_TYPE
%token FLOAT_TYPE
%token STRING_TYPE

%token ASSIGN /* := */

/* operators */
%token LE  /* <= */
%token GE  /* >= */
%token EQ  /* == */
%token NEQ /* != */
%token AND
%token OR
%token NOT /* ! */


%left OR
%left AND
%left EQ NEQ
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/'

%type <symbole> var_list
%type <symbol> var_list2
%type <symbole> const_declaration
%type <con> constant_value
%type <con> expression
%type <str> type

%start program

%%

program:
    MAINPRGM IDF ';'
    VAR declarations
    BEGINPG block
    ENDPG ';'
    {
      printf("\n\nCompilation terminée: programme MiniSoft\n");
      afficherToutesLesTablesSymboles();
    }
    ;

declarations:
    declarations declaration
    | /* empty */
    ;

declaration:
      /* Simple variables */
      LET var_list ':' type ';'
      {
        insertionTS_et_verif_double_declaration($2, $4);
      }
    /* Arrays */
    | LET var_list2 ':' type ';'
      {
        insertionTStab_et_verif_double_declaration($2, $4);
      }
    /* Constants */
    | DEFINE CONSTTK const_declaration ';'
      {
        if (insererTableConst($3->entite, $3->type, &($3->valeur)) == -1)
            yyerror("Double déclaration");
      }
    ;

var_list:
      var_list ',' IDF
      { 
        $$ = creationVarlist1($3, $1); 
      }
    | IDF
      { 
        $$ = creationVarlist1($1, NULL); 
      }
    ;

var_list2:
      var_list2 ',' IDF '[' INTCST ']'
      {
        $$ = creationVarlist2($3, $5, $1);
      }
    | IDF '[' INTCST ']'
      {
        $$ = creationVarlist2($1, $3, NULL);
      }
    ;

/* @define Const Pi : Float = 3.14 ; => We store it in 'const_declaration' */
const_declaration:
    IDF ':' type '=' constant_value
    {
      $$ = constDeclaration($5, $1, 1);
      $$->type = strdup($3);
    }
    | IDF ':' type
    {
      $$ = constDeclaration(NULL, $1, 0);
      $$->type = strdup($3);
    }
    ;

type:
      INT_TYPE   { $$ = strdup("Int"); }
    | FLOAT_TYPE { $$ = strdup("Float"); }
    | STRING_TYPE { $$ = strdup("String"); }
    ;


block:
    '{' statements '}'
    ;

statements:
    statements statement
    | /* empty */
    ;

statement:
      assignment ';'
    | conditional
    | loop
    | iostatement ';'
    | forstatement
    ;

assignment:
    IDF ASSIGN expression
    {
      gestionErreurAssig($3, $1);
    }
    | IDF '[' INTCST ']' ASSIGN expression
    {
      gestion_taille_tableau($1, $3);
    }
    | IDF '[' IDF ']' ASSIGN expression
    {
      gererTaille($1, $3);
    }
    ;

expression:
      expression '+' expression { $$ = gestionErreurType(1, $1, $3); }
    | expression '-' expression { $$ = gestionErreurType(2, $1, $3); }
    | expression '*' expression { $$ = gestionErreurType(3, $1, $3); }
    | expression '/' expression { $$ = gestionErreurType(4, $1, $3); }
    | '(' expression ')'        { $$ = $2; }
    | constant_value            { $$ = $1; }
    | IDF                       { $$ = gestionIDF($1); }
    ;

constant_value:
      INTCST
      {
        constant *c = (constant *)malloc(sizeof(constant));
        strcpy(c->type, "Int");
        c->valeur.i = $1;
        $$ = c;
      }
    | FLOATCST
      {
        constant *c = (constant *)malloc(sizeof(constant));
        strcpy(c->type, "Float");
        c->valeur.f = $1;
        $$ = c;
      }
    ;

conditional:
    IF '(' condition ')' THEN block ELSE block
    | IF '(' condition ')' THEN block
    ;

loop:
    DO block WHILE '(' condition ')' ';'
    ;

forstatement:
    FOR IDF FROM expression TO expression STEP expression block
    ;

condition: condition_or;

condition_or:
      condition_or OR condition_and
    | condition_and
    ;

condition_and:
      condition_and AND condition_not
    | condition_not
    ;

condition_not:
      NOT condition_not
    | condition_comp
    ;

condition_comp:
      expression '<' expression  { gestionIncompatibilite($1, $3); }
    | expression '>' expression  { gestionIncompatibilite($1, $3); }
    | expression LE expression   { gestionIncompatibilite($1, $3); }
    | expression GE expression   { gestionIncompatibilite($1, $3); }
    | expression EQ expression   { gestionIncompatibiliteEQ_NEQ($1, $3); }
    | expression NEQ expression  { gestionIncompatibiliteEQ_NEQ($1, $3); }
    | '(' condition ')'
    ;


iostatement:
      INPUT '(' IDF ')' {
        lire($3);
      }
    | OUTPUT '(' expression ')' {
        afficher($3);
      }
    | OUTPUT '(' STRING ')' {
        printf("%s\n", $3);
      }
    ;


%%

void yyerror(const char *s) {
    if (strstr(s, "syntax error")) {
        fprintf(stderr,"\n%sErreur syntaxique ligne %d, colonne %d: %s%s\n", RED, nb_ligne, col, s, RESET);
    } else {
        fprintf(stderr,"\n%sErreur sémantique ligne %d, colonne %d: %s%s\n", YELLOW, nb_ligne, col, s, RESET);
    }
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

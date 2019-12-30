%{
	#include "lex.yy.c"
	#ifndef YYSTYPE
	#define YYSTYPE struct Node_t*
	#endif
	struct Node_t* root = NULL;
%}
%locations
%token INT FLOAT
%token SEMI COMMA ASSIGNOP RELOP
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT
%token ID TYPE
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNLOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%
Program : ExtDefList {$$ = createNode(_Program, yylineno, 1, $1); root = $$;}
	;
ExtDefList : ExtDef ExtDefList {$$ = createNode(_ExtDefList, yylineno, 2, $1, $2);}
	| {$$ = createNode(_ExtDefList, yylineno, 1, createNode(None, yylineno, 0));}
	;
ExtDef : Specifier ExtDecList SEMI {$$ = createNode(_ExtDef, yylineno, 3, $1, $2, $3);}
	| Specifier SEMI {$$ = createNode(_ExtDef, yylineno, 2, $1, $2);}
	| Specifier FunDec CompSt {$$ = createNode(_ExtDef, yylineno, 3, $1, $2, $3);}
	| Specifier FunDec SEMI {$$ = createNode(_ExtDef, yylineno, 3, $1, $2, $3);}
	| Specifier error CompSt {fprintf(stderr, "ExtDef:FunDec is wrong\n");}
	;
ExtDecList : VarDec {$$ = createNode(_ExtDecList, yylineno, 1, $1);}
	| VarDec COMMA ExtDecList {$$ = createNode(_ExtDecList, yylineno, 3, $1, $2, $3);}
	;


Specifier : TYPE {$$ = createNode(_Specifier, yylineno, 1, $1);}
	| StructSpecifier {$$ = createNode(_Specifier, yylineno, 1, $1);}
	;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = createNode(_StructSpecifier, yylineno, 5, $1, $2, $3, $4, $5);}
	| STRUCT Tag {$$ = createNode(_StructSpecifier, yylineno, 2, $1, $2);}
	;
OptTag : ID {$$ = createNode(_OptTag, yylineno, 1, $1);}
	| {$$ = createNode(_OptTag, yylineno, 1, createNode(None, yylineno, 0));}
	;
Tag : ID {$$ = createNode(_Tag, yylineno, 1, $1);}
	;

VarDec : ID {$$ = createNode(_VarDec, yylineno, 1, $1);}
	| VarDec LB INT RB {$$ = createNode(_VarDec, yylineno, 4, $1, $2, $3, $4);}
	;
FunDec : ID LP VarList RP {$$ = createNode(_FunDec, yylineno, 4, $1, $2, $3, $4);}
	| ID LP RP {$$ = createNode(_FunDec, yylineno, 3, $1, $2, $3);}
	;
VarList : ParamDec COMMA VarList {$$ = createNode(_VarList, yylineno, 3, $1, $2, $3);}
	| ParamDec {$$ = createNode(_VarList, yylineno, 1, $1);}
	;
ParamDec : Specifier VarDec {$$ = createNode(_ParamDec, yylineno, 2, $1, $2);}
	;


CompSt : LC DefList StmtList RC {$$ = createNode(_CompSt, yylineno, 4, $1, $2, $3, $4);}
	;
StmtList : Stmt StmtList {$$ = createNode(_StmtList, yylineno, 2,  $1, $2);}
	| {$$ = createNode(_StmtList, yylineno, 1, createNode(None, yylineno, 0));}
	;
Stmt : Exp SEMI {$$ = createNode(_Stmt, yylineno, 2, $1, $2);}
	| CompSt {$$ = createNode(_Stmt, yylineno, 1, $1);}
	| RETURN Exp SEMI {$$ = createNode(_Stmt, yylineno, 3, $1, $2, $3);}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = createNode(_Stmt, yylineno, 5, $1, $2, $3, $4, $5);}
	| IF LP Exp RP Stmt ELSE Stmt {$$ = createNode(_Stmt, yylineno, 7, $1, $2, $3, $4, $5, $6, $7);}
	| IF LP error RP Stmt %prec LOWER_THAN_ELSE {fprintf(stderr, "Stmt:Exp between \"\(\)\" is wrong\n");}
	| IF LP error RP Stmt ELSE Stmt %prec LOWER_THAN_ELSE {fprintf(stderr, "Stmt:Exp between \"\(\)\" is wrong\n");}
	| WHILE LP Exp RP Stmt {$$ = createNode(_Stmt, yylineno, 5, $1, $2, $3, $4, $5);}
	| error SEMI {fprintf(stderr, "Stmt:Exp before \";\" is wrong\n");}
	| error RC {fprintf(stderr, "Stmt:Missing \"{\"\n");}
	;


DefList : Def DefList {$$ = createNode(_DefList, yylineno, 2, $1, $2);}
	| {$$ = createNode(_DefList, yylineno, 1, createNode(None, yylineno, 0));}
	;
Def : Specifier DecList SEMI {$$ = createNode(_Def, yylineno, 3, $1, $2, $3);}
	| error SEMI{fprintf(stderr, "Def:DecList before \";\" is wrong\n");}
   	| error RC {fprintf(stderr, "Def:Missing \"{\" \n");}
	;
DecList : Dec {$$ = createNode(_DecList, yylineno, 1, $1);}
	| Dec COMMA DecList {$$ = createNode(_DecList, yylineno, 3, $1, $2, $3);}
	;
Dec : VarDec {$$ = createNode(_Dec, yylineno, 1, $1);}
	| VarDec ASSIGNOP Exp {$$ = createNode(_Dec, yylineno, 3, $1, $2, $3);}
	;

Exp : Exp ASSIGNOP Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| Exp AND Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| Exp OR Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| Exp RELOP Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}  
	| Exp PLUS Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| Exp MINUS Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| Exp STAR Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| Exp DIV Exp {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| LP Exp RP {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| MINUS Exp {$$ = createNode(_Exp, yylineno, 2, $1, $2);}
	| NOT Exp {$$ = createNode(_Exp, yylineno, 2, $1, $2);}
	| ID LP Args RP {$$ = createNode(_Exp, yylineno, 4, $1, $2, $3, $4);}
	| ID LP RP {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| Exp LB Exp RB {$$ = createNode(_Exp, yylineno, 4, $1, $2, $3, $4);}
	| Exp DOT ID {$$ = createNode(_Exp, yylineno, 3, $1, $2, $3);}
	| ID {$$ = createNode(_Exp, yylineno, 1, $1);}
	| INT {$$ = createNode(_Exp, yylineno, 1, $1);}
	| FLOAT {$$ = createNode(_Exp, yylineno, 1, $1);}
	;
Args : Exp COMMA Args {$$ = createNode(_Args, yylineno, 3, $1, $2, $3);}
	| Exp {$$ = createNode(_Args, yylineno, 1, $1);}
	;
%%
void yyerror(const char* msg){
	error_occured = 1;
    	fprintf(stderr, "Error type B at Line %d: %s,", yylineno, msg);
}

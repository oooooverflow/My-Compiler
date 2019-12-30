%{
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include "multi_tree.h"
	#define YYSTYPE struct Node_t*
	#include "syntax.tab.h"
	int yycolumn = 1;
	#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
	int error_occured=0;
	float mychange(char *yytext);
%}
%option yylineno
%option pointer 
digit [0-9]
digits {digit}+
letter [_a-zA-Z]
int [1-9]{digit}*|0
float {digits}\.{digits}
scientific_float ({digit}*\.{digit}+|{digit}+\.{digit}*)[eE][+-]?{digits}
%%
\n {yycolumn = 1;}
[ \t]+ {}
\r {}
{int} {yylval = createNode(_INT, yylineno, 0); yylval->line = yylineno; yylval->value.intValue = atoi(yytext); return INT;}
{float} {yylval = createNode(_FLOAT, yylineno, 0); yylval->line = yylineno; yylval->value.floatValue = atof(yytext); return FLOAT;}
{scientific_float} {yylval = createNode(_FLOAT, yylineno, 0); yylval->line = yylineno;yylval->value.floatValue = mychange(yytext);return FLOAT;}
"int"|"float" {yylval = createNode(_TYPE, yylineno, 0); strcpy(yylval->name, yytext);yylval->line = yylineno; return TYPE;}
";" {yylval = createNode(_SEMI, yylineno, 0); yylval->line = yylineno; return SEMI;}
"," {yylval = createNode(_COMMA, yylineno, 0); yylval->line = yylineno; return COMMA;}
"=" {yylval = createNode(_ASSIGNOP, yylineno, 0); yylval->line = yylineno; return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" {yylval = createNode(_RELOP, yylineno, 0);strcpy(yylval->name, yytext);yylval->line = yylineno; return RELOP;}
"+" {yylval = createNode(_PLUS, yylineno, 0); yylval->line = yylineno; return PLUS;}
"-" {yylval = createNode(_MINUS, yylineno, 0); yylval->line = yylineno; return MINUS;}
"*" {yylval = createNode(_STAR, yylineno, 0); yylval->line = yylineno; return STAR;}
"/" {yylval = createNode(_DIV, yylineno, 0); yylval->line = yylineno; return DIV;}
"&&" {yylval = createNode(_AND, yylineno, 0); yylval->line = yylineno; return AND;}
"||" {yylval = createNode(_OR, yylineno, 0); yylval->line = yylineno; return OR;}
"." {yylval = createNode(_DOT, yylineno, 0); yylval->line = yylineno; return DOT;}
"!" {yylval = createNode(_NOT, yylineno, 0); yylval->line = yylineno; return NOT;}
"struct" {yylval = createNode(_STRUCT, yylineno, 0); yylval->line = yylineno; return STRUCT;}
"return" {yylval = createNode(_RETURN, yylineno, 0); yylval->line = yylineno; return RETURN;}
"if" {yylval = createNode(_IF, yylineno, 0); yylval->line = yylineno; return IF;}
"else" {yylval = createNode(_ELSE, yylineno, 0); yylval->line = yylineno; return ELSE;}
"while" {yylval = createNode(_WHILE, yylineno, 0); yylval->line = yylineno; return WHILE;}
{letter}({digit}|{letter})* {yylval = createNode(_ID, yylineno, 0); strcpy(yylval->name, yytext); yylval->line = yylineno; return ID;}
"(" {yylval = createNode(_LP, yylineno, 0); yylval->line = yylineno; return LP;}
")" {yylval = createNode(_RP, yylineno, 0); yylval->line = yylineno; return RP;}
"[" {yylval = createNode(_LB, yylineno, 0); yylval->line = yylineno; return LB;}
"]" {yylval = createNode(_RB, yylineno, 0); yylval->line = yylineno; return RB;}
"{" {yylval = createNode(_LC, yylineno, 0); yylval->line = yylineno; return LC;}
"}" {yylval = createNode(_RC, yylineno, 0); yylval->line = yylineno; return RC;}
. {
	printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
	error_occured = 1;
	}
%%
float mychange(char *text){
	char *ch = "eE";

	char *base;
	char *index;
	base = strtok(text,ch);
	index = strtok(NULL,ch);
	int exponent = atoi(index);
	float sum = 10;
	if(exponent == 0){
		sum = 1;
	}
	else if(exponent > 0){
		for(int i = 1;i < exponent; i++)
			sum *= 10;
	}
	else{
		for(int i = 1; i < -exponent; i++)
			sum *= 10;
		sum = 1.0/sum;
	}
		
	float result = atof(base) * sum;
	return result;
		
}


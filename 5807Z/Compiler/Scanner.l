/*
*	Project 5807 Z
*		author: Sebastien Cook
*		date: April 14 2021
*/
%{
#include <stdio.h>
#include "y.tab.h" 
extern YYSTYPE yylval;
%}
digit [0-9]
number {digit}+
char [A-Za-z_]
id {char}({char}|{digit})*
%%
"int" {return INT;}
"if" {return IF;}
"else" {return ELSE;}
"func" {return FUNC;}
"return" {return RETURN;}
"arg" {return ARGUMENT;}
"void" {return VOID;}
"while" {return WHILE;}
"for" {return FOR;}
"main" {return MAIN;}
{id} {yylval.var = strdup(yytext); return ID;}
{number} {yylval.val = atoi(yytext); return NUMBER;}
"+" {return ADD;}
"-" {return SUB;}
"*" {return MUL;}
"/" {return DIV;}
"=" {return ASSIGN;}
"!" {return NOT;}
"==" {return EQ;}
"!=" {return NEQ;}
"<" {return LT;}
"<=" {return LE;}
">" {return GT;}
">=" {return GE;}
"&&" {return AND;}
"||" {return OR;}
"," {return SEPER;}
";" {return SC;}
"(" {return LPAR;}
")" {return RPAR;}
"//"[^\n]*
[ \t\n]+
. {printf("Unrecognized character: %s\n", yytext);}
%%
int yywrap(){return 1;}

















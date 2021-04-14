LEX = lex
YACC = yacc -d
CC = gcc

default: compile

compile: y.tab.c lex.yy.c
	$(CC) -g y.tab.c lex.yy.c -ll

y.tab.c: Parser.y
	$(YACC) Parser.y

lex.yy.c: Scanner.l
	$(LEX) Scanner.l

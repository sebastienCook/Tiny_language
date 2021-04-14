/*
*	Project 5807 Z
*		author: Sebastien Cook
*		date: April 14 2021
*/
%{
#include <stdlib.h>
#include <stdio.h> 
#include "DST.c" 
#include "semantic_analysis.c"
#include "p_code_machine.c"
#include "codeGeneration.c"
extern FILE *yyin;
int yyerror(char * s);
int yylex();
struct dst_node *dst;
struct symbol_node *symbol_table;
%}

%union{
	char *var; 
	int val; 
	struct dst_node *dst_ptr;
};
%token ASSIGN SUB ADD MUL DIV NOT EQ NEQ LT LE GT GE AND OR LPAR RPAR SEPER SC;
%token <val> NUMBER;
%token <var> ID;
%token INT IF ELSE FUNC RETURN ARGUMENT VOID WHILE FOR MAIN;
%start program 

%type <dst_ptr> program functions function function_args function_body body_of_logic simple_statement variable_declaration variable_assignment args add_expr mul_expr factor expression function_call loop for while loop_body comparison if else else_if if_args if_else_body

%%

program: functions {
	dst = new_program_dst_node();
	dst->down = $1;
};
functions: function {$$ = $1;}
	| function functions{
		$1->side = $2;
		$$ = $1;
	}; 
function: FUNC ID LPAR function_args RPAR LPAR function_body RPAR
	{
		$$ = new_function_dst_node($2);
		$$->name = $2;
		$$->args = $4;
		$$->down = $7;
	}
	| FUNC ID LPAR VOID RPAR LPAR function_body RPAR
	{
		$$ = new_function_dst_node($2);
		$$->name = $2;
		$$->down = $7;
	}
	| FUNC MAIN LPAR VOID RPAR LPAR function_body RPAR
	{
		$$ = new_Mainfunction_dst_node();
		$$->down = $7;
	};
function_args: ARGUMENT ID 
	{
		$$ = new_argument_dst_node($2);
		$$->name = $2;
	}
	| function_args SEPER ARGUMENT ID
	{
		$$ = new_argument_dst_node($4);
		$$->name = $4;
		$$->side = $1;
	};
function_body: RETURN expression SC 
	{
		$$ = new_return_dst_node();
		$$->down = $2;
	}
	| body_of_logic RETURN expression SC
	{
		$$ = new_return_dst_node();
		$$->down = $3;
		$$->side = $1;
	};

/*logic body*/
body_of_logic: simple_statement{$$=$1;}
	| simple_statement body_of_logic
	{
		$1->side = $2;
		$$=$1;
	};
simple_statement: variable_declaration{$$=$1;}
	| variable_assignment{$$=$1;}
	| loop {$$=$1;}
	| if {$$=$1;}
	| else_if {$$=$1;}
	| else {$$=$1;};

/*variable_declaration*/
variable_declaration: INT ID SC
{$$ = new_dstnode_variabledeclaration($2);};
variable_assignment: ID ASSIGN expression SC
	{
		$$ = new_dstnode_variableAssignment($1); 
		$$->name = $1;
		$$->down = $3;
	};

/* loop */ 
loop: for{$$ = $1;}
	| while{$$ = $1;};
for: FOR LPAR variable_assignment comparison SC variable_assignment RPAR LPAR loop_body RPAR 
	{
		$$ = new_for_dst_node();
		$$->args = $3;
		$3->side = $6;
		$6->side = $4;
		$$->down = $9;
	};
while: WHILE LPAR if_args RPAR LPAR loop_body RPAR
	{
		$$ = new_while_dst_node();
		$$->down = $6;
		$$->args = $3;
	};
loop_body: {$$ = NULL;}
	| body_of_logic{$$ = $1;};
/*bool*/
comparison: LPAR expression EQ expression RPAR
	{
		$$ = new_bools_dst_node();
		$$->name = "EQ";
		$$->down = $2;
		$$->side = $4;
	}
	| LPAR expression NEQ expression RPAR
	{
		$$ = new_bools_dst_node();
		$$->name = "NEQ";
		$$->down = $2;
		$$->side = $4;
	}
	| LPAR expression LT expression RPAR
	{
		$$ = new_bools_dst_node();
		$$->name = "LT";
		$$->down = $2;
		$$->side = $4;
	}
	| LPAR expression LE expression RPAR
	{
		$$ = new_bools_dst_node();
		$$->name = "LE";
		$$->down = $2;
		$$->side = $4;
	}
	| LPAR expression GT expression RPAR	
	{
		$$ = new_bools_dst_node();
		$$->name = "GT";
		$$->down = $2;
		$$->side = $4;
	}
	| LPAR expression GE expression RPAR
	{
		$$ = new_bools_dst_node();
		$$->name = "GE";
		$$->down = $2;
		$$->side = $4;
	};
/*if_else grammar*/
if: IF LPAR if_args RPAR LPAR if_else_body RPAR
	{
		$$ = new_if_dst_node();
		$$->args = $3;
		$$->down = $6;
	};
else: ELSE LPAR if_else_body RPAR
	{
		$$ = new_else_dst_node();
		$$->down = $3;
	};
else_if: ELSE IF LPAR if_args RPAR LPAR if_else_body RPAR
	{
		$$ = new_else_if_dst_node();
		$$->args = $4;
		$$->down = $7;
	};

if_else_body: {$$ = NULL;}
	| body_of_logic {$$ = $1;}
	| function_body{$$ = $1;};
if_args: comparison{$$ = $1;}
	| if_args AND comparison{
		$$ = new_and_dst_node();
		$$->side = $3;
		$$->down = $1;
	};
	| if_args OR comparison{
		
		$$ = new_or_dst_node();
		$$->side = $3;
		$$->down = $1;
	};

/*Expression*/
expression: function_call{$$=$1;}
	| add_expr{$$=$1;}

function_call: ID LPAR args RPAR
	{
		$$ = new_function_call_dst_node($1);
		$$->down = $3;
	};
args: 	{$$ = NULL;}
	| add_expr{$$=$1;}
	| args SEPER add_expr
	{
		$3->side = $1;
		$$=$3;
	};
add_expr: mul_expr {$$ = $1;}
	| add_expr ADD mul_expr 
	{
		$$ = new_add_dst_node();
		$$->down = $3;
		$$->side = $1;
	}
	| add_expr SUB mul_expr 
	{
		$$ = new_sub_dst_node();
		$$->down = $3;
		$$->side = $1;
	};
mul_expr: factor {$$ = $1;}
	| mul_expr MUL factor 
	{
		$$ = new_mul_dst_node();
		$$->down = $3;
		$$->side = $1;
	}
	| mul_expr DIV factor 
	{
		$$ = new_div_dst_node();
		$$->down = $3;
		$$->side = $1;
	};
factor: NUMBER
	{
		$$ = new_num_dst_node();
		$$->value = $1;
	}
	| ID{$$ = new_var_dst_node($1);
		$$->name = $1;}
	| LPAR expression RPAR{$$=$2;};





%%
#define INTERPRET 0

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Undefined input file. Usage: \"<program name> <source file>\"\n");
		return 0;
	}
	yyin = fopen(argv[1], "r");
	if(yyin == NULL)
	{
		printf("Could not open input file %s\n", argv[1]);
		return 0;
	}
	yyparse();
	printf("\n************ DST TABLE *********\n\n");
	printf("side: --Type...\n");
	printf("      --Type...\n\n");
	printf("down: --Type...\n");
	printf("      ----Type...\n\n");
	printf("args: --Type...\n");
	printf("      ----------Type...\n\n");
	refactor_dst(dst,"");
	print_dst(dst,0);
	printf("\n\n************ SYMBOL TABLE *********\n\n");
	struct symbol_node *symbol_table = (struct symbol_node *)malloc(sizeof(struct symbol_node));
	symbol_table = build_symbol_table(dst,symbol_table);
	print_symbol_table(symbol_table);
	printf("\n\n*********** CHECKING SEMANTICS ************\n\n");
	if(check_semantics(dst) > 0){
		printf("EXITTING\n");
		return 0;
	}
	printf("\n\n************ P CODE *********\n\n");
	struct IR_node *IR = (struct IR_node *)malloc(sizeof(struct IR_node)); 
	IR = generate_IR(dst);
	print_IR(IR);
	
	//if(INTERPRET){
		printf("\n\n************ P CODE INTERPRETATION *********\n\n");
		interpret(IR,symbol_table);
	//}else{
		printf("\n\n************ Generate Code *********\n\n");
		generate_code(IR,symbol_table);
		printf("You can find the generated code in code_generated.m\n");
		
	//}
	
}
int yyerror(char * s){ 
	printf("yyerror: %s\n", s);
}













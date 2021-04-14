/*
*	Project 5807 Z
*		author: Sebastien Cook
*		date: April 14 2021
*/
#include <string.h>

enum node_type{PROGRAM=100, MAIN_FUNC, FUNCTION, VARIABLE, ASSIGNMENT, IF_CALL, ELSE_IF_CALL, ELSE_CALL, FOR_LOOP, WHILE_LOOP, ARG, RETURN_FUNC, FUNC_CALL, EXPRESSION, NUM, VAR, ADD_FUNC, SUB_FUNC, MUL_FUNC, DIV_FUNC, BOOL, AND_CALL,OR_CALL};

struct dst_node{
	enum node_type type;
	char *name;
	int value;
	struct dst_node *args;
	struct dst_node *down;
	struct dst_node *side; 	
};

//new program declaration
struct dst_node *new_program_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = PROGRAM;
	//zero all non-requied fields
	node->name = "program";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new function declaration
struct dst_node *new_Mainfunction_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = MAIN_FUNC;
	//zero all non-requied fields
	node->name = "MAIN";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
struct dst_node *new_function_dst_node(char *n){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = FUNCTION;
	//zero all non-requied fields
	node->name = (char *)malloc(strlen(n)+1);
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new return declaration
struct dst_node *new_return_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = RETURN_FUNC;
	//zero all non-requied fields
	node->name = "return";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new function argument node
struct dst_node *new_argument_dst_node(char *n){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = ARG;
	//zero all non-requied fields
	node->name = (char *)malloc(strlen(n)+1);
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};

//new if statement
struct dst_node *new_if_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = IF_CALL;
	//zero all non-requied fields
	node->name = "if";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new else if statement
struct dst_node *new_else_if_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = ELSE_IF_CALL;
	//zero all non-requied fields
	node->name = "else if";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new else statement 
struct dst_node *new_else_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = ELSE_CALL;
	//zero all non-requied fields
	node->name = "else";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new for loop declaration
struct dst_node *new_for_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = FOR_LOOP;
	//zero all non-requied fields
	node->name = "for";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new while loop declaration 
struct dst_node *new_while_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = WHILE_LOOP;
	//zero all non-requied fields
	node->name = "while";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new variable declaration 
struct dst_node *new_dstnode_variabledeclaration(char *n){
	
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = VARIABLE;
	node->name = (char *)malloc(strlen(n)+1);
	strcpy(node->name,n);
	//zero all non-requied fields
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	

	return node;
};
//new variable assignment
struct dst_node *new_dstnode_variableAssignment(char *n){
	
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = ASSIGNMENT;
	node->name = (char *)malloc(strlen(n)+1);
	strcpy(node->name,n);
	//zero all non-requied fields
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new function call
struct dst_node *new_function_call_dst_node(char *n){
	
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = FUNC_CALL;
	node->name = (char *)malloc(strlen(n)+1);
	strcpy(node->name,n);
	//zero all non-requied fields
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//arguent values made in a call
struct dst_node *new_arg_in_call(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = ARG;
	node->name = "";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//number in expression
struct dst_node *new_num_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = NUM;
	//zero all non-requied fields
	node->name = "";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
// new var meaining mus verify that variable exists 
struct dst_node *new_var_dst_node(char *n){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = VAR;
	//zero all non-requied fields
	node->name = (char *)malloc(strlen(n)+1);
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new sub declaration
struct dst_node *new_sub_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = SUB_FUNC;
	//zero all non-requied fields
	node->name = "-";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new mul declaration
struct dst_node *new_mul_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = MUL_FUNC;
	//zero all non-requied fields
	node->name = "*";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new DIV declaration
struct dst_node *new_div_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = DIV_FUNC;
	//zero all non-requied fields
	node->name = "/";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new add declaration
struct dst_node *new_add_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = ADD_FUNC;
	//zero all non-requied fields
	node->name = "+";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//new bool call
struct dst_node *new_bools_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = BOOL;
	//zero all non-requied fields
	node->name = "";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//AND in an if
struct dst_node *new_and_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = AND_CALL;
	//zero all non-requied fields
	node->name = "AND";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};
//OR in if
struct dst_node *new_or_dst_node(){
	struct dst_node *node = (struct dst_node *)malloc(sizeof(struct dst_node));
	node->type = OR_CALL;
	//zero all non-requied fields
	node->name = "OR";
	node->value = 0;
	node->down = NULL;
	node->side = NULL;
	node->args = NULL;
	return node;
};

/*******************************************/
/************** DST functions **************/
/*******************************************/

// counts the number of arguments a function has 
int count_args(struct dst_node *args){
	if(args == NULL)
	{return 0;}
	else{
		if(args->type == ADD_FUNC || args->type == SUB_FUNC || args->type == MUL_FUNC || args->type == DIV_FUNC){
			return 0 + count_args(args->side);
		}else{
			return 1 + count_args(args->side);
		}
	}
}


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// changes the sturcture of the DST tree
// 	- add number of arguments to value paramater of function nodes
// 	- concat the name of the function to variable names 
void refactor_dst(struct dst_node *dst, char *scope){
	if(dst == NULL){}
	else{
		if(dst->type == FUNCTION){
			dst->value = count_args(dst->args);
			refactor_dst(dst->side, "");
			refactor_dst(dst->down, dst->name);
			refactor_dst(dst->args, dst->name);
		}else if(dst->type == MAIN_FUNC){
			refactor_dst(dst->side, "");
			refactor_dst(dst->down, dst->name);
		}else{
			if(dst->type == FUNC_CALL){
				dst->value = count_args(dst->down);
				refactor_dst(dst->args, dst->name);
			}
			if(dst->type == VARIABLE || dst->type == ARG || dst->type == ASSIGNMENT || dst->type == VAR){
				char *name_scope = concat(dst->name,scope);
				dst->name = name_scope;
			}
			refactor_dst(dst->side, scope);
			refactor_dst(dst->down, scope);
			refactor_dst(dst->args, scope);
		}
		
		
	}

}
// pretty print!
void print_dst(struct dst_node *dst, int depth){
	int i;
	for(i = depth; i>0; i--){
		printf("--");
	}
	printf("Type: %d Name: %s Val: %d\n", dst->type, dst->name, dst->value);
	if(dst->type == FUNCTION || dst->type == FOR_LOOP || dst->type == WHILE_LOOP || dst->type == IF_CALL || dst->type == ELSE_IF_CALL){
		if(dst->args != NULL){print_dst(dst->args, depth+5);}
	}
	if(dst->down != NULL){print_dst(dst->down, depth+1);}
	if(dst->side != NULL){print_dst(dst->side, depth);}
}

/*******************************************/
/********* Symbol table functions **********/
/*******************************************/

struct symbol_node{
	char *name;	//name of token
	enum node_type type; //type of token
	struct symbol_node *next; //next token
};

//builds the symboltable
struct symbol_node *build_symbol_table(struct dst_node *dst, struct symbol_node *s){
	if(dst == NULL){
		return NULL;
	}else{
		switch(dst->type){
			case PROGRAM:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				symbol->type = dst->type;
				symbol->name = "GLOBAL";
				symbol->next = build_symbol_table(dst->down,symbol);
				return symbol;
				break;
				}
			case MAIN_FUNC:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				symbol->name = dst->name;
				symbol->type = dst->type;
				symbol->next = build_symbol_table(dst->down,symbol);
				struct symbol_node *next_s = symbol;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->side,s);
				return symbol;
				break;
				}
			case FUNCTION:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				symbol->name = dst->name;
				symbol->type = dst->type;
				symbol->next = build_symbol_table(dst->args,symbol);
				struct symbol_node *next_s = symbol;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->down,symbol);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->side,s);
				return symbol;
				break;
				}
			case VARIABLE:
				{
				struct symbol_node *next_s = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				next_s->name = dst->name;
				next_s->type = dst->type;
				next_s->next = build_symbol_table(dst->side,s);
				return next_s;
				break;
				}
			case ARG:
				{
				struct symbol_node *next_s = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				next_s->name = dst->name;
				next_s->type = dst->type;
				next_s->next = build_symbol_table(dst->side,s);
				return next_s;
				break;
				}
			case IF_CALL:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				struct symbol_node *next_s = symbol;
				next_s->name = dst->name;
				next_s->next = build_symbol_table(dst->down,next_s);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->side,s);
				struct symbol_node *temp = symbol;
				symbol = symbol->next;
				free(temp);
				return symbol;
				break;
				}
			case ELSE_IF_CALL:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				struct symbol_node *next_s = symbol;
				next_s->name = dst->name;
				next_s->next = build_symbol_table(dst->down,next_s);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->side,s);
				struct symbol_node *temp = symbol;
				symbol = symbol->next;
				free(temp);
				return symbol;
				break;
				}
			case ELSE_CALL:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				struct symbol_node *next_s = symbol;
				next_s->name = dst->name;
				next_s->next = build_symbol_table(dst->down,next_s);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->side,s);
				struct symbol_node *temp = symbol;
				symbol = symbol->next;
				free(temp);
				return symbol;
				break;
				}
			case FOR_LOOP:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				struct symbol_node *next_s = symbol;
				next_s->name = dst->name;
				next_s->next = build_symbol_table(dst->down,next_s);
				while(next_s->next!= NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->side,s);
				struct symbol_node *temp = symbol;
				symbol = symbol->next;
				free(temp);
				return symbol;
				break;
				}
			case WHILE_LOOP:
				{
				struct symbol_node *symbol = (struct symbol_node *)malloc(sizeof(struct symbol_node));
				struct symbol_node *next_s = symbol;
				next_s->name = dst->name;
				next_s->next = build_symbol_table(dst->down,next_s);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_symbol_table(dst->side,s);
				struct symbol_node *temp = symbol;
				symbol = symbol->next;
				free(temp);
				return symbol;
				break;
				}
			default:
				return build_symbol_table(dst->side,s);
		}
	} 
}
void print_symbol_table(struct symbol_node *s){
	if(s == NULL){}
	else{
		printf("TYPE: %d NAME: %s\n",s->type,s->name);
		print_symbol_table(s->next);
	}
}





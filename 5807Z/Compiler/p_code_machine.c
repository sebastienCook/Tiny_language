/*
*	Project 5807 Z
*		author: Sebastien Cook
*		date: April 14 2021
*/
// 3 implicite registers: 
// 	- PC -> program counter 
// 	- SP -> stack pointer -> points to the top of stack 
// 	- BP -> base pointer -> points to the bottom of the stack 

// Types of instructions 
// - push, pop
// - add, sub, mul, div, equals?, greater?...
// - jump, brct, brcf, call, ret, nop

enum p_code_inst{push = 500, pop, p_add, p_sub, p_mul, p_div, p_not, p_eq, p_neq, p_lt, p_le, p_gt, p_ge, p_and, p_or, jump, brct, brcf, call, ret, nop}; 

enum p_code_operand_type{p_reg=300, p_id, NUMS, OPERATION, p_return, IDENTIFIER, REG};

enum p_code_register{PC,SP,BP,LR}; 

struct IR_node{
	char *label; //labels location to jump to when that label is called
	int args;	//used for functions and function calls -> number of arguments 
	enum p_code_inst instruction; //type of instruction 
	enum p_code_operand_type operand_type; //what type of node it is 

	struct op{
		char *identifier; //funcction name or variable name ect... 
		int constant; 	//offset unless its a NUMS, which then hols the number as an int
		enum p_code_register p_register;
	} p_code_operand;
	struct IR_node *next; 
};

/****************************************/
/****************************************/
/********** P_code Generation ***********/
/****************************************/
/****************************************/

//pretty printer
void print_IR(struct IR_node *IR){
	if(IR== NULL){
	}else{
		char *instruction;
		switch(IR->instruction){
			case push:
				instruction = "PUSH";
				break;
			case pop:
				instruction = "POP";
				break;
			case p_add:
				instruction = "ADD";
				break;
			case p_sub:
				instruction = "SUB";
				break;
			case p_mul:
				instruction = "MUL";
				break;
			case p_div:
				instruction = "DIV";
				break;
			case p_not:
				instruction = "NOT";
				break;
			case p_eq:
				instruction = "EQUAL?";
				break;
			case p_neq:
				instruction = "NOT_EQUAL?";
				break;
			case p_lt:
				instruction = "LESS?";
				break;
			case p_le:
				instruction = "LESS_EQUAL?";
				break;
			case p_gt:
				instruction = "GREATER?";
				break;
			case p_ge:
				instruction = "GREATER_EQUAL?";
				break;
			case p_and:
				instruction = "AND";
				break;
			case p_or:
				instruction = "OR";
				break;
			case jump:
				instruction = "JUMP";
				break;
			case brct:
				instruction = "BRANCH_IF_TRUE";
				break;
			case brcf:
				instruction = "BRANCH_IF_FALSE";
				break;
			case call:
				instruction = "CALL";
				break;
			case ret:
				instruction = "RETURN";
				break;
			default:
				instruction = "NOP";
		}
		if(IR->operand_type == NUMS){
			printf("	%s: %d\n",instruction,IR->p_code_operand.constant);
		}else if(IR->operand_type == REG){
			printf("	%s: %s :%s\n",instruction,IR->p_code_operand.identifier,"REG");
		}
		else{
			if(IR->label != NULL){
				printf("%s:	%s: %s\n",IR->label,instruction,IR->p_code_operand.identifier);
			}else{
				printf("	%s: %s\n",instruction,IR->p_code_operand.identifier);
			}
		}
		print_IR(IR->next);
	}
}
//create labels with unique names 
char *gen_label(){
	static int label_counter = 0;
	char *new_label = (char *)malloc(5);
	sprintf(new_label, "L%d", label_counter);
	label_counter++;
	return new_label;
}

//creates the pcode
struct IR_node *generate_IR(struct dst_node *dst){
		
	if(dst == NULL){
		return NULL;
	}else{
		switch(dst->type){
			case PROGRAM:
				{
				struct IR_node *new_IR_node = (struct IR_node *)malloc(sizeof(struct IR_node));
				new_IR_node->label="exit";
				new_IR_node->instruction = call;
				new_IR_node->args = 0;
				new_IR_node->operand_type = IDENTIFIER;
				new_IR_node->p_code_operand.identifier = "MAIN";
				new_IR_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				struct IR_node *last_node = new_IR_node->next;
				last_node->instruction = jump;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = "exit";
				last_node->next = generate_IR(dst->down);
				return new_IR_node;
				break;
				}
			case MAIN_FUNC:
				{ 
				struct IR_node *new_IR_node = (struct IR_node *)malloc(sizeof(struct IR_node));
				new_IR_node->instruction = nop;
				new_IR_node->operand_type = IDENTIFIER;
				new_IR_node->p_code_operand.identifier = "MAIN";
				new_IR_node->label = "MAIN";
				new_IR_node->next = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node;
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				return new_IR_node;
				break;
				}
			case FUNCTION:
				{
				struct IR_node *new_IR_node = (struct IR_node *)malloc(sizeof(struct IR_node));
				new_IR_node->instruction = nop;
				new_IR_node->operand_type = IDENTIFIER;
				new_IR_node->p_code_operand.identifier = dst->name;
				new_IR_node->label = dst->name;
				struct IR_node *last_node = new_IR_node;
				last_node->next = generate_IR(dst->down);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				return new_IR_node;
				break;
				}
			case ASSIGNMENT:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = pop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = strdup(dst->name);
				last_node->next = generate_IR(dst->side);
				return new_IR_node;
				break;
				}
			case IF_CALL:
				{
				char *label_e=gen_label(), *label_else=gen_label();
				struct IR_node *new_IR_node = generate_IR(dst->args);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = brcf;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_else;
				last_node->next = generate_IR(dst->down);
				//jump at the end of the if logic
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = jump;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				//else label always made
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->label = label_else;
				last_node->instruction = nop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_else;
				//check if there is actually an else to fill
				if(dst->side!=NULL){
					if(dst->side->type == ELSE_CALL){
						last_node->next = generate_IR(dst->side);
						dst = dst->side;
					}else if(dst->side->type == ELSE_IF_CALL){
						last_node->next = generate_IR(dst->side);
						while(dst->side->type != ELSE_CALL){
							dst = dst->side;
						}
						dst = dst->side;
					}else{}
				}
				//labe if if jump used
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->label = label_e;
				last_node->instruction = nop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				last_node->next = generate_IR(dst->side);
				return new_IR_node;
				break;
				}
			case ELSE_IF_CALL:
				{
				char *label_e=gen_label(), *label_else=gen_label();
				struct IR_node *new_IR_node = generate_IR(dst->args);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = brcf;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_else;
				last_node->next = generate_IR(dst->down);
				//jump at the end of the if logic
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = jump;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				//else label always made
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->label = label_else;
				last_node->instruction = nop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_else;
				//check if there is actually an else to fill
				if(dst->side!=NULL){
					if(dst->side->type == ELSE_CALL){
						last_node->next = generate_IR(dst->side);
						dst = dst->side;
					}else if(dst->side->type == ELSE_IF_CALL){
						last_node->next = generate_IR(dst->side);
					}else{}
				}
				//labe if if jump used
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->label = label_e;
				last_node->instruction = nop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				return new_IR_node;
				break;
				}
			case ELSE_CALL:
				return generate_IR(dst->down);
				break;
			case FOR_LOOP:
				{
				//isolate argument nodes
				char *label_b=gen_label(), *label_e=gen_label();
				struct dst_node *set = (struct dst_node *)malloc(sizeof(struct dst_node));
				struct dst_node *next = (struct dst_node *)malloc(sizeof(struct dst_node));
				struct dst_node *check = (struct dst_node *)malloc(sizeof(struct dst_node));
				*set = *dst->args;
				*next = *dst->args->side;
				*check = *dst->args->side->side;
				set->side = NULL; next->side = NULL;
				//put the set at the top of the stack
				struct IR_node *new_IR_node = generate_IR(set);
				struct IR_node *last_node = new_IR_node; 
				//set up the for check with label
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(check);
				last_node = last_node->next;
				last_node->label = label_b;
				//creat branch if false body
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = brcf;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				last_node->next = generate_IR(dst->down);	
				//interate the variable for check
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(next);	
				//create jump back
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = jump;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = label_b;
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->label = label_e;
				last_node->instruction = nop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				last_node->next = generate_IR(dst->side);
				return new_IR_node;
				break;
				} 
			case WHILE_LOOP:
				{
				char *label_b=gen_label(), *label_e=gen_label();
				struct IR_node *new_IR_node = generate_IR(dst->args);
				struct IR_node *last_node = new_IR_node; 
				new_IR_node->label = label_b;
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = brcf;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				last_node->next = generate_IR(dst->down);	
				//go to last one again, generate post code
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = jump;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = label_b;
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->label = label_e;
				last_node->instruction = nop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = label_e;
				last_node->next = generate_IR(dst->side);
				return new_IR_node;
				break;
				} 
			case ARG:
				{
				struct IR_node *last_node = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node->instruction = pop;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = strdup(dst->name);
				last_node->next = generate_IR(dst->side);
				return last_node;
				break;
				}
			case RETURN_FUNC:
				{
				struct IR_node *new_IR_node = (struct IR_node *)malloc(sizeof(struct IR_node));
				new_IR_node = generate_IR(dst->side);
				struct IR_node *last_node = new_IR_node;
				//if the body is empty then make node the return statement
				if(new_IR_node == NULL){
					new_IR_node = generate_IR(dst->down);
					last_node = new_IR_node;
				}else{
					while(last_node->next != NULL){last_node = last_node->next;}
					last_node->next = generate_IR(dst->down);
					
				}
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = ret;
				last_node->operand_type = p_return;
				last_node->p_code_operand.identifier = "RETURN";
				return new_IR_node;
				break;
				}
			case FUNC_CALL:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node;
				//no argument function 
				if(new_IR_node == NULL){
					
				}else{
					while(last_node->next != NULL){last_node = last_node->next;}
					last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
					last_node = last_node->next;
				}
				last_node->instruction = call;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = dst->name;
				//number of args 
				last_node->args = dst->value;
				return new_IR_node;
				break;
				}
			case NUM:
				{
				struct IR_node *last_node = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node->instruction = push;
				last_node->operand_type = NUMS;
				last_node->p_code_operand.constant = dst->value;
				last_node->next = generate_IR(dst->side);
				return last_node;
				break;
				}
			case VAR:
				{
				struct IR_node *last_node = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node->instruction = push;
				last_node->operand_type = IDENTIFIER;
				last_node->p_code_operand.identifier = strdup(dst->name);
				last_node->next = generate_IR(dst->side);
				return last_node;
				break;
				}
			case ADD_FUNC:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = p_add;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = strdup(dst->name);
				return new_IR_node;
				break;
				}
			case SUB_FUNC:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = p_sub;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = strdup(dst->name);
				return new_IR_node;
				break;
				}
			case MUL_FUNC:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = p_mul;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = strdup(dst->name);
				return new_IR_node;
				break;
				}
			case DIV_FUNC:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = p_div;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = strdup(dst->name);
				return new_IR_node;
				break;
				}
			case BOOL:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				//what comparison is used
				if(!strcmp(dst->name, "EQ")){last_node->instruction = p_eq;}
				else if(!strcmp(dst->name, "NEQ")){last_node->instruction = p_neq;}
				else if(!strcmp(dst->name, "LT")){last_node->instruction = p_lt;}
				else if(!strcmp(dst->name, "LE")){last_node->instruction = p_le;}
				else if(!strcmp(dst->name, "GT")){last_node->instruction = p_gt;}
				else {last_node->instruction = p_ge;}
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = strdup(dst->name);
				return new_IR_node;
				break;
				}
			case AND_CALL:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = p_and;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = strdup(dst->name);
				return new_IR_node;
				break;
				}
			case OR_CALL:
				{
				struct IR_node *new_IR_node = generate_IR(dst->down);
				struct IR_node *last_node = new_IR_node; 
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = generate_IR(dst->side);
				while(last_node->next != NULL){last_node = last_node->next;}
				last_node->next = (struct IR_node *)malloc(sizeof(struct IR_node));
				last_node = last_node->next;
				last_node->instruction = p_or;
				last_node->operand_type = OPERATION;
				last_node->p_code_operand.identifier = strdup(dst->name);
				return new_IR_node;
				break;
				}
			default:
				return generate_IR(dst->side);
		}
	}
	return NULL;
}


/****************************************/
/****************************************/
/******* P_code Interpretation **********/
/****************************************/
/****************************************/

//helper function to label_resolving
void resolve_label(char *label, int offset, struct IR_node *IR){
	struct IR_node *IR_label = IR;
	int i = 0;
	while(IR_label != NULL){
		if((IR_label->instruction == jump) || (IR_label->instruction == brct) || (IR_label->instruction == brcf) || (IR_label->instruction == call)){
			if(strcmp(IR_label->p_code_operand.identifier,label)==0){
				IR_label->p_code_operand.constant = offset-i;
			}
		}
		i++; IR_label = IR_label->next;
	}

}
//finds the offset of label calls to the label destination and places it in the nodes constant param
void label_resolving(struct IR_node *IR){
	int offset = 0;	
	struct IR_node *IR_label_ptr = IR;

	while(IR_label_ptr != NULL){
		if(IR_label_ptr->label != NULL){
			resolve_label(IR_label_ptr->label, offset, IR);
		}
		IR_label_ptr = IR_label_ptr->next; offset++;
	}
}
//get instruction at pc
struct IR_node *get_instruction(struct IR_node *p_code,int pc){
	int i=0;
	struct IR_node *instruction = p_code;
	while(i!=pc && instruction!=NULL){
		instruction = instruction->next;
		i++;
	}
	return instruction;
}
//counts the number of variables a given function has in the symbol table
int mem_needed(char *scope, struct symbol_node *s){
	struct symbol_node *temp = s;
	int i=0;
	while(strcmp(temp->name,scope) && temp!=NULL){
		temp = temp->next;
	}
	temp = temp->next;
	while(temp != NULL && (temp->type != FUNCTION) && (temp->type != MAIN_FUNC)){
		i++;
		temp = temp->next;
	}
	return i;
}
//helper function to variable_offset_resolve
int offset_resolve(struct symbol_node *s, char *name){
	int i = -1;
	struct symbol_node *t = s;
	while(t!=NULL){
		
		if(t->type == FUNCTION || t->type == MAIN_FUNC || t->type == PROGRAM){
			i = -1;
		}
		if(!strcmp(t->name, name)){
			//printf("Offset of %s: %d\n",name,i);
			return i;
		}
		i++; t = t->next;
	}
	return 0;
}
//finds the offset in the symbol table and places it in the nodes constant param
void variable_offset_resolve(struct IR_node *IR, struct symbol_node *s){
	struct IR_node *temp = IR;
	while(temp!=NULL){
		if(temp->instruction == push || temp->instruction == pop){
			if(temp->operand_type == IDENTIFIER){
				temp->p_code_operand.constant = offset_resolve(s,temp->p_code_operand.identifier);
			}
		}
		temp = temp->next;
	}
}
void interpret(struct IR_node *p_code, struct symbol_node *s){
	char *scope = "GLOBAL";
	//Label resolving
	label_resolving(p_code);
	variable_offset_resolve(p_code,s);
	//set up environment 
	int pc = 0;
	int bp = 0;
	int sp = -1;
	int STACK_SIZE = 64;
	int stack[STACK_SIZE];
	int i;
	for(i=0;i<STACK_SIZE;i++){
		stack[i]=0;
	}
	struct IR_node *instruction;
	

	//interpret code
	while(pc!=1){
		instruction = get_instruction(p_code,pc);
		if(sp>=64){
			//this will make pc == 1 and exit the function
			printf("STACK OVERFLOW\n");
			pc = 0;
			sp=-1;
			bp=0;
			instruction->instruction = 666;
		}
		switch(instruction->instruction){
			case push:
				{
				//place val at ID on stack 
				if(instruction->operand_type == NUMS){
					stack[sp+1] = instruction->p_code_operand.constant;
					sp++;
					pc++;
				}else{
					stack[sp+1] = stack[bp + instruction->p_code_operand.constant];
					sp++;
					pc++;
				}
				break;
				}
			case pop:
				{
				//place val at the ID in mem
				stack[bp + instruction->p_code_operand.constant] = stack[sp];
				sp--;
				pc++;
				break;
				}
			case p_add:
				{
				//add two top values on stack and place back on stack 
				stack[sp-1] = stack[sp] + stack[sp-1];
				sp--;
				pc++;
				break;
				}
			case p_sub:
				{
				//sub two top values on stack and place back on stack
				stack[sp-1] = stack[sp] - stack[sp-1];
				sp--;
				pc++;
				break;
				}
			case p_mul:
				{
				//mul two top values on stack and place back on stack
				stack[sp-1] = stack[sp] * stack[sp-1];
				sp--;
				pc++;
				break;
				}
			case p_div:
				{
				//DIV two top values on stack and place back on stack
				stack[sp-1] = stack[sp] / stack[sp-1];
				sp--;
				pc++;
				break;
				}
			case p_not:
				{
				
				break;
				}
			case p_eq:
				{
				//check if equal 
				stack[sp-1] = (stack[sp] == stack[sp-1]) ? 1 : 0;
				sp--;
				pc++;
				break;
				}
			case p_neq:
				{
				//check if not equal 
				stack[sp-1] = (stack[sp] == stack[sp-1]) ? 0 : 1;
				sp--;
				pc++;
				break;
				}
			case p_lt:
				{
				stack[sp-1] = (stack[sp] > stack[sp-1]) ? 1 : 0;
				sp--;
				pc++;
				break;
				}
			case p_le:
				{
				stack[sp-1] = (stack[sp] >= stack[sp-1]) ? 1 : 0;
				sp--;
				pc++;
				break;
				}
			case p_gt:
				{
				stack[sp-1] = (stack[sp] < stack[sp-1]) ? 1 : 0;
				sp--;
				pc++;
				break;
				}
			case p_ge:
				{
				stack[sp-1] = (stack[sp] <= stack[sp-1]) ? 1 : 0;
				sp--;
				pc++;
				break;
				}
			case p_and:
				{
				stack[sp-1] = stack[sp] && stack[sp-1];
				sp--;
				pc++;
				break;
				}
			case p_or:
				{
				stack[sp-1] = stack[sp] || stack[sp-1];
				sp--;
				pc++;
				break;
				}
			case jump:
				{
				//pc = offset 
				pc = pc + instruction->p_code_operand.constant;
				break;
				}
			case brct:
				{
				//pc = offset if true 
				if(stack[sp] == 1)
					pc = pc + instruction->p_code_operand.constant;
				else
					pc++;
				sp--;
				break;
				}
			case brcf:
				{
				//pc = offset if false
				if((stack[sp]) == 0)
					pc = pc + instruction->p_code_operand.constant;
				else
					pc++;
				sp--;
				break;
				}
			case call:
				{
				//place all info needed to return to previous function 
				int old_sp = sp;
				int old_bp = bp;
				int num_args = mem_needed(instruction->p_code_operand.identifier,s);
				
				//allocate mem
				//since function args are already pushed bp
				bp = sp + 1 - instruction->args;
				//leave space on the stack for all variables instantiated in the func 
				sp = sp + num_args - instruction->args;

				//function call memory trace
				stack[sp+1] = old_sp;
				sp++;
	
				stack[sp+1] = pc;
				sp++;

				stack[sp+1] = old_bp;
				sp++;
				
				//jump pc to the new function 
				pc = pc + instruction->p_code_operand.constant;		
				break;
				}
			case ret:
				{
				//place return val at what will be the top of the stack 
				stack[bp] = stack[sp];
				sp--; 
				
				//retrive old function position
				bp = stack[sp];
				sp--;

				pc = stack[sp]+1;
				sp--;
				
				sp = stack[sp];
				break;
				}
			default:
				//NOP
				//increment pc by 1
				pc++;
		}
		
	}
	
	for(i=0;i<STACK_SIZE;i++){
		printf("[%d]: %d\n",i,stack[i]);
	}
	printf("\nOutput (stack[0]): %d", stack[0]);
}

























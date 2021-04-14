/*
*	Project 5807 Z
*		author: Sebastien Cook
*		date: April 14 2020
*/

/*******************************************/
/************ semantic analysis ************/
/*******************************************/

//sem node is an extended symbol tree that contains everything in order
// having the oder helps verify variable assignments and declarations
struct sem_node{
	char *name;	//name
	enum node_type type;
	int arg_num; //for functions
	struct sem_node *next; //next token

};

//pretty print
void print_sem(struct sem_node *s){
	if(s == NULL){}
	else{
		printf("TYPE: %d NAME: %s\n",s->type,s->name);
		print_sem(s->next);
	}
}

//build the semantics analysis table
struct sem_node *build_semantic_table(struct dst_node *dst){
	if(dst == NULL){
		return NULL;	
	}else{
		switch(dst->type){
			case PROGRAM:
				{
				struct sem_node *sem = (struct sem_node *)malloc(sizeof(struct sem_node));
				sem->type = dst->type;
				sem->name = "GLOBAL";
				sem->next = build_semantic_table(dst->down);
				return sem;
				break;
				}
			case MAIN_FUNC:
				{
				struct sem_node *sem = (struct sem_node *)malloc(sizeof(struct sem_node));
				sem->type = dst->type;
				sem->name = dst->name;
				sem->next = build_semantic_table(dst->down);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case FUNCTION:
				{
				struct sem_node *sem = (struct sem_node *)malloc(sizeof(struct sem_node));
				sem->type = dst->type;
				sem->name = dst->name;
				sem->next = build_semantic_table(dst->args);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->down);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case VARIABLE:
				{
				struct sem_node *sem = (struct sem_node *)malloc(sizeof(struct sem_node));
				sem->type = dst->type;
				sem->name = dst->name;
				sem->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case ARG:
				{
				struct sem_node *sem = (struct sem_node *)malloc(sizeof(struct sem_node));
				sem->type = dst->type;
				sem->name = dst->name;
				sem->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case ASSIGNMENT:
				{
				struct sem_node *sem = (struct sem_node *)malloc(sizeof(struct sem_node));
				sem->type = dst->type;
				sem->name = dst->name;
				sem->next = build_semantic_table(dst->down);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case VAR:
				{
				struct sem_node *sem = (struct sem_node *)malloc(sizeof(struct sem_node));
				sem->type = dst->type;
				sem->name = dst->name;
				sem->next = build_semantic_table(dst->down);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case FUNC_CALL:
				{
				struct sem_node *sem = build_semantic_table(dst->down);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case RETURN_FUNC:
				{
				struct sem_node *sem = build_semantic_table(dst->side);
				if(sem != NULL){
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->down);
				return sem;
				}else{
				return build_semantic_table(dst->down);
				}
				break;
				}
			case IF_CALL:
				{
				struct sem_node *sem = build_semantic_table(dst->args);
				struct sem_node *next_s = sem;	
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->down);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case ELSE_IF_CALL:
				{
				struct sem_node *sem = build_semantic_table(dst->args);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->down);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case ELSE_CALL:
				{
				struct sem_node *sem = build_semantic_table(dst->down);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case FOR_LOOP:
				{
				struct sem_node *sem = build_semantic_table(dst->args);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->down);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			case WHILE_LOOP:
				{
				struct sem_node *sem = build_semantic_table(dst->args);
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->down);
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				break;
				}
			default:
				{
				struct sem_node *sem = build_semantic_table(dst->down);
				if(sem != NULL){
				struct sem_node *next_s = sem;
				while(next_s->next != NULL){next_s = next_s->next;}
				next_s->next = build_semantic_table(dst->side);
				return sem;
				}else{
				return build_semantic_table(dst->side);
				}
				}
		}
	}
}

int find_num(char *name, int type,struct sem_node *sem){
	if(sem == NULL){
		return 0;
	}else if(!strcmp(name,sem->name) && type == sem->type){
		return 1 + find_num(name,type,sem->next);
	}else{
		return 0 + find_num(name,type,sem->next);
	}
}
int find_num_up_to(char *name, int type,struct sem_node *sem, struct sem_node *limit){
	if(sem == NULL){
		return 0;
	}else if(!strcmp(limit->name,sem->name) && (limit->type == sem->type)){
		return 0;
	}else if(!strcmp(name,sem->name) && type == sem->type){
		return 1 + find_num_up_to(name,type,sem->next,limit);
	}else{
		return 0 + find_num_up_to(name,type,sem->next,limit);
	}
}
int general_semantics(struct sem_node *sem, struct sem_node *top){
	if(sem == NULL){
		return 0;
	}else{
		switch(sem->type){
		case MAIN_FUNC:
			{
			int i = find_num(sem->name,sem->type,top);
			if(i>1){
				printf("ERROR: Function %s Defined More Than Once\n",sem->name);
				return 1 + general_semantics(sem->next,top);
			}else{
				return 0 + general_semantics(sem->next,top);
			}
			break;
			}
		case FUNCTION:
			{
			int i = find_num(sem->name,sem->type,top);
			if(i>1){
				printf("ERROR: Function %s Defined More Than Once\n",sem->name);
				return 1 + general_semantics(sem->next,top);
			}else{
				return 0 + general_semantics(sem->next,top);
			}
			break;
			}
		case VARIABLE:
			{
			int i = find_num(sem->name,sem->type,top);
			int j = find_num(sem->name,ASSIGNMENT,top);
			j = j + find_num(sem->name,VAR,top);
			if(i>1){
				printf("ERROR: Variable %s Defined More Than Once\n",sem->name);
				return 1 + general_semantics(sem->next,top);
			}else if(j<1){
				printf("WARNING: Variable %s Defined But Never Used\n",sem->name);
				return 0 + general_semantics(sem->next,top);
			}else{
				return 0 + general_semantics(sem->next,top);
			}
			break;
			}
		case ARG:
			{
			int i = find_num(sem->name,sem->type,top);
			i = i + find_num(sem->name,VARIABLE,top);
			int j = find_num(sem->name,ASSIGNMENT,top);
			j = j + find_num(sem->name,VAR,top);
			if(i>1){
				printf("ERROR: Variable %s Defined More Than Once\n",sem->name);
				return 1 + general_semantics(sem->next,top);
			}else if(j<1){
				printf("WARNING: Argument %s Defined But Never Used\n",sem->name);
				return 0 + general_semantics(sem->next,top);
			}else{
				return 0 + general_semantics(sem->next,top);
			}
			break;
			}
		case ASSIGNMENT:
			{
			int i = find_num_up_to(sem->name,VARIABLE,top,sem);
			i = i + find_num_up_to(sem->name,ARG,top,sem);
			if(i<1){
				printf("ERROR: Variable %s isn't denfined in this scope\n",sem->name);
				return 1 + general_semantics(sem->next,top);
			}else{
				return 0 + general_semantics(sem->next,top);
			}
			break;
			}
		case VAR:
			{
			int i = find_num_up_to(sem->name,ASSIGNMENT,top,sem);
			i = i + find_num_up_to(sem->name,ARG,top,sem);
			if(i<1){
				printf("ERROR: Variable %s isn't denfined or initialized in this scope\n",sem->name);
				return 1 + general_semantics(sem->next,top);
			}else{
				return 0 + general_semantics(sem->next,top);
			}
			break;
			}
		case FUNC_CALL:
			{
			int i = find_num(sem->name,FUNCTION,top);
			if(i<1){
				printf("ERROR: Function %s isn't denfined\n",sem->name);
				return 1 + general_semantics(sem->next,top);
			}else{
				return 0 + general_semantics(sem->next,top);
			}
			break;
			}
		default:
			return 0 + general_semantics(sem->next,top);
	}
	}

}
int if_semantics(struct dst_node *dst){
	if(dst == NULL){
		return 0;
	}else{
		if(dst->side != NULL){
		if(dst->side->type == ELSE_CALL || dst->side->type == ELSE_IF_CALL){
			if((dst->type != IF_CALL) && (dst->type != ELSE_IF_CALL)){
				printf("ERROR: else Or else if Must Be Preceeded By An if Or else if\n");
				return 1 + if_semantics(dst->down) + if_semantics(dst->side);
			}else{
				return 0 + if_semantics(dst->down) + if_semantics(dst->side);
			}
		}else if(dst->type == ELSE_IF_CALL){
			if(dst->side->type != ELSE_CALL || dst->side->type != ELSE_IF_CALL){
				printf("ERROR: else if Must Be Followed By An else Or else if\n");
				return 1 + if_semantics(dst->down) + if_semantics(dst->side);
			}else{
				return 0 + if_semantics(dst->down) + if_semantics(dst->side);
			}
		}else{
			return 0 + if_semantics(dst->down) + if_semantics(dst->side);
		}
		}
		return 0 + if_semantics(dst->down) + if_semantics(dst->side);
	}
}
// call semantic analysis functions and counts total errors
int check_semantics(struct dst_node *dst){
	struct sem_node *sem_analysis = build_semantic_table(dst);
	int errors = general_semantics(sem_analysis,sem_analysis) + if_semantics(dst);
	//print_sem(sem_analysis);
	printf("ERRORS Found: %d\n",errors);
	return errors;
}



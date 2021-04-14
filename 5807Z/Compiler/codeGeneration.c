/*
*	Project 5807 Z
*		author: Sebastien Cook
*		date: April 14 2021
*/

//helper function for Code_gen_label_resolving
void resolve_l(char *label, int offset, struct IR_node *IR){
	struct IR_node *IR_label = IR;
	int i = 0;
	while(IR_label != NULL){
		if(IR_label->instruction == call && strcmp(IR_label->p_code_operand.identifier,"MAIN") == 0){
			i += 16; 
		}else if(IR_label->instruction == call){
			i += 15; 
		}else{}
		if(IR_label->instruction == ret){
			i += 4; 
		}
		if((IR_label->instruction == jump) || (IR_label->instruction == brct) || (IR_label->instruction == brcf) || (IR_label->instruction == call)){
			if(strcmp(IR_label->p_code_operand.identifier,label)==0){
				
				if(!strcmp(label,"exit")){
					IR_label->p_code_operand.constant = 0-i;
				}else{
					IR_label->p_code_operand.constant = offset-i;
				}
			}
		}
		i++; IR_label = IR_label->next;
	}	
}
//special label resolver for jump distance for code generation... must take into acount added instructions
void Code_gen_label_resolving(struct IR_node *IR){
	int offset = 0;	
	struct IR_node *IR_label_ptr = IR;

	while(IR_label_ptr != NULL){
		if(IR_label_ptr->instruction == call && strcmp(IR_label_ptr->p_code_operand.identifier,"MAIN") == 0){
			offset += 16; //was 13
		}else if(IR_label_ptr->instruction == call){
			offset += 15; //was 12
		}else{}
		if(IR_label_ptr->instruction == ret){
			offset += 4; //was 3
		}
		if(IR_label_ptr->label != NULL){
			resolve_l(IR_label_ptr->label, offset, IR);
		}
		IR_label_ptr = IR_label_ptr->next; offset++;
	}
}

//generates the instruction and prints them into code_generated.mem
void generate_instruction(struct IR_node *IR,struct symbol_node *s, FILE *fp){
	int16_t machine_instruction = 0;
	switch(IR->instruction){
			case push:
				//class [00][0]
				if(IR->operand_type == REG){
					machine_instruction |= (IR->p_code_operand.p_register << 9);
				
				}else if(IR->operand_type == NUMS){
					machine_instruction |= (1 << 11);
					machine_instruction |= (IR->p_code_operand.constant);
				}else if(IR->operand_type == IDENTIFIER){
					machine_instruction |= (1 << 12);
					machine_instruction |= (IR->p_code_operand.constant);
				}else{
					machine_instruction |= (1 << 11);
					machine_instruction |= (1 << 12);
				}
				fprintf(fp,"%04x\n",machine_instruction);
				break;
			case pop:
				//class [00][1]
				machine_instruction |= (1 << 13);
				if(IR->operand_type == REG){
					machine_instruction |= (IR->p_code_operand.p_register << 9);
				}else if(IR->operand_type == IDENTIFIER){
					machine_instruction |= (1 << 12);
					machine_instruction |= (IR->p_code_operand.constant);
				}else{
					machine_instruction |= (1 << 11);
					machine_instruction |= (1 << 12);
				}
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_add:
				machine_instruction |= (1 << 14);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_sub:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 10);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_mul:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 11);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_div:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 11);
				machine_instruction |= (1 << 10);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_not:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 13);
				machine_instruction |= (1 << 12);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_eq:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 12);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_neq:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 12);
				machine_instruction |= (1 << 10);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_lt:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 12);
				machine_instruction |= (1 << 11);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_le:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 12);
				machine_instruction |= (1 << 11);
				machine_instruction |= (1 << 10);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_gt:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 13);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_ge:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 13);
				machine_instruction |= (1 << 10);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_and:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 13);
				machine_instruction |= (1 << 11);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case p_or:
				machine_instruction |= (1 << 14);
				machine_instruction |= (1 << 13);
				machine_instruction |= (1 << 11);
				machine_instruction |= (1 << 10);
				fprintf(fp,"%x\n",machine_instruction);
				break;
			case jump:
				{
				char *op = "8";
				char con[4];
				int i = IR->p_code_operand.constant;
				if(i<0){
					char s[8];
					sprintf(&s[0], "%x",i);
					con[0] = s[5];
					con[1] = s[6];
					con[2] = s[7];
				}else{
					sprintf(con, "%03x",i);
				}
				op = concat(op,con);
				fprintf(fp,"%s\n",op);
				break;
				}
			case brct:
				{
				char *op = "9";
				char con[4];
				int i = IR->p_code_operand.constant;
				if(i<0){
					char s[8];
					sprintf(&s[0], "%x",i);
					con[0] = s[5];
					con[1] = s[6];
					con[2] = s[7];
				}else{
					sprintf(con, "%03x",i);
				}
				op = concat(op,con);
				fprintf(fp,"%s\n",op);
				break;
				}
			case brcf:
				{
				char *op = "a";
				char con[4];
				int i = IR->p_code_operand.constant;
				if(i<0){
					char s[8];
					sprintf(&s[0], "%x",i);
					con[0] = s[5];
					con[1] = s[6];
					con[2] = s[7];
				}else{
					sprintf(con, "%03x",i);
				}
				op = concat(op,con);
				fprintf(fp,"%s\n",op);
				break;
				}
			case call:
				{

				//sp = sp + mem needed - args
				
				//push sp - (mem needed + 1)

				//push pc + 8
				//push bp

				//push sp
				//push meme needed + 3
				//sub
				//pop bp
				//jump

				//fprintf(fp,"%s\n","0000"); //push PC
				//fprintf(fp,"%s\n","0200"); //push SP
				//fprintf(fp,"%s\n","0400"); //push BP
				//fprintf(fp,"%s\n","0600"); //push LP
				

				//activation record
				fprintf(fp,"%s\n","0200"); //push SP
				int i = mem_needed(IR->p_code_operand.identifier,s) - IR->args;
				machine_instruction |= (1 << 11);
				machine_instruction |= (i);
				fprintf(fp,"%04x\n",machine_instruction);
				fprintf(fp,"%s\n","4000"); //ADD
				fprintf(fp,"%s\n","2200"); //pop SP

				machine_instruction = 0;
				i = mem_needed(IR->p_code_operand.identifier,s) - 1;
				machine_instruction |= (1 << 11);
				machine_instruction |= (i);
				fprintf(fp,"%04x\n",machine_instruction);
				fprintf(fp,"%s\n","0200"); //push SP
				fprintf(fp,"%s\n","4400"); //SUB

				fprintf(fp,"%s\n","0000"); //push PC
				
				machine_instruction = 0;
				machine_instruction |= (1 << 11);
				machine_instruction |= (8); //push 8
				fprintf(fp,"%04x\n",machine_instruction);
				fprintf(fp,"%s\n","4000"); //ADD
	
				fprintf(fp,"%s\n","0400"); //push BP

				//bp = sp - (mem needed+3)
				machine_instruction = 0;
				machine_instruction |= (1 << 11);

				if(!strcmp(IR->p_code_operand.identifier,"MAIN"))
					machine_instruction |= (mem_needed(IR->p_code_operand.identifier,s)+4);//was 3
				else 
					machine_instruction |= (mem_needed(IR->p_code_operand.identifier,s)+3);//was 2

				fprintf(fp,"%04x\n",machine_instruction);
				fprintf(fp,"%s\n","0200"); //push SP
				fprintf(fp,"%s\n","4400"); //SUB
				fprintf(fp,"%s\n","2400"); //pop BP
				
				//jump
				char *op = "c";
				char con[4];
				i = IR->p_code_operand.constant;
				
				//because the constant may be negative
				//C only does negative hex in 4 bytes
				if(i<0){
					char s[8];
					sprintf(&s[0], "%x",i);
					con[0] = s[5];
					con[1] = s[6];
					con[2] = s[7];
				}else{
					sprintf(con, "%03x",i);
				}
				op = concat(op,con);
				fprintf(fp,"%s\n",op);
				break;
				}
			case ret:
				
				//Push [bp] -> push id offset 0 ->saves return value in soon to be top of stack
				//pop bp
				//pop pc
				//pop sp
				//return -> pc = lp
				
				fprintf(fp,"%s\n","3000"); //pop id offset 0
				fprintf(fp,"%s\n","2400"); //pop BP
				fprintf(fp,"%s\n","2000"); //pop PC
				fprintf(fp,"%s\n","2200"); //pop SP
				fprintf(fp,"%s\n","d000"); //Return
				break;
			default:
				fprintf(fp,"%s\n","1800");
		}
	
}
//function that calls everything needed to generate the code properly
void generate_code(struct IR_node *IR, struct symbol_node *s){

	FILE *fp = fopen("code_generated.mem","w");
	Code_gen_label_resolving(IR);
	variable_offset_resolve(IR,s);
	fprintf(fp,"%s\n","3800"); //pop nothing used as the main return flag 
	while(IR != NULL){
		generate_instruction(IR,s,fp);
		IR = IR->next;
	}
	fclose(fp);
}

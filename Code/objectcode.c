#include "objectcode.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int stack_offset = 0;
int param_num = 0;

void InitConditions() {
    opHead = NULL;
    opTail = NULL;
    for(int i = 0; i < 32; i++) {
        Regs[i].index = i;
        Regs[i].isFree = 1;
    }
    for (int i = 0; i < 2; i++) {
        sprintf(Regs[i].name, "%s%d", "v", i);
    }

    for (int i = 0; i < 4; i++) {
        sprintf(Regs[i].name, "%s%d", "a", i);
    }

    for(int i = 0; i < 8; i++) {
        sprintf(Regs[i].name, "%s%d", "t", i);
    }

    for(int i = 0; i < 8; i++) {
        sprintf(Regs[i].name, "%s%d", "s", i);
    }

    for(int i = 8; i < 10; i++) {
        sprintf(Regs[i].name, "%s%d", "t", i);
    }

    for(int i = 0; i < 2; i++) {
        sprintf(Regs[i].name, "%s%d", "k", i);
    }

    strcpy(Regs[28].name, "gp");
    strcpy(Regs[29].name, "sp");
    strcpy(Regs[30].name, "fp");
    strcpy(Regs[31].name, "ra");
}

Operands SearchList(Operand key) {
    Operands tmp = opHead;
    while(tmp != NULL) {
        Operand curr = tmp->op;
        if(curr->kind == key->kind || key->kind == POINTER || key->kind == ADDRESS || curr->kind == POINTER || curr->kind == ADDRESS) {
            if(key->kind == CONSTANT) {
                if(key->u.value == curr->u.value) {
                    return tmp;
                }
            }
            else {
                if(strcmp(key->u.name, curr->u.name) == 0) {
                    return tmp;
                }
            }
        }
        tmp = tmp->next;
    }
    return NULL;
}

void InsertList(Operand key, int offset) {
    if(opHead == NULL) {
        opHead = malloc(sizeof(struct Operands_));
        opHead->op = key;
        opHead->offset = offset;
        opHead->next = NULL;
        opHead->prev = NULL;
        opTail = opHead ;
    }
    else {
        Operands p = malloc(sizeof(struct Operands_));
        p->op = key;
        p->offset = offset;
        p->next = NULL;
        p->prev = opTail;
        opTail->next = p;
        opTail = p;
    }
}

void GenerateOCs(FILE *fp) {
    fprintf(fp, ".data\n");
	fprintf(fp, " _prompt: .asciiz \"Enter an integer : \"\n");
	fprintf(fp, "_ret: .asciiz \"\\n\"\n");
    fprintf(fp, ".globl main\n");
    fprintf(fp, ".text\n");
	fprintf(fp, " read: \n");
	fprintf(fp, "    li $v0, 4 \n");
	fprintf(fp, "    la $a0, _prompt \n");
	fprintf(fp, "    syscall \n");
	fprintf(fp, "    li $v0, 5 \n");
	fprintf(fp, "    syscall \n");
	fprintf(fp, "    jr $ra \n");
	fprintf(fp, "\n");
	fprintf(fp, " write: \n");
	fprintf(fp, "    li $v0, 1 \n");
	fprintf(fp, "    syscall \n");
	fprintf(fp, "    li $v0, 4 \n");
	fprintf(fp, "    la $a0, _ret \n");
	fprintf(fp, "    syscall \n");
	fprintf(fp, "    move $v0, $0 \n");
	fprintf(fp, "    jr $ra \n");	
	fprintf(fp, "\n");
    InitConditions();
    InterCode tmp = IChead;
    while(tmp != NULL) {
        if (tmp->kind == PARAM) {
            if (tmp->prev != NULL && tmp->prev->kind != PARAM) {
			    int count = 0;
				InterCode start = tmp;
				while (tmp->kind == PARAM) {
					tmp = tmp->next;
					count = count + 1;
				}
				InterCode end = tmp;
				for (int i = 1; i <= count; i++) {
					param_num = i;
					GenerateOC(start, fp);
					start = start->next;
				}
				tmp = end;
			}
		}
		else if (tmp->kind == ARG) {
				if (tmp->prev != NULL && tmp->prev->kind != ARG) {
					int count = 0;
					InterCode start = tmp;
					while (tmp->kind == ARG) {
						tmp = tmp->next;
						count = count + 1;
					}
					InterCode end = tmp;
					for (int i = count; i >= 1; i--)
					{
						param_num = i;
						GenerateOC(start, fp);
						start = start->next;
					}
					tmp = end;
				}
			}
		else {			
			GenerateOC(tmp, fp);
			tmp = tmp->next;
		}
    }
}


void GenerateOC(InterCode i, FILE *fp){
	//fprintf(fp, "%d\n", i->kind);
	switch(i->kind){
		case LABEL_:{
printf("Label\n");
			Operand op = i->u.single.op;
			fprintf(fp, "%s:\n", op->u.name);
			break;
		}
		case FUNCTION_:{
printf("func\n");
			Operand op = i->u.single.op;
			fprintf(fp, "%s: \n", op->u.name);
	    		fprintf(fp, "addi $fp, $sp, 0\n");
        		stack_offset = 0;
			FreeReg(fp);
			break;
		}
//!!!!!!!!!!!!!!!!!!!!!!!!!POINTER,about address->op
		/*case ASSIGN:{
			Operand left = i->u.assign.left;
			Operand right = i->u.assign.right;
			if (left->kind != POINTER && right->kind == CONSTANT){
				int index = AllocateReg(left, 1, fp);    // 1 left 0 right
				fprintf(fp, "li $t%d, %d\n", index - 8, right->u.value);
				FreeReg(fp);
			}
			else if (left->kind != POINTER && (right->kind == VARIABLE || right->kind == TEMPVAR)){
				int index1 = AllocateReg(left, 1, fp);
				int index2 = AllocateReg(right, 0, fp);
				fprintf(fp, "move $t%d, $t%d\n", index1 - 8, index2 - 8);
				FreeReg(fp);
			}
			else if (left->kind != POINTER && right->kind == POINTER){

				int index1 = AllocateReg(left, 1, fp);
				int index2 = AllocateReg(right->op, 0, fp);
				fprintf(fp, "lw $t%d, 0($t%d)\n", index1 - 8, index2 - 8);
				FreeReg(fp);
			}
			else if (left->kind == POINTER && right->kind != POINTER)
			{
				int index1 = AllocateReg(left->op, 0, fp);
				int index2 = AllocateReg(right, 0, fp);
				fprintf(fp, "sw $t%d, 0($t%d)\n", index2 - 8, index1 - 8);
				FreeReg(fp);
			}
			else 
				assert(0); 
		}*/
		case ASSIGN:{
printf("assign\n");
			Operand left = i->u.assign.left;
			Operand right = i->u.assign.right;
			if (left->kind != POINTER && right->kind == CONSTANT){

				int index = AllocateReg(left, 1, fp);    // 1 left 0 right
				fprintf(fp, "li $t%d, %ld\n", index - 8, right->u.value);
				FreeReg(fp);
			}
			else if (left->kind != POINTER && (right->kind == VARIABLE || right->kind == TEMPVAR)){
				int index1 = AllocateReg(left, 1, fp);
				int index2 = AllocateReg(right, 0, fp);
				fprintf(fp, "move $t%d, $t%d\n", index1 - 8, index2 - 8);
				FreeReg(fp);
			}
			else if (left->kind != POINTER && right->kind == POINTER){

				int index1 = AllocateReg(left, 1, fp);
				//int index2 = AllocateReg(right, 0, fp);
				Operand t = (Operand)malloc(sizeof(struct Operand_));
				t->kind = VARIABLE;
				strcpy(t->u.name, right->u.name);
				Operands temp = SearchList(t);
				if(temp == NULL){
					t->kind = TEMPVAR;
					temp = SearchList(t);
				}
				int index2 = AllocateReg(temp->op,1, fp);
				fprintf(fp, "lw $t%d, 0($t%d)\n", index1 - 8, index2 - 8);
				FreeReg(fp);
			}
			else if (left->kind == POINTER && right->kind != POINTER)
			{
				//int index1 = AllocateReg(left, 0, fp);
				Operand t = (Operand)malloc(sizeof(struct Operand_));
				t->kind = VARIABLE;
				strcpy(t->u.name, left->u.name);
				Operands temp = SearchList(t);
				if(temp == NULL){
					t->kind = TEMPVAR;
					temp = SearchList(t);
				}
				int index1 = AllocateReg(temp->op,0, fp);
				int index2 = AllocateReg(right, 0, fp);
				fprintf(fp, "sw $t%d, 0($t%d)\n", index2 - 8, index1 - 8);
				FreeReg(fp);
			}
			else {
				printf("%d%d\n", left->kind,right->kind);
				assert(0); 
			}
			break;
		}
		case ADD:{
printf("add\n");
			Operand result = i->u.binop.result;
			Operand op1 = i->u.binop.op1;
			Operand op2 = i->u.binop.op2;

			if (op1->kind == ADDRESS && op2->kind == CONSTANT){
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op1, 0, fp);
				fprintf(fp, "addi $t%d, $t%d, -%ld\n", index1 - 8, index2 - 8, op2->u.value);
				FreeReg(fp);
			}
			else if (op1->kind == ADDRESS && op2->kind != CONSTANT)
			{
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op1, 0, fp);
				int index3 = AllocateReg(op2, 0, fp);
				fprintf(fp, "sub $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
				FreeReg(fp);
			}
			else if (op1->kind == CONSTANT && op2->kind == CONSTANT)
			{
				int index = AllocateReg(result, 1, fp);
				fprintf(fp, "li $t%d, %ld\n", index - 8, op1->u.value + op2->u.value);
				FreeReg(fp);
			}
			else if (op1->kind == CONSTANT && op2->kind != CONSTANT)
			{
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op2, 0, fp);
				fprintf(fp, "addi $t%d, $t%d, %ld\n", index1 - 8, index2 - 8, op1->u.value);
				FreeReg(fp);
			}
			else if (op1->kind != CONSTANT && op2->kind == CONSTANT)
			{
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op1, 0, fp);
				fprintf(fp, "addi $t%d, $t%d, %ld\n", index1 - 8, index2 - 8, op2->u.value);
				FreeReg(fp);
			}
			else {
				if (op1->kind == POINTER || op2->kind == POINTER)
					assert(0);
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op1, 0, fp);
				int index3 = AllocateReg(op2, 0, fp);

				fprintf(fp, "add $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
				FreeReg(fp);
			}
			break;
		}
		case SUB:{
printf("sub\n");
			Operand result = i->u.binop.result;
			Operand op1 = i->u.binop.op1;
			Operand op2 = i->u.binop.op2;

			if (op1->kind == CONSTANT && op2->kind == CONSTANT)
			{
				int index = AllocateReg(result, 1, fp);
				fprintf(fp, "li $t%d, %ld\n", index - 8, op1->u.value - op2->u.value);
				FreeReg(fp);
			}
			/*else if (op1->kind == CONSTANT && op2->kind != CONSTANT)
			{
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op2, 0, fp);

				fprintf(fp, "addi $t%d, $t%d, -%ld\n", index1 - 8, index2 - 8, op1->u.value);
				FreeReg(fp);
			}*/
			else if (op1->kind != CONSTANT && op2->kind == CONSTANT)
			{
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op1, 0, fp);
				

				fprintf(fp, "addi $t%d, $t%d, -%ld\n", index1 - 8, index2 - 8, op2->u.value);
				FreeReg(fp);
			}
			else {
				int index1 = AllocateReg(result, 1, fp);
				int index2 = AllocateReg(op1, 0, fp);
				int index3 = AllocateReg(op2, 0, fp);
				fprintf(fp, "sub $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
				FreeReg(fp);
			}
			break;
		}
		case MUL:{
printf("mul\n");
			Operand result = i->u.binop.result;
			Operand op1 = i->u.binop.op1;
			Operand op2 = i->u.binop.op2;

			int index1 = AllocateReg(result, 1, fp);
			int index2 = AllocateReg(op1, 0, fp);
			int index3 = AllocateReg(op2, 0, fp);
	
			fprintf(fp, "mul $t%d, $t%d, $t%d\n", index1 - 8, index2 - 8, index3 - 8);
			FreeReg(fp);
			break;
		}
		case DIV:{
printf("div\n");
			Operand result = i->u.binop.result;
			Operand op1 = i->u.binop.op1;
			Operand op2 = i->u.binop.op2;

			int index1 = AllocateReg(result, 1, fp);
			int index2 = AllocateReg(op1, 0, fp);
			int index3 = AllocateReg(op2, 0, fp);

			fprintf(fp, "div $t%d, $t%d\n", index2 - 8, index3 - 8);

			fprintf(fp, "mflo $t%d\n", index1 - 8);
			FreeReg(fp);
			break;
		}
		case GOTO:{
printf("goto\n");
			Operand op = i->u.single.op;
			fprintf(fp, "j %s\n", op->u.name);
			break;
		}
		case IFGOTO:{
printf("ifgoto\n");
			Operand op1 = i->u.condition.op1;
			Operand op2 = i->u.condition.op2;
			Operand destination = i->u.condition.destination;
			int index1, index2;
			if (op1->kind != POINTER && op2->kind == POINTER){
				index1 = AllocateReg(op1, 1, fp);
				Operand t = (Operand)malloc(sizeof(struct Operand_));
				t->kind = VARIABLE;
				strcpy(t->u.name, op2->u.name);
				Operands temp = SearchList(t);
				if(temp == NULL){
					t->kind = TEMPVAR;
					temp = SearchList(t);
				}
				index2 = AllocateReg(temp->op,1, fp);
				fprintf(fp, "lw $t%d, 0($t%d)\n", index2 - 8, index2 - 8);
			}
			else if (op1->kind == POINTER && op2->kind != POINTER)
			{
				Operand t = (Operand)malloc(sizeof(struct Operand_));
				t->kind = VARIABLE;
				strcpy(t->u.name, op1->u.name);
				Operands temp = SearchList(t);
				if(temp == NULL){
					t->kind = TEMPVAR;
					temp = SearchList(t);
				}
				index1 = AllocateReg(temp->op,0, fp);
				fprintf(fp, "lw $t%d, 0($t%d)\n", index1 - 8, index1 - 8);
				index2 = AllocateReg(op2, 0, fp);
			}
			else {
				index1 = AllocateReg(op1, 0, fp);
				index2 = AllocateReg(op2, 0, fp);
			}
			if (strcmp(i->u.condition.relop, "==") == 0){
				fprintf(fp, "beq");
			}
			else if (strcmp(i->u.condition.relop, "!=") == 0){
				fprintf(fp, "bne");
			}
			else if (strcmp(i->u.condition.relop, ">") == 0){
				fprintf(fp, "bgt");
			}
			else if (strcmp(i->u.condition.relop, "<") == 0){
				fprintf(fp, "blt");
			}
			else if (strcmp(i->u.condition.relop, ">=") == 0){
				fprintf(fp, "bge");
			}
			else if (strcmp(i->u.condition.relop, "<=") == 0){
				fprintf(fp, "ble");
			}
			else{
            			printf("%s\n",i->u.condition.relop);
				assert(0);
        		}
			fprintf(fp, " $t%d, $t%d, %s\n", index1 - 8, index2 - 8, destination->u.name);
			FreeReg(fp);
			break;
		}
		case RETURN:{
printf("return\n");
			Operand op = i->u.single.op;
			int index = AllocateReg(op, 0, fp);
			fprintf(fp, "move $v0, $t%d\n", index - 8);
			fprintf(fp, "jr $ra \n");
			FreeReg(fp);
			break;
		}
//!!!!!!!!!!!!! declare array, about address->op
/*		case DEC:{
			int size = i->u.dec.size;
			Operand op = i->u.dec.op;

			stack_offset = stack_offset + size;
			fprintf(fp, "addi $sp, $sp, -%d\n", size);

			Operand p = (Operand)malloc(sizeof(struct Operand_));
			p->kind = ADDRESS;
			p->op = op;

			InsertList(p, stack_offset - size);
		}*/
		case DEC:{
printf("dec\n");
			int size = i->u.dec.size;
			//Operand op = i->u.dec.op;

			stack_offset = stack_offset + size;
			fprintf(fp, "addi $sp, $sp, -%d\n", size);

			Operand p = (Operand)malloc(sizeof(struct Operand_));
			p->kind = ADDRESS;
			//p->op = op;
			strcpy(p->u.name, i->u.dec.op->u.name);
			InsertList(p, stack_offset - size);
			break;
		}
		case ARG:{
printf("arg\n");
			Operand op = i->u.single.op;
			int index = AllocateReg(op, 0, fp);
			if (param_num <= 4)
				fprintf(fp, "move $a%d, $t%d \n", param_num-1, index - 8);
			else if (param_num <= 4 + 8)
			{
				fprintf(fp, "move $s%d, $t%d \n", param_num - 5, index - 8);
			}
			else
				assert(0);
			FreeReg(fp);
			break;
		}
		case CALL:{
printf("call\n");
                	Operand op1 = i->u.call.op1;
			Operand op2 = i->u.call.op2;
			int index = AllocateReg(op1, 1, fp);
			fprintf(fp, "addi $sp, $sp, -8 \n");
			fprintf(fp, "sw $ra, 0($sp) \n");
			fprintf(fp, "sw $fp, 4($sp) \n");

			fprintf(fp, "jal %s \n", op2->u.name);
      
                	fprintf(fp,"move $sp, $fp \n");
			fprintf(fp, "lw $ra, 0($sp) \n");
			fprintf(fp, "lw $fp, 4($sp) \n");
			fprintf(fp, "addi $sp, $sp, 8 \n");
			fprintf(fp, "move $t%d, $v0 \n", index - 8);

			FreeReg(fp);
			break;
		}
		case READ:{
printf("read\n");
			Operand op = i->u.single.op;
			int index = AllocateReg(op, 1, fp);
			fprintf(fp, "addi $sp, $sp, -4 \n");
			fprintf(fp, "sw $ra, 0($sp) \n");
			fprintf(fp, "jal read \n");
			fprintf(fp, "lw $ra, 0($sp) \n");
			fprintf(fp, "addi $sp, $sp, 4 \n");
			fprintf(fp, "move $t%d, $v0 \n", index - 8);
			FreeReg(fp);
			break;
		}
		case WRITE:{
printf("write\n");
			Operand op = i->u.single.op;
			int index = AllocateReg(op, 0, fp);
			fprintf(fp, "move $a0, $t%d \n", index - 8);
			fprintf(fp, "addi $sp, $sp, -4 \n");
			fprintf(fp, "sw $ra, 0($sp) \n");
			fprintf(fp, "jal write  \n");
			fprintf(fp, "lw $ra, 0($sp) \n");
			fprintf(fp, "addi $sp, $sp, 4  \n");
			FreeReg(fp);
			break;
		}
		
		
		case PARAM:{
printf("param\n");
			Operand op = i->u.single.op;
			int index = AllocateReg(op, 1, fp);
			if (param_num > 4 && param_num <= 4 + 8){
				fprintf(fp, "move $t%d, $s%d \n", index - 8, param_num - 5);
			}
			else if (param_num <= 4){
				fprintf(fp, "move $t%d, $a%d \n", index - 8, param_num - 1);
			}
			else
				assert(0);
			FreeReg(fp);
			break;


		}
		default:
			assert(0);
	}

}


int  AllocateReg(Operand content, int flag,FILE *fp){
        int index;
        for (index = 8 ; index < 16 ; index++){
                if(Regs[index].isFree == 0)
                {
                        if(Regs[index].content->kind == CONSTANT)
                                continue;
                        if(EqualOperand(Regs[index].content,content))
                                return index;
                        else 
				continue ;
                }
                else if(Regs[index].isFree == 1)
                {
                        Regs[index].isFree = 0;
                        Regs[index].content = content;
                        break;
                }
           
        }     
        if(content->kind == CONSTANT){
                fprintf(fp,"li $t%d, %ld\n",index - 8, content->u.value);
                return index;
        }
                
        Operands temp = SearchList(content);
	
	//fprintf(fp, "%s\n",content->u.name);
        if(temp == NULL){
                InsertList(content,stack_offset);
                stack_offset += 4;
                fprintf(fp,"addi $sp, $sp, -4\n");
		
        }
        else if(flag == 0 && temp->op->kind != ADDRESS && temp->op->kind != POINTER) {
                int total_offset = 4 + temp->offset;
                fprintf(fp,"lw $t%d, -%d($fp)\n",index - 8,total_offset);
        }
        else if(flag == 1 && temp->op->kind != ADDRESS && temp->op->kind != POINTER) {
// !!!!!!!!!!!!!!!!!!!!!never touched ????????????
		//printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");
		int total_offset = 4 + temp->offset;
                fprintf(fp,"lw $t%d, -%d($fp)\n",index - 8,total_offset);
		
        }
        else if(flag == 0 && temp->op->kind == ADDRESS)
        {
                int total_offset = 4 + temp->offset;
                fprintf(fp,"addi $t%d, $fp, -%d\n",index - 8, total_offset);   
        }
        else if(flag == 1 && temp->op->kind == ADDRESS)
        {
                int total_offset = 4 + temp->offset;
                fprintf(fp,"addi $t%d, $fp, -%d\n",index - 8, total_offset);   
        }
        else
        {
                printf("%d,%d\n",flag, temp->op->kind);
                assert(0);
        }
        return index;
}

void FreeReg(FILE *fp){
        int index;
        for(int index = 8 ; index < 16 ; index++){
                if(Regs[index].isFree == 0){
                        Regs[index].isFree = 1;
                        if(Regs[index].content->kind == CONSTANT){
                                Regs[index].content = NULL;
                                continue;
                        }
                        Operands temp = SearchList(Regs[index].content);
                        if(temp == NULL)
                                assert(0);

                        if(temp->op->kind == ADDRESS){
                                Regs[index].content = NULL;
                                continue;
                        }
                        int total_offset = 4 + temp->offset;
                        fprintf(fp,"sw $t%d, -%d($fp)\n",index - 8,total_offset);
                        Regs[index].content = NULL;
                }
        }
}

int EqualOperand(Operand op1, Operand op2)
{
	if (op1->kind != op2->kind)
		return 0;
	else {
		switch(op1->kind){
			case VARIABLE:case TEMPVAR:case ADDRESS:case POINTER:{
				if (op1->u.name == op2->u.name)
					return 1;
				else 
					return 0;
			}
//!!!!!!!!!!!!!!!!!about address->op
/*
			case ADDRESS:{
				if (EqualOperand(op1->op, op2->op))
					return 1;
				else 
					return 0;
			}
*/
			default:{
				printf("op1->kind %d\n", op1->kind);
				assert(0);

			}
		}
	}
}

#include "intercode.h"
#include <stdlib.h>
#include <string.h>

InterCode IChead = NULL;
InterCode ICtail = NULL;
int labelNum = 1;
int tempNum = 1;
int varNum = 1;

char *my_itoa(long n){
	int i = 0, isNegative = 0;
	static char s[100];
	
	if(n < 0){
		isNegative = n;
		n = -n;
	}
	do{
		s[i++] = n % 10 + '0';
		n = n / 10;
	}while(n > 0);
	if(isNegative < 0)
		s[i++] = '-';
	//s[i++] = '#';
	s[i] = '\0';
	char temp;
	char *p = s;
	char *q = s;
	while(*q){
		++q;
	}
	q--;
	while(q>p){
		temp = *p;
		*p++ = *q;
		*q-- = temp;
	}
	return s;
}

long my_atol(char *p) {
	long res = 0;
	int i = 0;
	while(1) {
		if(p[i] == '\0') {
			break;
		}
		res = p[i]-'0'+res*10;
		i++;
	}
	return res;
}

char* newLabel() {
	char* label = (char *)malloc(sizeof(char)*5);
	sprintf(label, "label%d", labelNum);
	labelNum += 1;
	return label;
}

char* newTemp() {
	char* temp = (char *)malloc(sizeof(char)*5);
	sprintf(temp, "t%d", tempNum);
	tempNum += 1;
	return temp;
}

char* newVariable() {
	char* variable = (char *)malloc(sizeof(char)*5);
	sprintf(variable, "v%d", varNum);
	varNum += 1;
	return variable;
}

void insertIC(InterCode current) {
	if(current->kind == ASSIGN && current->u.assign.left->kind == POINTER && current->u.assign.right->kind == POINTER){
		char* temp = newTemp();
		InterCode ic = (InterCode)malloc(sizeof(struct InterCode_));
		ic->next = NULL;
		ic->prev = NULL;
		ic->kind = ASSIGN;
		Operand op1 = (Operand)malloc(sizeof(struct Operand_));
		Operand op2 = (Operand)malloc(sizeof(struct Operand_));
		op1->kind = TEMPVAR;
		op2->kind = TEMPVAR;
		strcpy(op1->u.name, temp);
		strcpy(op2->u.name, temp);
		ic->u.assign.left = op1;
		ic->u.assign.right = current->u.assign.right;
		current->u.assign.right = op2;
		insertIC(ic);
		insertIC(current);
		return;

	}
	if(current->kind == IFGOTO && current->u.condition.op1->kind == POINTER && current->u.condition.op2->kind == POINTER){
		char* temp1 = newTemp();
		char* temp2 = newTemp();
		InterCode ic1 = (InterCode)malloc(sizeof(struct InterCode_));
		ic1->next = NULL;
		ic1->prev = NULL;
		ic1->kind = ASSIGN;
		Operand op1 = (Operand)malloc(sizeof(struct Operand_));
		op1->kind = TEMPVAR;
		strcpy(op1->u.name, temp1);
		ic1->u.assign.left = op1;
		ic1->u.assign.right = current->u.condition.op1;
		insertIC(ic1);
		InterCode ic2 = (InterCode)malloc(sizeof(struct InterCode_));
		ic2->next = NULL;
		ic2->next = NULL;
		ic2->kind = ASSIGN;
		Operand op2 = (Operand)malloc(sizeof(struct Operand_));
		op2->kind = TEMPVAR;
		strcpy(op2->u.name, temp2);
		ic2->u.assign.left = op2;
		ic2->u.assign.right = current->u.condition.op2;
		insertIC(ic2);	
		Operand op3 = (Operand)malloc(sizeof(struct Operand_));
		Operand op4 = (Operand)malloc(sizeof(struct Operand_));
		op3->kind = TEMPVAR;
		op4->kind = TEMPVAR;
		strcpy(op3->u.name, temp1);
		strcpy(op4->u.name, temp2);
		current->u.condition.op1 = op3;
		current->u.condition.op2 = op4;
		
		
		insertIC(current);
		return;
		
	}
	if(current->kind == WRITE && current->u.single.op->kind == POINTER){
		char* temp = newTemp();
		InterCode ic = (InterCode)malloc(sizeof(struct InterCode_));
		ic->next = NULL;
		ic->prev = NULL;
		ic->kind = ASSIGN;
		Operand op1 = (Operand)malloc(sizeof(struct Operand_));
		Operand op2 = (Operand)malloc(sizeof(struct Operand_));
		op1->kind = TEMPVAR;
		op2->kind = TEMPVAR;
		strcpy(op1->u.name, temp);
		strcpy(op2->u.name, temp);
		ic->u.assign.left = op1;
		ic->u.assign.right = current->u.single.op;
		current->u.single.op = op2;
		insertIC(ic);
		insertIC(current);
		return;
	}
	// if(current->kind == ARG && current->u.single.op->kind == POINTER){
	// 	char* temp = newTemp();
	// 	InterCode ic = (InterCode)malloc(sizeof(struct InterCode_));
	// 	ic->next = NULL;
	// 	ic->prev = NULL;
	// 	ic->kind = ASSIGN;
	// 	Operand op1 = (Operand)malloc(sizeof(struct Operand_));
	// 	Operand op2 = (Operand)malloc(sizeof(struct Operand_));
	// 	op1->kind = TEMPVAR;
	// 	op2->kind = TEMPVAR;
	// 	strcpy(op1->u.name, temp);
	// 	strcpy(op2->u.name, temp);
	// 	ic->u.assign.left = op1;
	// 	ic->u.assign.right = current->u.single.op;
	// 	current->u.single.op = op2;
	// 	insertIC(ic);
	// 	insertIC(current);
	// 	return;
	// }
	if(current->kind == CALL && ICtail->kind == ARG) {
		InterCode ic = ICtail;
		while(ic->kind == ARG) {
			ic = ic->prev;
		}
		InterCode currentTail = ic;
		ic = ic->next;
		InterCode tempHead = NULL, tempTail = NULL, argHead = ic, argTail = ic;
		while(ic != NULL) {
			char* temp = newTemp();
			InterCode tmp = (InterCode)malloc(sizeof(struct InterCode_));
			tmp->kind = ASSIGN;
			tmp->next = NULL;
			tmp->prev = NULL;
			Operand op1 = (Operand)malloc(sizeof(struct Operand_));
			Operand op2 = (Operand)malloc(sizeof(struct Operand_));
			op1->kind = TEMPVAR;
			strcpy(op1->u.name, temp);
			op2->kind = argTail->u.single.op->kind;
			if(op2->kind == CONSTANT) {
				op2->u.value = argTail->u.single.op->u.value;
			}
			else {
				strcpy(op2->u.name, argTail->u.single.op->u.name);
			}
			tmp->u.assign.left = op1;
			tmp->u.assign.right = op2;
			argTail->u.single.op->kind = TEMPVAR;
			strcpy(argTail->u.single.op->u.name, temp);
			if(tempHead == NULL) {
				tempHead = tmp;
				tempTail = tmp;
			}
			else {
				tempTail->next = tmp;
				tmp->prev = tempTail;
				tempTail = tempTail->next;
			}
			ic = ic->next;
			if(argTail->next != NULL) {
				argTail = argTail->next;
			}
		}
		currentTail->next = tempHead;
		tempHead->prev = currentTail;
		tempTail->next = argHead;
		argHead->prev = tempTail;
		ICtail = argTail;
	}
	if(IChead == NULL) {
		// IChead = (Operand)(malloc(sizeof(struct Operand_)));
		IChead = current;
		ICtail = current;
		IChead->prev = IChead->next = current;
	}
	else {
		
		ICtail->next = current;
		current->prev = ICtail;
		current->next = NULL;
		ICtail = current;
		
	}
	//if(current->kind == LABEL_ && current != IChead && current->prev->kind == GOTO && strcmp(current->prev->u.single.op->u.name, current->u.single.op->u.name) == 0) {
	//	deleteIC(current->prev);
	//}
}

void deleteIC(InterCode current) {
	if(IChead == ICtail) {
		IChead = ICtail = NULL;
	}
	else if(IChead == current) {
		IChead = IChead->next;
		IChead->prev = IChead;
	}
	else {
		current->prev->next = current->next;
	}
	free(current);
}

char* printOperand(Operand op) {
	char *res = (char *)malloc(sizeof(char)*20);
	switch(op->kind) {
		case CONSTANT:{
			sprintf(res, "#%ld", op->u.value);
			break;
		}
		case ADDRESS:{
			sprintf(res, "&%s", op->u.name);
			break;
		}
		case POINTER:{
			sprintf(res, "*%s", op->u.name);
			break;
		}
		default:
			return op->u.name;
	}
	return res;
}

void printIC(char* filename) {
	FILE *fp = fopen(filename, "w");
	InterCode p = IChead;
	char contents[150];
	while(p != NULL) {
		memset(contents, 0, sizeof(contents));
		switch(p->kind) {
			case LABEL_: {
				sprintf(contents, "LABEL %s :\n", p->u.single.op->u.name);
				break;
			}
			case FUNCTION_:{
				sprintf(contents, "FUNCTION %s :\n", p->u.single.op->u.name);
				break;
			}
			case ASSIGN:{
				sprintf(contents, "%s := %s\n", printOperand(p->u.assign.left), printOperand(p->u.assign.right));
				break;
			}
			case ADD:{
				sprintf(contents, "%s := %s + %s\n", printOperand(p->u.binop.result), printOperand(p->u.binop.op1), printOperand(p->u.binop.op2));
            			break;
			}
			case SUB:{
				sprintf(contents, "%s := %s - %s\n", printOperand(p->u.binop.result), printOperand(p->u.binop.op1), printOperand(p->u.binop.op2));
            			break;
			}
			case MUL:{
				sprintf(contents, "%s := %s * %s\n", printOperand(p->u.binop.result), printOperand(p->u.binop.op1), printOperand(p->u.binop.op2));
            			break;
			}
			case DIV:{
				sprintf(contents, "%s := %s / %s\n", printOperand(p->u.binop.result), printOperand(p->u.binop.op1), printOperand(p->u.binop.op2));
            			break;
			}
			case GOTO:{
				sprintf(contents, "GOTO %s\n", p->u.single.op->u.name);
				break;
			}
			case IFGOTO:{
				sprintf(contents, "IF %s %s %s GOTO %s\n", printOperand(p->u.condition.op1), p->u.condition.relop, printOperand(p->u.condition.op2), printOperand(p->u.condition.destination));
				break;
			}
			case RETURN:{
				//printf("%s\n", printOperand(p->u.single.op));
				sprintf(contents, "RETURN %s\n", printOperand(p->u.single.op));
				break;
			}
			case DEC:{
				sprintf(contents, "DEC %s %d\n", printOperand(p->u.dec.op), p->u.dec.size);
				break;
			}
			case ARG:{
				sprintf(contents, "ARG %s\n", printOperand(p->u.single.op));
				break;
			}
			case CALL:{
				sprintf(contents, "%s := CALL %s\n", printOperand(p->u.call.op1), printOperand(p->u.call.op2));
				break;
			}
			case PARAM:{
				sprintf(contents, "PARAM %s\n", printOperand(p->u.single.op));
				break;
			}
			case READ:{
				sprintf(contents, "READ %s\n", printOperand(p->u.single.op));
				break;
			}
			case WRITE:{
				sprintf(contents, "WRITE %s\n", printOperand(p->u.single.op));
				break;
			}
		}
		fputs(contents, fp);
		p = p->next;
	}
}


void translate(struct Node_t* root){
	if(root == NULL)
		return;
	switch(root->attribute){
		case _Specifier:
			return;
		case _FunDec:
			translate_FunDec(root);
			break;
		case _CompSt:
			translate_CompSt(root);
			break;
		case _Stmt:
			translate_Stmt(root);
			break;
		case _Dec:
			translate_Dec(root);
			break;
		default:{
			struct Node_t* temp = root->firstChild;
			while(temp != NULL){
				translate(temp);
				temp = temp->next;
			}
			
		}
	}
			
}

int translate_Exp(struct Node_t* root, char *place){
//printf("Expin\n");
	switch(root->firstChild->attribute){
		case _INT:{
			
			if(place == NULL)
				return -1;
			strcpy(place, my_itoa(root->firstChild->value.intValue));
//printf("ExpINTout\n");
			return CONSTANT;
		}
		case _ID:{
			if(root->firstChild->next == NULL){
				if(place == NULL)
					return -1;
				
				//tempNum -= 1;
				FieldList basicFinder = getSymbol(root->firstChild->name, BASIC);
				FieldList arrayFinder = getSymbol(root->firstChild->name, ARRAY);
				FieldList structFinder = getSymbol(root->firstChild->name, STRUCTURE);
				if(structFinder != NULL){
					printf("Cannot translate: Code contains variables or parameters of struct type.\n");
					exit(-1);
				}
				if(basicFinder != NULL){
					if(strcmp(basicFinder->vname, "-1") == 0){
						place = newVariable();
						strcpy(basicFinder->vname, place);
					}
					else{
						strcpy(place, basicFinder->vname);
					}
//printf("ExpID1out\n");
					return VARIABLE;
					
				}
				else{
					if(strcmp(arrayFinder->vname, "-1") == 0){
						place = newVariable();
						strcpy(arrayFinder->vname, place);
					}
					else{
						strcpy(place, arrayFinder->vname);
					}
//printf("ExpID2out\n");
					if(arrayFinder->isParam == 1)
						return VARIABLE;
					return ADDRESS;
				}
				
				
			}
			else if(root->firstChild->next->next->attribute == _RP){
				if(place == NULL){
					place = newTemp();
				}
				FieldList funcFinder = getSymbol(root->firstChild->name, FUNCTION);
				if(strcmp(funcFinder->name, "read") == 0){
					InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
					newCode->prev = NULL;
					newCode->next = NULL;
					newCode->kind = READ;
					Operand op = (Operand)malloc(sizeof(struct Operand_));
					op->kind = TEMPVAR;
					strcpy(op->u.name, place);
					newCode->u.single.op = op;
					insertIC(newCode);
				}
				else{
					InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
					newCode->prev = NULL;
					newCode->next = NULL;
					newCode->kind = CALL;
					Operand op1 = (Operand)malloc(sizeof(struct Operand_));
					op1->kind = TEMPVAR;
					strcpy(op1->u.name, place);
					Operand op2 = (Operand)malloc(sizeof(struct Operand_));
					op2->kind = FUNCTION;
					strcpy(op2->u.name, funcFinder->name);
					newCode->u.call.op1 = op1;
					newCode->u.call.op2 = op2;
					insertIC(newCode);
				}
//printf("ExpID3out\n");
				return TEMPVAR;
				
			}
			else{
				if(place == NULL){
					place = newTemp();
				}
				FieldList funcFinder = getSymbol(root->firstChild->name, FUNCTION);
				if(strcmp(funcFinder->name, "write") == 0){
					char *newtemp = newTemp();
					int kind = translate_Exp(root->firstChild->next->next->firstChild, newtemp);
					InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
					newCode->prev = NULL;
					newCode->next = NULL;
					newCode->kind = WRITE;
					Operand op = (Operand)malloc(sizeof(struct Operand_));
					op->kind = kind;
					//strcpy(op->u.name, newtemp);
					if(kind == CONSTANT) {
						op->u.value = my_atol(newtemp);
					}
					else {
						strcpy(op->u.name, newtemp);
					}
					newCode->u.single.op = op;
					insertIC(newCode);
				}
				else{
					InterCode *argsHead = (InterCode *)malloc(sizeof(struct InterCode_*));
					InterCode *argsTail = (InterCode *)malloc(sizeof(struct InterCode_*));
					*argsHead = NULL;
					*argsTail = NULL;
					translate_Args(root->firstChild->next->next, argsHead, argsTail);
					InterCode t = NULL;
					while((*argsHead) != NULL) {
						t = (*argsHead)->next;
						insertIC(*argsHead);
						*argsHead = t;
					}
					InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
					newCode->prev = NULL;
					newCode->next = NULL;
					newCode->kind = CALL;
					Operand op1 = (Operand)malloc(sizeof(struct Operand_));
					op1->kind = TEMPVAR;
					strcpy(op1->u.name, place);
					Operand op2 = (Operand)malloc(sizeof(struct Operand_));
					op2->kind = FUNCTION;
					strcpy(op2->u.name, funcFinder->name);
					newCode->u.call.op1 = op1;
					newCode->u.call.op2 = op2;
					insertIC(newCode);
				}
//printf("ExpID4out\n");
				return TEMPVAR;
			}
		}
		case _Exp:{
			switch(root->firstChild->next->attribute){
				case _ASSIGNOP:{
					InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
					newCode->prev = NULL;
					newCode->next = NULL;
					
					char *t1 = newTemp();
					char *t2 = newTemp();
					int leftkind = translate_Exp(root->firstChild, t1);
					int rightkind = translate_Exp(root->firstChild->next->next, t2);
					
					newCode->kind = ASSIGN;
					Operand left = (Operand)malloc(sizeof(struct Operand_));
					left->kind = leftkind;
					//strcpy(left->u.name, t1);
					if(leftkind == CONSTANT) {
						left->u.value = my_atol(t1);
					}
					else {
						strcpy(left->u.name, t1);
					}
					Operand right = (Operand)malloc(sizeof(struct Operand_));
					right->kind = rightkind;
					//strcpy(right->u.name, t2);
					if(rightkind == CONSTANT) {
						right->u.value = my_atol(t2);
					}
					else {
						strcpy(right->u.name, t2);
					}
					newCode->u.assign.left = left;
					newCode->u.assign.right = right;
					insertIC(newCode);
					if(place != NULL){
						InterCode newCode2 = (InterCode)malloc(sizeof(struct InterCode_));
						newCode2->prev = NULL;
						newCode2->next = NULL;
						newCode2->kind = ASSIGN;
						Operand before = (Operand)malloc(sizeof(struct Operand_));
						before->kind = TEMPVAR;
						strcpy(before->u.name, place);
						Operand after = (Operand)malloc(sizeof(struct Operand_));
						after->kind = leftkind;
						strcpy(after->u.name, t1);
						newCode2->u.assign.left = before;
						newCode2->u.assign.right = after;
						insertIC(newCode2);
//printf("ExpASSIGNout\n");
						return TEMPVAR;
					}
//printf("ExpASSIGNout\n");
					return -1;
					
				}
				case _AND: case _OR: case _RELOP:{
					if(place == NULL)
						return -1;
					char *label1 = newLabel();
					char *label2 = newLabel();

					InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
					newCode1->prev = NULL;
					newCode1->next = NULL;
					newCode1->kind = ASSIGN;
					Operand left1 = (Operand)malloc(sizeof(struct Operand_));
					left1->kind = TEMPVAR;
					strcpy(left1->u.name, place);
					Operand right1 = (Operand)malloc(sizeof(struct Operand_));
					right1->kind = CONSTANT;
					right1->u.value = 0;
					newCode1->u.assign.left = left1;
					newCode1->u.assign.right = right1;
					insertIC(newCode1);
					
					translate_Cond(root,label1,label2);
					
					InterCode newCode2 = (InterCode)malloc(sizeof(struct InterCode_));
					newCode2->prev = NULL;
					newCode2->next = NULL;
					newCode2->kind = LABEL_;
					Operand op1 = (Operand)malloc(sizeof(struct Operand_));
					op1->kind = LABEL;
					strcpy(op1->u.name, label1);
					newCode2->u.single.op = op1;
					insertIC(newCode2);
					
					InterCode newCode3 = (InterCode)malloc(sizeof(struct InterCode_));
					newCode3->prev = NULL;
					newCode3->next = NULL;
					newCode3->kind = ASSIGN;
					Operand left2 = (Operand)malloc(sizeof(struct Operand_));
					left2->kind = TEMPVAR;
					strcpy(left2->u.name, place);
					Operand right2 = (Operand)malloc(sizeof(struct Operand_));
					right2->kind = CONSTANT;
					right2->u.value = 1;
					newCode3->u.assign.left = left2;
					newCode3->u.assign.right = right2;
					insertIC(newCode3);

					InterCode newCode4 = (InterCode)malloc(sizeof(struct InterCode_));
					newCode4->prev = NULL;
					newCode4->next = NULL;
					newCode4->kind = LABEL_;
					Operand op2 = (Operand)malloc(sizeof(struct Operand_));
					op2->kind = LABEL;
					strcpy(op2->u.name, label2);
					newCode4->u.single.op = op2;
					insertIC(newCode4);

					return TEMPVAR;
				}
				case _PLUS: case _MINUS: case _STAR: case _DIV:{
					if(place == NULL)
						return -1;
					if(root->firstChild->firstChild->attribute == _INT && root->firstChild->next->next->firstChild->attribute == _INT){
						int result = 0;
						if(root->firstChild->next->attribute == _PLUS)
							result = root->firstChild->firstChild->value.intValue + root->firstChild->next->next->firstChild->value.intValue;
						else if(root->firstChild->next->attribute == _MINUS)
							result = root->firstChild->firstChild->value.intValue - root->firstChild->next->next->firstChild->value.intValue;
						else if(root->firstChild->next->attribute == _STAR)
							result = root->firstChild->firstChild->value.intValue * root->firstChild->next->next->firstChild->value.intValue;
						else
							result = root->firstChild->firstChild->value.intValue / root->firstChild->next->next->firstChild->value.intValue;
						root->firstChild->attribute = _INT;
						root->firstChild->value.intValue = result;
						root->firstChild->next = NULL;
						return translate_Exp(root, place);
					}
					char *t1 = newTemp();
					char *t2 = newTemp();
					int leftkind = translate_Exp(root->firstChild, t1);
					int rightkind = translate_Exp(root->firstChild->next->next, t2);
					InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
					newCode->prev = NULL;
					newCode->next = NULL;

					Operand result = (Operand)malloc(sizeof(struct Operand_));
					result->kind = TEMPVAR;
					strcpy(result->u.name, place);
					Operand op1 = (Operand)malloc(sizeof(struct Operand_));
					op1->kind = leftkind;
					//strcpy(op1->u.name, t1);
					if(leftkind == CONSTANT) {
						op1->u.value = my_atol(t1);
					}
					else {
						strcpy(op1->u.name, t1);
					}
					Operand op2 = (Operand)malloc(sizeof(struct Operand_));
					op2->kind = rightkind;
					//strcpy(op2->u.name, t2);
					if(rightkind == CONSTANT) {
						op2->u.value = my_atol(t2);
					}
					else {
						strcpy(op2->u.name, t2);
					}
					newCode->u.binop.result = result;
					newCode->u.binop.op1 = op1;
					newCode->u.binop.op2 = op2;
					
					if(root->firstChild->next->attribute == _PLUS)
						newCode->kind = ADD;
					else if(root->firstChild->next->attribute == _MINUS)
						newCode->kind = SUB;
					else if(root->firstChild->next->attribute == _STAR)
						newCode->kind = MUL;
					else
						newCode->kind = DIV;
					insertIC(newCode);
//printf("Exp+-*/out\n");
					return TEMPVAR;
				}
				case _LB:{

					struct Node_t* temp = root->firstChild;
					ArrayNode head = NULL;
					while(temp->firstChild->attribute != _ID){
						ArrayNode newNode = (ArrayNode)malloc(sizeof(struct ArrayNode_));
						newNode->next = NULL;

						char *t1 = newTemp();
						newNode->kind = translate_Exp(temp->next->next, t1);
						strcpy(newNode->number, t1);
						newNode->next = head;
						head = newNode;
						temp = temp->firstChild;
					}
					ArrayNode newNode = (ArrayNode)malloc(sizeof(struct ArrayNode_));

					newNode->next = NULL;
					char *t1 = newTemp();
					newNode->kind = translate_Exp(temp->next->next, t1);
					strcpy(newNode->number, t1);
					newNode->next = head;
					head = newNode;

					char *arrayhead = newTemp();
					int arraykind = translate_Exp(temp, arrayhead);

					FieldList arrayFinder = getSymbol(temp->firstChild->name, ARRAY);
					int size = 1;
					Type tmp = arrayFinder->type;
					while(tmp->kind == ARRAY){
						size *= tmp->u.array.size;
						tmp = tmp->u.array.elem;
					}
					if(tmp->kind == BASIC && tmp->u.basic == 0)
						size *= 4;

					ArrayNode tempNode = head;
					tmp = arrayFinder->type;
					char* t2 = newTemp();
					InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
					newCode->prev = NULL;
					newCode->next = NULL;
					newCode->kind = MUL;
					Operand result = (Operand)malloc(sizeof(struct Operand_));
					result->kind = TEMPVAR;
					strcpy(result->u.name, t2);
					Operand op1 = (Operand)malloc(sizeof(struct Operand_));
					op1->kind = tempNode->kind;
					//strcpy(op1->u.name, tempNode->number);
					if(tempNode->kind == CONSTANT) {
						op1->u.value = my_atol(tempNode->number);
					}
					else {
						strcpy(op1->u.name, tempNode->number);
					}
					Operand op2 = (Operand)malloc(sizeof(struct Operand_));
					op2->kind = CONSTANT;
					size /= tmp->u.array.size;
					tmp = tmp->u.array.elem;
					op2->u.value = size;
					newCode->u.binop.result = result;
					newCode->u.binop.op1 = op1;
					newCode->u.binop.op2 = op2;
					insertIC(newCode);

					char* t3 = newTemp();
					InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
					newCode1->prev = NULL;
					newCode1->next = NULL;
					newCode1->kind = ADD;
					Operand result1 = (Operand)malloc(sizeof(struct Operand_));
					result1->kind = TEMPVAR;
					strcpy(result1->u.name, t3);
					Operand op11 = (Operand)malloc(sizeof(struct Operand_));
					op11->kind = ADDRESS;
					if(arrayFinder != NULL && arrayFinder->isParam == 1)
						op11->kind = TEMPVAR;
					//strcpy(op11->u.name, arrayhead);
					if(arraykind == CONSTANT) {
						op11->u.value = my_atol(arrayhead);
					}
					else {
						strcpy(op11->u.name, arrayhead);
					}
					Operand op12 = (Operand)malloc(sizeof(struct Operand_));
					op12->kind = TEMPVAR;
					strcpy(op12->u.name, t2);
					newCode1->u.binop.result = result1;
					newCode1->u.binop.op1 = op11;
					newCode1->u.binop.op2 = op12;
					insertIC(newCode1);
					strcpy(arrayhead, t3);
					tempNode = tempNode->next;
					while(tempNode != NULL){
						char* t2 = newTemp();
						InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
						newCode->prev = NULL;
						newCode->next = NULL;
						newCode->kind = MUL;
						Operand result = (Operand)malloc(sizeof(struct Operand_));
						result->kind = TEMPVAR;
						strcpy(result->u.name, t2);
						Operand op1 = (Operand)malloc(sizeof(struct Operand_));
						op1->kind = tempNode->kind;
						//strcpy(op1->u.name, tempNode->number);
						if(tempNode->kind == CONSTANT) {
							op1->u.value = my_atol(tempNode->number);
						}
						else {
							strcpy(op1->u.name, tempNode->number);
						}
						Operand op2 = (Operand)malloc(sizeof(struct Operand_));
						op2->kind = CONSTANT;
						size /= tmp->u.array.size;
						tmp = tmp->u.array.elem;
						op2->u.value = size;
						newCode->u.binop.result = result;
						newCode->u.binop.op1 = op1;
						newCode->u.binop.op2 = op2;
						insertIC(newCode);

						char* t3 = newTemp();
						InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
						newCode1->prev = NULL;
						newCode1->next = NULL;
						newCode1->kind = ADD;
						Operand result1 = (Operand)malloc(sizeof(struct Operand_));
						result1->kind = TEMPVAR;
						strcpy(result1->u.name, t3);
						Operand op11 = (Operand)malloc(sizeof(struct Operand_));
						op11->kind = TEMPVAR;
						strcpy(op11->u.name, arrayhead);
						Operand op12 = (Operand)malloc(sizeof(struct Operand_));
						op12->kind = TEMPVAR;
						strcpy(op12->u.name, t2);
						newCode1->u.binop.result = result1;
						newCode1->u.binop.op1 = op11;
						newCode1->u.binop.op2 = op12;
						insertIC(newCode1);
						strcpy(arrayhead, t3);
						tempNode = tempNode->next;
					}
					
					strcpy(place, arrayhead);
					
					return POINTER;
				}
			}
			break;
		}
		case _LP:{
			return translate_Exp(root->firstChild->next, place);
		}
		case _MINUS:{
			if(place == NULL)
				return -1;
			char *t1 = newTemp();
			int kind = translate_Exp(root->firstChild->next, t1);
			InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
			newCode->prev = NULL;
			newCode->next = NULL;
			newCode->kind = SUB;

			Operand result = (Operand)malloc(sizeof(struct Operand_));
			result->kind = TEMPVAR;
			strcpy(result->u.name, place);
			Operand op1 = (Operand)malloc(sizeof(struct Operand_));
			op1->kind = CONSTANT;
			op1->u.value = 0;
			Operand op2 = (Operand)malloc(sizeof(struct Operand_));
			op2->kind = kind;
			//strcpy(op2->u.name, t1);
			if(kind == CONSTANT) {
				op2->u.value = my_atol(t1);
			}
			else {
				strcpy(op2->u.name, t1);
			}
			newCode->u.binop.result = result;
			newCode->u.binop.op1 = op1;
			newCode->u.binop.op2 = op2;
			insertIC(newCode);
			return TEMPVAR;
		}
		case _NOT:{
			if(place == NULL)
				return -1;
			char *label1 = newLabel();
			char *label2 = newLabel();

			InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode1->prev = NULL;
			newCode1->next = NULL;
			newCode1->kind = ASSIGN;
			Operand left1 = (Operand)malloc(sizeof(struct Operand_));
			left1->kind = TEMPVAR;
			strcpy(left1->u.name, place);
			Operand right1 = (Operand)malloc(sizeof(struct Operand_));
			right1->kind = CONSTANT;
			right1->u.value = 0;
			newCode1->u.assign.left = left1;
			newCode1->u.assign.right = right1;
			insertIC(newCode1);
					
			translate_Cond(root,label1,label2);
					
			InterCode newCode2 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode2->prev = NULL;
			newCode2->next = NULL;
			newCode2->kind = LABEL_;
			Operand op1 = (Operand)malloc(sizeof(struct Operand_));
			op1->kind = LABEL;
			strcpy(op1->u.name, label1);
			newCode2->u.single.op = op1;
			insertIC(newCode2);
					
			InterCode newCode3 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode3->prev = NULL;
			newCode3->next = NULL;
			newCode3->kind = ASSIGN;
			Operand left2 = (Operand)malloc(sizeof(struct Operand_));
			left2->kind = TEMPVAR;
			strcpy(left2->u.name, place);
			Operand right2 = (Operand)malloc(sizeof(struct Operand_));
			right2->kind = CONSTANT;
			right2->u.value = 1;
			newCode3->u.assign.left = left2;
			newCode3->u.assign.right = right2;
			insertIC(newCode3);

			InterCode newCode4 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode4->prev = NULL;
			newCode4->next = NULL;
			newCode4->kind = LABEL_;
			Operand op2 = (Operand)malloc(sizeof(struct Operand_));
			op2->kind = LABEL;
			strcpy(op2->u.name, label2);
			newCode4->u.single.op = op2;
			insertIC(newCode4);

			return TEMPVAR;
		
		}
	}
		
}


void translate_Cond(struct Node_t* root, char *label_true, char *label_false){
//printf("Condin\n");
	if(root->firstChild->attribute == _Exp && root->firstChild->next != NULL && root->firstChild->next->attribute == _RELOP && root->firstChild->next->next != NULL && root->firstChild->next->next->attribute == _Exp){
		InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
		newCode->prev = NULL;
		newCode->next = NULL;
		newCode->kind = IFGOTO;
		Operand destination = (Operand)malloc(sizeof(struct Operand_));
		destination->kind = LABEL;
		strcpy(destination->u.name, label_true);

		Operand op1 = (Operand)malloc(sizeof(struct Operand_));
		Operand op2 = (Operand)malloc(sizeof(struct Operand_));

		if(root->firstChild->firstChild->attribute == _INT){
			op1->kind = CONSTANT;
			op1->u.value = root->firstChild->firstChild->value.intValue;
		}
		else{
			char *t1 = newTemp();
			int kind = translate_Exp(root->firstChild, t1);
			op1->kind = kind;
			/*if(kind == -2){
				op1->kind = CONSTANT;
				op1->u.value = root->firstChild->firstChild->value.intValue;
			}
			else{
				op1->kind = kind;
				strcpy(op1->u.name, t1);
			}*/
			if(kind == CONSTANT) {
				op1->u.value = my_atol(t1);
			}
			else {
				strcpy(op1->u.name, t1);
			}
		}
		if(root->firstChild->next->next->firstChild->attribute == _INT){
			op2->kind = CONSTANT;
			op2->u.value = root->firstChild->next->next->firstChild->value.intValue;
		}
		else{
			char *t2 = newTemp();
			int kind = translate_Exp(root->firstChild->next->next, t2);
			op2->kind = kind;
			/*if(kind == -2){
				op2->kind = CONSTANT;
				op2->u.value = root->firstChild->next->next->firstChild->value.intValue;
			}
			else{
				op2->kind = kind;
				strcpy(op2->u.name, t2);
			}*/
			if(kind == CONSTANT) {
				op2->u.value = my_atol(t2);
			}
			else {
				strcpy(op2->u.name, t2);
			}
		}
		
		newCode->u.condition.destination = destination;
		newCode->u.condition.op1 = op1;
		newCode->u.condition.op2 = op2;
		strcpy(newCode->u.condition.relop, root->firstChild->next->name);
		insertIC(newCode);

		InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
		newCode1->prev = NULL;
		newCode1->next = NULL;
		newCode1->kind = GOTO;
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = LABEL;
		strcpy(op->u.name, label_false);
		newCode1->u.single.op = op;
		insertIC(newCode1);
	}
	else if(root->firstChild->attribute == _Exp && root->firstChild->next != NULL && root->firstChild->next->attribute == _AND && root->firstChild->next->next != NULL && root->firstChild->next->next->attribute == _Exp){
		char *label1 = newLabel();
		translate_Cond(root->firstChild, label1, label_false);

		InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
		newCode->prev = NULL;
		newCode->next = NULL;
		newCode->kind = LABEL_;
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = LABEL;
		strcpy(op->u.name, label1);
		newCode->u.single.op = op;
		insertIC(newCode);

		translate_Cond(root->firstChild->next->next, label_true, label_false);
	}
	else if(root->firstChild->attribute == _Exp && root->firstChild->next != NULL && root->firstChild->next->attribute == _OR && root->firstChild->next->next != NULL && root->firstChild->next->next->attribute == _Exp){
		char *label1 = newLabel();
		translate_Cond(root->firstChild, label_true, label1);

		InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
		newCode->prev = NULL;
		newCode->next = NULL;
		newCode->kind = LABEL_;
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = LABEL;
		strcpy(op->u.name, label1);
		newCode->u.single.op = op;
		insertIC(newCode);

		translate_Cond(root->firstChild->next->next, label_true, label_false);
	}
	else if(root->firstChild->attribute == _NOT && root->firstChild->next != NULL && root->firstChild->next->attribute == _Exp){
		translate_Cond(root->firstChild->next, label_false, label_true);
	}	
	else{
		InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
		newCode->prev = NULL;
		newCode->next = NULL;
		newCode->kind = IFGOTO;
		Operand destination = (Operand)malloc(sizeof(struct Operand_));
		destination->kind = LABEL;
		strcpy(destination->u.name, label_true);

		Operand op1 = (Operand)malloc(sizeof(struct Operand_));
		if(root->firstChild->attribute == _INT){
			op1->kind = CONSTANT;
			op1->u.value = root->value.intValue;
		}
		else{
			char *t1 = newTemp();
			int kind = translate_Exp(root, t1);
			op1->kind = kind;
			/*if(kind == -2){
				op1->kind = CONSTANT;
				op1->u.value = root->firstChild->value.intValue;
			}
			else{
				op1->kind = kind;
				strcpy(op1->u.name, t1);
			}*/
			if(kind == CONSTANT) {
				op1->u.value = my_atol(t1);
			}
			else {
				strcpy(op1->u.name, t1);
			}
		}
		
		Operand op2 = (Operand)malloc(sizeof(struct Operand_));
		op2->kind = CONSTANT;
		op2->u.value = 0;
		newCode->u.condition.destination = destination;
		newCode->u.condition.op1 = op1;
		newCode->u.condition.op2 = op2;
		strcpy(newCode->u.condition.relop, "!=");
		insertIC(newCode);

		InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
		newCode1->prev = NULL;
		newCode1->next = NULL;
		newCode1->kind = GOTO;
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = LABEL;
		strcpy(op->u.name, label_false);
		newCode1->u.single.op = op;
		insertIC(newCode1);
	}
	
}

void translate_FunDec(struct Node_t* root){
//printf("FunDecin\n");
	FieldList funcFinder = getSymbol(root->firstChild->name, FUNCTION);
	if(funcFinder != NULL){
		InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
		newCode->prev = NULL;
		newCode->next = NULL;
		newCode->kind = FUNCTION_;
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = FUNC;
		strcpy(op->u.name, root->firstChild->name);
		newCode->u.single.op = op;
		insertIC(newCode);
		FieldList param = funcFinder->type->u.function.param;
		while(param != NULL){
			if(param->type->kind == STRUCTURE) {
				printf("Cannot translate: Code contains variables or parameters of struct type.\n");
				exit(-1);
			}
			char *v1 = newVariable();
			strcpy(param->vname, v1);
			InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode1->prev = NULL;
			newCode1->next = NULL;
			newCode1->kind = PARAM;
			Operand op1 = (Operand)malloc(sizeof(struct Operand_));
			op1->kind = VARIABLE;
			strcpy(op1->u.name, v1);
			newCode1->u.single.op = op1;
			insertIC(newCode1);
			param = param->ptail;
		}
	}
//printf("FunDecout\n");
}

void translate_CompSt(struct Node_t* root){
//printf("CompStin\n");
	translate(root->firstChild->next);
	translate(root->firstChild->next->next);
//printf("CompStout\n");
}

void translate_Stmt(struct Node_t* root){
//printf("Stmtin\n");
	switch(root->firstChild->attribute){
		case _Exp:
			translate_Exp(root->firstChild, NULL);
			break;
		case _CompSt:
			translate_CompSt(root->firstChild);
			break;
		case _RETURN:{
			char *t1 = newTemp();
			int kind = translate_Exp(root->firstChild->next, t1);	
			InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
			newCode->prev = NULL;
			newCode->next = NULL;
			newCode->kind = RETURN;
			Operand op = (Operand)malloc(sizeof(struct Operand_));
			op->kind = kind;
			//strcpy(op->u.name, t1);
			if(kind == CONSTANT) {
				op->u.value = my_atol(t1);
			}
			else {
				strcpy(op->u.name, t1);
			}
			newCode->u.single.op = op;
			insertIC(newCode);
			break;
		}
		case _IF:{
			if(root->firstChild->next->next->next->next->next == NULL){
				char *label1 = newLabel();
				char *label2 = newLabel();
				translate_Cond(root->firstChild->next->next, label1, label2);
			
				InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
				newCode->prev = NULL;
				newCode->next = NULL;
				newCode->kind = LABEL_;
				Operand op = (Operand)malloc(sizeof(struct Operand_));
				op->kind = LABEL;
				strcpy(op->u.name, label1);
				newCode->u.single.op = op;
				insertIC(newCode);

				translate_Stmt(root->firstChild->next->next->next->next);
				
				InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
				newCode1->prev = NULL;
				newCode1->next = NULL;
				newCode1->kind = LABEL_;
				Operand op1 = (Operand)malloc(sizeof(struct Operand_));
				op1->kind = LABEL;
				strcpy(op1->u.name, label2);
				newCode1->u.single.op = op1;
				insertIC(newCode1);
				
			}
			else{
				char *label1 = newLabel();
				char *label2 = newLabel();
				char *label3 = newLabel();
				translate_Cond(root->firstChild->next->next, label1, label2);

				InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
				newCode1->prev = NULL;
				newCode1->next = NULL;
				newCode1->kind = LABEL_;
				Operand op1 = (Operand)malloc(sizeof(struct Operand_));
				op1->kind = LABEL;
				strcpy(op1->u.name, label1);
				newCode1->u.single.op = op1;
				insertIC(newCode1);

				translate_Stmt(root->firstChild->next->next->next->next);

				InterCode newCode2 = (InterCode)malloc(sizeof(struct InterCode_));
				newCode2->prev = NULL;
				newCode2->next = NULL;
				newCode2->kind = GOTO;
				Operand op2 = (Operand)malloc(sizeof(struct Operand_));
				op2->kind = LABEL;
				strcpy(op2->u.name, label3);
				newCode2->u.single.op = op2;
				insertIC(newCode2);

				InterCode newCode3 = (InterCode)malloc(sizeof(struct InterCode_));
				newCode3->prev = NULL;
				newCode3->next = NULL;
				newCode3->kind = LABEL_;
				Operand op3 = (Operand)malloc(sizeof(struct Operand_));
				op3->kind = LABEL;
				strcpy(op3->u.name, label2);
				newCode3->u.single.op = op3;
				insertIC(newCode3);

				translate_Stmt(root->firstChild->next->next->next->next->next->next);

				InterCode newCode4 = (InterCode)malloc(sizeof(struct InterCode_));
				newCode4->prev = NULL;
				newCode4->next = NULL;
				newCode4->kind = LABEL_;
				Operand op4 = (Operand)malloc(sizeof(struct Operand_));
				op4->kind = LABEL;
				strcpy(op4->u.name, label3);
				newCode4->u.single.op = op4;
				insertIC(newCode4);
			}
			break;
		}
		case _WHILE:{
			char *label1 = newLabel();
			char *label2 = newLabel();
			char *label3 = newLabel();

			InterCode newCode1 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode1->prev = NULL;
			newCode1->next = NULL;
			newCode1->kind = LABEL_;
			Operand op1 = (Operand)malloc(sizeof(struct Operand_));
			op1->kind = LABEL;
			strcpy(op1->u.name, label1);
			newCode1->u.single.op = op1;
			insertIC(newCode1);
	
			translate_Cond(root->firstChild->next->next, label2, label3);

			InterCode newCode2 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode2->prev = NULL;
			newCode2->next = NULL;
			newCode2->kind = LABEL_;
			Operand op2 = (Operand)malloc(sizeof(struct Operand_));
			op2->kind = LABEL;
			strcpy(op2->u.name, label2);
			newCode2->u.single.op = op2;
			insertIC(newCode2);
			
			translate_Stmt(root->firstChild->next->next->next->next);

			InterCode newCode3 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode3->prev = NULL;
			newCode3->next = NULL;
			newCode3->kind = GOTO;
			Operand op3 = (Operand)malloc(sizeof(struct Operand_));
			op3->kind = LABEL;
			strcpy(op3->u.name, label1);
			newCode3->u.single.op = op3;
			insertIC(newCode3);

			InterCode newCode4 = (InterCode)malloc(sizeof(struct InterCode_));
			newCode4->prev = NULL;
			newCode4->next = NULL;
			newCode4->kind = LABEL_;
			Operand op4 = (Operand)malloc(sizeof(struct Operand_));
			op4->kind = LABEL;
			strcpy(op4->u.name, label3);
			newCode4->u.single.op = op4;
			insertIC(newCode4);
			break;
		}
	}
//printf("Stmtout\n");
}


void translate_Dec(struct Node_t* root) {
//printf("Decin\n");
	char *newVar = newVariable();
	if(root->firstChild->firstChild->next == NULL) {
		FieldList var = getSymbol(root->firstChild->firstChild->name, BASIC);
		if(var == NULL) {
			printf("Cannot translate: Code contains variables or parameters of struct type.\n");
			exit(-1);
		}
		strcpy(var->vname, newVar);
		
	}
	else {
		struct Node_t* temp = root->firstChild->firstChild;
		while(temp->attribute != _ID)
			temp = temp->firstChild;
		FieldList var = getSymbol(temp->name, ARRAY);
		if(var == NULL) {
			printf("Cannot translate: Code contains variables or parameters of struct type.\n");
			exit(-1);
		}
		strcpy(var->vname, newVar);
		Type t = var->type;
		int size = 1;
		while(t->kind == ARRAY) {
			size *= t->u.array.size;
			t = t->u.array.elem;
		}
		
		InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
		newCode->prev = NULL;
		newCode->next = NULL;
		newCode->kind = DEC;
		newCode->u.dec.op = (Operand)malloc(sizeof(struct Operand_));
		newCode->u.dec.op->kind = VARIABLE;
		strcpy(newCode->u.dec.op->u.name, newVar);
		newCode->u.dec.size = 4*size;
		insertIC(newCode);
	}
	if(root->firstChild->next != NULL) {
		char *temp = newTemp();
		int kind = translate_Exp(root->firstChild->next->next, temp);
		InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
		newCode->prev = NULL;
		newCode->next = NULL;
		newCode->kind = ASSIGN;
		newCode->u.assign.left = (Operand)malloc(sizeof(struct Operand_));
		newCode->u.assign.right = (Operand)malloc(sizeof(struct Operand_));
		newCode->u.assign.left->kind = VARIABLE;
		newCode->u.assign.right->kind = kind;
		strcpy(newCode->u.assign.left->u.name, newVar);
		if(kind == CONSTANT) {
			newCode->u.assign.right->u.value = my_atol(temp);
		}
		else {
			strcpy(newCode->u.assign.right->u.name, temp);
		}
		insertIC(newCode);
	}
//printf("Decout\n");
}

void translate_Args(struct Node_t* root, InterCode *argsHead, InterCode *argsTail) {
//printf("Argsin\n");
	if(root->firstChild->next != NULL) {
		translate_Args(root->firstChild->next->next, argsHead, argsTail);
	}
	
	InterCode newCode = (InterCode)malloc(sizeof(struct InterCode_));
	newCode->prev = NULL;
	newCode->next = NULL;
	newCode->kind = ARG;
	char* temp = newTemp();
	int kind = translate_Exp(root->firstChild, temp);
		
	newCode->u.single.op = (Operand)malloc(sizeof(struct Operand_));
	newCode->u.single.op->kind = kind;
	//strcpy(newCode->u.single.op->u.name, temp);
	if(kind == CONSTANT) {
		newCode->u.single.op->u.value = my_atol(temp);
	}
	else {
		strcpy(newCode->u.single.op->u.name, temp);
	}
//	insertIC(newCode);
	if(*argsHead == NULL) {
		*argsHead = newCode;
		*argsTail = newCode;
	}
	else {
		(*argsTail)->next = newCode;
		*argsTail = newCode;
	}
	
//printf("Argsout\n");
}


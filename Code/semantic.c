#include "semantic.h"

void semantic_analysis(struct Node_t* root){
	// printf("Analysis in\n");
	Handle_Program(root);
	for(int i = 0; i < TABLE_SIZE; i ++){
		FieldList temp = symbolTable[i];
		while(temp != NULL){
	
			if(temp->type != NULL && temp->type->kind == FUNCTION){
				if(temp->type->u.function.defined == 0){
					printf("Error Type 18 at Line %d: Undefined function \"%s\".\n",temp->type->u.function.line, temp->name);
				}
				
			}

			temp = temp->tail;
		}
	}
	// printf("Analysis out\n");
}

void Handle_Program(struct Node_t* root){
//printf("Program in\n");
	switch (root->firstChild->attribute){
		case _ExtDefList:
			Handle_ExtDefList(root->firstChild);
			break;
	}
//printf("Program out\n");
}

void Handle_ExtDefList(struct Node_t* root){
//printf("ExtDefList\n");
	switch (root->firstChild->attribute){
		case _ExtDef:
			Handle_ExtDef(root->firstChild);
			Handle_ExtDefList(root->firstChild->next);
			break;
		case None:
			break;
	}
	
}

int Var_Equal(Type t1,Type t2){
	if(t1->kind != t2->kind)
		return 0;
	switch(t1->kind){
		case BASIC:
			if(t1->u.basic != t2->u.basic)
				return 0;
			break;
		case ARRAY:
			return Var_Equal(t1->u.array.elem, t2->u.array.elem);
			break;
		case STRUCTURE:
			if(strcmp(t1->u.structure.member->name,t2->u.structure.member->name))
				return 0;
			break;
		default:
			return 0;
			break;
	}
	return 1;
}

int Func_Equal(FieldList func1, FieldList func2){
	Type ret1 = func1->type->u.function.ret;
	Type ret2 = func2->type->u.function.ret;
	if(!Var_Equal(ret1, ret2))
		return 0;
	FieldList param1 = func1->type->u.function.param;
	FieldList param2 = func2->type->u.function.param;
	while(param1 != NULL && param2 != NULL){
		if(!Var_Equal(param1->type, param2->type))
			return 0;
		param1 = param1->tail;
		param2 = param2->tail;
	}
	if(param1 == NULL && param2 != NULL)
		return 0;
	if(param1 != NULL && param2 == NULL)
		return 0;
	return 1;
}


void Handle_ExtDef(struct Node_t* root){
//printf("ExtDef\n");
	Type t = Handle_Specifier(root->firstChild);
	switch (root->firstChild->next->attribute){
		case _ExtDecList:
			if(t->kind == STRUCTURE)
				t->u.structure.flag = 0;
			Handle_ExtDecList(root->firstChild->next, t);
			break;
		case _SEMI:
			break;
		case _FunDec:
		{
			if(t->kind == STRUCTURE)
				t->u.structure.flag = 0;
			FieldList funcSymbol = Handle_FunDec(root->firstChild->next, t);
			switch (root->firstChild->next->next->attribute){
				case _CompSt:
				{	
					FieldList funcFinder = getSymbol(funcSymbol->name, funcSymbol->type->kind);
					if (funcFinder == NULL){
						funcSymbol->type->u.function.declared = 1;
						funcSymbol->type->u.function.defined = 1;
						insertSymbol(funcSymbol);
					}
					else{
						if(funcFinder->type->u.function.defined == 1){
							printf("Error Type 4 at Line %d: Redefined function \"%s\".\n", funcSymbol->type->u.function.line, funcSymbol->name);
						}
						else{
							if(Func_Equal(funcSymbol, funcFinder)){
								funcFinder->type->u.function.defined = 1;
							}
							else{
								printf("Error Type 19 at Line %d: Inconsistent declaration of function \"%s\".\n", funcSymbol->type->u.function.line, funcSymbol->name);
							}
							
						}
					}
					Handle_CompSt(root->firstChild->next->next, t);
					break;
				}
				case _SEMI:
				{
					FieldList funcFinder = getSymbol(funcSymbol->name, funcSymbol->type->kind);
					if (funcFinder == NULL){
						funcSymbol->type->u.function.declared = 1;
						insertSymbol(funcSymbol);
					}
					else if(!Func_Equal(funcSymbol, funcFinder)){
						printf("Error Type 19 at Line %d: Inconsistent declaration of function \"%s\".\n", funcSymbol->type->u.function.line, funcSymbol->name);
					}
					break;
				}
			}
			
			break;
		}
	}

	
}

void Handle_ExtDecList(struct Node_t* root, Type t){
//printf("ExtDecList\n");
	FieldList varSymbol = Handle_VarDec(root->firstChild, t);
	if(varSymbol != NULL){
		FieldList basicFinder = getSymbol(varSymbol->name, BASIC);
		FieldList arrayFinder = getSymbol(varSymbol->name, ARRAY);
		FieldList structFinder = getSymbol(varSymbol->name, STRUCTURE);
		
		if(basicFinder != NULL || arrayFinder != NULL || structFinder != NULL){
			printf("Error Type 3 at Line %d: Redefined variable \"%s\".\n", varSymbol->type->u.function.line, varSymbol->name);
		}
		else{
			insertSymbol(varSymbol);

		}
	}
	if(root->firstChild->next != NULL){
		Handle_ExtDecList(root->firstChild->next->next, t);
	}
}


Type Handle_Specifier(struct Node_t* root){
//printf("Specifier\n");
	switch(root->firstChild->attribute){
		case _TYPE:
		{
			struct Type_* current = (struct Type_*)malloc(sizeof(struct Type_));
			current->kind = BASIC;
			if(strcmp(root->firstChild->name, "int") == 0)
				current->u.basic = 0;
			else
				current->u.basic = 1;
			return current;
			break;
		}
		case _StructSpecifier:
			return Handle_StructSpecifier(root->firstChild);
			break;
	}
}


Type Handle_StructSpecifier(struct Node_t* root){
//printf("StructSpecifier\n");
	struct Type_* current = (struct Type_*)malloc(sizeof(struct Type_));
	current->kind = STRUCTURE;
	current->u.structure.member = (struct FieldList_*)malloc(sizeof(struct FieldList_));
	switch(root->firstChild->next->attribute){
		case _OptTag:
		{
			char *name = Handle_OptTag(root->firstChild->next);
			struct FieldList_* structSymbol = (struct FieldList_*)malloc(sizeof(struct FieldList_));
			structSymbol->type = (Type)malloc(sizeof(struct Type_));
			structSymbol->tail = NULL;
			structSymbol->ptail = NULL;
			if(name != NULL){
				structSymbol->name = name;
				FieldList basicFinder = getSymbol(structSymbol->name, BASIC);
				FieldList arrayFinder = getSymbol(structSymbol->name, ARRAY);
				FieldList structFinder = getSymbol(structSymbol->name, STRUCTURE);
				if(basicFinder != NULL || arrayFinder != NULL || structFinder != NULL){
					printf("Error Type 16 at Line %d: Duplicated name \"%s\".\n", root->firstChild->next->lineno, structSymbol->name);
					return NULL;
				}
			}
			localHead = NULL;
			localTail = NULL;
			structSymbol->type->kind = STRUCTURE;
			structSymbol->type->u.structure.member = Handle_DefList(root->firstChild->next->next->next,1);
			structSymbol->type->u.structure.flag = 1;
			insertSymbol(structSymbol);
			current->u.structure.member = structSymbol;

			break;
		}
		case _Tag:
		{
			char *name = Handle_Tag(root->firstChild->next);
			FieldList structFinder = getSymbol(name, STRUCTURE);		
			if(structFinder != NULL){
				FieldList fl = (struct FieldList_*)malloc(sizeof(struct FieldList_));
				fl->name = (char *)malloc(sizeof(char)*15);
				fl->type = (struct Type_*)malloc(sizeof(struct Type_));
				fl->tail = fl->ptail = NULL;
				fl->isParam = 0;
				strcpy(fl->name, structFinder->name);
				fl->type->kind = STRUCTURE;
				fl->type->u.structure.flag = 0;
				fl->type->u.structure.member = structFinder->type->u.structure.member;
				current->u.structure.member = fl;
				
			}
			else{
				printf("Error Type 17 at Line %d: Undefined structure \"%s\".\n", root->firstChild->next->lineno, name);
				return NULL;
			}
			break;
		}
	}
	return current;
}


char* Handle_OptTag(struct Node_t* root){
//printf("OptTag\n");
	switch(root->firstChild->attribute){
		case _ID:
			return root->firstChild->name;
			break;
		case None:
			return NULL;
			break;
	}
}

char* Handle_Tag(struct Node_t* root){
//printf("Tag\n");
	return root->firstChild->name;
}
	
FieldList Handle_VarDec(struct Node_t* root, Type t){
//printf("VarDec\n");
	if(t == NULL)
		return NULL;
	struct FieldList_* ret  = (struct FieldList_*)malloc(sizeof(struct FieldList_));
	ret->type = (Type)malloc(sizeof(struct Type_));
	switch(root->firstChild->attribute){
		case _ID:
			ret->type = (Type)malloc(sizeof(struct Type_));
				
			ret->type->kind = t->kind;
			if(t->kind == BASIC){
				ret->type->u.basic = t->u.basic;
			}
			else if(t->kind == ARRAY){
				ret->type->u.array.elem = t->u.array.elem;
				ret->type->u.array.size = t->u.array.size;
			}
			else if(t->kind == STRUCTURE){
				ret->type->u.structure.member = t->u.structure.member;
				ret->type->u.structure.flag = 0;
			}
			else{
				ret->type->u.function.ret = t->u.function.ret;
				ret->type->u.function.declared = t->u.function.declared;
				ret->type->u.function.defined = t->u.function.defined;
				ret->type->u.function.line = t->u.function.line;
				ret->type->u.function.param = t->u.function.param;

			}
				
			ret->name = root->firstChild->name;

			ret->tail = NULL;
			ret->ptail = NULL;
			break;
		case _VarDec:
		{
			ret = Handle_VarDec(root->firstChild, t);
			int size = root->firstChild->next->next->value.intValue;
			switch(ret->type->kind){
				case ARRAY: 
				{
					struct Type_* newelem = (struct Type_*)malloc(sizeof(struct Type_));
					newelem->kind = ARRAY;
					newelem->u.array.size = ret->type->u.array.size;
					newelem->u.array.elem = ret->type->u.array.elem;
					ret->type->u.array.elem = newelem;
					ret->type->u.array.size = size;
					break;
				}
				default:
				{
					struct Type_* newelem = (struct Type_*)malloc(sizeof(struct Type_));
					newelem->kind = ret->type->kind;
					newelem->u.array.size = ret->type->u.array.size;
					newelem->u.array.elem = ret->type->u.array.elem;
					ret->type->kind = ARRAY;
					ret->type->u.array.elem = newelem;
					ret->type->u.array.size = size;
					break;
				}
			}
			break;
		}
	}
	return ret;

}


FieldList Handle_FunDec(struct Node_t* root, Type t){
//printf("FunDec\n");
	FieldList retVal = (FieldList)malloc(sizeof(struct FieldList_));
	retVal->name = root->firstChild->name;
	//strcpy(retVal->name, root->firstChild->name);
	retVal->type = (Type)malloc(sizeof(struct Type_));
	retVal->type->kind = FUNCTION;
	retVal->type->u.function.ret = t;
	retVal->type->u.function.declared = 1;
	retVal->type->u.function.defined = 0;
	retVal->type->u.function.line = root->firstChild->lineno;
	retVal->tail = NULL;
	retVal->ptail = NULL;
	FieldList funcFinder = getSymbol(root->firstChild->name, FUNCTION);
	int firstDeclared;
	if(funcFinder == NULL)
		firstDeclared = 1;
	else
		firstDeclared = 0;
	switch(root->firstChild->next->next->attribute){
		case _VarList:
		{
			FieldList param = Handle_VarList(root->firstChild->next->next, firstDeclared);
			retVal->type->u.function.param = param;
			break;
		}
		case _RP:
			retVal->type->u.function.param = NULL;
			break;
	}
	
	return retVal;
}


FieldList Handle_VarList(struct Node_t* root, int firstDeclared){
//printf("VarList\n");
	FieldList param = Handle_ParamDec(root->firstChild, firstDeclared);
	if(root->firstChild->next != NULL){
		FieldList originparam = Handle_VarList(root->firstChild->next->next, firstDeclared);
		param->ptail = originparam;
	}
	return param;
}

FieldList Handle_ParamDec(struct Node_t* root, int firstDeclared){
//printf("ParamDec\n");
	Type t = Handle_Specifier(root->firstChild);
	FieldList varSymbol = Handle_VarDec(root->firstChild->next, t);
	varSymbol->ptail = NULL;
	FieldList basicFinder = getSymbol(varSymbol->name, BASIC);
	FieldList arrayFinder = getSymbol(varSymbol->name, ARRAY);
	FieldList structFinder = getSymbol(varSymbol->name, STRUCTURE);
	if(basicFinder == NULL && arrayFinder == NULL && structFinder == NULL){
		insertSymbol(varSymbol);
		varSymbol->isParam = 1;
	}
	else{
		if(firstDeclared){
			printf("Error Type 3 at Line %d: Redefined variable \"%s\".\n", varSymbol->type->u.function.line, varSymbol->name);
			return NULL;
		}
	}
	return varSymbol;
}

void Handle_CompSt(struct Node_t* root, Type t) {
//printf("CompSt\n");
	Handle_DefList(root->firstChild->next, 0);
	Handle_StmtList(root->firstChild->next->next, t);
}

void Handle_StmtList(struct Node_t* root, Type t) {
//printf("StmtList\n");
	if(root->attribute != None) {
		Handle_Stmt(root->firstChild, t);
		Handle_StmtList(root->firstChild->next, t);
	}
}

void Handle_Stmt(struct Node_t* root, Type t) {
//printf("Stmt\n");
	switch(root->firstChild->attribute){
		case _Exp:
			Handle_Exp(root->firstChild);
			break;
		case _CompSt:
			Handle_CompSt(root->firstChild, t);
			break;
		case _RETURN:
		{
			Type retType = Handle_Exp(root->firstChild->next);
			if(retType != NULL && Var_Equal(retType,t) == 0){
				printf("Error Type 8 at Line %d: Type mismatched for return.\n", root->firstChild->next->lineno);
			}
			break;
		}
		case _IF:
			Handle_Exp(root->firstChild->next->next);
			Handle_Stmt(root->firstChild->next->next->next->next, t);
			if(root->firstChild->next->next->next->next->next != NULL){
				Handle_Stmt(root->firstChild->next->next->next->next->next->next, t);
			}
			break;
		case _WHILE:
			Handle_Exp(root->firstChild->next->next);
			Handle_Stmt(root->firstChild->next->next->next->next, t);
			break;
	}
}

FieldList Handle_DefList(struct Node_t* root, int isStructure) {
//printf("DefList\n");
	if(root->attribute != None) {
		if(isStructure == 1) {

			FieldList head = Handle_Def(root->firstChild, isStructure);
			FieldList tail = Handle_DefList(root->firstChild->next, isStructure);
			if(head != NULL) {
				head->tail = tail;
				return head;
			}
			else {
				return tail;
			}
		}
		else {
			FieldList retDef = Handle_Def(root->firstChild, isStructure);
			if(retDef != NULL) {
				FieldList tmp = NULL;
				while(retDef != NULL) {
					tmp = retDef->tail;
					insertSymbol(retDef);
					retDef = tmp;
				}
			}
			return Handle_DefList(root->firstChild->next, isStructure);
		}
	}
	return NULL;
}

FieldList Handle_Def(struct Node_t* root, int isStructure) {
//printf("Def\n");
	Type retType = Handle_Specifier(root->firstChild);
	return Handle_DecList(root->firstChild->next, retType, isStructure);
}

FieldList Handle_DecList(struct Node_t* root, Type t, int isStructure) {
//printf("DecList\n");	
	FieldList head = Handle_Dec(root->firstChild, t, isStructure);
	FieldList tail = NULL;
	if(root->firstChild->next != NULL) {
		tail = Handle_DecList(root->firstChild->next->next, t, isStructure);
	}
	
	if(head != NULL) {
		head->tail = tail;
		return head;
	}
	else {
		return tail;
	}
}

FieldList Handle_Dec(struct Node_t* root, Type t, int isStructure) {
//printf("Dec\n");	
	FieldList retField = Handle_VarDec(root->firstChild, t);
	if(retField == NULL)
		return NULL;
	if(root->firstChild->next != NULL) {
		if(isStructure == 1) {
			printf("Error Type 15 at Line %d: Invalid definition in structure.\n", root->firstChild->lineno);
			if(localHead == NULL) {
				localHead = retField;
				localTail = localHead;
			}
			else {
				FieldList head = localHead;
				int flag = 0;
				while(head != NULL) {
					if(!strcmp(retField->name, head->name)) {
						printf("Error Type 15 at Line %d: Invalid definition in structure.\n", root->firstChild->lineno);
						flag = 1;
						retField = NULL;
						break;
					}
					head = head->tail;
				}
				
				if(flag == 0) {
					localTail->tail = retField;
					localTail = localTail->tail;
				}
			}
			return retField;
		}
		else{
			FieldList findBasic = getSymbol(retField->name, BASIC);
			FieldList findArray = getSymbol(retField->name, ARRAY);
			FieldList findStruct = getSymbol(retField->name, STRUCTURE);
			
			if(findBasic != NULL || findArray != NULL || findStruct != NULL) {
				printf("Error Type 3 at Line %d: Redefined variable \"%s\".\n", root->firstChild->lineno, retField->name);
				return NULL;
			}
			Type retType = Handle_Exp(root->firstChild->next->next);
			if(retType == NULL) {
				return NULL;
			}
			int res = Var_Equal(retType, t);
			if(res == 0) {
				printf("Error Type 5 at Line %d: Type mismatched.\n", root->firstChild->next->next->lineno);
				return NULL;
			}
			else {
				return retField;
			}
		}
	}
	else {
		
		if(isStructure == 1) {
			if(localHead == NULL) {
				localHead = retField;
				localTail = localHead;
			}
			else {
				FieldList head = localHead;
				int flag = 0;
				while(head != NULL) {
					if(!strcmp(retField->name, head->name)) {
						printf("Error Type 15 at Line %d: Invalid definition in structure.\n", root->firstChild->lineno);
						flag = 1;
						retField = NULL;
						break;
					}
					head = head->tail;
				}
				
				if(flag == 0) {
					localTail->tail = retField;
					localTail = localTail->tail;
				}
			}
		}
		else {

			FieldList arrayFinder = getSymbol(retField->name, ARRAY);
			FieldList basicFinder = getSymbol(retField->name, BASIC);
			FieldList structFinder = getSymbol(retField->name, STRUCTURE);
			
			if(arrayFinder == NULL && basicFinder == NULL && structFinder == NULL) {
				insertSymbol(retField);
			}
			else {
				printf("Error Type 3 at Line %d: Redefined variable \"%s\".\n", root->firstChild->lineno, retField->name);
				retField = NULL;
			}
		}
		return retField;
	}
}

int check_left(struct Node_t* root){
	if(root == NULL)
		return 0;
	if(root->firstChild->attribute == _ID && root->firstChild->next == NULL)
		return 1;
	if(root->firstChild->attribute == _Exp && root->firstChild->next->attribute == _DOT && root->firstChild->next->next->attribute == _ID)
		return 1;
	if(root->firstChild->attribute == _Exp && root->firstChild->next->attribute == _LB && root->firstChild->next->next->attribute == _Exp && root->firstChild->next->next->next->attribute == _RB)
		return 1;
	return 0;
}

Type Handle_Exp(struct Node_t* root){
//printf("Exp\n");
	switch(root->firstChild->attribute){
		case _Exp:
			
			switch(root->firstChild->next->attribute){
				case _ASSIGNOP:
				{	
					int flag = 1;
					if(check_left(root->firstChild) == 0){
						printf("Error Type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", root->firstChild->lineno);
						flag = 0;
					}
					Type left = Handle_Exp(root->firstChild);
					Type right = Handle_Exp(root->firstChild->next->next);
					if(left == NULL || right == NULL){
						printf("Error Type 5 at Line %d: Type mismatched for assignment.\n", root->firstChild->lineno);
						return NULL;
					}
					if(!Var_Equal(left,right)){
						printf("Error Type 5 at Line %d: Type mismatched for assignment.\n", root->firstChild->lineno);
						return NULL;
					}
					if(flag == 0)
						return NULL;
					return left;	
					break;
				}
				case _AND:case _OR:case _RELOP:case _PLUS:case _MINUS:case _STAR:case _DIV:
				{
					Type left = Handle_Exp(root->firstChild);
					Type right = Handle_Exp(root->firstChild->next->next);
					if(left == NULL || right == NULL){
						printf("Error Type 7 at Line %d: Type mismatched for operands.\n", root->firstChild->lineno);
						return NULL;
					}
					if(left->kind != BASIC || !Var_Equal(left,right)){
						printf("Error Type 7 at Line %d: Type mismatched for operands.\n", root->firstChild->lineno);
						return NULL;
					}
					return left;
					break;
				}	
				case _LB:
				{
					int flag = 1;
					Type left = Handle_Exp(root->firstChild);
					Type mid = Handle_Exp(root->firstChild->next->next);
					//if(left == NULL)
					//	return NULL;
					if(left==NULL || mid == NULL){
						printf("Error Type 12 at Line %d: subscript is not integer.\n", root->firstChild->next->next->lineno);
						return NULL;
					}
					if(left->kind != ARRAY){
						printf("Error Type 10 at Line %d: \"%s\" is not an array.\n", root->firstChild->lineno, root->firstChild->firstChild->name);
						flag = 0;
					}
					if(!(mid->kind == BASIC && mid->u.basic == 0)){
						printf("Error Type 12 at Line %d: subscript is not integer.\n", root->firstChild->next->next->lineno);
						return NULL;
					}
					if(flag == 0)
						return NULL;
					return left->u.array.elem;
					break;
				}
				case _DOT:
				{	
					Type left = Handle_Exp(root->firstChild);
					char name[100];
					strcpy(name, root->firstChild->next->next->name);
					if(left == NULL)
						return NULL;
					if(left->kind != STRUCTURE){
						printf("Error Type 13 at Line %d: Illegal use of \".\".\n", root->firstChild->lineno);
						return NULL;
					}
					else{
						FieldList temp = left->u.structure.member->type->u.structure.member;
						while(temp != NULL){
							if(strcmp(name, temp->name) == 0)
								return temp->type;

							temp = temp->tail;
						}
						printf("Error Type 14 at Line %d: Non-existent field \"%s\".\n", root->firstChild->next->next->lineno, root->firstChild->next->next->name);
						return NULL;
					}
					break;
				}
			}
			break;
		case _LP:
			return Handle_Exp(root->firstChild->next);
			break;
		case _MINUS:
		{
			Type right = Handle_Exp(root->firstChild->next);
			if(right == NULL)
				return NULL;
			switch(right->kind){
				case BASIC:
					return right;
					break;
				case ARRAY: case STRUCTURE: case FUNCTION:
					printf("Error Type 7 at Line %d: Type mismatched for operands.\n", root->firstChild->next->lineno);
					return NULL;
					break;
			}	
			break;
		}
		case _NOT:
		{
			Type right = Handle_Exp(root->firstChild->next);
			if(right == NULL)
				return NULL;
			if(right->kind == BASIC && right->u.basic == 0)
				return right;
			else{
				printf("Error Type 7 at Line %d: Type mismatched for operands.\n", root->firstChild->next->lineno);
				return NULL;
			}
			break;
		}
		case _ID:			
			if(root->firstChild->next != NULL){

				switch(root->firstChild->next->next->attribute){
					case _Args:
					{
						FieldList basicFinder = getSymbol(root->firstChild->name, BASIC);
						FieldList arrayFinder = getSymbol(root->firstChild->name, ARRAY);
						FieldList structFinder = getSymbol(root->firstChild->name, STRUCTURE);
						FieldList funcFinder = getSymbol(root->firstChild->name, FUNCTION);

						if((basicFinder != NULL || arrayFinder != NULL || structFinder != NULL) && funcFinder == NULL){
							printf("Error Type 11 at Line %d: \"%s\" is not a function.\n", root->firstChild->lineno, root->firstChild->name);
							Handle_Rest(root->firstChild->next->next);
							return NULL;
						}
						if(funcFinder == NULL || (funcFinder->type->u.function.defined == 0 && funcFinder->type->u.function.declared == 0)){
							printf("Error Type 2 at Line %d: Undefined function \"%s\".\n", root->firstChild->lineno, root->firstChild->name);
							Handle_Rest(root->firstChild->next->next);
							return NULL;
						}
						FieldList param = funcFinder->type->u.function.param;
						if(Handle_Args(root->firstChild->next->next, param) == 0){
							printf("Error Type 9 at Line %d: invocation arguments mismatched.\n", root->firstChild->lineno);
							return NULL;
						}
						return funcFinder->type->u.function.ret;
						break;
					}
					case _RP:
					{
						FieldList basicFinder = getSymbol(root->firstChild->name, BASIC);
						FieldList arrayFinder = getSymbol(root->firstChild->name, ARRAY);
						FieldList structFinder = getSymbol(root->firstChild->name, STRUCTURE);
						FieldList funcFinder = getSymbol(root->firstChild->name, FUNCTION);
						if((basicFinder != NULL || arrayFinder != NULL || structFinder != NULL) && funcFinder == NULL){
							printf("Error Type 11 at Line %d: \"%s\" is not a function.\n", root->firstChild->lineno, root->firstChild->name);
							return NULL;
						}
						if(funcFinder == NULL ||(funcFinder->type->u.function.defined == 0 && funcFinder->type->u.function.declared == 0)){
							printf("Error Type 2 at Line %d: Undefined function \"%s\".\n", root->firstChild->lineno, root->firstChild->name);
							return NULL;
						}
						FieldList param = funcFinder->type->u.function.param;
						if(param != NULL){
							printf("Error Type 9 at Line %d: invocation arguments mismatched.\n", root->firstChild->lineno);
							return NULL;
						}
						return funcFinder->type->u.function.ret;
						break;
					}
				}
			}
			else{	
				FieldList basicFinder = getSymbol(root->firstChild->name, BASIC);
				FieldList arrayFinder = getSymbol(root->firstChild->name, ARRAY);
				FieldList structFinder = getSymbol(root->firstChild->name, STRUCTURE);
				
				if(basicFinder == NULL && arrayFinder == NULL && structFinder == NULL){

					//printf("Error Type 1 at Line %d:Undefined variable \"%s\".\n", root->firstChild->lineno, root->firstChild->name);
					printf("Error Type 1 at Line %d: undefined variable.\n", root->firstChild->lineno);
					return NULL;
				}
				if(basicFinder != NULL){
					return basicFinder->type;
				}
				if(arrayFinder != NULL)
					return arrayFinder->type;
				if(structFinder != NULL)
					return structFinder->type;
			}
			break;
		case _INT:
		{ 
			struct Type_* current = (struct Type_*)malloc(sizeof(struct Type_));
			current->kind = BASIC;
			current->u.basic = 0;
			return current;
			break;
		}
		case _FLOAT:
		{  
			struct Type_* current = (struct Type_*)malloc(sizeof(struct Type_));
			current->kind = BASIC;
			current->u.basic = 1;
			return current;
			break;
		}
		
	}
}

int Handle_Args(struct Node_t* root, FieldList params) {
//printf("Args\n");
	if(params == NULL) {
		return 0;
	}
	struct Type_* retType = Handle_Exp(root->firstChild);
	if(retType != NULL && Var_Equal(retType, params->type)) {
		if(root->firstChild->next == NULL) {
			if(params->ptail != NULL) {
				//printf("Error Type 5 at Line %d:Type mismatched.\n", root->firstChild->next->lineno);
				return 0;
			}
			else {
			
				return 1;
			}
		}
		else {
			return Handle_Args(root->firstChild->next->next, params->ptail);
		}
	}
	else {
		return 0;
	}
}

void Handle_Rest(struct Node_t* root) {
	Handle_Exp(root->firstChild);
	if(root->firstChild->next != NULL) {
		Handle_Rest(root->firstChild->next->next);
	}
}

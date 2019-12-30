#include "symbols.h"

void initTable(){
	memset(symbolTable, 0, sizeof(symbolTable));

	FieldList readFunc = (FieldList)malloc(sizeof(struct FieldList_));
	readFunc->name = "read";
	readFunc->tail = NULL;
	readFunc->ptail = NULL;
	readFunc->type = (Type)malloc(sizeof(struct Type_));
	readFunc->type->kind = FUNCTION;
	Type retType = (Type)malloc(sizeof(struct Type_));
	retType->kind = BASIC;
	retType->u.basic = 0;
	readFunc->type->u.function.ret = retType;
	readFunc->type->u.function.declared = 1;
	readFunc->type->u.function.defined = 1;
	readFunc->type->u.function.line = 0;
	readFunc->type->u.function.param = NULL;
	insertSymbol(readFunc);

	FieldList writeFunc = (FieldList)malloc(sizeof(struct FieldList_));
	writeFunc->name = "write";
	writeFunc->tail = NULL;
	writeFunc->ptail = NULL;
	writeFunc->type = (Type)malloc(sizeof(struct Type_));
	writeFunc->type->kind = FUNCTION;
	writeFunc->type->u.function.ret = retType;
	writeFunc->type->u.function.declared = 1;
	writeFunc->type->u.function.defined = 1;
	writeFunc->type->u.function.line = 0;
	//writeFunc->type->u.function.param = NULL;
	FieldList args = (FieldList)malloc(sizeof(struct FieldList_));
	args->tail = NULL;
	args->ptail = NULL;
	args->type = (Type)malloc(sizeof(struct Type_));
	args->type->kind = BASIC;
	args->type->u.basic = 0;
	writeFunc->type->u.function.param = args;
	insertSymbol(writeFunc);
}


unsigned int hash(char* name)
{
	unsigned int val = 0, i;
	for(; *name; ++name)
	{
		val = (val << 2) + *name;
		if(i = val & ~0x3fff)
			val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val%1024;
}

int insertSymbol(FieldList symbol){
	unsigned int value = hash(symbol->name);
	symbol->isParam = 0;
	symbol->vname = (char *)malloc(sizeof(char)*10);
	strcpy(symbol->vname, "-1");
	if(symbolTable[value] != NULL){
		FieldList temp = symbolTable[value];
		for(; temp->tail != NULL; temp = temp->tail){
			if((temp->type->kind == symbol->type->kind) && (strcmp(temp->name, symbol->name) == 0))
				return -1;
		}
		if(strcmp(temp->name, symbol->name) == 0 && (temp->type->kind == symbol->type->kind) )
			return -1;
		temp->tail = symbol;
	}
	else
		symbolTable[value] = symbol;
	return 0;
}

FieldList getSymbol(char *name, int kind){
	unsigned int value = hash(name);
	FieldList temp = symbolTable[value];
	
	while(temp != NULL){
		if((temp->type->kind == kind) && (strcmp(temp->name, name) == 0))
			return temp;
		temp = temp->tail;

	}
	return NULL;
}

		

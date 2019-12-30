#include <stdio.h>
#include <string.h>
#ifndef _SYMBOLS_
#define _SYMBOLS_
#define TABLE_SIZE 1024
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Caller_* Caller;
typedef struct Param_* Param;


FieldList symbolTable[TABLE_SIZE];

struct Type_
{
	enum { BASIC, ARRAY, STRUCTURE, FUNCTION} kind;
	union
	{
		int basic;
		struct {Type elem; int size;} array;
		struct {FieldList member; int flag;} structure;
		struct {Type ret; int declared; int defined; int line; FieldList param; /*Caller callers*/} function;
	}u;
};

struct FieldList_
{
	char* name;
	Type type;
	FieldList tail;
	FieldList ptail;
	char *vname;
	int isParam;
};
/*
struct Caller_
{
	int lineno;
	Param params;
	Caller tail;
};


struct Param_
{
	Type type;
	Param next;
};
*/

void initTable();

unsigned int hash(char *name);

int insertSymbol(FieldList symbol);

FieldList getSymbol(char *name, int kind);


#endif

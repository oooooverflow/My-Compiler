#ifndef _INTERCODE_
#define _INTERCODE_

#include "multi_tree.h"
#include <stdio.h>
#include "symbols.h"
typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct ArrayNode_* ArrayNode;

struct Operand_ {
	enum { TEMPVAR, VARIABLE, CONSTANT, LABEL, FUNC, ADDRESS, POINTER} kind;
	union {
		long value;
		char name[10];
	} u;
};

struct InterCode_ {
	enum { LABEL_, FUNCTION_, ASSIGN, ADD, SUB, MUL, DIV, GOTO, IFGOTO, RETURN, DEC, ARG, CALL, PARAM, READ, WRITE } kind;
	union {
		struct { Operand op; } single;
		struct { Operand left, right; } assign;
		struct { Operand result, op1, op2; } binop;
		struct { Operand op1, op2; char relop[10]; Operand destination;  } condition;
		struct { Operand op; int size; } dec;
		struct { Operand op1, op2; } call;
	} u;
	InterCode prev;
	InterCode next;
};


struct ArrayNode_ {
	char number[10];
	int kind;
	ArrayNode next;
};
extern InterCode IChead;
extern InterCode ICtail;


void insertIC(InterCode current);
void deleteIC(InterCode current);
char* printOperand(Operand op);
void printIC(char* filename);
char* newLabel();
char* newTemp();
char* newVariable();

void translate(struct Node_t* root);
int translate_Exp(struct Node_t* root, char *place);
void translate_Cond(struct Node_t* root, char *label_true, char *label_false);
void translate_FunDec(struct Node_t* root);
void translate_CompSt(struct Node_t* root);
void translate_Stmt(struct Node_t* root);
void translate_Dec(struct Node_t* root);
void translate_Args(struct Node_t* root, InterCode *argsHead, InterCode *argsTail);
#endif

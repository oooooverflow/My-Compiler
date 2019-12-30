#include "multi_tree.h"
#include "symbols.h"
#ifndef _SEMANTIC_
#define _SEMANTIC_

FieldList localHead;
FieldList localTail;

void semantic_analysis(struct Node_t* root);
int Var_Equal(Type t1,Type t2);
int Func_Equal(FieldList func1,FieldList func2);
int check_left(struct Node_t* root);

void Handle_Program(struct Node_t* root);
void Handle_ExtDefList(struct Node_t* root);
void Handle_ExtDef(struct Node_t* root);
void Handle_ExtDecList(struct Node_t* root, Type t);

Type Handle_Specifier(struct Node_t* root);
Type Handle_StructSpecifier(struct Node_t* root);
char* Handle_OptTag(struct Node_t* root);
char* Handle_Tag(struct Node_t* root);

FieldList Handle_VarDec(struct Node_t* root, Type t);
FieldList Handle_FunDec(struct Node_t* root, Type t);
FieldList Handle_VarList(struct Node_t* root, int firstDeclared);
FieldList Handle_ParamDec(struct Node_t* root, int firstDeclared);

void Handle_CompSt(struct Node_t* root, Type t);
void Handle_StmtList(struct Node_t* root, Type t);
void Handle_Stmt(struct Node_t* root, Type t);

FieldList Handle_DefList(struct Node_t* root, int isStructure);
FieldList Handle_Def(struct Node_t* root, int isStructure);
FieldList Handle_DecList(struct Node_t* root, Type t, int isStructure);
FieldList Handle_Dec(struct Node_t* root, Type t, int isStructure);

Type Handle_Exp(struct Node_t* root);
int Handle_Args(struct Node_t* root, FieldList params);
void Handle_Rest(struct Node_t* root);

#endif

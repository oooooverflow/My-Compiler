#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifndef _MULTI_TREE_
#define _MULTI_TREE_
#define DEFAULT_LEN 100

enum Attribute_e{
    // epsilon
    None,
    /*******/
    _INT, _FLOAT,
    _SEMI, _COMMA, _ASSIGNOP, _RELOP,
    _PLUS, _MINUS, _STAR, _DIV,
    _AND, _OR, _DOT, _NOT,
    _ID, _TYPE,
    _LP, _RP, _LB, _RB, _LC, _RC,
    _STRUCT, _RETURN, _IF, _ELSE, _WHILE,
    /*******/
    _Program, _ExtDefList, _ExtDef, _ExtDecList,
    _Specifier, _StructSpecifier, _OptTag,
    _Tag, _VarDec, _FunDec, _VarList, _ParamDec,
    _CompSt, _StmtList, _Stmt, _DefList,
    _Def, _DecList, _Dec, _Exp, _Args
};

// union {
//     Attribute_e attribute;
//     char name[DEFAULT_LEN];
// }Type;

union Value{
    int intValue;
    float floatValue;
};

struct Node_t{
    enum Attribute_e attribute;
    char name[DEFAULT_LEN];
    union Value value;
    int line;
    int lineno;
    struct Node_t *parent;
    struct Node_t *firstChild;
    struct Node_t *next;
};

struct Node_t* createNode(enum Attribute_e attribute, int n,int lineno,...);
void showTree(struct Node_t* root, int depth);
void clearTree(struct Node_t* root);

#endif

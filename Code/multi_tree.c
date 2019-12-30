#include "multi_tree.h"
#include <assert.h>
const char* attributeName[] =
{
    // epsilon
    "None",
    /*******/
    "INT", "FLOAT",
    "SEMI", "COMMA", "ASSIGNOP", "RELOP",
    "PLUS", "MINUS", "STAR", "DIV",
    "AND", "OR", "DOT", "NOT",
    "ID", "TYPE",
    "LP", "RP", "LB", "RB", "LC", "RC",
    "STRUCT", "RETURN", "IF", "ELSE", "WHILE",
    /*******/
    "Program", "ExtDefList", "ExtDef", "ExtDecList",
    "Specifier", "StructSpecifier", "OptTag",
    "Tag", "VarDec", "FunDec", "VarList", "ParamDec",
    "CompSt", "StmtList", "Stmt", "DefList",
    "Def", "DecList", "Dec", "Exp", "Args"
};

struct Node_t* createNode(enum Attribute_e attribute, int lineno, int n,...) {
    struct Node_t* current = (struct Node_t*)malloc(sizeof(struct Node_t));
    current->attribute = attribute;
    current->parent = NULL;
    current->next = NULL;
    current->firstChild = NULL;
    current->lineno = lineno;
    //printf("here!  %s\n", attributeName[current->attribute]);
    if(n == 0)
        return current;
    //printf("here!  %s\n", attributeName[current->attribute]);
    va_list ap;
    va_start(ap, n);
    struct Node_t* head = NULL;
    struct Node_t* tail = NULL;
    int i = 0;
    for(; i < n; i++) {
        if(head == NULL) {
            head = va_arg(ap, struct Node_t*);
            head->parent = current;
            tail = head;
        }
        else {
            tail->next = va_arg(ap, struct Node_t*);
            tail->parent = current;
            tail = tail->next;
        }
    }
    current->firstChild = head;
    current->line = -1;
    while(head != NULL) {
        if(head->attribute != None) {
            current->line = head->line;
            break;
        }
        head = head->next;
    }
    if(current->line == -1)
        current->attribute = None;
    return current;
}

void showTree(struct Node_t* root, int depth) {
    if(root == NULL || root->attribute == None) {
        return;
    }
    int i = 0;
    for(; i < depth; i++)
        printf("  ");
    printf("%s", attributeName[root->attribute]);
    if(root->attribute > _WHILE) {
        printf(" (%d)\n", root->line);
    }
    else if(root->attribute == _ID || root->attribute == _TYPE) {
        printf(": %s\n", root->name);
    }
    else if(root->attribute == _INT) {
        printf(": %d\n", root->value.intValue);
    }
    else if(root->attribute == _FLOAT) {
        printf(": %f\n", root->value.floatValue);
    }
    else {
        printf("\n");
    }
    struct Node_t* p = root->firstChild;
    while(p != NULL) {
        showTree(p, depth+1);
        p = p->next;
    }
    /*FILE *fp;
    fp = fopen("../Test/test.output","a");
    if(root == NULL || root->attribute == None) {
        return;
    }
    int i = 0;
    for(; i < depth; i++)
	fprintf(fp,"  ");
    fprintf(fp, "%s", attributeName[root->attribute]);
    if(root->attribute > _WHILE) {
        fprintf(fp," (%d)\n", root->line);
    }
    else if(root->attribute == _ID || root->attribute == _TYPE) {
        fprintf(fp,": %s\n", root->name);
    }
    else if(root->attribute == _INT) {
        fprintf(fp,": %d\n", root->value.intValue);
    }
    else if(root->attribute == _FLOAT) {
        fprintf(fp,": %f\n", root->value.floatValue);
    }
    else {
        fprintf(fp,"\n");
    }
    fclose(fp);
    struct Node_t* p = root->firstChild;
    while(p != NULL) {
        showTree(p, depth+1);
        p = p->next;
    }*/
    
}

void clearTree(struct Node_t* root) {
    // printf("1\n");
    if(root == NULL) {
        return;
    }
    // printf("2\n");
    // assert(root->name != NULL);
    // assert(root != NULL);
    struct Node_t* head = root->firstChild;
    struct Node_t* tmp = NULL;
    while(head != NULL) {
        tmp = head->next;
        // assert(tmp != NULL);
        clearTree(head);
        head = tmp;
    }
    // printf("3\n");
    // assert(root != NULL);
    free(root);
    root = NULL;
}

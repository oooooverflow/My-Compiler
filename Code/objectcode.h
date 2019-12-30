#ifndef OBJECTCODE_H
#define OBJECTCODE_H
#include "semantic.h"
#include "intercode.h"
typedef struct Reg_* Reg;
typedef struct Operands_ * Operands;
struct Reg_ {
        char name[8];
        int index;
        int isFree;   
        Operand content;
} Regs[32];

struct Operands_{
        Operand op;
        int offset;
        Operands next;
        Operands prev;
};
Operands opHead,opTail;
InterCode IChead, ICtail;
void InitConditions();
Operands SearchList(Operand key);
void InsertList(Operand key, int offset);
void GenerateOCs(FILE *fp);
void GenerateOC(InterCode i,FILE *fp);
int AllocateReg(Operand content, int flag,FILE *fp);
void FreeReg(FILE *fp);
int EqualOperand(Operand op1, Operand op2);
#endif


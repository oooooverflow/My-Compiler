#include <stdio.h>
#include "multi_tree.h"
#include "semantic.h"
#include "intercode.h"
#include "objectcode.h"
extern int yylineno;

extern FILE* yyin;
extern struct Node_t* root;
extern int error_occured;

int main(int argc, char **argv)
{
	if (argc <= 1){
        	return 1;
    	}
    	FILE* fp = fopen(argv[1],"r");
    	if (!fp){
        	perror(argv[1]);
        	return 1;
    	}
   	yylineno=1;
    	yyrestart(fp);
    	yyparse();
	
	if(error_occured==0){
		// showTree(root, 0);
		initTable();
		semantic_analysis(root);
		translate(root);
		//printIC(argv[2]);
		FILE *fptr = fopen(argv[2],"w");
		GenerateOCs(fptr);
	}
	clearTree(root);
	return 0;
}


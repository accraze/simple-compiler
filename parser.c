#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"

struct statementNode* parse_program_and_generate_intermediate_representation(){
	struct statementNode* program;
	program = make_statementNode();
	program->stmt_type = NOOPSTMT;
	
	ttype = getToken();
	if (ttype == ID)
	{
		//var section
		ungetToken();
		program->next = parse_id_list();
	} else
	if (ttype == LBRACE)
	{
		//body section
		program->next = parse_body();
	}else{
		//error?
	}
	
	return program;
}

struct statementNode* parse_id_list(){
	struct statementNode* id_list;

	return id_list;
}

struct statementNode* parse_body(){
	struct statementNode* body;

	return body;

}


struct statementNode* make_statementNode(){        
        return (struct statementNode*) malloc(sizeof(struct statementNode));
}





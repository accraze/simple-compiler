#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"

struct statementNode* parse_program_and_generate_intermediate_representation(){
	struct statementNode* program;
	// program = make_statementNode();
	// program->stmt_type = NOOPSTMT;
	program = parse_program();
	
	return program;
}

struct statementNode* parse_program(){
	struct statementNode* prog;
	prog = make_statementNode();
	ttype = getToken();
	
	if (ttype == ID)
	{
		//var section
		ungetToken();
		prog->stmt_type = ASSIGNSTMT;
		prog->assign_stmt = parse_assign_stmt();
		ttype = getToken();
		if (ttype == LBRACE)
		{
			program->next = parse_body();
		}
	} else
	if (ttype == LBRACE)
	{
		//body section
		program->next = parse_body();
	}else{
		//error?
	}

	return prog;
}


struct assignmentStatement* parse_assign_stmt(){
	struct statementNode* assign_stmt;
	ttype = getToken();
	
	if (ttype == ID)
	{
		assign_stmt = make_assignmentStatement();
		assign_stmt->op1 = parse_var();
		ttype = getToken();
		if (ttype == SEMICOLON)
		{
			assign_stmt->op = 0;
			return assign_stmt;
		}


	}
	
	return assign_stmt;
}

struct varNode* parse_var(){
	struct varNode* var;
	
	var = make_VarNode();
	var->name = token;
	var->value = 0; // all variables are initialized to 0.
	
	return var;
}

struct statementNode* parse_id_list(){
	struct statementNode* id_list;

	return id_list;
}

struct statementNode* parse_body(){
	struct statementNode* body;

	return body;

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
struct statementNode* make_statementNode(){        
        return (struct statementNode*) malloc(sizeof(struct statementNode));
}

struct assignmentStatement* make_assignmentStatement(){        
        return (struct assignmentStatement*) malloc(sizeof(struct assignmentStatement));
}

struct varNode* make_varNode(){        
        return (struct varNode*) malloc(sizeof(struct varNode));
}





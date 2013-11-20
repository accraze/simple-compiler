#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"





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


// entrance point from sample code:
struct statementNode* parse_program_and_generate_intermediate_representation(){
	struct statementNode* program;
	init_flag = TRUE; // variables can initialized!!
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
			prog->next = parse_body();
		}
	} else
	if (ttype == LBRACE)
	{
		//body section
		prog->next = parse_body();
	}else{
		//error?
	}

	return prog;
}


struct assignmentStatement* parse_assign_stmt(){
	struct assignmentStatement* assign_stmt;
	ttype = getToken();
	
	if (ttype == ID)
	{
		assign_stmt = make_assignmentStatement();
		assign_stmt->op1 = parse_var();
		ttype = getToken();
		if ( ttype == SEMICOLON || ttype == EQUAL)
		{
			ungetToken();
			assign_stmt->op = 0;
			assign_stmt->lhs = assign_stmt->op1;
			return assign_stmt;
		}


	} else
	if (ttype == NUM)
	{
		ungetToken();
		assign_stmt = make_assignmentStatement();
		assign_stmt->op1 = parse_var(ttype);
	}
	
	return assign_stmt;
}

struct varNode* parse_var(){
	struct varNode* var;
	
	var = make_varNode();
	
	
	if(init_flag == TRUE){
		var->name = token;
		var->value = 0; // all variables are initialized to 0.
	} else {
		
		if (ttype == NUM)
		{
			var->name = token;
			var->value = atoi(token); //For literals (NUM), the value is the value of the number.
		}
	}

	return var;
}

// struct statementNode* parse_id_list(){
// 	struct statementNode* id_list;

// 	return id_list;
// }

struct statementNode* parse_body(){
	struct statementNode* body;
	// make sure that we have a body open brace
	if (ttype == LBRACE)
	{
		init_flag = FALSE;
		body = parse_stmt_list(); 
		ttype = getToken();
        if (ttype == RBRACE)
                return body;
	}

	return body;

}

struct statementNode* parse_stmt_list(){
	struct statementNode* stmt_list;
	ttype = getToken();
	if (ttype == ID || ttype == NUM)
	{
		stmt_list = parse_stmt();

	}

	return stmt_list;
}


struct statementNode* parse_stmt(){
	struct statementNode* stmt;
		 if (ttype == ID)
		 {
		 	ungetToken();
		 	stmt = make_statementNode();
		 	stmt->stmt_type = ASSIGNSTMT;
		 	stmt->assign_stmt = parse_assign_stmt();
		 	ttype = getToken();
		 	if (ttype == SEMICOLON)
		 	{	
		 		ttype = getToken();
		 		if (ttype == ID || ttype == PRINT)
		 		{
		 			ungetToken();
		 			stmt->next = parse_stmt();
		 		} else
		 		if (ttype == RBRACE)
		 		{
		 			ungetToken();
		 			stmt->next = NULL;
		 		}
		 		
		 		// now assign the rhs value to lhs
		 		stmt->assign_stmt->op1->value = stmt->next->assign_stmt->op1->value;
				return stmt;

		 	} 
	return stmt;
	}
}







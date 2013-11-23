#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"

#define MAX 20
struct varNode* var_store[MAX];
int var_total = 0;


struct statementNode* var_lookup(char* token){
	int i;
	struct varNode* var;
	for (i = 0; i < var_total; ++i)
	{
		if (strcmp(var_store[i]->id, token))
		{
			var = var_store[i];
			return var;
		}
	}

}



//-----------------------------------------------------------------------------
// Parser Memory Build
//-----------------------------------------------------------------------------
struct statementNode* make_statementNode(){        
        return (struct statementNode*) malloc(sizeof(struct statementNode));
}

struct assignmentStatement* make_assignmentStatement(){        
        return (struct assignmentStatement*) malloc(sizeof(struct assignmentStatement));
}

struct printStatement* make_printStatement(){        
        return (struct printStatement*) malloc(sizeof(struct printStatement));
}

struct varNode* make_varNode(){        
        return (struct varNode*) malloc(sizeof(struct varNode));
}

void init_var_store(){
	int i;
	for (i = 0; i < MAX; ++i)
	{
		var_store[i] = (struct varNode*)malloc(sizeof(struct varNode));
		var_store[i]->id = (char*)malloc(sizeof(char) *MAX_TOKEN_LENGTH);
	}
	//debug
	//printf("all done!!");
}


//-----------------------------------------------------------------------------
// Parsing Functions
//-----------------------------------------------------------------------------

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

void parse_var_section(){
	init_var_store(); // set up var store data structure
	ttype = getToken();
	if (ttype == ID)
	{
		ungetToken();
		parse_id_list();
		ttype = getToken();
		if (ttype == SEMICOLON)
		{
			//test
			//printf("all clear!!\n");
		}
	}
}

void parse_id_list(){
	ttype = getToken();
	if (ttype == ID)
	{
		strcpy(var_store[var_total]->id, token);
		var_store[var_total]->value = 0; // always init to zero!!
		var_total++;
	}

	ttype = getToken();
	if (ttype == COMMA)
	{
		parse_id_list(); // call it again
	} else
	if (ttype == SEMICOLON)
	{
		ungetToken();
		//debug print
		printf("id list complete\n");
	}
}


struct assignmentStatement* parse_assign_stmt(){
	struct assignmentStatement* assign_stmt;
	ttype = getToken();
	
	if (ttype == ID)
	{
		assign_stmt = make_assignmentStatement();
		assign_stmt->op1 = parse_var();
		ttype = getToken();
		if (ttype == EQUAL)
		{
			// parse expr 
			// op1 op op2
			ttype = getToken();
			if (ttype == SEMICOLON)
			{
				ungetToken();
				return assign_stmt;
			} else
			if (ttype == ID || ttype == NUM)
			 {
			 	// expr
			 	struct assignmentStatement* expr;

			 } 
			
		} else
		if (ttype == SEMICOLON)
		{
			// declaration case
			assign_stmt->op = 0;
			assign_stmt->lhs = assign_stmt->op1;
		}


	} else
	if (ttype == NUM)
	{
		ungetToken();
		assign_stmt = make_assignmentStatement();
		assign_stmt->op1 = parse_var();
	}
	
	return assign_stmt;
}

struct printStatement* parse_print_stmt(){
	struct printStatement* print_stmt;
	ttype = getToken();
	if (ttype == PRINT)
	{
		print_stmt = make_printStatement();
		ttype = getToken();
		if (ttype == ID)
		{
			print_stmt->id = parse_var();
		}
	}
	return print_stmt;
}

struct varNode* parse_var(){
	struct varNode* var;
	
	var = make_varNode();
	
	
	if(init_flag == TRUE){
		strcpy(var->name, token);
		var->value = 0; // all variables are initialized to 0.
	} else {
		
		if (ttype == ID)
		{
			strcpy(var->name, token);
		}
		if (ttype == NUM)
		{
			strcpy(var->name, token);
			var->value = atoi(token); //For literals (NUM), the value is the value of the number.
		}
	}

	return var;
}

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
	 		//stmt->assign_stmt->op1->value = stmt->next->assign_stmt->op1->value;
			//return stmt;

	 	} 
		return stmt;
	} else 
	if (ttype == PRINT)
	{
		ungetToken();
		stmt->print_stmt = make_printStatement();
		stmt->stmt_type = PRINTSTMT;
		stmt->print_stmt = parse_print_stmt();
		ttype = getToken();
		if (ttype == SEMICOLON)
		{
			ttype = getToken();
			if (ttype == ID || ttype == PRINT)
			{
				ungetToken();
				stmt->next = parse_stmt();
			} else 
			if (ttype == RBRACE){
				ungetToken();
				stmt->next = NULL;
			}
		}
	}
}







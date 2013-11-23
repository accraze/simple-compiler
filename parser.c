#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"

//------------------------------\
//       Global Decl
//_______________________________\

struct varNode* var_store[MAX];
int var_total = 0;


// looks up previously declared variables
// and returns varNode that matches the token
struct varNode* var_lookup(char* token){
	int i;
	struct varNode* var;
	for (i = 0; i < var_total; ++i)
	{
		// printf("iter# %d = %s\n", i, var_store[i]->name);
		// printf("token = %s\n", token );
		if (strcmp(var_store[i]->name, token)==0)
		{
			//printf("WASSUP!!\n");
			var = var_store[i];
			return var;
		}
	}
	printf("WE CANT FIND SHIT!!\n" );
}

void debug_print_var_store(){
	int i;
	for (i = 0; i < var_total; ++i)
		{
			printf("Name= %s  ",var_store[i]->name);
			printf("Value= %d\n", var_store[i]->value);
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
		var_store[i]->name = (char*)malloc(sizeof(char) *MAX_TOKEN_LENGTH);
	}
	//debug
	//printf("init store all done!!");
}


//-----------------------------------------------------------------------------
// Parsing Functions
//-----------------------------------------------------------------------------

// entrance point from sample code:
struct statementNode* parse_program_and_generate_intermediate_representation(){
	struct statementNode* program;
	init_flag = TRUE; // variables can initialized!!
	//printf("HELLO!!!!!!\n");

	ttype=getToken();
	//printf("%s\n", token);
	if (ttype == ID)
	{
		ungetToken();
		parse_var_section();
		ttype = getToken();
		if (ttype == LBRACE)
		{
			ungetToken();
			program = parse_body();

		}
	} else
	if (ttype == LBRACE)
	{
		ungetToken();
		program = parse_body();
	}
	else{
		// error?
	}
	return program;
}

// struct statementNode* parse_program(){
// 	struct statementNode* prog;
// 	prog = make_statementNode();
// 	ttype = getToken();
// 	if (ttype == ID)
// 	{
// 		//var section
// 		ungetToken();
// 		parse_var_section();
// 		ttype = getToken();
// 		if (ttype == LBRACE)
// 		{
// 			prog->next = parse_body();
// 		}
// 	} else
// 	if (ttype == LBRACE)
// 	{
// 		//body section
// 		prog->next = parse_body();
// 	}else{
// 		//error?
// 	}

// 	return prog;
// }

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
			//debug_print_var_store();
		}
	}
}

void parse_id_list(){
	ttype = getToken();
	if (ttype == ID)
	{
		strcpy(var_store[var_total]->name, token);
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
		//printf("id list complete\n");
	}
}


struct assignmentStatement* parse_assign_stmt(){
	struct assignmentStatement* assign_stmt;
	ttype = getToken();
	
	if (ttype == ID)
	{
		assign_stmt = make_assignmentStatement();
		assign_stmt->lhs = var_lookup(token);
		// printf("HELLO\n");
		// printf("LHS NAME%s\n", assign_stmt->lhs->name);
		// printf("LHS VALUE%d\n", assign_stmt->lhs->value);
		ttype = getToken();
		if (ttype == EQUAL)
		{
			// parse expr 
			// op1 op op2

			ttype = getToken();
			//printf("token =%s\n", token );
			// if (ttype == SEMICOLON)
			// {
			// 	ungetToken();
			// 	return assign_stmt;
			// } else
			if (ttype == ID || ttype == NUM)
			 {
			 	// expr
			 	assign_stmt->op1 = parse_var();
			 	ttype = getToken();
			 	
			 	// only one operand case....
			 	if (ttype == SEMICOLON)
			 	{
			 		assign_stmt->op = 0;
			 		assign_stmt->lhs->value = assign_stmt->op1->value;
			 		update_var_store(assign_stmt->lhs->name, assign_stmt->lhs->value);
			 		//debug_print_var_store();
			 		// printf("ASSIGN STMT LHS NAME= %s", assign_stmt->lhs->name);
			 		// printf("ASSIGN STMT LHS Value= %d", assign_stmt->lhs->value);
			 	}
			 } 	
		} 
	} 
	return assign_stmt;
}

void update_var_store(char* token, int new_value){
	int i;
	for (i = 0; i < var_total; ++i)
	{
		// printf("iter# %d = %s\n", i, var_store[i]->name);
		// printf("token = %s\n", token );
		if (strcmp(var_store[i]->name, token)==0){
			var_store[i]->value = new_value;
			//printf("UPDATED!\n");
			//printf("%d\n",var_store[i]->value );
		}
	}		

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
			
			print_stmt->id = var_lookup(token);
		}
	}
	return print_stmt;
}

struct varNode* parse_var(){
	struct varNode* var;
	
	//make space for var node
	var = make_varNode();
	// make space for char name
	var->name = (char*)malloc(sizeof(char) *MAX_TOKEN_LENGTH);
		
		// if (ttype == ID)
		// 	strcpy(var->name, token);
		if (ttype == NUM)
		{
			//printf("yep its a num = %s \n", token );
			strcpy(var->name, token);
			//printf("var name%s\n", var->name );
			var->value = atoi(token); //For literals (NUM), the value is the value of the number.
		}

	return var;
}

struct statementNode* parse_body(){
	struct statementNode* body;
	// make sure that we have a body open brace
	ttype = getToken();
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
	 	//printf("assigned %s %d\n",stmt->assign_stmt->lhs->name, stmt->assign_stmt->lhs->value );
	 	ttype = getToken();
	 	//printf("token = %s\n", token );
	 	if (ttype == SEMICOLON)
	 	{	
	 		//printf("WUNDERBAR\n");
	 		ttype = getToken();
	 		if (ttype == ID || ttype == PRINT)
	 		{
	 			//ungetToken();
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







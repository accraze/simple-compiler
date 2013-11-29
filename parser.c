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
struct varNode* var_lookup(char* token1){
	int i;
	struct varNode* var;
	for (i = 0; i < var_total; ++i)
	{
		// //printf("iter# %d = %s\n", i, var_store[i]->name);
		// //printf("token = %s\n", token );
		if (strcmp(var_store[i]->name, token1)==0)
		{
			////printf("WASSUP!!\n");
			var = var_store[i];
			return var;
		}
	}
	////printf("WE CANT FIND SHIT!!\n" );
}

void debug_print_var_store(){
	int i;
	for (i = 0; i < var_total; ++i)
		{
			//printf("Name= %s  ",var_store[i]->name);
			//printf("Value= %d\n", var_store[i]->value);
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

struct ifStatement* make_ifStatement(){        
        return (struct ifStatement*) malloc(sizeof(struct ifStatement));
}

void init_var_store(){
	int i;
	for (i = 0; i < MAX; ++i)
	{
		var_store[i] = (struct varNode*)malloc(sizeof(struct varNode));
		var_store[i]->name = (char*)malloc(sizeof(char) *MAX_TOKEN_LENGTH);
	}
	//debug
	////printf("init store all done!!");
}


//-----------------------------------------------------------------------------
// Parsing Functions
//-----------------------------------------------------------------------------

// entrance point from sample code:
struct statementNode* parse_program_and_generate_intermediate_representation(){
	struct statementNode* program;
	init_flag = TRUE; // variables can initialized!!
	////printf("HELLO!!!!!!\n");

	ttype=getToken();
	////printf("%s\n", token);
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
			////printf("all clear!!\n");
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
		////printf("id list complete\n");
	}
}


struct assignmentStatement* parse_assign_stmt(){
	struct assignmentStatement* assign_stmt;
	int sum = 0;
	int diff = 0;
	int product = 0;
	int quotient = 0;
	////printf("HELLOOOOOO3\n");
	ttype = getToken();
	if (ttype == ID)
	{
		assign_stmt = make_assignmentStatement();
		assign_stmt->lhs = var_lookup(token);
		// //printf("HELLO\n");
		// //printf("LHS NAME%s\n", assign_stmt->lhs->name);
		// //printf("LHS VALUE%d\n", assign_stmt->lhs->value);
		ttype = getToken();
		if (ttype == EQUAL)
		{
			// parse expr 
			// op1 op op2

			ttype = getToken();
			////printf("token =%s\n", token );
			// if (ttype == SEMICOLON)
			// {
			// 	ungetToken();
			// 	return assign_stmt;
			// } else
			if (ttype == ID || ttype == NUM)
			 {
			 	if (ttype == ID)
			 	{
			 		assign_stmt->op1 = var_lookup(token);
			 	} else
			 	if (ttype == NUM)
			 	{
			 		ungetToken();
			 		assign_stmt->op1 = parse_var();
			 	}
			 	
			 	ttype = getToken();
			 	
			 	// only one operand case....
			 	if (ttype == SEMICOLON)
			 	{
			 		ungetToken();
			 		assign_stmt->op = 0;
			 		assign_stmt->lhs->value = assign_stmt->op1->value;
			 		update_var_store(assign_stmt->lhs->name, assign_stmt->lhs->value);
			 		//debug_print_var_store();
			 		// //printf("ASSIGN STMT LHS NAME= %s", assign_stmt->lhs->name);
			 		// //printf("ASSIGN STMT LHS Value= %d", assign_stmt->lhs->value);
			 	} else
			 	if (ttype == PLUS || ttype == MINUS || ttype == MULT || ttype == DIV)
			 	{
			 		assign_stmt->op = ttype;
			 		ttype = getToken();
			 		if (ttype == ID || ttype == NUM)
			 		{
			 			if (ttype == ID)
			 			{
			 				//ungetToken();
			 				assign_stmt->op2 = var_lookup(token);
			 			} else 
			 			if (ttype == NUM)
			 			{
			 				ungetToken();
			 				assign_stmt->op2 = parse_var();
			 			}
			 			ttype = getToken();
			 			if (ttype == SEMICOLON)
			 			{
			 				ungetToken();
			 				switch (assign_stmt->op)
			 				{
			 					case PLUS:
			 						sum = assign_stmt->op1->value + assign_stmt->op2->value;
			 						assign_stmt->lhs->value = sum;
			 						update_var_store(assign_stmt->lhs->name, assign_stmt->lhs->value);
			 						break;
			 					case MINUS:
			 						
			 						diff = assign_stmt->op1->value - assign_stmt->op2->value;
			 						assign_stmt->lhs->value = diff;
			 						update_var_store(assign_stmt->lhs->name, assign_stmt->lhs->value);
			 						break;
			 					case MULT:
			 						
			 						product = assign_stmt->op1->value * assign_stmt->op2->value;
			 						assign_stmt->lhs->value = product;
			 						update_var_store(assign_stmt->lhs->name, assign_stmt->lhs->value);
			 						break;
			 					case DIV:
			 						
			 						sum = assign_stmt->op1->value / assign_stmt->op2->value;
			 						assign_stmt->lhs->value = quotient;
			 						update_var_store(assign_stmt->lhs->name, assign_stmt->lhs->value);
			 						break;
			 					default:
			 						//printf("ERROR! Unknown Operator in Assignment Statement\n");
			 						break;

			 				}
			 				return assign_stmt;
			 			}
			 		}
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
		// //printf("iter# %d = %s\n", i, var_store[i]->name);
		// //printf("token = %s\n", token );
		if (strcmp(var_store[i]->name, token)==0){
			var_store[i]->value = new_value;
			////printf("UPDATED!\n");
			////printf("%d\n",var_store[i]->value );
		}
	}		

}

struct ifStatement* parse_if_stmt(){
	struct ifStatement* if_stmt;
	//struct statementNode* noop;
		ttype = getToken();
		//ttype = getToken();
		if(ttype == IF){
			ttype = getToken();
		if_stmt = make_ifStatement();
		//printf("Parsing if stmt.....\n");
		//printf("%s\n", token );
		if (ttype == ID || ttype == NUM)
		{

			//ungetToken();
			// nows its a condition
			// add condition code here
			// ok?
			if (ttype == ID)
			{
				//if_stmt->op1 = make_varNode();
				//printf("heres the token op1 = %s\n", token);
				if_stmt->op1 = var_lookup(token);
			} else
			if (ttype == NUM)
			{
				ungetToken();
				//if_stmt->op1 = make_varNode();
				if_stmt->op1 = parse_var();

			}
			ttype = getToken();
			if (ttype == GREATER || ttype == NOTEQUAL || ttype ==LESS)
			{
				if_stmt->relop = ttype;
				ttype = getToken();
				if (ttype == ID || ttype == NUM)
				{
					if (ttype == ID )
					{
						//if_stmt->op2 = make_varNode();
						if_stmt->op2 = var_lookup(token);
					} else 
					if (ttype == NUM)
					{
						ungetToken();
						//if_stmt->op2 = make_varNode();
						if_stmt->op2 = parse_var();
					}
				}
				ttype = getToken();
				//printf("here's yer token = %d \n", ttype);
				if(ttype == LBRACE){
					//ungetToken();
					
					// get the body
					
					if_stmt->true_branch = parse_stmt();
					if(if_stmt->true_branch == NULL)
						//printf("OH FUCK!!!\n");
					//printf("if_stmt->true type %d\n", if_stmt->true_branch->stmt_type);
					ttype = getToken();
					//printf("here's yer token = %d \n", ttype);
					if(ttype == RBRACE){
						//printf("done parsing if sTMT!!!\n");
						return if_stmt;
					}

				} 
			 }
		}
	}
	return if_stmt;
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
	ttype = getToken();
	//make space for var node
	var = make_varNode();
	// make space for char name
	var->name = (char*)malloc(sizeof(char) *MAX_TOKEN_LENGTH);
		
		// if (ttype == ID)
		// 	strcpy(var->name, token);
		if (ttype == NUM)
		{
			////printf("yep its a num = %s \n", token );
			strcpy(var->name, token);
			////printf("var name%s\n", var->name );
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
	////printf("HELLOOOOOOO2\n");
	ttype = getToken();
	if (ttype == ID || ttype == NUM)
	{
		ungetToken();
		stmt_list = parse_stmt();

	}

	return stmt_list;
}


struct statementNode* parse_stmt(){
	 struct statementNode* stmt;
	 struct statementNode* noop;
	 stmt = make_statementNode();
	//printf("parsing new stmt\n");
	ttype = getToken();
	//printf("ttype = %d\n", ttype );
	if (ttype == ID)
	{
	 	ungetToken();
	 	//printf("parsing Assign Statement\n");
	 	stmt->stmt_type = ASSIGNSTMT;
	 	stmt->assign_stmt = parse_assign_stmt();
	 	////printf("assigned %s %d\n",stmt->assign_stmt->lhs->name, stmt->assign_stmt->lhs->value );
	 	ttype = getToken();
	 	//debug_print_var_store();
	 	////printf("token = %s\n", token );
	 	if (ttype == SEMICOLON)
	 	{	
	 		////printf("WUNDERBAR\n");
	 		ttype = getToken();
	 		if (ttype == ID || ttype == PRINT || ttype == IF || ttype == WHILE)
	 		{
	 			ungetToken();
	 			stmt->next = parse_stmt();
	 		} else
	 		if (ttype == RBRACE)
	 		{
	 			ungetToken();
	 			stmt->next = NULL;
	 		}
	 		
	 		//printf("Returning Assign Statement\n");
			return stmt;
	 	}

	} 
	else 
	if (ttype == PRINT)
	{
		ungetToken();
		//printf("WE HAVE A PRINT STM\n");
		stmt->print_stmt = make_printStatement();
		stmt->stmt_type = PRINTSTMT;
		stmt->print_stmt = parse_print_stmt();
		//printf("print var %s\n", stmt->print_stmt->id->name );
		ttype = getToken();
		if (ttype == SEMICOLON)
		{
			//printf("have another??\n");
			ttype = getToken();
			//printf("ttype %d\n", ttype);
			//printf("token %s\n", token );
			if (ttype == ID || ttype == PRINT)
			{
				ungetToken();
				stmt->next = parse_stmt();
			} else 
			if (ttype == RBRACE){
				ungetToken();
				//printf("Returning Print Statement\n");
				stmt->next = NULL;
				return stmt;
			}
		}
	}
	else
	if (ttype == IF)
	{
		//printf("IF detected.... now make stmt type IFSTMT\n");
		ungetToken();
		stmt->if_stmt = make_ifStatement();
		stmt->stmt_type = IFSTMT;
		stmt->if_stmt = parse_if_stmt();
		//stmt->if_stmt->true_branch = parse_body();
		noop = make_statementNode();
		noop->stmt_type = NOOPSTMT;
		
		
			if(stmt->if_stmt->true_branch->next == NULL){
				//printf("STMT->NEXT IS NULL! pre assign NOOP\n");
				stmt->if_stmt->true_branch->next = noop;
			}
		
		stmt->if_stmt->false_branch = noop;
		ttype = getToken();
		if (ttype == ID || ttype == PRINT || ttype == IF || ttype == WHILE)
		{
			ungetToken();
			stmt->next = noop;
			////printf(" next stmt type !! %d\n", stmt->next->stmt_type );
			noop->next=parse_stmt();
		 } 
		 else
		if (ttype == RBRACE)
		{
			ungetToken();
			stmt->next = NULL;
			stmt->next = noop;
			////printf(" next stmt type !! %d\n", stmt->next->stmt_type );
			noop->next= NULL;
		}
		////printf("Returning If Statement\n");
		return stmt;
	 } else
	 if(ttype == NULL){

	 }
	// //printf("Exiting Statement Node\n");
	return stmt;
}







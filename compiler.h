#ifndef _COMPILER_H_
#define _COMPILER_H_

/*
 * compiler.h
 * 
 */
 
#define TRUE 1
#define FALSE 0

#define NOOPSTMT	100
#define PRINTSTMT	101
#define ASSIGNSTMT	102
#define IFSTMT		103		// This is used for all control statements (if, while, repeat)
#define GOTOSTMT	104

#define KEYWORDS	7
#define RESERVED	27
#define VAR			1
#define IF			2
#define WHILE		3 
#define REPEAT		4
#define UNTIL 		5
#define PRINT 		6
#define ARRAY 		7
#define PLUS 		8
#define MINUS 		9
#define DIV 		10
#define MULT 		11
#define EQUAL 		12
#define COLON 		13
#define COMMA 		14
#define SEMICOLON 	15
#define LBRAC 		16
#define RBRAC 		17
#define LPAREN 		18
#define RPAREN 		19
#define LBRACE 		20
#define RBRACE 		21
#define NOTEQUAL 	22
#define GREATER 	23
#define LESS 		24
#define ID 			25
#define NUM 		26
#define ERROR 		27

// This implementation does not allow tokens that are more
// than 100 characters long
#define MAX_TOKEN_LENGTH 100

// The following global variables are defined in compiler.c:
extern char token[MAX_TOKEN_LENGTH];
extern int  ttype;

//---------------------------------------------------------
// Data structures:

struct varNode
{
	char* name;
	int   value;
};

struct gotoStatement
{
	struct statementNode * target;
};

struct assignmentStatement
{
	struct varNode * lhs;
	struct varNode * op1;
	struct varNode * op2;
	int op;		// PLUS, MINUS, MULT, DIV --> lhs = op1 op op2;
				// 0                      --> lhs = op1;
};

struct printStatement
{
	struct varNode * id;
};

struct ifStatement
{
	struct varNode * op1;
	struct varNode * op2;
	int relop;	// GREATER, LESS, NOTEQUAL
	struct statementNode * true_branch;
	struct statementNode * false_branch;
};

struct statementNode
{
	int stmt_type;		// NOOPSTMT, PRINTSTMT, ASSIGNSTMT, IFSTMT, GOTOSTMT
	struct assignmentStatement * assign_stmt;	// NOT NULL iff stmt_type == ASSIGNSTMT
	struct printStatement      * print_stmt;	// NOT NULL iff stmt_type == PRINTSTMT
	struct ifStatement         * if_stmt;		// NOT NULL iff stmt_type == IFSTMT
	struct gotoStatement       * goto_stmt;		// NOT NULL iff stmt_type == GOTOSTMT
	struct statementNode       * next;			// next statement in the list or NULL 
};

//---------------------------------------------------------
// Functions that are provided:

void print_debug(const char * format, ...);
void ungetToken();
int  getToken();

/*
 * You need to write a function with the signature that follows this comment. 
 * It is supposed to parse the input program and generate an intermediate 
 * representation for it. Write all your code in a separate file and include 
 * this header file.
 *
 * If you write your code in C, compile using the following command:
 *
 *   $ gcc compiler.c yourcode.c
 *
 * In case you use C++, use the following commands:
 *
 *   $ gcc -c compiler.c
 *   $ g++ yourcode.cpp compiler.o
 *
 * And also make sure that you include this header file in the following way in
 * your C++ code:
 *
 *  extern "C" {
 *  	#include "compiler.h"
 *  }
 *
 */
struct statementNode * parse_program_and_generate_intermediate_representation();

#endif /* _COMPILER_H_ */

#ifndef _PARSER_H_
#define _PARSER_H_

// Parsing Functions
struct statementNode* parse_program();
struct statementNode* parse_id_list();
struct statementNode* parse_body();
struct assignmentStatement* parse_assign_stmt();
struct varNode* parse_var();


// Memory Management Functions
struct varNode* make_varNode();
struct statementNode* make_statementNode();
struct assignmentStatement* make_assignmentStatement();



#endif /* _PARSER_H_ */

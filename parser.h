#ifndef _PARSER_H_
#define _PARSER_H_


var init_flag;

// Parsing Functions
struct statementNode* parse_program();
struct statementNode* parse_id_list();
struct statementNode* parse_body();
struct statementNode* parse_stmt_list();

struct assignmentStatement* parse_assign_stmt();
struct varNode* parse_var();


// Memory Management Functions
struct varNode* make_varNode(ttype);
struct statementNode* make_statementNode();
struct assignmentStatement* make_assignmentStatement();



#endif /* _PARSER_H_ */

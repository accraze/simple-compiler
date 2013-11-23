#ifndef _PARSER_H_
#define _PARSER_H_


int init_flag;

// Parsing Functions
struct statementNode* parse_program();
struct statementNode* parse_id_list();
struct statementNode* parse_body();
struct statementNode* parse_stmt_list();
struct statementNode* parse_stmt();

struct assignmentStatement* parse_assign_stmt();
struct statementNode* var_lookup(char* token);
struct varNode* parse_var();


// Memory Management Functions
void init_var_store();
struct varNode* make_varNode();
struct statementNode* make_statementNode();
struct printStatement* make_printStatement();
struct assignmentStatement* make_assignmentStatement();




#endif /* _PARSER_H_ */

#ifndef _PARSER_H_
#define _PARSER_H_

#define MAX 20

int init_flag;

// Parsing Functions

void parse_var_section();
void parse_id_list();
struct statementNode* parse_stmt();
struct statementNode* parse_body();
struct statementNode* parse_program();
struct statementNode* parse_stmt_list();

struct ifStatement* parse_if_stmt()
struct assignmentStatement* parse_assign_stmt();
struct varNode* var_lookup(char* token);
struct varNode* parse_var();


// Memory Management Functions
void init_var_store();
struct varNode* make_varNode();
struct ifStatement* make_ifStatement();
struct statementNode* make_statementNode();
struct printStatement* make_printStatement();
struct assignmentStatement* make_assignmentStatement();


void debug_print_var_store();
void update_var_store(char* token, int new_value);


#endif /* _PARSER_H_ */

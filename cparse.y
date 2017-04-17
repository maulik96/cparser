%{
#include <bits/stdc++.h>
#include "header.h"

using namespace std;

typedef node * node_pointer;

#define YYSTYPE node_pointer

%}

%define parse.error verbose

%token IF ELSE WHILE DO FOR RETURN CONTINUE BREAK STRUCT STRING_LITERAL CHAR_LITERAL DATA_TYPE INT_LITERAL FLOAT_LITERAL IDENTIFIER ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN RIGHT_OP LEFT_OP INC_OP DEC_OP REL_OP MATH_OP EQUAL_OP

%start start

%%

start 	: strt_ 	{ $$ = make_node(1,$1);}
		;

strt_	: 							{ $$ = make_node(0);}
		| ext_declaration strt_ 	{ $$ = make_node(2,1,$2);}

ext_declaration : function_definition			{ $$ = make_node(1,$1);}
				| function_declaration			{ $$ = make_node(1,$1);}
				| declaration_stmt			{ $$ = make_node(1,$1);}
				| struct_decl			{ $$ = make_node(1,$1);}
				;

function_definition : 	data_type id '(' arg_list ')' '{'	{	
																mss temp;
																symbolTable.push_back(temp);	
																if(!isPresent($2->label))
																{
																	function_dt *temp = new function_dt;
																	temp->type = $1->label;
																	temp->v = giveArgList($4);
																	functionTable[$2->label] = *temp;
																	addListToSymTable($4);
																}
																else
																	printMultiDeclMsg($2->label);
															} 
 						stmt_list							{$$ = make_node(4,$1,$2,$4,$8);}
						'}'									{symbolTable.pop_back();addToSymTable($2->label, $1->label);}
					;

function_declaration: data_type id '(' arg_list ')' ';' { 
															if(!isPresent($2->label))
															{
																function_dt *temp = new function_dt;
																temp->type = $1->label;
																temp->v = giveArgList($4);
																functionTable[$2->label] = *temp;
																addToSymTable($2->label, $1->label);
																$$ = make_node(3,$1,$2,$4);
															}
															else
																printMultiDeclMsg($2->label);
														}
					;

struct_decl	: 	STRUCT id '{' 		{	
										mss temp;
										symbolTable.push_back(temp);	
									} 

				declaration_list 	{
										if(!isPresent($2->label))
										{
											struct_dt *temp = new struct_dt;
											temp->v = getAllVarsCurrentScope();
											structTable[$2->label] = *temp;
											$$ = make_node(2,$2,$5);			
										}
										else
											printMultiDeclMsg($2->label);
									}
				'}' ';'             {symbolTable.pop_back();addToSymTable($2->label, "struct");}
			;

declaration_list	: declaration_stmt						{$$ = make_node(1, $1);}
					| declaration_stmt declaration_list		{$$ = make_node(2, $1, $2);}
					;

id_list	: id 				{$$ = make_node(1, $1);}
		| id ',' id_list	{$$ = make_node(2, $1, $3);}
		;

arg_list	: 								{$$ = make_node(0);}
			| data_type id 					{$$ = make_node(2, $1, $2);}
			| data_type id ',' arg_list 	{$$ = make_node(3, $1,$2,$4);}

declaration_stmt	: data_type id_list ';' {
												vs list = getIdList($2);
												for(vsi it=list.begin();it!=list.end();it++)
												{
													if(isPresentCurrentScope(*it))
														printMultiDeclMsg(*it);
													else
														addToSymTable(*it, $1->label);
												}
												$$ = make_node(2, $1, $2);
											}
					;

compound_stmt	: '{'			{	
									mss temp;
									symbolTable.push_back(temp);	
								} 
				stmt_list '}'	{ 
									symbolTable.pop_back();
									$$ = make_node(1,$2);
								}	
				;

stmt_list	: 					{ $$ = make_node(0); }
			| stmt stmt_list	{ $$ = make_node(2,$1,$2); }
			;

stmt 	: declaration_stmt		{ $$ = make_node(1,$1); }
		| compound_stmt			{ $$ = make_node(1,$1); }
		| loop_stmt				{ $$ = make_node(1,$1); }
		| if_stmt				{ $$ = make_node(1,$1); }
		| expr_stmt				{ $$ = make_node(1,$1); }
		| error				
		;

loop_stmt	: WHILE '(' expr ')' stmt 	{ $$ = make_node(2,$2,$3); }
			;

if_stmt	: IF '(' expr ')' stmt 				{ $$ = make_node(2,$3,$5); }
		| IF '(' expr ')' stmt ELSE stmt 	{ $$ = make_node(3,$3,$5,$7); }
		;

expr_stmt	: expr ';'		{ $$ = make_node(1,$1); }
			;

expr 	: INT_LITERAL				{$$ = make_node(1, make_terminal_node(string(yytext, yyleng), int_node)); $$->data_type = "int";}
		| CHAR_LITERAL				{$$ = make_node(1, make_terminal_node(string(yytext, yyleng), char_node)); $$->data_type = "char";}	
		| FLOAT_LITERAL				{$$ = make_node(1, make_terminal_node(string(yytext, yyleng), float_node)); $$->data_type = "float";}
		| id 						{ 
										if(!isPresent($1->label))
											printUndefinedMsg($1->label);
										else
										{
											$$ = make_node(1,$1); 
											$$->data_type = $1->data_type;
										}
									}
		| id '.' id 				{
										if(!isPresent($1->label))
											printUndefinedMsg($1->label);
										else if(structTable.find(getDataType($1->label)) == structTable.end())
											printNotMember($1->label, $3->label);
										else
										{
											mss m = ((structTable.find(getDataType($1->label)))->second).v;
											mssi it = m.find($3->label);
											if(it != m.end())
											{
												$$ = make_node(2,$1,$3);
												$$->data_type = it->second;
											}
											else
												printNotMember($1->label, $3->label);
										}
									}
		| expr op expr 				{
										$$ = make_node(3,$1,$2,$3); 
										$$->data_type = "int";
										if(!similarDataType($1, $3))
											printDtMismatch();
									}
		| id '(' param_list ')' 	{ 
										if(!isPresent($1->label))
											printUndefinedMsg($1->label);
										else if(similarArgs($1, $3))
										{
											$$ = make_node(2,$1,$3);
											$$->data_type = $1->data_type;
										}
										else
											printArgsMismatch($1->label);
									}
		;

op: 	  EQUAL_OP  {$$ = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}
		| REL_OP 	{$$ = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}
		| MATH_OP	{$$ = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}	
		;
						
id 	: IDENTIFIER 	{ $$ = make_terminal_node(string(yytext, yyleng), id_node); $$->data_type = getDataType(string(yytext, yyleng));}
	; 


data_type 	: DATA_TYPE {$$ = make_terminal_node(string(yytext, yyleng), dt_node);}
			| STRUCT id {
							if(!isPresent($2->label) || getDataType($2->label)!="struct")
								printUndefinedMsg("struct "+$2->label);
							$$ = make_terminal_node($2->label, dt_node);
						}
			; 

param_list	: 
			| expr 					{ $$ = make_node(1,$1);}
			| expr ',' param_list	{ $$ = make_node(2,$1,$3);}

%%

int main(void)
{
	map<string, string> temp;
	symbolTable.push_back(temp);
    return yyparse();
}

void yyerror(const char *s)
{
    fprintf(stderr, "%s at line no %d\n", s, yylineno);
}
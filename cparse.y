%{
#include <bits/stdc++.h>
#include "header.h"

using namespace std;

typedef node * node_pointer;

#define YYSTYPE node_pointer


%}

%define parse.error verbose

%token IF ELSE WHILE DO FOR RETURN CONTINUE BREAK STRUCT STRING_LITERAL CHAR_LITERAL DATA_TYPE INT_LITERAL FLOAT_LITERAL IDENTIFIER ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN RIGHT_OP LEFT_OP INC_OP DEC_OP REL_OP MATH_OP EQUAL_OP LOGICAL_OP BIT_OP

%start start

%%

start 	: strt_ 	{ $$ = make_node(1,$1);root=$$;}
		;

strt_	: 							{ $$ = make_node(0);}
		| ext_declaration strt_ 	{ $$ = make_node(2,$1,$2);}

ext_declaration : function_definition			{ $$ = make_node(1,$1);}
				| function_declaration			{ $$ = make_node(1,$1);}
				| declaration_stmt				{ $$ = make_node(1,$1);}
				| struct_decl					{ $$ = make_node(1,$1);}
				| expr_stmt						{ $$ = make_node(1,$1);}
				;

function_definition : 	data_type id '(' arg_list ')' '{'	{
																inFuncScope = true;
																curFunc = $2->label;
																pushSymTable();
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
 						stmt_list '}'					    {
 																$$ = make_node(4,$1,$2,$4,$8);
 																$$->code = FUNC_DEF;
 																popSymTable();
 																addToSymTable($2->label, $1->label);
 																inFuncScope = false;
 															}
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
										pushSymTable();
									}

				declaration_list '}' ';'{
											if(!isPresent($2->label))
											{
												struct_dt *temp = new struct_dt;
												temp->v = getAllVarsCurrentScope();
												structTable[$2->label] = *temp;
												$$ = make_node(2,$2,$5);
												$$->code = STRUCT_DECL;
											}
											else
												printMultiDeclMsg($2->label);
											popSymTable();
											addToSymTable($2->label, "struct");
										}
			;

declaration_list	: declaration_stmt						{$$ = make_node(1, $1);}
					| declaration_stmt declaration_list		{$$ = make_node(2, $1, $2);}
					;

id_list	: id 					{$$ = make_node(1, $1);}
		| '@' id    			{$2->code = POINTER; $$ = make_node(1, $2); }
		| id ',' id_list		{$$ = make_node(2, $1, $3);}
		| '@' id ',' id_list   	{$2->code = POINTER; $$ = make_node(2, $2, $4); }
		;

arg_list	: 								{$$ = make_node(0);}
			| data_type id 					{$$ = make_node(2, $1, $2);}
			| data_type id ',' arg_list 	{$$ = make_node(3, $1,$2,$4);}

declaration_stmt	: data_type id_list ';'	{
													vs list = getIdList($2);
													for(vsi it=list.begin();it!=list.end();it++)
													{
														if(isPresentCurrentScope(*it))
															printMultiDeclMsg(*it);
														else
														{
															if ((*it)[0]=='@')
															{
																*it = (*it).substr(1);
																addToSymTable(*it, $1->label+"@");
															}
															else
																addToSymTable(*it, $1->label);
														}
													}
													$$ = make_node(2, $1, $2);
												}
					| data_type id brackets ';' {
													$$ = make_node(3,$1,$2,$3);
													addToSymTable($2->label, $1->label+"*");
													addToArrayTable($2->label, $3);
												}
					;

brackets	: '[' expr ']' 			{
										if($2->data_type != "int")
											printArrayIndexdt();
										$$ = make_node(1,$2);
									}
			| '[' expr ']' brackets {
										if($2->data_type != "int")
											printArrayIndexdt();
										$$ = make_node(2,$2,$4);
									}

compound_stmt	: '{'			{
									pushSymTable();
								}
				stmt_list '}'	{
									popSymTable();
									$$ = make_node(1,$3);
									$$->code = COMPD_STMT;
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
		| return_stmt			{ $$ = make_node(1,$1); }
		| break_stmt			{ $$ = make_node(1,$1); }
		| error
		;

return_stmt	: RETURN expr ';'	{
									if(!inFuncScope)
										printReturnNotInScope();
									else if(functionTable[curFunc].type != $2->data_type)
									{
										cout << getDataType(curFunc);
										printReturnDtMismatch();
									}
									else
									{
										$$ = make_node(1,$2);
										$$->code = RET;
									}
								}
			| RETURN ';'		{
									if(!inFuncScope)
										printReturnNotInScope();
									else if(functionTable[curFunc].type != "void")
										printReturnDtMismatch();
									else
									{
										$$ = make_node(0);
										$$->code = RET;
									}
								}
			;

break_stmt	: BREAK ';' 		{
									if(loopLevel>0)
									{
										$$ = make_node(0); $$->code = BRK;
									}
									else
										printBreakNotInScope();
								}
			;

loop_stmt	: WHILE '(' expr ')'	{loopLevel++;}
				stmt 				{ $$ = make_node(2,$3,$6); $$->code = LOOP_STMT; loopLevel--;}
			;

if_stmt	: IF '(' expr ')' stmt 				{ $$ = make_node(2,$3,$5); $$->code = IF_STMT;}
		| IF '(' expr ')' stmt ELSE stmt 	{ $$ = make_node(3,$3,$5,$7); $$->code = IF_STMT;}
		;

expr_stmt	: expr ';'		{ $$ = make_node(1,$1); }
			;

expr 	: INT_LITERAL				{$$ = make_terminal_node(string(yytext, yyleng), int_node);}
		| CHAR_LITERAL				{$$ = make_terminal_node(string(yytext, yyleng), char_node);}
		| FLOAT_LITERAL				{$$ = make_terminal_node(string(yytext, yyleng), float_node);}
		| id 						{
										if(!isPresent($1->label))
											printUndefinedMsg($1->label);
										else
											$$ = $1;
									}
		| '@' id					{$$ = make_node(1, $2); $$->data_type = $2->data_type.substr(0, $2->data_type.size()-1);}
		| '$' id					{$$ = make_node(1, $2); $$->data_type = $2->data_type+"@";}
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
												$$->code = STRCT;
											}
											else
												printNotMember($1->label, $3->label);
										}
									}
		| id brackets				{
										if(!isPresent($1->label))
											printUndefinedMsg($1->label);
										else
										{
											$$ = make_node(2,$1,$2);
											$$->code = ARRAY;
											int n = isValidArray($1->label, $2);
											if(n == 1)
												$$->data_type = $1->data_type.substr(0,($1->data_type).size()-1);
											else if(n==2)
												$$->data_type = $1->data_type;
											else if(n==0)
											{
												$$->data_type = $1->data_type.substr(0,($1->data_type).size()-1);
												printOutOfBoundsMsg($1->label);
											}
											else
												printUndefinedMsg($1->label);
										}
									}
		| expr op expr 				{
										$$ = make_node(3,$1,$2,$3);
										$$->data_type = "int";
										if($2->label == "=")
											$$->code = ASSIGN_STMT;
										else
											$$->code = OP;
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
											$$->code = CALL_FUNC;
										}
										else
											printArgsMismatch($1->label);
									}
		;

op: 	  EQUAL_OP  {$$ = make_terminal_node(string(yytext, yyleng), op_node);}
		| REL_OP 	{$$ = make_terminal_node(string(yytext, yyleng), op_node);}
		| MATH_OP	{$$ = make_terminal_node(string(yytext, yyleng), op_node);}
		| LOGICAL_OP	{$$ = make_terminal_node(string(yytext, yyleng), op_node);}
		| BIT_OP	{$$ = make_terminal_node(string(yytext, yyleng), op_node);}
		;

id 	: IDENTIFIER 		{
							$$ = make_terminal_node(string(yytext, yyleng), id_node);
							$$->data_type = getDataType(string(yytext, yyleng));
						}
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
	pushSymTable();
    yyparse();
    popSymTable();
    if(!semanticError && !syntacticError)
    {
		sort(completeTable.begin(), completeTable.end(), compare);
    	printCompleteSymTable();
    	// dfs(root,0);
		pushSymTable();
    	generateIC(root);
    	popSymTable();
    }
    return 0;
}

void yyerror(const char *s)
{
	syntacticError = true;
    fprintf(stderr, "%s at line no %d\n", s, yylineno);
}

%{
#include <bits/stdc++.h>

using namespace std;

void yyerror(const char *s);
int yylex();

extern int yylineno;
extern char* yytext;
extern int yyleng;

enum nodetype {nt_node, int_node, float_node, char_node, op_node, id_node, dt_node};

typedef struct node {
	vector<struct node * > v;
	enum nodetype type;
	string data_type;
	string label;	
}node;

typedef struct struct_dt {
	vector<pair<string, string> > v;
} struct_dt;

typedef struct function_dt {
	vector<string> v;
	string type;
} function_dt;

vector<map<string, string> > symbolTable;
map<string, struct_dt>  structTable;
map<string, function_dt> functionTable;


typedef node * node_pointer;

#define YYSTYPE node_pointer

node *make_node(int nargs, ...)
{
	node *temp= new node;
    temp->type = nt_node;
    va_list valist;
    va_start(valist, nargs);
    int i;
    for(i=0;i<nargs;i++)
        (temp->v).push_back(va_arg(valist, node *));
    return temp;
}

string getDataType(string s)
{
	int n = symbolTable.size();
	for(int i=n-1;i>=0;i--)
	{
		map<string, string>::iterator it;
		it = symbolTable[i].find(s);
		if(it != symbolTable[i].end())
			return it->second;
	}
	return "";
}

bool isPresent(string s)
{
	int n = symbolTable.size();
	for(int i=n-1;i>=0;i--)
	{
		if(symbolTable[i].find(s) != symbolTable[i].end())
			return true;
	}
	return false;
}

bool isPresentCurrentScope(string s)
{
	int n = symbolTable.size();
	if(symbolTable[n-1].find(s) != symbolTable[n-1].end())
		return true;
	return false;
}

vector<string> giveArgList(node *a)
{
	vector<string> ans;
	node *temp = a;
	while((temp->v).size()>0)
	{
		ans.push_back(((temp->v)[0])->label);
		if((temp->v).size() == 2)
			break;
		temp = (temp->v)[2];
	}
	return ans;
}	

void addToSymTable(string a, string dt)
{
	int n = symbolTable.size();
	symbolTable[n-1][a] = dt;
}

void addListToSymTable(node *a)
{
	node *temp = a;
	int n = symbolTable.size();
	while((temp->v).size()>0)
	{
		symbolTable[n-1][((temp->v)[1])->label] = ((temp->v)[0])->label;
		if((temp->v).size() == 2)
			break;
		temp = (temp->v)[2];
	}
}	

vector<string> getIdList(node *a)
{
	vector<string> ans;
	node *temp = a;
	while((temp->v).size()>0)
	{
		ans.push_back(((temp->v)[0])->label);
		if((temp->v).size() == 1)
			break;
		temp = (temp->v)[1];
	}
	return ans;
}	

vector<pair<string, string> > getMemberList(node *a)
{
	vector<pair<string, string> > ans;
	node *temp = a;
	while((temp->v).size()>0)
	{
		node *temp2 = (temp->v)[0];
		string dt = (temp2->v)[0]->label;
		vector<string> id_list = getIdList((temp2->v)[1]);
		for(int i=0;i<id_list.size();i++)
			ans.push_back(make_pair(id_list[i], dt));
		if((temp->v).size()==1)
			break;
		temp = (temp->v)[1];
	}
	return ans;
}

vector<string> getParamTypes(node *a)
{
	vector<string> ans;
	node *temp = a;
	while((temp->v).size()>0)
	{
		ans.push_back(((temp->v)[0])->data_type);
		if((temp->v).size() == 2)
			break;
		temp = (temp->v)[2];
	}
	return ans;
}

bool similarArgs(node *a, node *b)
{
	vector<string> x = ((functionTable.find(a->label))->second).v;
	vector<string> y = getParamTypes(b);
	if(x.size() != y.size())
		return false;
	for(int i=0;i<x.size();i++)
		if(x[i] != y[i])
			return false;
	return true;
}

node *make_terminal_node(string s, enum nodetype type)
{
	node *temp= new node;
    temp->type = type;
    temp->label = s;
    if(type == int_node)
    	temp->data_type = "int";
    else if(type == char_node)
    	temp->data_type = "char";
    else if(type == float_node)
    	temp->data_type = "float";
    return temp;
}


void printMultiDeclMsg(string var)
{
	cout << "Multiple declarations for " << var << " at line no. " << yylineno << endl;
}
void printUndefinedMsg(string var)
{
	cout << "Undefined " << var << " at line no. " << yylineno << endl;
}

void printSymTable()
{
	map<string, string>::iterator it;
	int n = symbolTable.size();
	for(it=symbolTable[n-1].begin();it!=symbolTable[n-1].end();it++)
		cout << it->first << " " << it->second << endl;
}

%}

%define parse.error verbose

%token IF
%token ELSE
%token WHILE
%token DO
%token FOR
%token RETURN
%token CONTINUE
%token BREAK
%token STRUCT
%token STRING_LITERAL
%token CHAR_LITERAL
%token DATA_TYPE
%token INT_LITERAL
%token FLOAT_LITERAL
%token IDENTIFIER
%token ADD_ASSIGN
%token SUB_ASSIGN
%token MUL_ASSIGN
%token DIV_ASSIGN
%token MOD_ASSIGN
%token AND_ASSIGN
%token XOR_ASSIGN
%token OR_ASSIGN
%token RIGHT_OP
%token LEFT_OP
%token INC_OP
%token DEC_OP
%token REL_OP
%token MATH_OP
%token EQUAL_OP


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
																map<string, string>  temp;
																symbolTable.push_back(temp);	
															} 

 						stmt_list '}'						{
																symbolTable.pop_back();
																if(!isPresent($2->label))
																{
																	function_dt *temp = new function_dt;
																	temp->type = $1->label;
																	temp->v = giveArgList($4);
																	functionTable[$2->label] = *temp;
																	addToSymTable($2->label, $1->label);
																	addListToSymTable($4);
																	$$ = make_node(4,$1,$2,$4,$8);
																}
																// else if(sameFunction($1->label, $2->label, $4))
																// {
																// 	addListToSymTable($4);
																// 	$$ = make_node(4,$1,$2,$4,$6);
																// }
																else
																	printMultiDeclMsg($2->label);
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

struct_decl	: 	STRUCT id '{' 				{	
												map<string, string>  temp;
												symbolTable.push_back(temp);	
											} 

				declaration_list '}' ';'	{
											symbolTable.pop_back();
											if(!isPresent($2->label))
											{
												struct_dt *temp = new struct_dt;
												temp->v = getMemberList($5);
												structTable[$2->label] = *temp;
												addToSymTable($2->label, "struct");
												// printSymTable();
												$$ = make_node(2,$2,$5);			
											}
											else
												printMultiDeclMsg($2->label);
										}
			;

declaration_list	: declaration_stmt						{$$ = make_node(1, $1);}
					| declaration_stmt declaration_list		{$$ = make_node(2, $1, $2);}
					;

id_list	: id 				{$$ = make_node(1, $1);}
		| id ',' id_list	{$$ = make_node(2, $1, $3);}
		;

arg_list	: 								{$$ = make_node(0);}
			| data_type id 					{$$ = make_node(2, $1, $2);}
			| data_type id ',' arg_list 	{$$ = make_node(3, $1,$2,$3);}

declaration_stmt	: data_type id_list ';' {
												vector<string> list = getIdList($2);
												for(vector<string>::iterator it=list.begin();it!=list.end();it++)
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
									map<string, string>  temp;
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
		| expr op expr 				{$$ = make_node(3,$1,$2,$3); $$->data_type = "int";}
		| id '(' param_list ')' 	{ 
										if(!isPresent($1->label))
											printUndefinedMsg($1->label);
										else if(similarArgs($1, $3))
										{
											$$ = make_node(2,$1,$3);
											$$->data_type = $1->data_type;
										}
										else
											cout << "Arguments mismatch for " << $1->label << " at line no. " << yylineno << endl; 
									}
		;

op: 	  EQUAL_OP  {$$ = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}
		| REL_OP 	{$$ = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}
		| MATH_OP	{$$ = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}	
		;
						
id 	: IDENTIFIER 	{ $$ = make_terminal_node(string(yytext, yyleng), id_node); $$->data_type = getDataType(string(yytext, yyleng));}
	; 


data_type 	: DATA_TYPE {$$ = make_terminal_node(string(yytext, yyleng), dt_node);}
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
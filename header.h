#include <bits/stdc++.h>

using namespace std;

#define vs vector<string>
#define vsi vs::iterator
#define mss map<string, string>
#define mssi mss::iterator

enum nodetype {nt_node, int_node, float_node, char_node, op_node, id_node, dt_node};

typedef struct node {
	vector<struct node * > v;
	enum nodetype type;
	string data_type;
	string label;	
}node;

typedef struct struct_dt {
	mss v;
} struct_dt;

typedef struct function_dt {
	vs v;
	string type;
} function_dt;


void yyerror(const char *s);
int yylex();

extern int yylineno;
extern char* yytext;
extern int yyleng;
vector<mss> symbolTable;
vector<pair<mss, int> > completeTable;
map<string, struct_dt>  structTable;
map<string, function_dt> functionTable;


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
		mssi it = symbolTable[i].find(s);
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

vs giveArgList(node *a)
{
	vs ans;
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

void popSymTable()
{
	int n = symbolTable.size();
	completeTable.push_back(make_pair(symbolTable[n-1], n-1));
	symbolTable.pop_back();
}

void printCompleteSymTable()
{
	vector<pair<mss, int> >::iterator it;
	for(it=completeTable.begin();it!=completeTable.end();it++)
	{
		mss m = it->first;
		for(mssi j=m.begin();j!=m.end();j++)
			cout << it->second << "\t" << j->first << "\t" << j->second << endl;
	}
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
vs getIdList(node *a)
{
	vs ans;
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

mss getAllVarsCurrentScope()
{
	int n = symbolTable.size();
	return symbolTable[n-1];
}

vs getParamTypes(node *a)
{
	vs ans;
	node *temp = a;
	while((temp->v).size()>0)
	{
		ans.push_back(((temp->v)[0])->data_type);
		if((temp->v).size() == 1)
			break;
		temp = (temp->v)[1];
	}
	return ans;
}

bool similarArgs(node *a, node *b)
{
	vs x = ((functionTable.find(a->label))->second).v;
	vs y = getParamTypes(b);
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

bool similarDataType(node *a, node *b)
{
	if(a && b)
		if(a->data_type == b->data_type)
			return true;
	return false;
}

void dfs(node *root,int level)
{

	printf("%*s", level,"");
	if(root==NULL){
		cout<<"NULL"<<endl;
		return;
	}
	cout << (root->v).size() << " ";
	if(root->type != nt_node)
		cout << root->label << endl;
	else
		cout << root->type <<  endl;
	for (int i = 0; i < (root->v).size(); ++i)
	{
		dfs((root->v)[i],level+4);
	}
}




void printMultiDeclMsg(string var)
{
	cout << "Multiple declarations for " << var << " at line no. " << yylineno << endl;
}
void printUndefinedMsg(string var)
{
	cout << "Undefined " << var << " at line no. " << yylineno << endl;
}
void printDtMismatch()
{
	cout << "Mismatching data types of operands at line no. " << yylineno << endl; 
}
void printNotMember(string a, string b)
{
	cout << b << "is not a member of " << a << " at line no. " << yylineno << endl; 
}
void printArgsMismatch(string a)
{
	cout << "Arguments mismatch for " << a << " at line no. " << yylineno << endl; 
}
void printSymTable()
{
	int n = symbolTable.size();
	for(mssi it=symbolTable[n-1].begin();it!=symbolTable[n-1].end();it++)
		cout << it->first << " " << it->second << endl;
}
void printVec(vector<string> v)
{
	for(vsi it=v.begin();it!=v.end();it++)
		cout << *it << " ";
	cout << endl;
}

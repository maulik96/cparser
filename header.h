#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define vs vector<string>
#define vsi vs::iterator
#define mss map<string, string>
#define mssi mss::iterator

enum nodetype {nt_node, int_node, float_node, char_node, op_node, id_node, dt_node};
enum codetype {DEFAULT, COMPD_STMT, LOOP_STMT, IF_STMT, ASSIGN_STMT, OP, ID, FUNC_DEF, CALL_FUNC, INTEGER, FLOAT, CHAR, STRCT, ARRAY};

struct node {
	vector<struct node * > v;
	enum nodetype type;
	enum codetype code;
	string data_type;
	string label;	
};

struct struct_dt {
	mss v;
};

struct function_dt {
	vs v;
	string type;
};


void yyerror(const char *s);
int yylex();

extern int yylineno;
extern char* yytext;
extern int yyleng;
vector<mss> symbolTable;
vector<pair<mss, int> > completeTable;
map<string, struct_dt>  structTable;
map<string, function_dt> functionTable;
vector<map<string, pair<int, vi> > > arrayTable;
bool semanticError = false, syntacticError = false;
node *root;

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

node *make_terminal_node(string s, enum nodetype type)
{
	node *temp= new node;
    temp->type = type;
    temp->label = s;
    if(type == int_node)
    {
    	temp->code = INTEGER;
    	temp->data_type = "int";
    }
    else if(type == char_node)
    {
    	temp->code = CHAR;
    	temp->data_type = "char";
    }
    else if(type == float_node)
    {
    	temp->code = FLOAT;
    	temp->data_type = "float";
    }
    else if(type == id_node)
    	temp->code = ID;
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

void pushSymTable()
{
	mss temp;
	symbolTable.push_back(temp);
	map<string, pair<int, vi> > m;
	arrayTable.push_back(m);
}

void popSymTable()
{
	int n = symbolTable.size();
	completeTable.push_back(make_pair(symbolTable[n-1], n-1));
	symbolTable.pop_back();
	arrayTable.pop_back();
}

void printCompleteSymTable()
{
	cout << "Level\tvar\tdata tyoe" << endl;
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


bool similarDataType(node *a, node *b)
{
	if(a && b)
		if(a->data_type == b->data_type)
			return true;
	return false;
}

void addToArrayTable(string name, node *a)
{
	int n = arrayTable.size();
	node *temp = a;
	vi v;
	while((temp->v).size() > 0)
	{
		node *x = (temp->v)[0];
		if(x->type == int_node)
			v.push_back(atoi(x->label.c_str()));
		else
			v.push_back(1000000000);
		if((temp->v).size() == 1)
			break;
		else
			temp = (temp->v)[1];
	}
	arrayTable[n-1][name] = make_pair(v.size(), v);
}

int isValidArray(string name, node *a)
{
	int n = arrayTable.size();
	map<string, pair<int,vi> >::iterator it = arrayTable[n-1].find(name);
	if(it==arrayTable[n-1].end())
		return -1;
	node *temp = a;
	int size = 0, flag = 1;
	while((temp->v).size() > 0)
	{
		if(size == (it->second).first)
			return 0;
		node *x = (temp->v)[0];
		if(x->type == int_node)
			if((atoi(x->label.c_str()) >= ((it->second).second)[size]) || atoi(x->label.c_str()) < 0)
				flag = 0;
		size++;
		if((temp->v).size() == 1)
			break;
		else
			temp = (temp->v)[1];
	}
	if(flag == 0)
		return 0;
	if(size != (it->second).first)
		return 2;
	return 1;
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
	semanticError = true;
	cout << "Multiple declarations for " << var << " at line no. " << yylineno << endl;
}
void printUndefinedMsg(string var)
{
	semanticError = true;
	cout << "Undefined " << var << " at line no. " << yylineno << endl;
}
void printDtMismatch()
{
	semanticError = true;
	cout << "Mismatching data types of operands at line no. " << yylineno << endl; 
}
void printNotMember(string a, string b)
{
	semanticError = true;
	cout << b << "is not a member of " << a << " at line no. " << yylineno << endl; 
}
void printArgsMismatch(string a)
{
	semanticError = true;
	cout << "Arguments mismatch for " << a << " at line no. " << yylineno << endl; 
}
void printOutOfBoundsMsg(string a)
{
	semanticError = true;
	cout << "Array index out of bounds for " << a << " at line no. " << yylineno << endl; 
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

string getNewReg()
{
	static int count=0;
	string c = to_string(count);
	count++;
	return "r"+c;
}

string genNewLabel()
{
	static int label=0;
	string l = to_string(label);
	label++;
	return "label"+l+":";
}

string generateIC(node *n)
{
	string res = "";
	if(n==NULL)
		return res;
	switch(n->code)
	{
		case FUNC_DEF :
		{
			cout << "func begin " << ((n->v)[1])->label << endl;
			generateIC((n->v)[3]);
			cout << "func end" << endl;
		}
		case COMPD_STMT:
		{
			pushSymTable();
			generateIC((n->v)[0]);
			popSymTable;
			break;
		}
		case LOOP_STMT:
		{
			string entryLabel = genNewLabel();
			cout << entryLabel << endl;
			string r1=generateIC((n->v)[0]);
			string bodyLabel = genNewLabel();
			string exitLabel = genNewLabel();
			cout<<"if "<<r1<<" goto "<<bodyLabel<<endl;
			cout<<"goto "<<exitLabel<<endl;
			cout<<bodyLabel<<endl;
			generateIC((n->v)[1]);
			cout<<"goto "<<entryLabel<<endl;
			cout<<exitLabel<<endl;
			break;
		}
		case IF_STMT:
		{
			string r1=generateIC((n->v)[0]);
			string trueLabel = genNewLabel();
			string falseLabel = genNewLabel();
			cout<<"if "<<r1<<" goto "<<trueLabel<<endl;
			cout<<"goto "<<falseLabel<<endl;
			cout<<trueLabel<<endl;
			generateIC((n->v)[1]);
			cout<<falseLabel<<endl;
			if((n->v).size()==3)
				generateIC((n->v)[2]);
			break;
		}
		case ASSIGN_STMT:
		{
			cout << generateIC((n->v)[0]) << "=" << generateIC((n->v)[2]) << endl;
			break;
		}
		case OP:
		{
			string r = getNewReg();
			cout << r << "=" << generateIC((n->v)[0]) << ((n->v)[1])->label << generateIC((n->v)[2]) << endl;
			res = r;
			break;
		}
		case CALL_FUNC :
		{
			node *temp = (n->v)[1];
			while((temp->v).size()>0)
			{
				cout << "param " << ((temp->v)[0])->label << endl;
				if((temp->v).size() == 1)
					break;
				temp = (temp->v)[1];
			}
			string r = getNewReg();
			cout << "param " << r << endl;
			cout << "call func " << ((n->v)[0])->label << endl;
			return r;
		}
		case ID:
		{
			res = n->label;
			break;
		}
		case INTEGER: 
		case FLOAT: 
		case CHAR: 
		{
			res = n->label;
			break;
		}
		default:
		{
			for (int i = 0; i < (n->v).size(); ++i)
				generateIC((n->v)[i]);
		}
	}
	// cout << res << endl;
	return res;
}






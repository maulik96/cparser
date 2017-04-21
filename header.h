#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define ii pair<int, int>
#define vs vector<string>
#define vsi vs::iterator
#define mss map<string, string>
#define mssi mss::iterator

enum nodetype {nt_node, int_node, float_node, char_node, op_node, id_node, dt_node};
enum codetype {DEFAULT, COMPD_STMT, LOOP_STMT, IF_STMT, ASSIGN_STMT, OP, ID, FUNC_DEF, CALL_FUNC, INTEGER, FLOAT, CHAR, STRCT, ARRAY, RET, BRK};

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
vector<pair<mss, ii> > completeTable;
map<string, struct_dt>  structTable;
map<string, function_dt> functionTable;
vector<map<string, pair<int, vi> > > arrayTable;
bool semanticError = false, syntacticError = false;
bool inFuncScope = false;
int loopLevel = 0;
string curFunc = "";
node *root;
vs loopExitLabel;
vi startscope;


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


map<string, pair<int, vi> >::iterator findArrayTable(string s)
{
	int n = arrayTable.size();
	for(int i=n-1;i>=0;i--)
	{
		if(arrayTable[i].find(s) != arrayTable[i].end())
			return arrayTable[i].find(s);
	}
	return arrayTable[n-1].end();
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
	startscope.push_back(yylineno);
	map<string, pair<int, vi> > m;
	arrayTable.push_back(m);
}

void popSymTable()
{
	int n = symbolTable.size();
	completeTable.push_back(make_pair(symbolTable[n-1], make_pair(startscope[startscope.size()-1], yylineno)));
	startscope.pop_back();
	symbolTable.pop_back();
	arrayTable.pop_back();
}

void printCompleteSymTable()
{
	cout << "Symbol table" << endl;
	string x = "---------------------------------------------------------------------------------";
	cout<<x<<endl;
	printf("|%-15s|%-15s|%-15s|%-15s|%-15s|\n",  "Name", "Data Type", "Type", "Scope begin", "Scope end");
	cout<<x<<endl;
	vector<pair<mss, ii> >::iterator it;
	for(it=completeTable.begin();it!=completeTable.end();it++)
	{
		mss m = it->first;
		for(mssi j=m.begin();j!=m.end();j++)
		{
			string type = "variable";
			if(functionTable.find(j->first.c_str()) != functionTable.end())
				type = "function";
			else if(structTable.find(j->second.c_str()) != structTable.end())
				type = "struct var";
			else if(j->second[j->second.size()-1] == '*')
				type = "pointer";
			printf("|%-15s|%-15s|%-15s|%-15d|%-15d|\n", j->first.c_str(), j->second.c_str(), type.c_str(), it->second.first, it->second.second);
		}
			// printf(output_format, it->second, j->first.c_str(), j->second.c_str());
	}
	cout<< x << "\n\n\n";
	cout << "Function table" << endl;
	cout << x << endl;
	printf("|%-15s|%-15s|%-47s|\n",  "Name", "Data Type", "Parameters");
	cout << x << endl;
	map<string, function_dt>::iterator i;
	for(i=functionTable.begin();i!=functionTable.end();i++)
	{
		vs v = i->second.v;
		string s = "";
		for(int j=0;j<v.size();j++)
			s += v[j] + " ";
		printf("|%-15s|%-15s|%-47s|\n", i->first.c_str(), i->second.type.c_str(), s.c_str());
	}
	cout << x << endl << endl;
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
	vs v = {"int", "float", "char"};
	if(a && b)
	{
		if(find(v.begin(), v.end(),a->data_type)!=v.end() && find(v.begin(), v.end(), b->data_type)!=v.end())
			return true;
		if(a->data_type == b->data_type)
			return true;
	}
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
	map<string, pair<int,vi> >::iterator it = findArrayTable(name);
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
void printReturnDtMismatch()
{
	semanticError = true;
	cout << "Return type does not match at line no. " << yylineno << endl;
}
void printReturnNotInScope()
{
	semanticError = true;
	cout << "Return statement not inside function at line no. " << yylineno << endl;
}
void printBreakNotInScope()
{
	semanticError = true;
	cout << "Break statement not inside loop at line no. " << yylineno << endl;
}
void printArrayIndexdt()
{
	semanticError = true;
	cout << "Array index must be an integer at line no. " << yylineno << endl;	
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
			cout << "func begin " << generateIC((n->v)[1]) << endl;
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
			loopExitLabel.push_back(exitLabel);
			cout<<"if "<<r1<<" goto "<<bodyLabel<<endl;
			cout<<"goto "<<exitLabel<<endl;
			cout<<bodyLabel<<endl;
			generateIC((n->v)[1]);
			cout<<"goto "<<entryLabel<<endl;
			cout<<exitLabel<<endl;
			loopExitLabel.pop_back();
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
			cout << generateIC((n->v)[0]) << " = " << generateIC((n->v)[2]) << endl;
			break;
		}
		case OP:
		{
			string r = getNewReg();
			cout << r << " = " << generateIC((n->v)[0]) << ((n->v)[1])->label << generateIC((n->v)[2]) << endl;
			res = r;
			break;
		}
		case CALL_FUNC :
		{
			node *temp = (n->v)[1];
			while((temp->v).size()>0)
			{
				cout << "param " << generateIC((temp->v)[0]) << endl;
				if((temp->v).size() == 1)
					break;
				temp = (temp->v)[1];
			}
			string r = getNewReg();
			cout << "param " << r << endl;
			cout << "call func " << generateIC((n->v)[0]) << endl;
			return r;
		}
		case ARRAY :
		{
			string final, r1 = getNewReg();
			cout << r1 << " = addr(" << generateIC((n->v)[0]) << ")" << endl;
			node *temp = (n->v)[1];
			while((temp->v).size() > 0)
			{
				string s = getNewReg(), multiplier="4", dt = n->data_type;
				if(dt == "char")
					multiplier = "1";
				if(dt == "float")
					multiplier = "8";
				cout << s << " = " << generateIC((temp->v)[0]) << "*" << multiplier << endl;
				final = getNewReg();
				cout << final << " = " << r1 << "[" << s << "]" << endl;
				if((temp->v).size() == 1)
					break;
				temp = (temp->v)[1];
				r1 = final;
			}
			return final;
		}
		case STRCT :
		{
			res = generateIC((n->v)[0]) + "." + generateIC((n->v)[1]);
			break;
		}
		case RET :
		{
			if((n->v).size()>0)
				cout << "return " << generateIC((n->v)[0]) << endl;
			else
				cout << "return" << endl;
			break;
		}
		case BRK:
		{
			cout << "goto " << loopExitLabel[loopExitLabel.size()-1] << endl;
			break;
		}
		case ID:
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
	return res;
}

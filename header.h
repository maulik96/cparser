#include <bits/stdc++.h>

using namespace std;

#define vi vector<int>
#define ii pair<int, int>
#define vs vector<string>
#define vsi vs::iterator
#define mss map<string, string>
#define mssi mss::iterator

enum nodetype {nt_node, int_node, float_node, char_node, op_node, id_node, dt_node};
enum codetype {DEFAULT, COMPD_STMT, LOOP_STMT, IF_STMT, ASSIGN_STMT, OP, ID, FUNC_DEF, CALL_FUNC, INTEGER, FLOAT, CHAR, STRCT, ARRAY, RET, BRK, STRUCT_DECL, POINTER, REFPTR, DEREFPTR};

/* Structure declaratins */

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

struct compSym {
    map<string, pair<string, int> >m;
    int start, end, level;
};

void yyerror(const char *s);
int yylex();

extern int yylineno;
extern char* yytext;
extern int yyleng;
vector<mss> symbolTable;
vector<compSym> completeTable;
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
int scope=0;
vs ircode;
vector<vs> quadform;
vs qcode;

/* Make non-terminal node */
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

/* Make terminal node */
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

/* Get datatype from symbol table */
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

/* If variable/function already declared */
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

/* Find array table */
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

/* Check if variable is present in current scope */
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

/* Add to symbol table */
void addToSymTable(string a, string dt)
{
    int n = symbolTable.size();
    symbolTable[n-1][a] = dt;
}

/* Push to symbol table */
void pushSymTable()
{
    mss temp;
    symbolTable.push_back(temp);
    startscope.push_back(yylineno);
    map<string, pair<int, vi> > m;
    arrayTable.push_back(m);
}

/* Pop from symbol table */
void popSymTable()
{
    int n = symbolTable.size();
    compSym *temp = new compSym;
    for(mssi it=symbolTable[n-1].begin();it!=symbolTable[n-1].end();it++)
        temp->m[it->first] = make_pair(it->second, 1);
    temp->start = startscope[startscope.size()-1];
    temp->end = yylineno;
    temp->level = n-1;
    completeTable.push_back(*temp);
    startscope.pop_back();
    symbolTable.pop_back();
    arrayTable.pop_back();
}

/* Print symbol table */
void printCompleteSymTable()
{
    int index = 0;
    cout << "Symbol table" << endl;
    string x = "------------------------------------------------------------------------------------------------------------------------";
    cout<<x<<endl;
    printf("| %-15s| %-15s| %-15s| %-15s| %-15s| %-15s| %-15s|\n",  "Index", "Name", "Data Type", "Type", "Nesting level", "Scope begin", "Scope end");
    cout<<x<<endl;
    vector<compSym >::iterator it;
    for(it=completeTable.begin();it!=completeTable.end();it++)
    {
        map<string, pair<string, int> > m = it->m;
        for(map<string, pair<string, int> >::iterator j=m.begin();j!=m.end();j++)
        {
            string type = "variable";
            if(functionTable.find(j->first.c_str()) != functionTable.end())
                type = "function";
            else if(structTable.find(j->second.first.c_str()) != structTable.end())
                type = "struct var";
            else if(j->second.first[j->second.first.size()-1] == '*')
                type = "array";
            else if(j->second.first[j->second.first.size()-1] == '@')
                type = "pointer";
            it->m[j->first] = make_pair(j->second.first, index);
            printf("| %-15d| %-15s| %-15s| %-15s| %-15d| %-15d| %-15d|\n", index++, j->first.c_str(), j->second.first.c_str(), type.c_str(), it->level, it->start, it->end);
        }
    }
    cout<< x << "\n\n\n";
    cout << "Function table" << endl;
    string y = "--------------------------------------------------------------------------------------------------------------------";
    cout<<y<<endl;
    printf("| %-15s| %-15s| %-79s|\n",  "Name", "Data Type", "Parameters");
    cout << y << endl;
    map<string, function_dt>::iterator i;
    for(i=functionTable.begin();i!=functionTable.end();i++)
    {
        vs v = i->second.v;
        string s = "";
        for(int j=0;j<v.size();j++)
            s += v[j] + " ";
        printf("| %-15s| %-15s| %-79s|\n", i->first.c_str(), i->second.type.c_str(), s.c_str());
    }
    cout << y << endl << endl;
}

/* Add list of variables */
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

/* Utility to get list of identifiers */
vs getIdList(node *a)
{
    vs ans;
    node *temp = a;
    while((temp->v).size()>0)
    {
        if((temp->v)[0]->code == POINTER)
            ans.push_back("@"+(((temp->v)[0])->label));
        else
            ans.push_back(((temp->v)[0])->label);
        if((temp->v).size() == 1)
            break;
        temp = (temp->v)[1];
    }
    return ans;
}

/* Get all variables in current scope */
mss getAllVarsCurrentScope()
{
    int n = symbolTable.size();
    return symbolTable[n-1];
}

/* Get data types of function arguments */
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

/* Check if function argument and parameter data types match */
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

/* Check if 2 nodes are coercible */
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

/* Add to array table */
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

/* Check if array index is in range */
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


/* Print error messages */

void printMultiDeclMsg(string var)
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Multiple declarations for " << var << endl;
}
void printUndefinedMsg(string var)
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Undefined identifier " << var << endl;
}
void printOpCompatibilityError(string var)
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Operator " << var << " not compatible with given operands" << endl;
}
void printDtMismatch()
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Mismatching data types of operands" << endl;
}
void printNotMember(string a, string b)
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       " << b << "is not a member of " << a << endl;
}
void printArgsMismatch(string a)
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Arguments mismatch for " << a << endl;
}
void printOutOfBoundsMsg(string a)
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Array index out of bounds for " << a << endl;
}
void printReturnDtMismatch()
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Return type does not match" << endl;
}
void printReturnNotInScope()
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Return statement not inside function" << endl;
}
void printBreakNotInScope()
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Break statement not inside loop" << endl;
}
void printArrayIndexdt()
{
    semanticError = true;
    cout << "ERROR: At line no. " << yylineno << endl;
    cout << "       Array index must be an integer" << endl;
}

/* Print current scope symbol table */
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

/* Get new register */
string getNewReg()
{
    static int count=0;
    string c = to_string(count);
    count++;
    return "R"+c;
}

/* Generate new label */
string genNewLabel()
{
    static int label=0;
    string l = to_string(label);
    label++;
    return "Label"+l+": ";
}

/* Get symbol table reference index */
int findCompleteTable(string name)
{
    int curlevel = completeTable[scope].level;
    for(int i=scope;i>=0;i--)
    {
        if(completeTable[i].level == curlevel)
        {
            map<string,pair<string, int> >::iterator it = completeTable[i].m.find(name);
            if(it!=completeTable[i].m.end())
                return it->second.second;
            curlevel--;
        }
    }
    return -1;
}

/* Utility to compare in sort */
bool compare(compSym a, compSym b)
{
    return a.start < b.start;
}

/* Generate intermediate code */
string generateIC(node *n)
{
    string res = "", s = "",t="", u="",v="";
    if(n==NULL)
        return res;
    switch(n->code)
    {
        case FUNC_DEF :
        {
            scope++;
            s = "func begin ";
            t= generateIC((n->v)[1]);
            ircode.push_back(s+t);
            
            qcode.push_back(s+t);
            qcode.push_back("FBEG");
            qcode.push_back(t);
            quadform.push_back(qcode);
            qcode.clear();

            generateIC((n->v)[3]);
            s = "func end";
            ircode.push_back(s);

            qcode.push_back(s);
            qcode.push_back("FEND");
            quadform.push_back(qcode);
            qcode.clear();
            break;
        }
        case STRUCT_DECL : 
        {
            scope++;
            break;
        }
        case COMPD_STMT:
        {
            scope++;
            generateIC((n->v)[0]);
            break;
        }
        case LOOP_STMT:
        {
            string entryLabel = genNewLabel();
            s = entryLabel;
            ircode.push_back(s);
            
            qcode.push_back(s);
            s="LABEL";
            qcode.push_back(s);
            s=entryLabel;
            qcode.push_back(s);
            quadform.push_back(qcode);
            qcode.clear();
            
            string r1=generateIC((n->v)[0]);
            string bodyLabel = genNewLabel();
            string exitLabel = genNewLabel();
            loopExitLabel.push_back(exitLabel);
            
            s = "if " + r1 +  " goto " + bodyLabel;
            ircode.push_back(s);
            
            qcode.push_back(s);
            s="IF";
            qcode.push_back(s);
            qcode.push_back(r1);
            qcode.push_back(bodyLabel);
            quadform.push_back(qcode);
            qcode.clear();
            
            s = "goto " + exitLabel;
            ircode.push_back(s);
            
            qcode.push_back(s);
            s="GOTO";
            qcode.push_back(s);
            qcode.push_back(exitLabel);
            quadform.push_back(qcode);
            qcode.clear();
            
            s = bodyLabel;
            ircode.push_back(s);
            
            qcode.push_back(s);
            s="LABEL";
            qcode.push_back(s);
            qcode.push_back(bodyLabel);
            quadform.push_back(qcode);
            qcode.clear();

            generateIC((n->v)[1]);
            s = "goto " + entryLabel;
            ircode.push_back(s);

            qcode.push_back(s);
            s="GOTO";
            qcode.push_back(s);
            qcode.push_back(entryLabel);
            quadform.push_back(qcode);
            qcode.clear();

            s = exitLabel;
            ircode.push_back(s);

            qcode.push_back(s);
            s="LABEL";
            qcode.push_back(s);
            qcode.push_back(exitLabel);
            quadform.push_back(qcode);
            qcode.clear();
            loopExitLabel.pop_back();
            break;
        }
        case IF_STMT:
        {
            string r1=generateIC((n->v)[0]);
            string trueLabel = genNewLabel();
            string falseLabel = genNewLabel();
            s = "if " + r1 + " goto " + trueLabel;
            ircode.push_back(s);

            qcode.push_back(s);
            s="IF";
            qcode.push_back(s);
            qcode.push_back(r1);
            qcode.push_back(trueLabel);
            quadform.push_back(qcode);
            qcode.clear();

            s = "goto " + falseLabel;
            ircode.push_back(s);

            qcode.push_back(s);
            s="GOTO";
            qcode.push_back(s);
            qcode.push_back(falseLabel);
            quadform.push_back(qcode);
            qcode.clear();
            s = trueLabel;
            ircode.push_back(s);

            qcode.push_back(s);
            s="LABEL";
            qcode.push_back(s);
            qcode.push_back(trueLabel);
            quadform.push_back(qcode);
            qcode.clear();

            generateIC((n->v)[1]);
            s = falseLabel;
            ircode.push_back(s);

            qcode.push_back(s);
            s="LABEL";
            qcode.push_back(s);
            qcode.push_back(falseLabel);
            quadform.push_back(qcode);
            qcode.clear();
            if((n->v).size()==3)
                generateIC((n->v)[2]);
            break;
        }
        case ASSIGN_STMT:
        {
            s = generateIC((n->v)[2]);
            t= " := " ;
            u =generateIC((n->v)[0]);
            ircode.push_back(u+t+s);

            qcode.push_back(u+t+s);
            qcode.push_back("ASSIGN");
            qcode.push_back(u);
            qcode.push_back(s);
            quadform.push_back(qcode);
            qcode.clear();
            break;
        }
        case OP:
        {
            string r = getNewReg();
            s = r + " := " ;
            t= generateIC((n->v)[2]);
            u= ((n->v)[1])->label;
            v= generateIC((n->v)[0]);
            ircode.push_back(s+v+u+t);

            qcode.push_back(s+v+u+t);
            qcode.push_back(u);
            qcode.push_back(r);
            qcode.push_back(v);
            qcode.push_back(t);
            quadform.push_back(qcode);
            qcode.clear();
            res = r;
            break;
        }
        case CALL_FUNC :
        {
            int params = 0;
            node *temp = (n->v)[1];
            while((temp->v).size()>0)
            {
                s = "param ";
                t= generateIC((temp->v)[0]);
                ircode.push_back(s+t);

                qcode.push_back(s+t);
                qcode.push_back("PAR");
                qcode.push_back(t);
                params++;
                quadform.push_back(qcode);
                qcode.clear();
                if((temp->v).size() == 1)
                    break;
                temp = (temp->v)[1];
            }
            string r = getNewReg();
            s = "param " + r;
            ircode.push_back(s);

            qcode.push_back(s);
            qcode.push_back("PAR");
            qcode.push_back(r);
            quadform.push_back(qcode);
            qcode.clear();

            s = "call func " ;
            t= generateIC((n->v)[0]);
            ircode.push_back(s+ t);

            qcode.push_back(s+ t + ", " + to_string(params+1));
            qcode.push_back("FCALL");
            qcode.push_back(t);
            qcode.push_back("");
            qcode.push_back(to_string(params+1));
            quadform.push_back(qcode);
            qcode.clear();
            return r;
        }
        case ARRAY :
        {
            string final, r1 = getNewReg();
            s = r1 + " := addr(" ;
            t= generateIC((n->v)[0]) ;
            u= ")";
            ircode.push_back(s+t+u);

            qcode.push_back(s+t+u);
            qcode.push_back("ADDR");
            qcode.push_back(r1);
            qcode.push_back(t);
            quadform.push_back(qcode);
            qcode.clear();

            node *temp = (n->v)[1];
            while((temp->v).size() > 0)
            {
                string nr = getNewReg(), multiplier="4", dt = n->data_type;
                if(dt == "char")
                    multiplier = "1";
                if(dt == "float")
                    multiplier = "8";
                s = nr + " := ";
                t= generateIC((temp->v)[0]);
                u= "*" ;
                v=multiplier;
                ircode.push_back(s+t+u+v);

                qcode.push_back(s+t+u+v);
                qcode.push_back("*");
                qcode.push_back(nr);
                qcode.push_back(t);
                qcode.push_back(v);
                quadform.push_back(qcode);
                qcode.clear();

                final = getNewReg();
                s = final + " := " + r1 + "[" + nr + "]";
                ircode.push_back(s);

                qcode.push_back(s);
                qcode.push_back("ARR_REF");
                qcode.push_back(r1);
                qcode.push_back(nr);
                quadform.push_back(qcode);
                qcode.clear();

                if((temp->v).size() == 1)
                    break;
                temp = (temp->v)[1];
                r1 = final;
            }
            return final;
        }
        case STRCT :
        {
            res = generateIC((n->v)[0]) + "." + ((n->v)[1])->label;
            break;
        }
        case RET :
        {
            if((n->v).size()>0)
            {
                s="return ";
                t=generateIC((n->v)[0]);
                ircode.push_back(s+t);

                qcode.push_back(s+t);
                qcode.push_back("RET");
                qcode.push_back(t);
                quadform.push_back(qcode);
                qcode.clear();
            }
            else
            {
                ircode.push_back("return");

                qcode.push_back("return");
                qcode.push_back("RET");
                quadform.push_back(qcode);
                qcode.clear();
            }
            break;
        }
        case BRK:
        {
            ircode.push_back("goto " + loopExitLabel[loopExitLabel.size()-1]);

            qcode.push_back("goto " + loopExitLabel[loopExitLabel.size()-1]);
            qcode.push_back("GOTO");
            qcode.push_back(loopExitLabel[loopExitLabel.size()-1]);
            quadform.push_back(qcode);
            qcode.clear();
            break;
        }
        case ID:
        {
            res = "Symb[" + to_string(findCompleteTable(n->label)) + "]";
            break;
        }
        case DEREFPTR:
        {
            res = "@"+generateIC((n->v)[0]);
            break;
        }
        case REFPTR:
        {
            res = "addr("+generateIC((n->v)[0])+")";
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
    return res;
}

/* Print quadruple format */
void printQuadTable()
{
    string x = "--------------------------------------------------------------------------------------------------------------------------------------";
    
    printf("\n\n");
    printf("Quadruple form\n");
    cout<<x<<endl;
    printf("| %-63s| %-15s| %-15s| %-15s| %-15s|\n", "Three-Address-Code" ,"Operator", "Op1", "Op2","Op3");
    cout<<x<<endl;

    for(int i=0;i<quadform.size();i++)
    {
        printf("| %-63s",quadform[i][0].c_str());
        for(int j=1;j<5;j++)
        {
            if(j<quadform[i].size())
                printf("| %-15s",quadform[i][j].c_str());
            else
                printf("| %-15s","");
        }
        printf("|");
        cout<<endl;
    }
    cout<<x<<endl;
}

/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "cparse.y" /* yacc.c:339  */

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
	map<string, string> v;
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

void printVec(vector<string> v)
{
	for(vector<string>::iterator it=v.begin();it!=v.end();it++)
		cout << *it << " ";
	cout << endl;
}

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

map<string, string> getAllVarsCurrentScope()
{
	int n = symbolTable.size();
	return symbolTable[n-1];
}

vector<string> getParamTypes(node *a)
{
	vector<string> ans;
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

bool similarDataType(node *a, node *b)
{
	if(a && b)
		if(a->data_type == b->data_type)
			return true;
	return false;
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
void printSymTable()
{
	map<string, string>::iterator it;
	int n = symbolTable.size();
	for(it=symbolTable[n-1].begin();it!=symbolTable[n-1].end();it++)
		cout << it->first << " " << it->second << endl;
}


#line 283 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    ELSE = 259,
    WHILE = 260,
    DO = 261,
    FOR = 262,
    RETURN = 263,
    CONTINUE = 264,
    BREAK = 265,
    STRUCT = 266,
    STRING_LITERAL = 267,
    CHAR_LITERAL = 268,
    DATA_TYPE = 269,
    INT_LITERAL = 270,
    FLOAT_LITERAL = 271,
    IDENTIFIER = 272,
    ADD_ASSIGN = 273,
    SUB_ASSIGN = 274,
    MUL_ASSIGN = 275,
    DIV_ASSIGN = 276,
    MOD_ASSIGN = 277,
    AND_ASSIGN = 278,
    XOR_ASSIGN = 279,
    OR_ASSIGN = 280,
    RIGHT_OP = 281,
    LEFT_OP = 282,
    INC_OP = 283,
    DEC_OP = 284,
    REL_OP = 285,
    MATH_OP = 286,
    EQUAL_OP = 287
  };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define WHILE 260
#define DO 261
#define FOR 262
#define RETURN 263
#define CONTINUE 264
#define BREAK 265
#define STRUCT 266
#define STRING_LITERAL 267
#define CHAR_LITERAL 268
#define DATA_TYPE 269
#define INT_LITERAL 270
#define FLOAT_LITERAL 271
#define IDENTIFIER 272
#define ADD_ASSIGN 273
#define SUB_ASSIGN 274
#define MUL_ASSIGN 275
#define DIV_ASSIGN 276
#define MOD_ASSIGN 277
#define AND_ASSIGN 278
#define XOR_ASSIGN 279
#define OR_ASSIGN 280
#define RIGHT_OP 281
#define LEFT_OP 282
#define INC_OP 283
#define DEC_OP 284
#define REL_OP 285
#define MATH_OP 286
#define EQUAL_OP 287

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 398 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   98

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  53
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  88

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      33,    34,     2,     2,    38,     2,    39,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    37,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   256,   256,   259,   260,   262,   263,   264,   265,   268,
     288,   268,   295,   310,   315,   310,   329,   330,   333,   334,
     337,   338,   339,   341,   354,   354,   364,   365,   368,   369,
     370,   371,   372,   373,   376,   379,   380,   383,   386,   387,
     388,   389,   398,   416,   422,   435,   436,   437,   440,   444,
     445,   452,   453,   454
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "WHILE", "DO", "FOR",
  "RETURN", "CONTINUE", "BREAK", "STRUCT", "STRING_LITERAL",
  "CHAR_LITERAL", "DATA_TYPE", "INT_LITERAL", "FLOAT_LITERAL",
  "IDENTIFIER", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN",
  "MOD_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN", "RIGHT_OP",
  "LEFT_OP", "INC_OP", "DEC_OP", "REL_OP", "MATH_OP", "EQUAL_OP", "'('",
  "')'", "'{'", "'}'", "';'", "','", "'.'", "$accept", "start", "strt_",
  "ext_declaration", "function_definition", "$@1", "@2",
  "function_declaration", "struct_decl", "$@3", "@4", "declaration_list",
  "id_list", "arg_list", "declaration_stmt", "compound_stmt", "@5",
  "stmt_list", "stmt", "loop_stmt", "if_stmt", "expr_stmt", "expr", "op",
  "id", "data_type", "param_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    40,    41,   123,   125,    59,    44,    46
};
# endif

#define YYPACT_NINF -57

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-57)))

#define YYTABLE_NINF -27

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      41,   -13,   -57,     8,   -57,    41,   -57,   -57,   -57,   -57,
     -13,   -57,   -18,   -57,   -57,   -19,    -3,   -57,   -57,    59,
     -13,    59,   -13,     0,   -13,   -57,    -2,   -57,    59,   -13,
     -57,    53,     7,    17,   -57,   -57,   -57,    59,    32,    26,
     -57,   -57,   -57,    56,    58,   -57,   -57,   -57,   -57,   -57,
     -57,   -57,    26,   -57,   -57,   -57,    35,   -26,    34,    34,
      26,    38,   -57,   -57,   -57,   -57,   -57,    34,    34,   -13,
      45,    50,    47,   -57,    55,   -16,    60,   -57,    43,    43,
     -57,    34,   -57,    88,   -57,   -57,    43,   -57
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,    49,     0,     2,     3,     5,     6,     8,     7,
       0,    48,    50,     1,     4,     0,    18,    13,    23,    20,
       0,     0,     0,     0,     0,    19,    18,    14,    16,     0,
      50,     0,    21,     0,    17,     9,    12,    20,     0,     0,
      22,    15,    33,     0,     0,    39,    38,    40,    24,    28,
      29,    10,     0,    30,    31,    32,     0,    41,     0,     0,
       0,     0,    27,    46,    47,    45,    37,     0,    51,     0,
       0,     0,     0,    11,    43,    52,     0,    42,     0,     0,
      25,    51,    44,    35,    34,    53,     0,    36
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   -57,    90,   -57,   -57,   -57,   -57,   -57,   -57,   -57,
     -57,    65,    76,    61,     5,   -57,   -57,   -28,   -15,   -57,
     -57,   -57,   -56,   -57,    -1,     1,    16
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    39,    61,     7,     8,    21,
      33,    27,    15,    23,    49,    50,    60,    51,    52,    53,
      54,    55,    56,    67,    57,    29,    76
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      12,    10,    70,    71,    11,     9,    10,    68,    13,    16,
       9,    74,    75,    69,    63,    64,    65,    17,    18,    26,
      24,    30,    81,    32,    62,    75,    28,    42,    26,    43,
      19,    44,    72,    28,    31,    20,    20,    22,    24,    45,
       2,    46,    47,    11,    42,    37,    43,    45,    44,    46,
      47,    11,     1,    38,    22,     2,    45,     2,    46,    47,
      11,    48,   -26,    83,    84,    63,    64,    65,    77,    41,
      22,    87,    66,     2,    73,    63,    64,    65,    48,    78,
      63,    64,    65,    80,    79,    63,    64,    65,    35,    58,
      36,    59,    86,    34,    82,    14,    25,    85,    40
};

static const yytype_uint8 yycheck[] =
{
       1,     0,    58,    59,    17,     0,     5,    33,     0,    10,
       5,    67,    68,    39,    30,    31,    32,    35,    37,    20,
      19,    22,    38,    24,    52,    81,    21,     1,    29,     3,
      33,     5,    60,    28,    34,    38,    38,    11,    37,    13,
      14,    15,    16,    17,     1,    38,     3,    13,     5,    15,
      16,    17,    11,    36,    11,    14,    13,    14,    15,    16,
      17,    35,    36,    78,    79,    30,    31,    32,    69,    37,
      11,    86,    37,    14,    36,    30,    31,    32,    35,    34,
      30,    31,    32,    36,    34,    30,    31,    32,    35,    33,
      37,    33,     4,    28,    34,     5,    20,    81,    37
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    11,    14,    41,    42,    43,    44,    47,    48,    54,
      65,    17,    64,     0,    42,    52,    64,    35,    37,    33,
      38,    49,    11,    53,    65,    52,    64,    51,    54,    65,
      64,    34,    64,    50,    51,    35,    37,    38,    36,    45,
      53,    37,     1,     3,     5,    13,    15,    16,    35,    54,
      55,    57,    58,    59,    60,    61,    62,    64,    33,    33,
      56,    46,    57,    30,    31,    32,    37,    63,    33,    39,
      62,    62,    57,    36,    62,    62,    66,    64,    34,    34,
      36,    38,    34,    58,    58,    66,     4,    58
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    43,    43,    45,
      46,    44,    47,    49,    50,    48,    51,    51,    52,    52,
      53,    53,    53,    54,    56,    55,    57,    57,    58,    58,
      58,    58,    58,    58,    59,    60,    60,    61,    62,    62,
      62,    62,    62,    62,    62,    63,    63,    63,    64,    65,
      65,    66,    66,    66
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     0,
       0,    10,     6,     0,     0,     8,     1,     2,     1,     3,
       0,     2,     4,     3,     0,     4,     0,     2,     1,     1,
       1,     1,     1,     1,     5,     5,     7,     2,     1,     1,
       1,     1,     3,     3,     4,     1,     1,     1,     1,     1,
       2,     0,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 256 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0]));}
#line 1540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 259 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(0);}
#line 1546 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 260 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(2,1,(yyvsp[0]));}
#line 1552 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 262 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0]));}
#line 1558 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 263 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0]));}
#line 1564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 264 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0]));}
#line 1570 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 265 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0]));}
#line 1576 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 268 "cparse.y" /* yacc.c:1646  */
    {	
																map<string, string>  temp;
																symbolTable.push_back(temp);	
																if(!isPresent((yyvsp[-4])->label))
																{
																	function_dt *temp = new function_dt;
																	temp->type = (yyvsp[-5])->label;
																	temp->v = giveArgList((yyvsp[-2]));
																	functionTable[(yyvsp[-4])->label] = *temp;
																	addListToSymTable((yyvsp[-2]));
																}
																// else if(sameFunction($1->label, $2->label, $4))
																// {
																// 	addListToSymTable($4);
																// 	$$ = make_node(4,$1,$2,$4,$6);
																// }
																else
																	printMultiDeclMsg((yyvsp[-4])->label);
															}
#line 1600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 288 "cparse.y" /* yacc.c:1646  */
    {
																	(yyval) = make_node(4,(yyvsp[-7]),(yyvsp[-6]),(yyvsp[-4]),(yyvsp[0]));
																
															}
#line 1609 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 292 "cparse.y" /* yacc.c:1646  */
    {symbolTable.pop_back();addToSymTable((yyvsp[-8])->label, (yyvsp[-9])->label);}
#line 1615 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 295 "cparse.y" /* yacc.c:1646  */
    { 
														if(!isPresent((yyvsp[-4])->label))
														{
															function_dt *temp = new function_dt;
															temp->type = (yyvsp[-5])->label;
															temp->v = giveArgList((yyvsp[-2]));
															functionTable[(yyvsp[-4])->label] = *temp;
															addToSymTable((yyvsp[-4])->label, (yyvsp[-5])->label);
															(yyval) = make_node(3,(yyvsp[-5]),(yyvsp[-4]),(yyvsp[-2]));
														}
														else
															printMultiDeclMsg((yyvsp[-4])->label);
													}
#line 1633 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 310 "cparse.y" /* yacc.c:1646  */
    {	
										map<string, string>  temp;
										symbolTable.push_back(temp);	
									}
#line 1642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 315 "cparse.y" /* yacc.c:1646  */
    {
										if(!isPresent((yyvsp[-3])->label))
										{
											struct_dt *temp = new struct_dt;
											temp->v = getAllVarsCurrentScope();
											structTable[(yyvsp[-3])->label] = *temp;
											(yyval) = make_node(2,(yyvsp[-3]),(yyvsp[0]));			
										}
										else
											printMultiDeclMsg((yyvsp[-3])->label);
									}
#line 1658 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 326 "cparse.y" /* yacc.c:1646  */
    {symbolTable.pop_back();addToSymTable((yyvsp[-6])->label, "struct");}
#line 1664 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 329 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, (yyvsp[0]));}
#line 1670 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 330 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(2, (yyvsp[-1]), (yyvsp[0]));}
#line 1676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 333 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, (yyvsp[0]));}
#line 1682 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 334 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(2, (yyvsp[-2]), (yyvsp[0]));}
#line 1688 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 337 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(0);}
#line 1694 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 338 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(2, (yyvsp[-1]), (yyvsp[0]));}
#line 1700 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 339 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(3, (yyvsp[-3]),(yyvsp[-2]),(yyvsp[0]));}
#line 1706 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 341 "cparse.y" /* yacc.c:1646  */
    {
												vector<string> list = getIdList((yyvsp[-1]));
												for(vector<string>::iterator it=list.begin();it!=list.end();it++)
												{
													if(isPresentCurrentScope(*it))
														printMultiDeclMsg(*it);
													else
														addToSymTable(*it, (yyvsp[-2])->label);
												}
												(yyval) = make_node(2, (yyvsp[-2]), (yyvsp[-1]));
											}
#line 1722 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 354 "cparse.y" /* yacc.c:1646  */
    {	
									map<string, string>  temp;
									symbolTable.push_back(temp);	
								}
#line 1731 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 358 "cparse.y" /* yacc.c:1646  */
    { 
									symbolTable.pop_back();
									(yyval) = make_node(1,(yyvsp[-2]));
								}
#line 1740 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 364 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(0); }
#line 1746 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 365 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(2,(yyvsp[-1]),(yyvsp[0])); }
#line 1752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 368 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0])); }
#line 1758 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 369 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0])); }
#line 1764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 370 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0])); }
#line 1770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 371 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0])); }
#line 1776 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 372 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0])); }
#line 1782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 376 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(2,(yyvsp[-3]),(yyvsp[-2])); }
#line 1788 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 379 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(2,(yyvsp[-2]),(yyvsp[0])); }
#line 1794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 380 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(3,(yyvsp[-4]),(yyvsp[-2]),(yyvsp[0])); }
#line 1800 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 383 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[-1])); }
#line 1806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 386 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, make_terminal_node(string(yytext, yyleng), int_node)); (yyval)->data_type = "int";}
#line 1812 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 387 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, make_terminal_node(string(yytext, yyleng), char_node)); (yyval)->data_type = "char";}
#line 1818 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 388 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, make_terminal_node(string(yytext, yyleng), float_node)); (yyval)->data_type = "float";}
#line 1824 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 389 "cparse.y" /* yacc.c:1646  */
    { 
										if(!isPresent((yyvsp[0])->label))
											printUndefinedMsg((yyvsp[0])->label);
										else
										{
											(yyval) = make_node(1,(yyvsp[0])); 
											(yyval)->data_type = (yyvsp[0])->data_type;
										}
									}
#line 1838 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 398 "cparse.y" /* yacc.c:1646  */
    {
										if(!isPresent((yyvsp[-2])->label))
											printUndefinedMsg((yyvsp[-2])->label);
										else if(structTable.find(getDataType((yyvsp[-2])->label)) == structTable.end())
											printNotMember((yyvsp[-2])->label, (yyvsp[0])->label);
										else
										{
											map<string, string> m = ((structTable.find(getDataType((yyvsp[-2])->label)))->second).v;
											map<string, string>::iterator it = m.find((yyvsp[0])->label);
											if(it != m.end())
											{
												(yyval) = make_node(2,(yyvsp[-2]),(yyvsp[0]));
												(yyval)->data_type = it->second;
											}
											else
												printNotMember((yyvsp[-2])->label, (yyvsp[0])->label);
										}
									}
#line 1861 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 416 "cparse.y" /* yacc.c:1646  */
    {
										(yyval) = make_node(3,(yyvsp[-2]),(yyvsp[-1]),(yyvsp[0])); 
										(yyval)->data_type = "int";
										if(!similarDataType((yyvsp[-2]), (yyvsp[0])))
											printDtMismatch();
									}
#line 1872 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 422 "cparse.y" /* yacc.c:1646  */
    { 
										if(!isPresent((yyvsp[-3])->label))
											printUndefinedMsg((yyvsp[-3])->label);
										else if(similarArgs((yyvsp[-3]), (yyvsp[-1])))
										{
											(yyval) = make_node(2,(yyvsp[-3]),(yyvsp[-1]));
											(yyval)->data_type = (yyvsp[-3])->data_type;
										}
										else
											cout << "Arguments mismatch for " << (yyvsp[-3])->label << " at line no. " << yylineno << endl; 
									}
#line 1888 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 435 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}
#line 1894 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 436 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}
#line 1900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 437 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_node(1, make_terminal_node(string(yytext, yyleng), op_node));}
#line 1906 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 440 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_terminal_node(string(yytext, yyleng), id_node); (yyval)->data_type = getDataType(string(yytext, yyleng));}
#line 1912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 444 "cparse.y" /* yacc.c:1646  */
    {(yyval) = make_terminal_node(string(yytext, yyleng), dt_node);}
#line 1918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 445 "cparse.y" /* yacc.c:1646  */
    {
							if(!isPresent((yyvsp[0])->label) || getDataType((yyvsp[0])->label)!="struct")
								printUndefinedMsg("struct "+(yyvsp[0])->label);
							(yyval) = make_terminal_node((yyvsp[0])->label, dt_node);
						}
#line 1928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 453 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(1,(yyvsp[0]));}
#line 1934 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 454 "cparse.y" /* yacc.c:1646  */
    { (yyval) = make_node(2,(yyvsp[-2]),(yyvsp[0]));}
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1944 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 456 "cparse.y" /* yacc.c:1906  */


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

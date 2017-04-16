%{
    #include "y.tab.h"
    #include "stdio.h"
    static void comment(void);
    extern void yyerror(const char *);
%}

%s HASH
%option yylineno

string          \"([^\"]|\\.)*\"
charLit         \'([^\']|\\.)?\'

letter  [a-zA-Z]
digit [0-9]
ws [ \t\v\f\n]
digits {digit}+
number {digits}("."{digits})?(E("+"|"-")?{digits})?

dt ("char"|"double"|"float"|"int"|"long"|"short"|"signed"|"static"|"unsigned")

id {letter}({letter}|{digit})*

%%

"//".*                                  {}
"/*"                                    { comment(); }
"if"                                    {return IF;}
"else"                                  {return ELSE;}
"while"                                 {return WHILE;}
"do"                                    {return DO;}
"for"                                   {return FOR;}
"return"                                {return RETURN;}
"continue"                              {return CONTINUE;}
"break"                                 {return BREAK;}
"struct"                                {return STRUCT;}
{string}                                {return STRING_LITERAL;}
{charLit}                               {return CHAR_LITERAL;}
{dt}                                    {return DATA_TYPE;}
{digits}                                {return INT_LITERAL;}
{number}                                {return FLOAT_LITERAL;}
{id}                                    {return IDENTIFIER;}
"+="                                    {return(ADD_ASSIGN); }
"-="                                    {return(SUB_ASSIGN); }
"*="                                    {return(MUL_ASSIGN); }
"/="                                    {return(DIV_ASSIGN); }
"%="                                    {return(MOD_ASSIGN); }
"&="                                    {return(AND_ASSIGN); }
"^="                                    {return(XOR_ASSIGN); }
"|="                                    {return(OR_ASSIGN); }
">>"                                    {return(RIGHT_OP); }
"<<"                                    {return(LEFT_OP); }
"++"                                    {return(INC_OP); }
"--"                                    {return(DEC_OP); }
"=="                                    {return (REL_OP);}
">="                                    {return (REL_OP);}
"<="                                    {return (REL_OP);}
"<"                                     {return (REL_OP);}
">"                                     {return (REL_OP);}
"!="                                    {return (REL_OP);}
"+"                                     {return (MATH_OP);}
"-"                                     {return (MATH_OP);}
"/"                                     {return (MATH_OP);}
"*"                                     {return (MATH_OP);}
"="                                     {return (EQUAL_OP);}
";"                                     {return(';'); }
("{"|"<%")                              {return('{'); }
("}"|"%>")                              {return('}'); }
","                                     {return(','); }
":"                                     {return(':'); }
"("                                     {return('('); }
")"                                     {return(')'); }
("["|"<:")                              {return('['); }
("]"|":>")                              {return(']'); }
"."                                     {return('.'); }
"&"                                     {return('&'); }
"!"                                     {return('!'); }
"~"                                     {return('~'); }
"%"                                     {return('%'); }
"^"                                     {return('^'); }
"|"                                     {return('|'); }
"?"                                     {return('?'); }
{ws}                                    {}
.                                       {}

%%

int yywrap(void)
{
    return 1;
}

static void comment(void)
{
    int c;

    while ((c = yyinput()) != 0)
        if (c == '*')
        {
            while ((c = yyinput()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}
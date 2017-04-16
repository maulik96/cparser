yacc -d -Wnone  cparse.y
lex cparse.l 
g++ y.tab.c lex.yy.c -o parser
./parser < in.c
yacc -d -Wnone  cparse.y
lex cparse.l 
g++ -std=c++11 y.tab.c lex.yy.c -o parser
./parser < in.c
rm parser
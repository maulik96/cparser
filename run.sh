yacc -d  cparse.y
lex cparse.l 
g++ y.tab.c lex.yy.c
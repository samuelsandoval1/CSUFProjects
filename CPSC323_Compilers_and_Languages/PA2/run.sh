yacc -d synAnalyzer.y
lex lexParser.l
gcc lex.yy.c y.tab.c -w
./a.out
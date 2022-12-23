# PA 2 Assignment

## Description 
Compile by running:
``` 
sh run.sh
```

This will execute:
```
yacc parser_source.y -d
flex lexical_analyzer.l
gcc lex.yy.c y.tab.c -w
./a.out

```

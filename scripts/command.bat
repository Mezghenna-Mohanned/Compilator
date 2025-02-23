@echo off
flex Lexical.l
bison -d syntax.y
gcc lex.yy.c syntax.tab.c -o compilateur.exe -lm
echo Test 1:
compilateur.exe < test1.txt
echo.
echo Test 2:
compilateur.exe < test2.txt
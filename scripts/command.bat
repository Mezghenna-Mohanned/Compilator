@echo off
REM Run Flex and Bison
flex Lexical.l
bison -d syntax.y

REM Compile with GCC (Windows does NOT need -lfl or -ly)
gcc lex.yy.c syntax.tab.c -o compilateur.exe -lm

REM Run tests
echo Test 1:
compilateur.exe < test1.txt

echo.
echo Test 2:
compilateur.exe < test2.txt

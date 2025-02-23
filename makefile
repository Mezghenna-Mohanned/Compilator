CC=gcc
BISON=bison
FLEX=flex

all:
    $(BISON) -d src/syntax/syntax.y
    $(FLEX) src/lexical/Lexical.l
    $(CC) src/lexical/lex.yy.c src/syntax/syntax.tab.c -o src/compiler/compilateur -lm
    echo "Build complete!"

clean:
    rm -f src/syntax/syntax.tab.c src/syntax/syntax.tab.h src/lexical/lex.yy.c src/compiler/compilateur
    echo "Cleaned up!"

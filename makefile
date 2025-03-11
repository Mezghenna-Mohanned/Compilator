CC = gcc
CFLAGS = -Wall -g
LEX = flex
YACC = bison

SRC = src/lexical/Lexical.l src/syntax/syntax.y src/syntax/syntax.tab.c src/syntax/syntax.tab.h src/semantique.c src/ts.c

EXEC = compilateur

LEX_OUTPUT = src/lexical/lex.yy.c
YACC_OUTPUT_C = src/syntax/syntax.tab.c
YACC_OUTPUT_H = src/syntax/syntax.tab.h

all: $(EXEC)

$(LEX_OUTPUT): src/lexical/Lexical.l
	$(LEX) -o $(LEX_OUTPUT) src/lexical/Lexical.l

$(YACC_OUTPUT_C): src/syntax/syntax.y
	$(YACC) -d src/syntax/syntax.y

$(EXEC): $(SRC) $(LEX_OUTPUT) $(YACC_OUTPUT_C) $(YACC_OUTPUT_H)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(LEX_OUTPUT) $(YACC_OUTPUT_C)

clean:
	rm -f $(EXEC) $(LEX_OUTPUT) $(YACC_OUTPUT_C) $(YACC_OUTPUT_H) src/semantique.o src/ts.o
run: $(EXEC)
	./$(EXEC)

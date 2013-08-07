$GCC = gcc
all:
	flex user_mod.lex
	gcc lex.yy.c -lfl -o a.out
clean:
	rm a.out
	rm lex.yy.c
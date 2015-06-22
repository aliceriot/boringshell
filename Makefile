CC=gcc
CFLAGS=-I -lm
DEPS = src/shell.h

out/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

vizmake: out/shell.o out/main.o
	$(CC) -lm -o shell.bin out/main.o out/shell.o
	

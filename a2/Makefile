# Makefile para o A2 - Programação II

all: csvreader

csvreader: csvreader.o io.o
	gcc -Wall -Wextra csvreader.o io.o -o csvreader

csvreader.o: csvreader.c
	gcc -Wall -Wextra csvreader.c io.c -o csvreader.o

io.o: io.c
	gcc -Wall -Wextra io.c -o io.o

clean:
	rm -rf *.o *~ csvreader

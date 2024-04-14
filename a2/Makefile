# Makefile para o A2 - Programação II

all: csvreader

csvreader: csvreader.o io.o
	gcc -Wall -Wextra csvreader.o io.o -o csvreader -lm -g

csvreader.o: csvreader.c
	gcc -Wall -Wextra -c csvreader.c
		
io.o: io.c
	gcc -Wall -Wextra -c io.c -lm

clean:
	rm -rf *.o *~

purge: clean
	rm -rf csvreader

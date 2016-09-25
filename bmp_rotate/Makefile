all: main.o bmp.o
	gcc main.o bmp.o -o main
main.o: main.c bmp.h
	gcc -std=c99 -c main.c
bmp.o: bmp.c bmp.h
	gcc -std=c99 -c bmp.c
clean:
	rm -f main.o bmp.o main

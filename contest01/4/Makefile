all: main
	
main: 4.o main.o
	gcc -o main 4.o main.o

main.o: main.c
	gcc -c -o main.o main.c

4.o: 4.c
	gcc -c -o 4.o 4.c -Wall -Werror


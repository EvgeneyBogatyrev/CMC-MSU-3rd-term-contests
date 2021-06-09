all: main
	
main: 5.o function.o
	gcc -Wall -Werror -Wextra -D_FILE_OFFSET_BITS=64 -o a 5.o function.o

5.o: 5.c
	gcc -c -o 5.o 5.c

clear: 
	rm 5.o a


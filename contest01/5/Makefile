all: main
main: f.o
	gcc -m32 -o 5 f.o
f.o: 5.s
	as --32 5.s -o f.o
clean: main_clean
	rm -rf *.o
main_clean:
	rm 5

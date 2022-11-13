all: build

build: tema1.c
	gcc -o tema1 tema1.c -g -Wall -lm
clean:
	rm -f tema1
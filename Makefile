all: trps trpc

trps: trps.c
	gcc -o trps trps.c

trpc: trpc.c
	gcc -o trpc trpc.c

clean:
	rm trps trpc

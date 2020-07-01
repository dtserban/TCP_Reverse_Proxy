all: trps trpc

trps: trps.c
	gcc -o trps trps.c -lpthread

trpc: trpc.c
	gcc -o trpc trpc.c -lpthread

clean:
	rm trps trpc

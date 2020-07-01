all: trps trpc

trps: trps.c
	gcc -o trps -lpthread trps.c

trpc: trpc.c
	gcc -o trpc -lpthread trpc.c

clean:
	rm trps trpc

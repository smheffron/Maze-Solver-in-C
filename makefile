CFLAGS = -g

final: final.o vector.o graph.o
	gcc final.o vector.o graph.o -g -o final

vector.o: src/vector.c
	gcc src/vector.c -g -c

final.o: src/final.c
	gcc src/final.c -g -c

graph.o: src/graph.c
	gcc src/graph.c -g -c

clean:
	rm *.o final

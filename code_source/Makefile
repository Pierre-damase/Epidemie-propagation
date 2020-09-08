CFLAGS=-Wall -lm -g

all: yersin

yersin: main.o plague.o listechaine.o stanford.o erdosrenyi.o anneau.o clique.o simulation.o prairie.o watts.o barabasi.o
	gcc -o yersin main.o plague.o listechaine.o stanford.o erdosrenyi.o anneau.o clique.o simulation.o prairie.o watts.o barabasi.o $(CFLAGS)
plague.o: plague.c plague.h simulation.h listechaine.h stanford.h barabasi.h
	gcc -o plague.o -c plague.c $(CFLAGS)
listechaine.o: listechaine.c listechaine.h plague.h
	gcc -o listechaine.o -c listechaine.c $(CFLAGS)
stanford.o: stanford.c stanford.h plague.h listechaine.h
	gcc -o stanford.o -c stanford.c $(CFLAGS)
erdosrenyi.o: erdosrenyi.c erdosrenyi.h plague.h listechaine.h
	gcc -o erdosrenyi.o -c erdosrenyi.c $(CFLAGS)
anneau.o: anneau.c anneau.h plague.h listechaine.h
	gcc -o anneau.o -c anneau.c $(CFLAGS)
clique.o: clique.c clique.h plague.h listechaine.h
	gcc -o clique.o -c clique.c $(CFLAGS)
simulation.o: simulation.c simulation.h plague.h listechaine.h stanford.h erdosrenyi.h anneau.h clique.h prairie.h watts.h barabasi.h
	gcc -o simulation.o -c simulation.c $(CFLAGS)
prairie.o: prairie.c prairie.h plague.h listechaine.h
	gcc -o prairie.o -c prairie.c $(CFLAGS)
watts.o: watts.c watts.h plague.h listechaine.h
	gcc -o watts.o -c watts.c $(CFLAGS)
barabasi.o: barabasi.c barabasi.h plague.h listechaine.h clique.h simulation.h
	gcc -o barabasi.o -c barabasi.c $(CFLAGS)
main.o: main.c plague.h
	gcc -o main.o -c main.c $(CFLAGS)

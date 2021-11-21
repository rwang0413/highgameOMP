CC = g++
CFLAGS = -fopenmp -O2

all: highgame 

highgame: highgame.cpp highgame.hpp
	$(CC) $(CFLAGS) highgame.cpp highgame.hpp -o highgame -lm

clean:
	rm -f highgame	

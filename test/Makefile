all: main.o config.o
	g++ main.o config.o

main.o: main.cc
	g++ -std=c++11 -c main.cc

config.o: config.cc
	g++ -std=c++11 -c config.cc

clean:
	rm ./*.o
	rm ./a.out

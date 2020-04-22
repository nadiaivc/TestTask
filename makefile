all: prog

prog: CSVTable.o main.o
	g++ CSVTable.o main.o -o prog

main.o: main.cpp
	g++ -c main.cpp -std=c++11

CSVTable.o: CSVTable.cpp
	g++ -c CSVTable.cpp -std=c++11

clean:
	rm -rf *.o prog

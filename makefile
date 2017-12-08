all: BigNumber.o main.o
	g++ -Wall -o keizervanvelzen4 BigNumber.o main.o
BigNumber.o:
	g++ -Wall -c BigNumber.cpp
main.o:
	g++ -Wall -c main.cpp
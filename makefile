all: BigNumber.o main.o
	g++ -Wall -o keizervanvelzen4 keizervanvelzen_bignumber.o keizervanvelzen_main.o
keizervanvelzen_bignumber.o:
	g++ -Wall -c keizervanvelzen_bignumber.cpp
keizervanvelzen_main.o:
	g++ -Wall -c keizervanvelzen_main.cpp
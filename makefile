all: keizervanvelzen_bignumber.o keizervanvelzen_main.o
	g++ -Wall -std=c++11 -o keizervanvelzen4 keizervanvelzen_bignumber.o keizervanvelzen_main.o
keizervanvelzen_bignumber.o:
	g++ -Wall -std=c++11 -c keizervanvelzen_bignumber.cpp
keizervanvelzen_main.o:
	g++ -Wall -std=c++11 -c keizervanvelzen_main.cpp

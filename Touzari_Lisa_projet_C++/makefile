CXXFLAGS  =  -std=c++11 -Wall -Werror -Wextra -pedantic -O3

all: main

main: main.cpp Myclass.cpp Grade.o
	g++ main.cpp Myclass.cpp Grade.o $(CXXFLAGS) -o main
	
clean: 
	rm main.o Myclass.o

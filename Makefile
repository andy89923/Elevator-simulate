CXX=g++
CFLAGS=-std=c++17 -Wall -O3 -pedantic -pthread

all: elevator controller 

UTILS = socket.cpp

elevator:
	$(CXX) elevator.cpp $(UTILS) -o elevator $(CFLAGS)

controller:
	$(CXX) controller.cpp $(UTILS) -o controller $(CFLAGS)

clean:
	rm -f elevator controller
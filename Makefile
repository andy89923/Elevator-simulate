CXX=g++
CFLAGS=-std=c++17 -Wall -O3 -pedantic -pthread

all: elevator controller 

UTILSDIR = utils
UTILS = $(wildcard $(UTILSDIR)/*.cpp)

elevator: elevator.cpp $(UTILS)
	$(CXX) elevator.cpp -I $(UTILSDIR) -o elevator $(CFLAGS)

controller: controller.cpp $(UTILS)
	$(CXX) controller.cpp -I $(UTILSDIR) -o controller $(CFLAGS)

clean:
	rm -f elevator controller
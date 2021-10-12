# collects all build commands for a project

CXX=g++

# target 
minimal: square.o minimal.o include/square.hpp # dependencies
	$(CXX) square.o minimal.o -o minimal     # generate the target file "minimal"

square.o: include/square.hpp lib/square.cpp # collect all dependencies
	$(CXX) -c lib/square.cpp -o square.o   # produce "square.o"

minimal.o: include/square.hpp minimal.cpp # collect all dependencies
	$(CXX) -c minimal.cpp -o minimal.o   # produce "minimal.o"

clean:
	rm square.o minimal.o minimal
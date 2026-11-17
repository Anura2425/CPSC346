# FILE: Makefile
# DESC: Builds the HW-4 driver and unit tests.
#
#   make        build the driver (hw4) and the unit tests
#   make test   build and run the unit tests
#   make clean  remove built files

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

all: hw4 run_tests

hw4: main.o adj_list.o dfs_algos.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp adj_list.h dfs_algos.h
	$(CXX) $(CXXFLAGS) -c main.cpp

run_tests: test_main.o adj_list_test.o dfs_algos_test.o adj_list.o dfs_algos.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# doctest's main function: compiled once and reused on later builds
test_main.o: test_main.cpp doctest.h
	$(CXX) $(CXXFLAGS) -c test_main.cpp

adj_list_test.o: adj_list_test.cpp adj_list.h doctest.h
	$(CXX) $(CXXFLAGS) -c adj_list_test.cpp

dfs_algos_test.o: dfs_algos_test.cpp dfs_algos.h adj_list.h doctest.h
	$(CXX) $(CXXFLAGS) -c dfs_algos_test.cpp

adj_list.o: adj_list.cpp adj_list.h
	$(CXX) $(CXXFLAGS) -c adj_list.cpp

dfs_algos.o: dfs_algos.cpp dfs_algos.h adj_list.h
	$(CXX) $(CXXFLAGS) -c dfs_algos.cpp

test: run_tests
	./run_tests

clean:
	rm -f hw4 run_tests *.o

.PHONY: all test clean

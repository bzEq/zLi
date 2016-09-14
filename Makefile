.PHONY: all clean

CXX := g++
CXXFLAGS := -Wall -Weffc++ -g -std=c++14

all: source test

source:
	$(MAKE) -C src
test:
	$(MAKE) -C tests

clean:
	$(MAKE) -C tests clean
	$(MAKE) -C src clean

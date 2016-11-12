.PHONY: all clean source test

CXX := clang++
CXXFLAGS := -Wall -Weffc++ -g -std=c++14

all: test

source:
	$(MAKE) -C src

test: source
	$(MAKE) -C tests

clean:
	$(MAKE) -C tests clean
	$(MAKE) -C src clean

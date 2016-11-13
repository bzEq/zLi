.PHONY: all clean source test

CXX := clang++
HEADERS := include/
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS)
SRC := src/
LDFLAGS := -lm -lpthread -L$(SRC) -lzli

all: main

source:
	$(MAKE) -C src

test: source
	$(MAKE) -C tests

main: main.cc source tests
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@rm -vf main
	$(MAKE) -C tests clean
	$(MAKE) -C src clean

.PHONY: all clean source test

CXX := clang++
HEADERS := include/
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS) -O2
SRC := src/
LDFLAGS := -lm -lpthread -L$(SRC) -lzli -lboost_program_options -lX11

all: main

source:
	$(MAKE) -C src

test: source
	$(MAKE) -C tests

main: main.cc source test
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@rm -vf main
	$(MAKE) -C tests clean
	$(MAKE) -C src clean

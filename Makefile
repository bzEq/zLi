.PHONY: all clean

CXX := g++
CXXFLAGS := -Wall -g -std=c++11
OBJECTS :=

all: $(OBJECTS) test

test:
	$(MAKE) -C tests

clean:
	$(MAKE) -C tests clean

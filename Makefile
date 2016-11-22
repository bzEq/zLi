.PHONY: all clean source test

CXX := clang++
HEADERS := include/
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS) -O2
SRC := src/
LDFLAGS := -lm -lpthread -L$(SRC) -lzli -lboost_program_options -lX11 \
	  $(shell pkg-config --libs OpenEXR)

all: zlirender

source:
	$(MAKE) -C src

test: source
	$(MAKE) -C tests

zlirender: zlirender.cc source test
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@rm -vf zlirender
	$(MAKE) -C tests clean
	$(MAKE) -C src clean

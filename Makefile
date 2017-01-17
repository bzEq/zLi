.PHONY: all clean test

KL := kl
KL_LIB := libkl.a
CXX := g++
CXXFLAGS := -Wall -g -std=c++14 -O2
LDFLAGS := -lpthread -L. -lkl -lzli -lpthread -lm -lboost_program_options -lX11 \
	$(shell pkg-config --libs OpenEXR)

ZLI_LIB := libzli.a
OBJECTS := $(patsubst %.cc, %.o, $(wildcard *.cc))

all: zlirender

$(KL_LIB): $(KL)
	@cd $< && $(MAKE) all && cp libkl.a ../

$(KL):
	@git submodule update --remote $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -fPIC -c $<

$(ZLI_LIB): $(OBJECTS)
	@ar rcsv $@ $^

zlirender: zlirender.o $(KL_LIB) $(ZLI_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test:
	$(MAKE) -C tests

clean:
	@rm -rvf *.o *.a *.so

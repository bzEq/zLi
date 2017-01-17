.PHONY: all

KL_LIB := ../libkl.a
ZLI_LIB := ../libzli.a
CXX := g++
CXXFLAGS := -Wall -g -std=c++14
LDFLAGS := -lpthread -L.. -lkl -lzli
TESTS := $(patsubst %.cc, %, $(wildcard *_test.cc))

all: $(TESTS)
	@for test in $^; do ./$$test || exit 1; done
	@echo "==== CONG! ALL TESTS PASSED."

$(TESTS): $(ZLI_LIB) $(KL_LIB)
	$(MAKE) -C ..

$(TESTS): %_test: %_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	@rm -rvf *.o *.a *.so $(TESTS)

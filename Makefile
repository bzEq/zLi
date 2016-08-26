.PHONY: all clean

CXX := g++
CXXFLAGS := -Wall -g -std=c++11
OBJECTS :=
TESTS := geometry_test

all: $(OBJECTS) test

test: $(TESTS)

%: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

clean:
	@rm -f $(TESTS)

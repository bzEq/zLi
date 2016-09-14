.PHONY: all clean

CXX := g++
HEADERS := ../include
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS)
LDFLAGS :=

OBJECTS := integrator.o

all: $(OBJECTS)

%.o: %.cc
	$(CXX) -c $< $(CXXFLAGS)

clean:
	@rm -f $(OBJECTS)

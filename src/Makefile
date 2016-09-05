.PHONY: all clean

CXX := g++
HEADERS := ../include
CXXFLAGS := -Wall -g -std=c++11 -I$(HEADERS)
LDFLAGS := -lxcb -lvulkan

OBJECTS := integrator.o window.o

all: $(OBJECTS)

%.o: %.cc
	$(CXX) -c $< $(CXXFLAGS)

clean:
	@rm -f $(OBJECTS)

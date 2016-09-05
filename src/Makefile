.PHONY: all clean

CXX := g++
CXXFLAGS := -Wall -g -std=c++11
INCLUDEDIR := ../include
INCLUDE := -I../include
LIBS := -lxcb

OBJECTS := integrator.o window.o

all: $(OBJECTS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

clean:
	@rm -f $(OBJECTS)

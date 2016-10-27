.PHONY: all clean

CXX := clang++
HEADERS := ../include
CXXFLAGS := -Wall -g -O2 -std=c++14 -I$(HEADERS)
LDFLAGS := -lfltk -lpthread

OBJECTS := main

all: $(OBJECTS)

%.o: %.cc
	$(CXX) -c $< $(CXXFLAGS)

main: main.o logging.o renderer.o triangle.o sphere.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	@rm -f $(OBJECTS)

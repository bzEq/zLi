.PHONY: all clean

CXX := clang++
HEADERS := ../include
CXXFLAGS := -Wall -g -O2 -std=c++14 -I$(HEADERS)
LDFLAGS := -lpthread
SOURCE := $(wildcard *.cc)
OBJECTS := $(patsubst %.cc,%.o,$(SOURCE))

all: $(OBJECTS)

%.o: %.cc
	$(CXX) -c $< $(CXXFLAGS)

clean:
	@rm -f $(OBJECTS)

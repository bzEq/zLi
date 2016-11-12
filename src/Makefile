.PHONY: all clean

CXX := clang++
HEADERS := ../include
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS)
LDFLAGS :=
SOURCE := $(wildcard *.cc)
OBJECTS := $(patsubst %.cc,%.o,$(SOURCE))
STATICLIB := libzli.a

all: $(STATICLIB)

%.o: %.cc
	$(CXX) -fPIC -c $< $(CXXFLAGS)

$(STATICLIB): $(OBJECTS)
	ar rcsv $@ $^

clean:
	@rm -vf *.a *.o *.so

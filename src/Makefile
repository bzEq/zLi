.PHONY: all clean

CXX := clang++
HEADERS := ../include
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS)
LDFLAGS := -lm -lpthread -L. -lzli
SOURCE := $(wildcard *.cc)
OBJECTS := $(patsubst %.cc,%.o,$(SOURCE))
STATICLIB := libzli.a

all: $(STATICLIB)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -fPIC -c $<

$(STATICLIB): $(OBJECTS)
	ar rcsv $@ $^

clean:
	@rm -vf *.a *.o *.so

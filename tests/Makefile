.PHONY: all clean

CXX := clang++
HEADERS := ../include
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS)
SRC := ../src
LDFLAGS := -lpthread -L../src -lzli
TESTS := math_test ray_test boundbox_test kdtree_test triangle_test \
	sphere_test camera_test diagram2d_test


all: $(TESTS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

math_test: math_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

ray_test: ray_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

kdtree_test: kdtree_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

boundbox_test: boundbox_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

triangle_test: triangle_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

sphere_test: sphere_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

camera_test: camera_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

diagram2d_test: diagram2d_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	@rm -vf *.o
	@rm -vf $(TESTS)

.PHONY: all clean

CXX := clang++
HEADERS := ../include
SRC := ../src
ZLILIB := -L$(SRC) -lzli
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS)
LDFLAGS := -lpthread -lm -lX11 $(ZLILIB)
TESTS := math_test ray_test boundbox_test kdtree_test triangle_test \
	sphere_test camera_test diagram2d_test filter_test window_test \
	CIE_test


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

filter_test: filter_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

window_test: window_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

CIE_test: CIE_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	@rm -vf *.o
	@rm -vf $(TESTS)

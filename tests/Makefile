.PHONY: all clean

CXX := g++
HEADERS := ../include
CXXFLAGS := -Wall -g -std=c++11 -I$(HEADERS)
SRC := ../src
LDFLAGS := -lxcb -lvulkan
TESTS := geometry_test ray_test boundbox_test kdtree_test triangle_test \
	sphere_test camera_test window_test


all: $(TESTS)

geometry_test: geometry_test.cc $(HEADERS)/geometry.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

ray_test: ray_test.cc $(HEADERS)/ray.h $(HEADERS)/geometry.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

kdtree_test: kdtree_test.cc $(HEADERS)/kdtree.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

boundbox_test: boundbox_test.cc $(HEADERS)/boundbox.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

triangle_test: triangle_test.cc $(HEADERS)/triangle.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

sphere_test: sphere_test.cc $(HEADERS)/sphere.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

camera_test: camera_test.cc $(HEADERS)/camera.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

window_test: window_test.cc 
	$(CXX) $(CXXFLAGS) $< $(SRC)/window.o -o $@ $(LDFLAGS)

clean:
	rm -f $(TESTS)

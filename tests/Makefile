.PHONY: all clean

CXX := g++
HEADERS := ../include
CXXFLAGS := -Wall -g -std=c++14 -I$(HEADERS)
SRC := ../src
LDFLAGS :=
TESTS := geometry_test ray_test boundbox_test kdtree_test triangle_test \
	sphere_test camera_test


all: $(TESTS)

geometry_test: geometry_test.cc $(HEADERS)/geometry.hh
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

ray_test: ray_test.cc $(HEADERS)/ray.hh $(HEADERS)/geometry.hh
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

kdtree_test: kdtree_test.cc $(HEADERS)/kdtree.hh
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

boundbox_test: boundbox_test.cc $(HEADERS)/boundbox.hh
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

triangle_test: triangle_test.cc $(HEADERS)/triangle.hh
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

sphere_test: sphere_test.cc $(HEADERS)/sphere.hh
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

camera_test: camera_test.cc $(HEADERS)/camera.hh
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TESTS)

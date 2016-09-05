.PHONY: all clean

CXX := g++
CXXFLAGS := -Wall -g -std=c++11
INCLUDEDIR := ../include
INCLUDE := -I../include
SRCDIR := ../src
LIBS := -lxcb
TESTS := geometry_test ray_test boundbox_test kdtree_test triangle_test \
	sphere_test camera_test window_test


all: $(TESTS)

geometry_test: geometry_test.cc $(INCLUDEDIR)/geometry.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

ray_test: ray_test.cc $(INCLUDEDIR)/ray.h $(INCLUDEDIR)/geometry.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

kdtree_test: kdtree_test.cc $(INCLUDEDIR)/kdtree.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

boundbox_test: boundbox_test.cc $(INCLUDEDIR)/boundbox.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

triangle_test: triangle_test.cc $(INCLUDEDIR)/triangle.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

sphere_test: sphere_test.cc $(INCLUDEDIR)/sphere.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

camera_test: camera_test.cc $(INCLUDEDIR)/camera.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LIBS)

window_test: window_test.cc 
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< $(SRCDIR)/window.o -o $@ $(LIBS)

clean:
	rm -f $(TESTS)

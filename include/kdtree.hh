#ifndef _ZLI_KDTREE_HH_
#define _ZLI_KDTREE_HH_
#include "boundbox.hh"
#include "geometry.hh"
#include "math.hh"
#include "ray.hh"

#include <iostream>
#include <stack>
#include <vector>

namespace zLi {
class KdTree {
public:
  KdTree(KdTree &&) = default;
  KdTree(const std::vector<Geometry> &);
  KdTree(std::vector<Geometry> &&);
  std::optional<RaySurfaceIntersection> Intersect(const Ray &);
  ~KdTree();

private:
  static constexpr const int NonAxis = -1;
  struct Node {
    Node *child[2];
    BoundBox bounds;
    int axis;
    Float plane;
    std::unique_ptr<Geometry> geometry;
    Node();
    ~Node();
    Node(const Node &) = delete;
    Node(Node &&) = default;
    void Insert(Geometry &&);
  };
  KdTree() = default;
  KdTree(const KdTree &) = default;
  void Insert(Geometry &&);
  Node *root_;
};
} // zLi
#endif

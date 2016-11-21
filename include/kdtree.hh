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
  static KdTree BuildKdTree(std::vector<Geometry> &&gs);
  KdTree(KdTree &&) = default;
  std::optional<RaySurfaceIntersection> Intersect(const Ray &);
  ~KdTree();

private:
  static constexpr const int NonAxis = -1;
  struct Node {
    Node *child[2];
    int split_axis;
    Float split_plane;
    std::unique_ptr<Geometry> geometry;

    Node();
    ~Node();
    Node(const Node &) = delete;
    Node(Node &&) = default;
    void Insert(int, const Geometry &);
  };
  KdTree() : root_(nullptr) {}
  KdTree(const KdTree &) = default;
  KdTree(BoundBox &&b) : root_(nullptr), world_(std::move(b)) {}
  Node *root_;
  BoundBox world_;
};
} // zLi
#endif

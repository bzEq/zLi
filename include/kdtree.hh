// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_KDTREE_HH_
#define _ZLI_KDTREE_HH_
#include <iostream>
#include <memory>
#include <stack>
#include <utility>
#include <vector>

#include "boundbox.hh"
#include "geometry.hh"
#include "math.hh"
#include "ray.hh"

namespace zLi {
class KdTree {
public:
  static KdTree BuildKdTree(std::vector<Geometry> &&gs);
  KdTree(KdTree &&) = default;
  std::optional<RaySurfaceIntersection> Intersect(const Ray &);
  BoundBox World();
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
  explicit KdTree(BoundBox &&b) : root_(nullptr), world_(std::move(b)) {}
  Node *root_;
  BoundBox world_;
};
}  // namespace zLi
#endif

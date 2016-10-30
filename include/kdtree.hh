#ifndef _ZLI_KDTREE_HH_
#define _ZLI_KDTREE_HH_
#include "boundbox.hh"
#include "math.hh"
#include "ray.hh"

#include <iostream>
#include <stack>
#include <vector>

namespace zLi {

struct KdNode;

struct Todo {
  KdNode *node;
  Float tmin, tmax;
  Todo(KdNode *node, Float tmin, Float tmax)
      : node(node), tmin(tmin), tmax(tmax) {}
};

struct KdNode {
  int axis;
  Float d;
  BoundBox box;
  KdNode *child[2];
  int Side(const Vector3f &p) const { return p[axis] > d; }
  int OtherSide(const Vector3f &p) const { return !Side(p); }
  KdNode(const BoundBox &box) : axis(-1), d(0), box(box) {
    child[0] = nullptr;
    child[1] = nullptr;
  }
  KdNode(int axis, Float d, const BoundBox &box) : axis(axis), d(d), box(box) {
    child[0] = nullptr;
    child[1] = nullptr;
  }
  std::optional<std::tuple<Float, Float>> Intersect(const Ray &ray) const {
    return box.Intersect(ray);
  }
  ~KdNode() {
    delete child[0];
    delete child[1];
  }
};

struct KdTree {
  KdNode root;
  KdTree() : root(BoundBox()) {}
  KdTree(const std::vector<BoundBox> &boxList);
  void Traverse(const Ray &ray);
};

} // end namespace zLi

#endif

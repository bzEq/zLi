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

struct KdNode;

class KdTree {
public:
  KdTree() : root_(BoundBox()) {}
  KdTree(const std::vector<Geometry> &g);
  std::optional<RaySurfaceIntersection> Intersect(const Ray &ray);

private:
  struct KdNode;
  struct Todo;

  struct Todo {
    KdNode *node;
    Float tmin, tmax;
    Todo(KdNode *node, Float tmin, Float tmax)
        : node(node), tmin(tmin), tmax(tmax) {}
  };

  struct KdNode {
    int axis;
    Float d;
    std::unique_ptr<Geometry> g;
    BoundBox box;
    KdNode *child[2];

    KdNode() {
      child[0] = nullptr;
      child[1] = nullptr;
    }

    KdNode(const BoundBox &box) : box(box) {
      child[0] = nullptr;
      child[1] = nullptr;
    }
    KdNode(int axis, Float d, const BoundBox &box)
        : axis(axis), d(d), box(box) {
      child[0] = nullptr;
      child[1] = nullptr;
    }

    int Side(const Vector3f &p) const { return p[axis] > d; }
    int OtherSide(const Vector3f &p) const { return !Side(p); }
    std::optional<std::tuple<Float, Float>> Intersect(const Ray &ray) const {
      return box.Intersect(ray);
    }
    void Insert(const Geometry &g);

    ~KdNode() {
      delete child[0];
      delete child[1];
    }
  };

  KdNode root_;
};

} // end namespace zLi

#endif

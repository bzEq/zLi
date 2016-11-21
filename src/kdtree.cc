#include "kdtree.hh"
#include "boundbox.hh"
#include "geometry.hh"
#include "logging.hh"
#include "ray.hh"

#include <queue>
#include <vector>

namespace zLi {

KdTree::Node::Node() : child{nullptr, nullptr}, axis(NonAxis) {}

KdTree::Node::~Node() {
  delete child[0];
  delete child[1];
}

KdTree::~KdTree() { delete root_; }

void KdTree::Node::Insert(Geometry &&g) {
  if (!geometry) {
    geometry = std::make_unique<Geometry>(std::move(g));
    return;
  } else if (axis == NonAxis) {
    Vector3f mid = bounds.Middle();
    axis = UniformInt(0, 999) % 3;
    plane = mid[axis];
    child[0] = new Node();
    child[1] = new Node();
    auto split = bounds.Split(axis, plane);
    assert(split);
    child[0]->bounds = std::get<0>(*split);
    child[1]->bounds = std::get<1>(*split);
  }
  BoundBox f = g.Bounds();
  if (f.pMin[axis] > plane) {
    return child[1]->Insert(std::move(g));
  } else if (f.pMax[axis] < plane) {
    return child[0]->Insert(std::move(g));
  } else {
    auto gg(g);
    child[0]->Insert(std::move(g));
    child[1]->Insert(std::move(gg));
  }
}

KdTree::KdTree(std::vector<Geometry> &&gs) : root_(nullptr) {
  root_ = new Node();
  for (auto &g : gs) {
    CheckGeometry(g);
    root_->bounds = Union(root_->bounds, g.Bounds());
  }
  for (auto &g : gs) {
    root_->Insert(std::move(g));
  }
}

std::optional<RaySurfaceIntersection> KdTree::Intersect(const Ray &ray) {
  auto test = root_->bounds.Intersect(ray);
  if (!test) {
    return {};
  }
  int loop_count = 0;
  int test_count = 0;

  std::unique_ptr<RaySurfaceIntersection> ret;
  std::queue<std::tuple<Node *, Float, Float>> q;
  q.push(std::make_tuple(root_, std::get<0>(*test), std::get<1>(*test)));
  while (!q.empty()) {
    ++loop_count;
    INFO("loop_count: %d", loop_count);
    auto t = q.front();
    q.pop();
    Node *current = std::get<0>(t);
    Float tmin = std::get<1>(t);
    Float tmax = std::get<2>(t);
    Ray r(ray.o, ray.d, tmin, tmax);
    assert(current);
    if (current->geometry) {
      CheckGeometry(*current->geometry);
      ++test_count;
      auto test = current->geometry->Intersect(r);
      if (test && (!ret || (*test).t < ret->t)) {
        ret = std::make_unique<RaySurfaceIntersection>(std::move(*test));
      }
    }
    if (current->axis == NonAxis) {
      continue;
    }
    assert(current->child[0] && current->child[1]);
    if (r.d[current->axis] == 0) {
      if (r.o[current->axis] <= current->plane) {
        q.push(std::make_tuple(current->child[0], tmin, tmax));
      } else {
        q.push(std::make_tuple(current->child[1], tmin, tmax));
      }
      continue;
    }
    Float tsplit = (current->plane - r.o[current->axis]) / r.d[current->axis];
    if (tmin <= tsplit && tsplit <= tmax) {
      if (!ret || tmin < ret->t) {
        q.push(std::make_tuple(current->child[0], tmin, tsplit));
      }
      if (!ret || tsplit < ret->t) {
        q.push(std::make_tuple(current->child[1], tsplit, tmax));
      }
    } else if (tsplit < tmin && (!ret || tmin < ret->t)) {
      q.push(std::make_tuple(current->child[1], tmin, tmax));
    } else if (tsplit > tmax) {
      q.push(std::make_tuple(current->child[0], tmin, tmax));
    }
  }
  INFO("test_count: %d", test_count);

  if (ret) {
    return *ret;
  }
  return {};
}
}

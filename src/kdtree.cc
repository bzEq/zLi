#include "kdtree.hh"
#include "boundbox.hh"
#include "geometry.hh"
#include "logging.hh"
#include "ray.hh"

#include <queue>
#include <vector>

namespace zLi {

KdTree::Node::Node() : axis(NonAxis) { child[0] = child[1] = nullptr; }

KdTree::Node::~Node() {
  delete child[0];
  delete child[1];
}

KdTree::~KdTree() { delete root_; }

void KdTree::Node::Insert(Geometry &&g) {
  if (!geometry) {
    geometry = std::make_unique<Geometry>(std::forward<Geometry>(g));
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
  Vector3f f = g.Bounds().Middle();
  if (f[axis] <= plane) {
    return child[0]->Insert(std::move(g));
  }
  return child[1]->Insert(std::move(g));
}

KdTree::KdTree(const std::vector<Geometry> &gs) : root_(nullptr) {
  root_ = new Node();
  for (const auto &g : gs) {
    root_->bounds = Union(root_->bounds, g.Bounds());
  }
  for (const auto &g : gs) {
    Insert(g);
  }
}

void KdTree::Insert(const Geometry &g) {
  auto gg(g);
  return Insert(std::move(gg));
}

void KdTree::Insert(Geometry &&g) { root_->Insert(std::move(g)); }

KdTree::KdTree(std::vector<Geometry> &&gs) : root_(nullptr) {
  root_ = new Node();
  for (auto &g : gs) {
    root_->bounds = Union(root_->bounds, g.Bounds());
  }
  for (auto &g : gs) {
    Insert(std::move(g));
  }
}

std::optional<RaySurfaceIntersection> KdTree::Intersect(const Ray &ray) const {
  auto test = root_->bounds.Intersect(ray);
  if (!test) {
    return {};
  }
  int loop_count = 0;
  int test_count = 0;

  std::unique_ptr<RaySurfaceIntersection> ret;
  std::queue<std::tuple<const Node *, Float, Float>> q;
  q.push(std::make_tuple((const Node *)&root_, ray.tmin, ray.tmax));
  while (!q.empty()) {
    ++loop_count;
    auto t = q.front();
    q.pop();
    const Node *current = std::get<0>(t);
    Float tmin = std::get<1>(t);
    Float tmax = std::get<2>(t);
    assert(current);
    if (current->geometry) {
      ++test_count;
      auto test = current->geometry->Intersect(ray);
      if (test && (!ret || (*test).t < ret->t)) {
        ret = std::make_unique<RaySurfaceIntersection>(std::move(*test));
      }
    }
    if (current->axis == NonAxis) {
      continue;
    }
    assert(current->child[0] && current->child[1]);
    Float tsplit =
        (current->plane - ray.o[current->axis]) / ray.d[current->axis];
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
  DEBUG("loop_count: %d", loop_count);
  DEBUG("test_count: %d", test_count);
  if (ret) {
    return *ret;
  }
  return {};
}
}

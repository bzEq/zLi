#include "kdtree.hh"
#include "boundbox.hh"
#include "geometry.hh"
#include "logging.hh"
#include "math.hh"

#include <queue>

namespace zLi {

KdTree::Node::Node() : child{nullptr, nullptr}, split_axis(NonAxis) {}

void KdTree::Node::Insert(int axis, const Geometry &g) {
  if (!geometry) {
    geometry = std::make_unique<Geometry>(g);
    return;
  }
  if (split_axis == NonAxis) {
    split_axis = axis;
    split_plane = ++axis & 1 ? geometry->Bounds().pMin[split_axis]
                             : geometry->Bounds().pMax[split_axis];
    child[0] = new Node();
    child[1] = new Node();
  }
  BoundBox b = g.Bounds();
  if (b.pMin[split_axis] > split_plane) {
    child[1]->Insert(++axis % 3, g);
  } else if (b.pMax[split_axis] < split_plane) {
    child[0]->Insert(++axis % 3, g);
  } else {
    child[0]->Insert(++axis % 3, g);
    child[1]->Insert(++axis % 3, g);
  }
}

KdTree KdTree::BuildKdTree(std::vector<Geometry> &&gs) {
  KdTree kd;
  for (auto &g : gs) {
    kd.world_ = Union(g.Bounds(), kd.world_);
  }
  kd.root_ = new Node();
  // int k = 0;
  for (auto &g : gs) {
    kd.root_->Insert(UniformInt(0, 1023) % 3, g);
  }
  return kd;
}

std::optional<RaySurfaceIntersection> KdTree::Intersect(const Ray &r) {
  auto test = world_.Intersect(r);
  if (!test) {
    return {};
  }
  std::queue<std::tuple<Node *, Float, Float>> q;
  q.push(std::make_tuple(root_, std::get<0>(*test), std::get<1>(*test)));
  std::unique_ptr<RaySurfaceIntersection> ans;
  while (!q.empty()) {
    auto task = q.front();
    q.pop();
    Node *current = std::get<0>(task);
    Float tmin = std::get<1>(task), tmax = std::get<2>(task);
    assert(tmin >= 0 && tmin <= tmax);
    Ray ray(r.o, r.d, tmin, tmax);
    if (current->geometry) {
      auto test = current->geometry->Intersect(ray);
      if (test && (!ans || ans->t > (*test).t)) {
        ans = std::make_unique<RaySurfaceIntersection>(std::move(*test));
      }
    }
    if (current->split_axis == NonAxis) {
      continue;
    }
    if (ray.d[current->split_axis] == 0) {
      if (ray.o[current->split_axis] < current->split_plane &&
          (!ans || ans->t > tmin)) {
        q.push(std::make_tuple(current->child[0], tmin, tmax));
      } else if (ray.o[current->split_axis] > current->split_plane &&
                 (!ans || ans->t > tmin)) {
        q.push(std::make_tuple(current->child[1], tmin, tmax));
      } else if (!ans || ans->t > tmin) {
        q.push(std::make_tuple(current->child[0], tmin, tmax));
        q.push(std::make_tuple(current->child[1], tmin, tmax));
      }
      continue;
    }
    Float tsplit = (current->split_plane - ray.o[current->split_axis]) /
                   ray.d[current->split_axis];
    assert(!std::isnan(tsplit));
    if (tmin <= tsplit && tsplit <= tmax) {
      if (ray(tmin)[current->split_axis] < current->split_plane) {
        if (!ans || ans->t > tmin) {
          q.push(std::make_tuple(current->child[0], tmin, tsplit));
        }
        if (!ans || ans->t > tsplit) {
          q.push(std::make_tuple(current->child[1], tsplit, tmax));
        }
      } else {
        if (!ans || ans->t > tmin) {
          q.push(std::make_tuple(current->child[1], tmin, tsplit));
        }
        if (!ans || ans->t > tsplit) {
          q.push(std::make_tuple(current->child[0], tsplit, tmax));
        }
      }
    } else {
      if (ray(tmin)[current->split_axis] < current->split_plane) {
        if (!ans || ans->t > tmin) {
          q.push(std::make_tuple(current->child[0], tmin, tmax));
        }
      } else {
        if (!ans || ans->t > tmin) {
          q.push(std::make_tuple(current->child[1], tmin, tmax));
        }
      }
    }
  }
  if (ans) {
    return *ans;
  }
  return {};
}

BoundBox KdTree::World() { return world_; }
KdTree::~KdTree() {}

} // .zLi

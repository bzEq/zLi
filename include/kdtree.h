#ifndef _ZLI_KDTREE_H_
#define _ZLI_KDTREE_H_
#include "geometry.h"
#include "boundbox.h"
#include "ray.h"

#include <boost/optional.hpp>
#include <stack>
#include <vector>
#include <iostream>

namespace zLi {

struct KdNode;

struct Todo {
  const KdNode* node;
  Float tmin, tmax;
  Todo(const KdNode* node, const Float tmin, const Float tmax)
    : node(node), tmin(tmin), tmax(tmax) {}
};

struct KdNode {
  int axis;
  Float d;
  BoundBox box;
  KdNode* child[2];
  int Side(const Vector3f& p) const { return p[axis] > d; }
  int OtherSide(const Vector3f& p) const { return !Side(p); }
  KdNode(const BoundBox& box): axis(-1), d(0), box(box) { child[0] = nullptr; child[1] = nullptr; }
  KdNode(const int axis, const Float d, const BoundBox& box)
    : axis(axis), d(d), box(box) { child[0] = nullptr; child[1] = nullptr; }
  boost::optional< std::tuple<Float, Float> > Intersect(const Ray& ray) const {
    return box.Intersect(ray);
  }
  ~KdNode() { delete child[0]; delete child[1]; }
};

struct KdTree {
  KdNode root;
  KdTree(): root(BoundBox()) {}
  KdTree(const std::vector<BoundBox>& boxList);
  void Traverse(const Ray& ray) const {
    auto test = root.Intersect(ray);
    if(!test) return;
    std::stack<Todo> todoList;
    todoList.push(Todo(&root, std::get<0>(*test), std::get<1>(*test)));
    while (!todoList.empty()) {
      Todo todo = todoList.top();
      todoList.pop();
      if (todo.node->axis == -1) {
        // leaf node
        continue;
      }
      assert(todo.node->child[0]);
      assert(todo.node->child[1]);
      auto side = todo.node->Side(ray(todo.tmin));
      auto otherSide = !side;
      auto axis = todo.node->axis;
      auto d = todo.node->d;
      auto tsplit = (d - ray.o[axis]) / ray.d[axis];
      if (tsplit >= todo.tmin && tsplit <= todo.tmax) {
        todoList.push(Todo(todo.node->child[otherSide], tsplit, todo.tmax));
        todoList.push(Todo(todo.node->child[side], todo.tmin, tsplit));
      } else {
        todoList.push(Todo(todo.node->child[side], todo.tmin, todo.tmax));
      }
    }
  }
};

} // end namespace zLi

#endif

#include "kdtree.hh"
#include "boundbox.hh"
#include "logging.hh"

#include <vector>

namespace zLi {

KdTree::KdTree(const std::vector<Geometry> &gs) {
  for (auto &g : gs) {
    root_.box = Union(root_.box, g.Bounds());
  }
  for (auto &g : gs) {
    root_.Insert(g);
  }
}

void KdTree::KdNode::Insert(const Geometry &g) {
  BoundBox b = g.Bounds();
  if (!this->g) {
    this->g = std::make_unique<Geometry>(g);
    return;
  }
  if (!child[0]) {
    assert(!child[1]);
    this->axis = UniformInt(0, 1023) % 3;
    this->d = (box.pMin[this->axis] + b.pMax[this->axis]) / 2;
    auto res = box.Split(this->axis, this->d);
    child[0] = new KdNode(std::get<0>(*res));
    child[1] = new KdNode(std::get<1>(*res));
    assert(!child[0]->g);
    assert(!child[1]->g);
    assert(this->axis >= 0 && this->axis < 3);
  }
  Vector3f mid = (b.pMin + b.pMax) * (Float)0.5;
  assert(this->axis >= 0 && this->axis < 3);
  if (mid[this->axis] <= this->d) {
    child[0]->Insert(g);
  } else {
    child[1]->Insert(g);
  }
}

std::optional<RaySurfaceIntersection> KdTree::Intersect(const Ray &ray) {
  int loop_count = 0;
  int compare_count = 0;
  std::unique_ptr<RaySurfaceIntersection> ret;
  auto test = root_.Intersect(ray);
  if (!test) {
    return {};
  }
  std::stack<Todo> todoList;
  todoList.push(Todo(&root_, std::get<0>(*test), std::get<1>(*test)));
  while (!todoList.empty()) {
    ++loop_count;
    Todo todo = todoList.top();
    todoList.pop();
    if (todo.node->g) {
      auto res = todo.node->g->Intersect(ray);
      if (res && (!ret || (*res).t < (*ret).t)) {
        ++compare_count;
        ret = std::make_unique<RaySurfaceIntersection>(*res);
      }
    }
    if (!todo.node->child[0]) {
      assert(!todo.node->child[1]);
      continue;
    }
    auto side = todo.node->Side(ray(todo.tmin));
    auto otherSide = !side;
    auto axis = todo.node->axis;
    auto d = todo.node->d;
    auto tsplit = (d - ray.o[axis]) / ray.d[axis];
    if (tsplit >= todo.tmin && tsplit <= todo.tmax) {
      if (!ret || tsplit < (*ret).t) {
        todoList.push(Todo(todo.node->child[otherSide], tsplit, todo.tmax));
      }
      if (!ret || todo.tmin < (*ret).t) {
        todoList.push(Todo(todo.node->child[side], todo.tmin, tsplit));
      }
    } else if (!ret || todo.tmin < (*ret).t) {
      todoList.push(Todo(todo.node->child[side], todo.tmin, todo.tmax));
    }
  }
  INFO("loop_count: %d", loop_count);
  INFO("compare_count: %d", compare_count);
  return *ret;
}

} // namespace zLi

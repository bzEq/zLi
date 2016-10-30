#include "kdtree.hh"

namespace zLi {
void KdTree::Traverse(const Ray &ray) {
  auto test = root.Intersect(ray);
  if (!test) {
    return;
  }
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

} // namespace zLi

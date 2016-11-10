#include "diagram2d.hh"

#include <iostream>

int main() {
  zLi::Float xs[5] = {0, 3, 2, 1, 4};
  zLi::Float ys[5] = {3, 13, 23, 34, 1};
  zLi::Diagram2d d(xs, ys, 5);
  d.Display(std::cout);
  return 0;
}

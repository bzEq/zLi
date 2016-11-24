// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_DIAGRAM2D_HH_
#define _ZLI_DIAGRAM2D_HH_
#include <iostream>
#include <tuple>
#include <vector>

#include "math.hh"

namespace zLi {
class Diagram2D {
public:
  Diagram2D() = delete;
  Diagram2D(const Diagram2D &) = default;
  Diagram2D(Diagram2D &&) = default;
  Diagram2D(const Float *xs, const Float *ys, size_t l);
  Diagram2D(const Float *xs, Float y, size_t l);

  Diagram2D &operator+=(const Diagram2D &);
  Diagram2D &operator+=(Float);

  Diagram2D &operator*=(const Diagram2D &);
  Diagram2D &operator*=(Float);
  void Display(std::ostream &out = std::cout);
  Float Query(Float x) const;

  friend Diagram2D operator*(Float lhs, const Diagram2D &rhs);
  friend Diagram2D operator*(const Diagram2D &lhs, Float rhs);
  friend Diagram2D operator*(const Diagram2D &lhs, const Diagram2D &rhs);

  friend Diagram2D operator+(Float lhs, const Diagram2D &rhs);
  friend Diagram2D operator+(const Diagram2D &lhs, Float rhs);
  friend Diagram2D operator+(const Diagram2D &lhs, const Diagram2D &rhs);

private:
  std::vector<std::tuple<Float, Float>> diagram_;
};
}  // namespace zLi
#endif

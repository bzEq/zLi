#ifndef _ZLI_DIAGRAM2D_HH_
#define _ZLI_DIAGRAM2D_HH_
#include "math.hh"

#include <iostream>
#include <vector>

namespace zLi {
class Diagram2d {
public:
  Diagram2d() = default;
  Diagram2d(const Diagram2d &) = default;
  Diagram2d(Diagram2d &&) = default;
  Diagram2d(Float *xs, Float *ys, size_t l);

  Diagram2d &operator+=(const Diagram2d &);
  Diagram2d &operator+=(Float);

  Diagram2d &operator*=(const Diagram2d &);
  Diagram2d &operator*=(Float);
  void Display(std::ostream &out = std::cout);
  Float Query(Float x) const;

private:
  std::vector<std::tuple<Float, Float>> diagram_;
};
}
#endif

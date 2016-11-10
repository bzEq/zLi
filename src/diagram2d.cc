#include "diagram2d.hh"

#include <algorithm>

namespace zLi {

Diagram2d::Diagram2d(Float *xs, Float *ys, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    diagram_.push_back(std::make_tuple(xs[i], ys[i]));
  }
  std::sort(diagram_.begin(), diagram_.end());
}

void Diagram2d::Display(std::ostream &out) {
  for (const auto &p : diagram_) {
    out << "(" << std::get<0>(p) << ", " << std::get<1>(p) << ")\n";
  }
}

std::optional<Float> Diagram2d::Query(Float x) {
  auto iter = std::lower_bound(
      diagram_.begin(), diagram_.end(), std::make_tuple(x, 0),
      [](const std::tuple<Float, Float> &a, const std::tuple<Float, Float> &b) {
        return std::get<0>(a) < std::get<0>(b);
      });
  if (iter == diagram_.begin() || iter == diagram_.end()) {
    return {};
  }
  int j = iter - diagram_.begin();
  int i = j - 1;
  return Lerp(std::get<0>(diagram_[i]), std::get<1>(diagram_[i]),
              std::get<0>(diagram_[j]), std::get<1>(diagram_[j]), x);
}
}

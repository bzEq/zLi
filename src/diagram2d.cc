#include "diagram2d.hh"

#include <algorithm>

namespace zLi {

Diagram2d::Diagram2d(const Float *xs, const Float *ys, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    diagram_.push_back(std::make_tuple(xs[i], ys[i]));
  }
  std::sort(diagram_.begin(), diagram_.end());
}

Diagram2d::Diagram2d(const Float *xs, Float y, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    diagram_.push_back(std::make_tuple(xs[i], y));
  }
  std::sort(diagram_.begin(), diagram_.end());
}

void Diagram2d::Display(std::ostream &out) {
  for (const auto &p : diagram_) {
    out << "(" << std::get<0>(p) << ", " << std::get<1>(p) << ")\n";
  }
}

Float Diagram2d::Query(Float x) const {
  auto iter = std::lower_bound(
      diagram_.begin(), diagram_.end(), std::make_tuple(x, 0),
      [](const std::tuple<Float, Float> &a, const std::tuple<Float, Float> &b) {
        return std::get<0>(a) < std::get<0>(b);
      });
  if (iter == diagram_.begin() || iter == diagram_.end()) {
    return 0;
  }
  int j = iter - diagram_.begin();
  int i = j - 1;
  return Lerp(std::get<0>(diagram_[i]), std::get<1>(diagram_[i]),
              std::get<0>(diagram_[j]), std::get<1>(diagram_[j]), x);
}

Diagram2d &Diagram2d::operator+=(const Diagram2d &rhs) {
  for (auto &p : diagram_) {
    std::get<1>(p) += rhs.Query(std::get<0>(p));
  }
  return *this;
}

Diagram2d &Diagram2d::operator+=(Float d) {
  for (auto &p : diagram_) {
    std::get<1>(p) += d;
  }
  return *this;
}

Diagram2d &Diagram2d::operator*=(const Diagram2d &rhs) {
  for (auto &p : diagram_) {
    std::get<1>(p) *= rhs.Query(std::get<0>(p));
  }
  return *this;
}

Diagram2d &Diagram2d::operator*=(Float f) {
  for (auto &p : diagram_) {
    std::get<1>(p) *= f;
  }
  return *this;
}

Diagram2d operator*(Float f, const Diagram2d &rhs) {
  Diagram2d res(rhs);
  for (auto &p : res.diagram_) {
    std::get<1>(p) *= f;
  }
  return res;
}

Diagram2d operator*(const Diagram2d &lhs, Float f) { return operator*(f, lhs); }
Diagram2d operator*(const Diagram2d &lhs, const Diagram2d &rhs) {
  Diagram2d res(lhs);
  for (auto &p : res.diagram_) {
    std::get<1>(p) *= rhs.Query(std::get<0>(p));
  }
  return res;
}

Diagram2d operator+(Float f, const Diagram2d &rhs) {
  Diagram2d res(rhs);
  for (auto &p : res.diagram_) {
    std::get<1>(p) += f;
  }
  return res;
}

Diagram2d operator+(const Diagram2d &lhs, Float f) { return operator*(f, lhs); }
Diagram2d operator+(const Diagram2d &lhs, const Diagram2d &rhs) {
  Diagram2d res(lhs);
  for (auto &p : res.diagram_) {
    std::get<1>(p) += rhs.Query(std::get<0>(p));
  }
  return res;
}
}

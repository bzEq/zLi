#ifndef _ZLI_FILTER_HH_
#define _ZLI_FILTER_HH_
#include "math.hh"

#include <cmath>

namespace zLi {
namespace filter {

class Tent1D {
public:
  Tent1D(Float r) : r_(r) {}
  Float f(Float x) { return tent(x / r_) / r_; }
  template <typename Func> Float Convolve(Func F, Float x) {
    auto conv = [&](Float t) { return f(t) * F(x - t); };
    return EstimateIntegration1D(conv, -r_, r_);
  }

private:
  static Float tent(Float x) {
    x = std::abs(x);
    return x >= 1 ? 0 : 1 - x;
  }
  Float r_;
};

class Box1D {
public:
  Box1D(Float r) : r_(r) {}
  Float f(Float x) {
    x = std::abs(x);
    if (x >= r_) {
      return 0;
    }
    return 1 / (2 * r_);
  }
  template <typename Func> Float Convolve(Func F, Float x) {
    auto conv = [&](Float t) { return f(t) * F(x - t); };
    return EstimateIntegration1D(conv, -r_, r_);
  }

private:
  Float r_;
};

class B_Spline1D {
public:
  static Float f(Float x) {
    x = std::abs(x);
    if (x <= 1) {
      auto y = 1 - x;
      return 1 / 6 * (-3 * std::pow(y, 3) + 3 * std::pow(y, 2) + 3 * y + 1);
    } else if (x <= 2) {
      auto y = 2 - x;
      return 1 / 6 * std::pow(y, 3);
    }
    return 0;
  }
  template <typename Func> static Float Convolve(Func F, Float x) {
    auto conv = [&](Float t) { return f(t) * F(x - t); };
    return EstimateIntegration1D(conv, -2, 2);
  }
};

class Gauss1D {
public:
  Gauss1D() : A_(1), fx_(1) {}
  Gauss1D(Float A, Float fx) : A_(A), fx_(fx) {}
  Float f(Float x) { return A_ * std::exp(-(x * x) / (2 * fx_ * fx_)); }
  template <typename Func> Float Convolve(Func F, Float x) {
    static const Float r = std::sqrt(fx_ * fx_);
    auto conv = [&](Float t) { return f(t) * F(x - t); };
    return EstimateIntegration1D(conv, -r, r);
  }

private:
  Float A_, fx_;
};

// class Gauss2D {
// public:
//   Gauss2D() : A_(1), fx_(1), fy_(1) {}
//   Gauss2D(Float A, Float fx, Float fy) : A_(A), fx_(fx), fy_(fy) {}
//   Float f(Float x, Float y) {
//     return A_ *
//            std::exp(-(x * x) / (2 * fx_ * fx_) - (y * y) / (2 * fy_ * fy_));
//   }
//   template <typename Func> Float Convolve(Func F, Float x, Float y) {
//     static const Float r = std::sqrt(fx_ * fx_ + fy_ * fy_);
//     auto conv = [&](Float u, Float v) { return f(u, v) * F(x - u, y - v); };
//     return EstimateIntegration2D(conv, 0, 0, r);
//   }

// private:
//   Float A_, fx_, fy_;
// };

} // filter
} // zLi
#endif

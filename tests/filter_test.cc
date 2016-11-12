#include "filter.hh"

#include <iostream>

int main() {
  using namespace zLi;
  zLi::filter::Gauss1D g1;
  auto s = EstimateIntegration1D([&](Float x) { return g1.f(x); }, -PI, PI,
                                 1024); // should be approximate to sqrt(2*PI)
  std::cout << s << std::endl;
  return 0;
}

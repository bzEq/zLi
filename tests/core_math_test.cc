// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <iostream>

#include "core_math.h"
#include "kl/testkit.h"

class T {};

TEST(T, Inverse) {
  // From octave
  //   m =
  //      1    23    34     8
  //     23    34   234    34
  //     26    42    13    32
  //     11    34    54    23
  // >> m^-1
  // ans =
  //    9.1219e-02   3.0614e-02   9.8309e-02  -2.1376e-01
  //    1.0318e-01   6.4001e-04   2.6859e-02  -7.4202e-02
  //    6.8947e-03   5.2155e-03  -1.4644e-03  -8.0706e-03
  //   -2.1233e-01  -2.7833e-02  -8.3283e-02   2.7435e-01
  zLi::Matrix4x4f m(1, 23, 34, 8, 23, 34, 234, 34, 26, 42, 13, 32, 11, 34, 54,
                    23);
  std::cout << *m.Inverse() << std::endl;
}

int main() { return KL_TEST(); }

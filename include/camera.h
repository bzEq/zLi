// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_CAMERA_H_
#define _ZLI_CAMERA_H_
#include <iostream>

#include "core_math.h"
#include "ray.h"

namespace zLi {

struct PerspectiveCamera {
  // position and direction in world space
  Vector3f e, front, up, right;
  // world2camera, camera2world;
  Transform wd2cam0, wd2cam, cam2wd0, cam2wd;
  Float fov, lensr, lensp;
  Float l;
  PerspectiveCamera() = default;
  PerspectiveCamera(const Vector3f &e_, const Vector3f &lookat_,
                    const Vector3f &up_, Float fov_ = 30, Float lensr_ = 0,
                    Float lensp_ = 0)
      : e(e_), front((lookat_ - e_).Normalize()), up(up_.Normalize()),
        right(up ^ front), fov(fov_), lensr(lensr_), lensp(lensp_),
        l(std::tan(fov_ * PI / 360)) {
    assert(front * up == 0);
    // FIXME: check if the matrix is singular
    wd2cam0 = *Matrix4x4f(right.x, up.x, front.x, 0, right.y, up.y, front.y, 0,
                          right.z, up.z, front.z, 0, 0, 0, 0, 1)
                   .Inverse();

    wd2cam = wd2cam0 * TranslateTransform(-e);

    cam2wd = *(wd2cam.Inverse());
    cam2wd0 = *(wd2cam0.Inverse());
  }
  Ray GenerateRay(Float u, Float v) const {
    assert(std::abs(u) <= 0.5);
    assert(std::abs(v) <= 0.5);
    Float x = u * l;
    Float y = v * l;
    Ray r(Vector3f(), Vector3f(x, y, 1).Normalize());
    if (lensr > 0) {
      Float t = lensp / r.d.z;
      Vector3f point = r(t);
      auto s = SampleFromDisk();
      r.o.x += lensr * std::get<0>(s);
      r.o.y += lensr * std::get<1>(s);
      r.d = (point - r.o).Normalize();
    }
    return Ray(*(cam2wd * r.o), (*(cam2wd0 * r.d)).Normalize());
    // return r;
  }
};

}  // end namespace zLi

#endif

#ifndef _ZLI_CAMERA_H_
#define _ZLI_CAMERA_H_
#include "geometry.hh"
#include "sample.hh"
#include "ray.hh"

#include <iostream>


namespace zLi {

struct PerspectiveCamera {
  Vector3f e, front, up, right; // position and direction in world space
  Transform wd2cam, cam2wd; // world2camera, camera2world;
  Float fov, lensr, lensp;
  Float l; // length of image
  PerspectiveCamera(const Vector3f& e_, const Vector3f& lookat_, const Vector3f& up_,
                    Float fov_ = 30, Float lensr_ = 0, Float lensp_ = 0)
    : e(e_), front((lookat_-e_).Normalize()), up(up_.Normalize()), right(front^up),
      fov(fov_), lensr(lensr_), lensp(lensp_), l(std::tan(fov_*PI/360)) {
    assert(front * up == 0);
    // FIXME: check if the matrix is singular
    wd2cam = *(Matrix4x4f(right.x, up.x, front.x, 0,
                          right.y, up.y, front.y, 0,
                          right.z, up.z, front.z, 0,
                          0,    0,    0,       1).Inverse()) * TranslateTransform(-e);
    cam2wd = *(wd2cam.Inverse());
  }
  Ray GenerateRay(Float u, Float v) const {
    assert(u >= 0 and u < 1);
    assert(v >= 0 and v < 1);
    Float x = (u-0.5)*l;
    Float y = (v-0.5)*l;
    Ray r(Vector3f(),
          Vector3f(x, y, 1).Normalize());
    if (lensr > 0) {
      Float t = lensp/r.d.z;
      Vector3f point = r(t);
      auto s = SampleFromDisk();
      r.o.x += lensr * std::get<0>(s);
      r.o.y += lensr * std::get<1>(s);
      r.d = (point-r.o).Normalize();
    }
    return Ray(*(cam2wd * r.o),
               *(cam2wd * r.d));
  }
};

} // end namespace zLi

#endif

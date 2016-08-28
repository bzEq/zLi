#ifndef _ZLI_GEOMETRY_H_
#define _ZLI_GEOMETRY_H_
#include <boost/optional.hpp>
#include <iostream>
#include <limits>
#include <cassert>
#include <cmath>
#include <cstring>


namespace zLi {

template<typename T>
struct Vector4;

template<typename T>
struct Vector3 {
  T x, y, z;
  Vector3(): x(0), y(0), z(0) {}
  Vector3(const T& x, const T& y, const T& z): x(x), y(y), z(z) {}
  bool IsZero() const { return x == 0 && y == 0 && z == 0; }
  T LengthSquared() const { return x * x + y * y + z * z; }
  T Length() const { return std::sqrt(LengthSquared()); }
  Vector3 Normalize() const {
    T ll = LengthSquared();
    if (ll == 0) { return Vector3(); }
    T f = 1 / std::sqrt(ll);
    return Vector3(x*f, y*f, z*f);
  }
  Vector3& operator+=(const Vector3<T>& v) {
    x += v.x; y += v.y; z += v.z;
    return *this;
  }
  Vector3& operator-=(const Vector3<T>& v) {
    x -= v.x; y -= v.y; z -= v.z;
    return *this;    
  }
  Vector3& operator*=(const T f) {
    x *= f; y *= f; z *= f;
    return *this;
  }
  bool HasNaNs() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }
  T operator[](int i) const { assert(i >= 0 && i < 3); return (&x)[i]; }
  T& operator[](int i) { assert(i >= 0 && i < 3); return (&x)[i]; }
  Vector4<T> ToVector4() const;
};

template<typename T>
struct Vector4 {
  T x, y, z, w;
  Vector4(): x(0), y(0), z(0), w(0) {}
  Vector4(const T x, const T y, const T z, const T w): x(x), y(y), z(z), w(w) {}
  Vector4(const Vector3<T>& v, T w): 
    x(v.x), y(v.y), z(v.z), w(w) {}
  boost::optional<Vector3<T>> ToVector3() const {
    if (w == 0) return {};
    return Vector3<T>(x/w, y/w, z/w);
  }
  bool HasNaNs() const { return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w); }  
  T operator[](int i) const { assert(i >= 0 && i < 4); return (&x)[i]; }
  T& operator[](int i) { assert(i >= 0 && i < 4); return (&x)[i]; }
};

template<typename T>
struct Matrix4x4 {
  T m[4][4];
  
  Matrix4x4() {
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] =
      m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0;    
  }
  
  Matrix4x4(const T mm[4][4]) {
    std::memcpy(m, mm, 16*sizeof(T));
  }
  
  Matrix4x4(const T m00, const T m01, const T m02, const T m03,
            const T m10, const T m11, const T m12, const T m13,
            const T m20, const T m21, const T m22, const T m23,
            const T m30, const T m31, const T m32, const T m33) {
    m[0][0]=m00; m[0][1]=m01; m[0][2]=m02; m[0][3]=m03;
    m[1][0]=m10; m[1][1]=m11; m[1][2]=m12; m[1][3]=m13;
    m[2][0]=m20; m[2][1]=m21; m[2][2]=m22; m[2][3]=m23;
    m[3][0]=m30; m[3][1]=m31; m[3][2]=m32; m[3][3]=m33;
  }
  
  Matrix4x4 Transpose() const {
    return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                     m[0][1], m[1][1], m[2][1], m[3][1],
                     m[0][2], m[1][2], m[2][2], m[3][2],
                     m[0][3], m[1][3], m[2][3], m[3][3]);
  }
  
  Matrix4x4 operator*(const Matrix4x4& rhs) const {
    Matrix4x4 r;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        r.m[i][j] = 
          m[i][0] * rhs.m[0][j] +
          m[i][1] * rhs.m[1][j] +
          m[i][2] * rhs.m[2][j] +
          m[i][3] * rhs.m[3][j];
      }
    }
    return r;    
  }
  
  boost::optional<Matrix4x4> Inverse() const {
    T minv[4][4];
    int ind[4] = { 0, 1, 2, 3 };
    std::memcpy(minv, m, 4*4*sizeof(T));
    for (int i = 0; i < 4; i++) {
      int pick = -1;
      for (int j = i; j < 4; j++) {
        if (std::abs(minv[j][i]) != 0) {
          pick = j;
          break;
        }
      }
      if (pick < 0) return {}; // singular matrix
      if (i != pick) {
        for (int j = 0; j < 4; j++) {
          std::swap(minv[i][j], minv[pick][j]);
        }
      }
      std::swap(ind[i], ind[pick]);
      
      T f = 1. / minv[i][i];
      minv[i][i] = 1.;
      for (int j = 0; j < 4; j++) {
        minv[i][j] *= f;
      }
      
      for (int j = 0; j < 4; j++) {
        if (i != j) {
          T old = minv[j][i];
          minv[j][i] = 0;
          for (int k = 0; k < 4; k++) minv[j][k] -= minv[i][k] * old;
        }
      }
    }
    
    for (int i = 0; i < 4; ) {
      if (ind[i] != i) {
        for (int j = 0; j < 4; j++) {
          std::swap(minv[j][i], minv[j][ind[i]]);
        }
        int tmp = ind[i];
        ind[i] = ind[tmp];
        ind[tmp] = tmp;
      } else
        i++;
    }
    return Matrix4x4(minv);
  }
};

template <typename T>
struct Quaternion {
  T r, i, j, k;
  Quaternion(): r(0), i(0), j(0), k(0) {}
  Quaternion(const T r, const T i, const T j, const T k): r(r), i(i), j(j), k(k) {}
  Quaternion(const T r, const Vector3<T>& v): r(r), i(v.x), j(v.y), k(v.z) {}
  Quaternion& operator+=(const Quaternion& q) {
    r += q.r; i += q.i; j += q.j; k += q.k;
    return *this;
  }
  Quaternion& operator-=(const Quaternion& q) {
    r -= q.r; i -= q.i; j -= q.j; k -= q.k;
    return *this;
  }
  Quaternion& operator*=(const T f) {
    r *= f; i *= f; j *= f; k *= f;
    return *this;
  }
  Quaternion& operator/=(const T f) {
    r /= f; i /= f; j /= f; k /= f;
    return *this;    
  }
  T operator[](int i) const { 
    assert(i >= 0 && i < 4);
    return (&r)[i];
  }
  T& operator[](int i) {
    assert(i >= 0 && i < 4);
    return (&r)[i];    
  }
  Quaternion Conjugation() const { return Quaternion(r, -i, -j, -k); }
  T NormSquared() const {
    return r * r + i * i + j * j + k * k;
  }
  T Norm() const {
    return std::sqrt(NormSquared());
  }
  Quaternion Inverse() const { 
    assert(!IsZero());
    return Conjugation() / NormSquared();
  }
  bool IsZero() const {
    return r == 0 && i == 0 && j == 0 && k == 0;
  }
  bool HasNaNs() const {
    return std::isnan(r) || std::isnan(i) || std::isnan(j) || std::isnan(k);
  }
};

template<typename T>
inline Vector4<T> Vector3<T>::ToVector4() const {
  return Vector4<T>(*this, 1);
}

template<typename T>
inline Vector3<T> operator+(const Vector3<T>& u, const Vector3<T>& v) {
  return Vector3<T>(u.x + v.x,
                    u.y + v.y,
                    u.z + v.z);
}

template<typename T>
inline Vector3<T> operator-(const Vector3<T>& u, const Vector3<T>& v) {
  return Vector3<T>(u.x - v.x,
                    u.y - v.y,
                    u.z - v.z);
}

template<typename T>
inline T operator*(const Vector3<T>& u, const Vector3<T>& v) {
  return 
    u.x * v.x + 
    u.y * v.y + 
    u.z * v.z;
}

template<typename T>
inline Vector3<T> operator^(const Vector3<T>& u, const Vector3<T>& v) {
  return Vector3<T>(u.y * v.z - u.z * v.y,
                    u.z * v.x - u.x * v.z,
                    u.x * v.y - u.y * v.x);
}

template<typename T>
inline Vector3<T> operator-(const Vector3<T>& v) {
  return Vector3<T>(-v.x, -v.y, -v.z);
}

template<typename T>
inline Vector3<T> operator*(const Vector3<T>& v, const T f) {
  return Vector3<T>(v.x*f, v.y*f, v.z*f);
}

template<typename T>
inline Vector3<T> operator*(const T f, const Vector3<T>& v) {
  return Vector3<T>(v.x*f, v.y*f, v.z*f);
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const Vector3<T>& v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return out;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const Vector4<T>& v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return out;
}

template<typename T>
inline Vector4<T> operator*(const Matrix4x4<T>& m, const Vector4<T>& v) {
  Vector4<T> ret;
  for (int i = 0; i < 4; i++) {
    ret[i] = 0;
    for (int j = 0; j < 4; j++) {
      ret[i] += m.m[i][j] * v[j];
    }
  }
  return ret;
}

template<typename T>
inline Vector3<T> operator*(const Matrix4x4<T>& m, const Vector3<T>& v) {
  return (m*(v.ToVector4())).ToVector3();
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const Matrix4x4<T>& m) {
  for (int i = 0; i < 4; i++) {
    out << "(" << m.m[i][0] << ", " << m.m[i][1] << ", " << m.m[i][2] << ", " << m.m[i][3] << ")" << std::endl;
  }
  return out;
}

template<typename T>
inline Quaternion<T> operator-(const Quaternion<T>& q) {
  return Quaternion<T>(-q.r, -q.i, -q.j, -q.k);
}

template<typename T>
inline Quaternion<T> operator+(const Quaternion<T>& p, const Quaternion<T>& q) {
  return Quaternion<T>(p.r + q.r,
                       p.i + q.i,
                       p.j + q.j,
                       p.k + q.k);
}

template<typename T>
inline Quaternion<T> operator-(const Quaternion<T>& p, const Quaternion<T>& q) {
  return Quaternion<T>(p.r - q.r,
                       p.i - q.i,
                       p.j - q.j,
                       p.k - q.k);
}

template<typename T>
inline Quaternion<T> operator*(const Quaternion<T>& p, const Quaternion<T>& q) {
  return Quaternion<T>(p.r * q.r - (p.i * q.i + p.j * q.j + p.k * q.k),
                       p.r * q.i + q.r * p.i + (p.j * q.k - p.k * q.j),
                       p.r * q.j + q.r * p.j + (p.k * q.i - p.i * q.k),
                       p.r * q.k + q.r * p.k + (p.i * q.j - p.j * q.i));
}

template<typename T>
inline Quaternion<T> operator*(const Quaternion<T>& q, const T f) {
  return Quaternion<T>(q.r * f,
                       q.i * f,
                       q.j * f,
                       q.k * f);
}

template<typename T>
inline Quaternion<T> operator*(const T f, const Quaternion<T>& q) {
  return Quaternion<T>(q.r * f,
                       q.i * f,
                       q.j * f,
                       q.k * f);
}

template<typename T>
inline Quaternion<T> operator/(const Quaternion<T>& q, const T f) {
  assert(f != 0);
  return Quaternion<T>(q.r / f,
                       q.i / f,
                       q.j / f,
                       q.k / f);
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const Quaternion<T>& q) {
  out << "(" << q.r << ", " << q.i << ", " << q.j << ", " << q.k << ")";
  return out;
}

typedef double Float;
typedef Vector3<Float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector4<Float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Matrix4x4<Float> Matrix4x4f;
typedef Matrix4x4<double> Matrix4x4d;
typedef Quaternion<Float> Quaternion4f;
typedef Quaternion<double> Quaternion4d;
typedef Matrix4x4f Transform;

const Float INF = std::numeric_limits<Float>::max();
const Float NINF = std::numeric_limits<Float>::lowest();
const Float EPSILON = 1e-6;
const Float PI = 4*std::atan((Float)1);

inline boost::optional< std::tuple<Float, Float > > Quadratic(Float a, Float b, Float c) {
  Float delta = b*b - 4*a*c;
  if (delta < 0) return {};
  Float f = 0.5/a;
  Float d = std::sqrt(delta);
  Float x0 = f*(-d-b);
  Float x1 = f*(d-b);
  return std::make_tuple(std::min(x0, x1), std::max(x0, x1));
}

inline Float Determinant3x3(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
  return (-a^c) * b;
}

inline Matrix4x4f Translate(const Vector3f& v) {
  return Matrix4x4f(1, 0, 0, v.x,
                    0, 1, 0, v.y,
                    0, 0, 1, v.z,
                    0, 0, 0, 1);
}

inline Matrix4x4f Ortho(const Float l, const Float r,
                        const Float b, const Float t,
                        const Float n, const Float f) {
  return Matrix4x4f(2/(r-l), 0, 0, -(r+l)/(r-l),
                    0, 2/(t-b), 0, -(t+b)/(t-b),
                    0, 0, -2/(f-n), -(f+n)/(f-n), // use gl convention
                    0, 0, 0, 1);
}

inline Matrix4x4f Frustum(const Float l, const Float r,
                              const Float b, const Float t,
                              const Float n, const Float f) {
  return Matrix4x4f(2*n/(r-l), 0, (r+l)/(r-l), 0,
                    0, 2*n/(t-b), (t+b)/(t-b), 0,
                    0, 0, -(f+n)/(f-n), -2*f*n/(f-n),
                    0, 0, -1, 0);
}

} // end namespace zLi

#endif

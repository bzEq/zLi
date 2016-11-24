// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _RESULT_HH_
#define _RESULT_HH_
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

template <typename V>
struct result_value {
  V v;
  result_value() = default;
  explicit result_value(const V &vv) : v(vv) {}
  explicit result_value(V &&vv) : v(std::move(vv)) {}
};

template <>
struct result_value<void> {};

template <typename VV, typename V = typename std::decay<VV>::type>
inline result_value<V> Ok(VV &&v) {
  return result_value<V>(std::forward<VV>(v));
}

inline result_value<void> Ok() { return result_value<void>(); }

template <typename E>
struct result_error {
  E e;
  result_error() = default;
  explicit result_error(const E &ee) : e(ee) {}
  explicit result_error(E &&ee) : e(std::move(ee)) {}
};

template <typename EE, typename E = typename std::decay<EE>::type>
inline result_error<E> Error(EE &&e) {
  return result_error<E>(std::forward<EE>(e));
}

template <typename V, typename E>
class result {
public:
  result() = default;
  result(const result<V, E> &r) : ok_(r.ok_), v_(nullptr), e_(nullptr) {
    if (r.v_)
      v_ = std::make_unique<result_value<V>>(*r.v_);
    if (r.e_)
      e_ = std::make_unique<result_error<E>>(*r.e_);
  }
  result(result<V, E> &&) = default;
  result(result_value<V> &&v)
      : ok_(true), v_(std::make_unique<result_value<V>>(std::move(v))) {}
  result(result_error<E> &&e)
      : ok_(false), e_(std::make_unique<result_error<E>>(std::move(e))) {}

  result &operator=(const result<V, E> &r) {
    ok_ = r.ok_;
    v_ = nullptr;
    e_ = nullptr;
    if (r.v_)
      v_ = std::make_unique<result_value<V>>(*r.v_);
    if (r.e_)
      e_ = std::make_unique<result_error<E>>(*r.e_);
  }
  result &operator=(result<V, E> &&) = default;

  explicit operator bool() const { return ok_; }
  V &operator*() {
    assert(v_);
    return v_->v;
  }
  V &Get() {
    assert(v_);
    return v_->v;
  }
  E &Error() {
    assert(e_);
    return e_->e;
  }

private:
  bool ok_;
  std::unique_ptr<result_value<V>> v_;
  std::unique_ptr<result_error<E>> e_;
};

template <typename E>
class result<void, E> {
public:
  result() = default;
  result(const result<void, E> &r) : ok_(r.ok_), e_(nullptr) {
    if (r.e_) {
      e_ = std::make_unique<result_error<E>>(*r.e_);
    }
  }
  result(result<void, E> &&) = default;
  result(result_value<void> &&v) : ok_(true) {}
  result(result_error<E> &&e)
      : ok_(false), e_(std::make_unique<result_error<E>>(std::move(e))) {}

  explicit operator bool() const { return ok_; }
  E &Error() {
    assert(e_);
    return e_->e;
  }

private:
  bool ok_;
  std::unique_ptr<result_error<E>> e_;
};

template <typename V>
using Result = result<V, std::string>;

inline result_error<std::string> Error(const char *err) {
  return Error(std::string(err));
}

inline result_error<std::string> FormatError(const char *err) {
  return Error(err);
}

template <typename... Args>
inline result_error<std::string> FormatError(const char *fmt, Args &&... args) {
  static const int LIMITS = (1 << 16);
  int cap = 64;
  char *buf = new char[cap];
  while (cap <= LIMITS) {
    int len = std::snprintf(buf, cap, fmt, std::forward<Args>(args)...);
    if (len < cap) {
      break;
    }
    delete[] buf;
    cap <<= 1;
    buf = new char[cap];
  }
  std::string s(buf);
  delete[] buf;
  return Error(std::move(s));
}

#endif

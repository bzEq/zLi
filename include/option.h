// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _OPTION_H_
#define _OPTION_H_
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

template <typename T>
class Option {
public:
  Option() : none_(true) {}
  Option(const Option<T> &o) : none_(o.none_), v_(nullptr) {
    if (o.v_) {
      v_ = std::make_unique<T>(o.v_);
    }
  }
  Option(Option<T> &&) = default;

  Option(const T &v) : none_(false), v_(std::make_unique<T>(v)) {}
  Option(T &&v) : none_(false), v_(std::make_unique<T>(std::move(v))) {}
  explicit operator bool() const { return !none_; }
  T &operator*() {
    assert(v_);
    return *v_;
  }
  T &Get() {
    assert(v_);
    return *v_;
  }
  Option(std::nullptr_t) : none_(true), v_(nullptr) {}

private:
  bool none_;
  std::unique_ptr<T> v_;
};

template <typename TT, typename T = typename std::decay<TT>::type>
inline Option<T> Some(TT &&v) {
  return Option<T>(std::forward<TT>(v));
}

#endif

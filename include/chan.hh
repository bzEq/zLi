#ifndef _ZLI_CHAN_HH_
#define _ZLI_CHAN_HH_
#include "math.hh"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

namespace zLi {

// a blocking channel impl
template <typename T> class Chan {
public:
  Chan() = default;
  Chan(const Chan &c) = delete;
  Chan(Chan &&c) = delete;
  void Close() {
    std::unique_lock<std::mutex> l(mu_);
    closed_ = true;
  }

  size_t Size() {
    std::unique_lock<std::mutex> l(mu_);
    return q_.size();
  }

  // TODO: version of dealing with time out is required.
  friend bool operator<<(Chan &c, const T &v) {
    std::unique_lock<std::mutex> l(c.mu_);
    if (c.closed_)
      return false;
    c.q_.push(v);
    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again.
    l.unlock();
    c.cv_.notify_one();
    return true;
  }

  friend bool operator<<(Chan &c, T &&v) {
    std::unique_lock<std::mutex> l(c.mu_);
    if (c.closed_)
      return false;
    c.q_.push(std::move(v));
    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again.
    l.unlock();
    c.cv_.notify_one();
    return true;
  }

  friend bool operator<<(T &v, Chan &c) {
    std::unique_lock<std::mutex> l(c.mu_);
    while (!c.closed_ && c.q_.empty()) {
      c.cv_.wait(l);
    }
    if (c.q_.empty())
      return false;
    auto f = c.q_.front();
    c.q_.pop();
    l.unlock();
    v = std::move(f);
    return true;
  }

  bool Push(T &&v) { return operator<<(*this, std::forward<T>(v)); }

  std::optional<T> Pop() {
    std::unique_lock<std::mutex> l(mu_);
    while (!closed_ && q_.empty()) {
      cv_.wait(l);
    }
    if (q_.empty())
      return {};
    auto f = q_.front();
    q_.pop();
    l.unlock();
    return f;
  }
  // non blocking pop
  std::optional<T> Popi() {
    std::unique_lock<std::mutex> l(mu_);
    if (q_.empty()) {
      return {};
    }
    auto f = q_.front();
    q_.pop();
    l.unlock();
    return f;
  }

private:
  std::mutex mu_;
  std::condition_variable cv_;
  std::queue<T> q_;
  bool closed_;
};
}
#endif

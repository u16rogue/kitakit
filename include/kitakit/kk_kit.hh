#pragma once

#include <chrono>
#include <future>
#include <thread>
#include <optional>

namespace kitakit {

/*
 *  A wrapper around std::future for executing tasks
 *  asynchronously from the render thread preventing
 *  thread locks with a much simpler API.
 */
template <typename T>
class AsyncTask {
public:
  AsyncTask() {};

  template <typename Handler>
  auto operator=(Handler && fn) -> void {
    if (running()) {
      return;
    }
    async = std::async(std::launch::async, fn);
  }

  auto get() -> std::optional<T> {
    if (!async.valid() || running()) {
      return std::nullopt;
    }

    return async.get();
  }

  auto running() const noexcept -> bool {
    return async.valid() && async.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready;
  }

  operator bool() const noexcept {
    return running();
  }

private:
  std::future<T> async;
};

} // namespace kitakit

//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once
#include <functional>
#include <condition_variable>
#include <thread>
#include <future>
#include "attributes.hpp"

namespace higs {

class Context;

template <typename T = void>
using ScheduledFunctionT = std::function<T(Context&)>;

using ScheduledFunction = std::function<void(Context&)>;

enum class EventLoopPriority {
  IDLE = 0,
  NORMAL = 1,
  HIGH = 2,
  NOW = 3,
};

/**
 * Represents a context event loop.
 */
class EventLoop final {
public:
  explicit EventLoop();
  ~EventLoop();

  // Non-copyable
  EventLoop(const EventLoop &) = delete;
  EventLoop &operator=(const EventLoop &) = delete;

  // Non-Moveable
  EventLoop(EventLoop &&) = delete;
  EventLoop &operator=(EventLoop &&) = delete;

  void runNowBlocking(ScheduledFunction&&);
  void schedule(ScheduledFunction&&);

  HIGS_NODISCARD
  bool isRunning() const noexcept;
  void stop() noexcept;

  template <typename T = void>
  std::promise<T> runAsync(ScheduledFunctionT<T>&& func) const {
    std::promise<T> promise;
    this->schedule([func = std::move(func)](Context& ctx) {
      auto res = func(ctx);
      // promise
    });
  }

private:
  std::mutex mutex_;
  std::condition_variable cv_;
  std::queue<ScheduledFunction> queue_;
  std::atomic_bool running_{false};
  std::thread thread_;

  void threadMain();
};

} // higs

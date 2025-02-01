//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#include "EventLoop.hpp"

namespace higs {
EventLoop::EventLoop(): running_{true} {
  thread_ = std::thread(&EventLoop::threadMain, this);
}

EventLoop::~EventLoop() {

}

void EventLoop::runNowBlocking(ScheduledFunction &&func) {
  {
    std::lock_guard guard {mutex_};
    queue_.emplace(std::move(func));
    cv_.notify_all();
  }
}

void EventLoop::schedule(ScheduledFunction &&func) {
  {
    std::lock_guard guard {mutex_};
    queue_.emplace(std::move(func));
    cv_.notify_all();
  }
}


bool EventLoop::isRunning() const noexcept {
  return running_.load();
}

void EventLoop::stop() noexcept {
  running_.store(false);
}


void EventLoop::threadMain() {
  std::queue<ScheduledFunction> readQueue;

  while (running_.load()) HIGS_LIKELY {
    {
      std::unique_lock guard {mutex_};
      cv_.wait(guard, [&] { return !queue_.empty(); });
      std::swap(readQueue, queue_);
    }

    for (; !readQueue.empty(); readQueue.pop()) {
      // readQueue.front()();
    }
  }
}



} // higs
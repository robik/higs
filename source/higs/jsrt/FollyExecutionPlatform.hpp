//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <memory>

#include <folly/Executor.h>
#include <folly/executors/ThreadPoolExecutor.h>
#include <folly/io/async/EventBase.h>
#include <jsrt/jsrt.hpp>

namespace higs {

class FollyExecutionPlatform final : public jsrt::ExecutionPlatform {
  public:
    FollyExecutionPlatform(size_t numThreads) noexcept;
    ~FollyExecutionPlatform() noexcept override = default;

    [[nodiscard]]
    auto getIOExecutor() noexcept -> folly::IOExecutor&;

    [[nodiscard]]
    auto getBackgroundExecutor() noexcept -> folly::Executor&;
private:
    std::unique_ptr<folly::EventBase> _eventBase;
    std::unique_ptr<folly::ThreadPoolExecutor> _threadPoolExecutor;
    std::unique_ptr<folly::IOExecutor> _ioExecutor;
};

}

//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//
#include "FollyExecutionPlatform.hpp"
#include <folly/executors/CPUThreadPoolExecutor.h>


namespace higs {

FollyExecutionPlatform::FollyExecutionPlatform(size_t numThreads) noexcept
{
    _threadPoolExecutor = std::move(std::make_unique<folly::CPUThreadPoolExecutor>(numThreads));
    // _ioExecutor = std::move(std::make_unique<folly::IOExecutor>());
}

folly::IOExecutor& FollyExecutionPlatform::getIOExecutor() noexcept
{
    assert(_ioExecutor != nullptr);
    return *_ioExecutor;
}

folly::Executor& FollyExecutionPlatform::getBackgroundExecutor() noexcept
{
    assert(_threadPoolExecutor != nullptr);
    return *_threadPoolExecutor;
}

}

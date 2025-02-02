//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <folly/executors/DrivableExecutor.h>
#include <folly/executors/ScheduledExecutor.h>
#include <folly/executors/SequencedExecutor.h>
#include <folly/executors/TimedDrivableExecutor.h>
#include <folly/io/async/EventBase.h>
#include <folly/io/async/ScopedEventBaseThread.h>
#include <higs/jsrt/RefCounted.hpp>

namespace higs {

class Agent final
    : public jsrt::Agent
    , public RefCounted<Agent> {
  protected:
    Agent() noexcept = default;
    ~Agent() noexcept = default;

  public:
    void runLater(Func function) override
    {
        _eventBaseThread.add(std::move(function));
    }

    void runWithPriority(Func function, int32_t priority) override
    {
        _eventBaseThread.addWithPriority(std::move(function), priority);
    }

    void runAfter(Func function, std::chrono::milliseconds delay) override
    {
        _eventBaseThread.getEventBase()->runAfterDelay(std::move(function), delay.count());
    }

    void runNowBlocking(Func func) override
    {
        folly::Baton<> ready;
        if (_eventBaseThread.getEventBase()->isInEventBaseThread()) {
            func();
        }
        else {
            _eventBaseThread.getEventBase()->runInEventBaseThread([&ready, func = std::move(func)]() mutable {
                SCOPE_EXIT
                {
                    ready.post();
                };
                folly::copy(std::move(func))();
            });
        }

        ready.wait();
    }

    void runInBackground(Func func) override
    {
        _eventBaseThread.getEventBase()->add(std::move(func));
    }

    bool isRunning() const noexcept override
    {
        return _eventBaseThread.getEventBase()->isRunning();
    }

    void stop() override
    {
        return _eventBaseThread.getEventBase()->terminateLoopSoon();
    }

  private:
    folly::ScopedEventBaseThread _eventBaseThread;
    friend class higs::RefCounted<Agent>;
};

}

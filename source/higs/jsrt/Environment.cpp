//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#include "Environment.hpp"
#include "Runtime.hpp"

namespace higs {

Environment::Environment(Runtime& host, Agent& agent, const std::string& name) noexcept
    : _host(host), _name(name), _agent(agent)
{
    _jsRuntime = std::move(facebook::hermes::makeHermesRuntime(host.jsRuntimeBuilder().build()));
}

void Environment::runNowBlocking(ScheduledFunction func)
{
    this->agent().runNowBlocking([func = std::move(func), this]() mutable {
        // This should be safe as it is blocking
        func(*this);
    });
}

void Environment::runLater(ScheduledFunction func)
{
    this->agent().runNowBlocking([func = std::move(func), selfRef = asRef()]() mutable {
        // This should be safe as it is blocking
        func(*selfRef);
    });
}

void Environment::runWithPriority(ScheduledFunction func, int32_t priority)
{
    this->agent().runWithPriority([func = std::move(func), selfRef = asRef()]() mutable {
        // This should be safe as it is blocking
        func(*selfRef);
    }, priority);
}

void Environment::runInBackground(ScheduledFunction func)
{
    this->agent().runInBackground([func = std::move(func), selfRef = asRef()]() mutable {
        // This should be safe as it is blocking
        func(*selfRef);
    });
}

void Environment::runAfter(ScheduledFunction func, std::chrono::milliseconds delay)
{
    this->agent().runAfter([func = std::move(func), selfRef = asRef()]() mutable {
        // This should be safe as it is blocking
        func(*selfRef);
    }, delay);
}

}

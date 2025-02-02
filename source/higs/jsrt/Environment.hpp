//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once
#include <memory>
#include <unordered_map>

#include <folly/executors/ExecutorWithPriority.h>
#include <hermes/hermes.h>
#include <higs/common.hpp>
#include <higs/jsrt/Agent.hpp>
#include <higs/jsrt/RefCounted.hpp>
#include <higs/utility/NonCopyable.hpp>
#include <jsrt/jsrt.hpp>

namespace higs {

class Runtime;
class Agent;

/**
 * Represents a single JavaScript execution environment.
 *
 * Each Environment has its own JavaScript runtime and EventLoop.
 */
class Environment final: public jsrt::Environment, public RefCounted<Environment> {
protected:
    Environment(Runtime& host, Agent& agent, const std::string& name) noexcept;
public:
    HIGS_MAKE_NON_COPYABLE(Environment);

    using ScheduledFunction = std::function<void(jsrt::Environment&)>;

    ~Environment() noexcept = default;

    [[nodiscard]]
    auto name() const noexcept -> const std::string& override
    {
        return _name;
    }

    auto agent() noexcept -> jsrt::Agent& override
    {
        return _agent;
    }

    auto jsVirtualMachine() noexcept -> jsi::Runtime& override
    {
        return *_jsRuntime;
    }
// --
    void runNowBlocking(ScheduledFunction func) override;
    void runLater(ScheduledFunction func) override;
    void runWithPriority(ScheduledFunction func, int32_t priority) override;
    void runInBackground(ScheduledFunction func) override;
    void runAfter(ScheduledFunction function, std::chrono::milliseconds delay) override;

private:
    Runtime& _host;
    std::string _name;
    std::unique_ptr<HermesRuntime> _jsRuntime;

    Agent& _agent;

    /**
     * Represents a map of instantiated imported modules.
     *
     * If a path is not here, that means the module is required/imported for the first
     * time within this realm.
     */
    Synchronized<std::unordered_map<String, jsi::Object>> _importInstanceMap;

    friend class higs::RefCounted<Environment>;
};

}

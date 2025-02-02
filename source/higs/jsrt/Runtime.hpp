//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <unordered_map>

#include <folly/Executor.h>
#include <folly/Synchronized.h>
#include <folly/executors/IOExecutor.h>
#include <hermes/hermes.h>
#include <higs/common.hpp>
#include <higs/jsrt/Environment.hpp>
#include <higs/jsrt/FollyExecutionPlatform.hpp>
#include <higs/jsrt/RefCounted.hpp>
#include <jsrt/jsrt.hpp>

namespace higs {

class Runtime final
    : public jsrt::Runtime
    , public RefCounted<Runtime> {
  protected:
    // TODO: accept options / builder
    Runtime() noexcept;

  public:
    HIGS_MAKE_NON_COPYABLE(Runtime);
    ~Runtime() noexcept = default;

    auto jsRuntimeBuilder() noexcept -> hermes::vm::RuntimeConfig::Builder&
    {
        return _builder;
    }

    // [[nodiscard]]
    // auto createEnvironment(const std::string& name) & noexcept -> Environment&;

    auto sourceProviders() noexcept -> std::vector<jsrt::SourceProvider*> override
    {
        std::vector<jsrt::SourceProvider*> res {_sourceProviders.size()};

        for (auto& src: _sourceProviders) {
            res.push_back(src);
        }

        return res;
    }

    [[nodiscard]]
    auto executionPlatform() & noexcept -> jsrt::ExecutionPlatform& override
    {
        return *_platform;
    }

    [[nodiscard]]
    auto mainEnvironment()& noexcept -> jsrt::Environment& override
    {
        return *_mainEnv;
    }

    [[nodiscard]]
    auto mainEnvironment() const& noexcept -> const jsrt::Environment& override
    {
        return *_mainEnv;
    }

  private:
    /**
     *
     */
    hermes::vm::RuntimeConfig::Builder _builder;

    /**
     * Represents a map of known modules.
     *
     * If a path is not here, that means the module is required/imported for the first
     * time within this realm.
     */
    std::unordered_map<std::string, jsi::Object> _moduleMap;

    std::vector<jsrt::SourceProvider*> _sourceProviders {};

    Agent::Ptr _mainAgent;
    Environment::Ptr _mainEnv;
    std::vector<Environment::Ptr> _envs;
    std::unique_ptr<FollyExecutionPlatform> _platform;

    friend class higs::RefCounted<Runtime>;
};

}

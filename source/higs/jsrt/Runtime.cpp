//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#include "Runtime.hpp"

#include <boost/filesystem.hpp>
#include "Agent.hpp"
#include "FileSystemSourceProvider.hpp"

namespace fs = boost::filesystem;

namespace higs {

Runtime::Runtime() noexcept
{
    _builder = hermes::vm::RuntimeConfig::Builder()
                   .withArrayBuffer(true)
                   .withEnableGenerator(true)
                   .withMicrotaskQueue(true)
                   .withES6Class(true)
                   .withES6Promise(true)
                   .withES6Proxy(true)
                   .withES6BlockScoping(true)
                   .withIntl(false);

    _mainAgent = Agent::create();
    _mainEnv = Environment::create(*this, *_mainAgent, "main");
    // auto provider = FileSystemSourceProvider::create(fs::current_path());
    // _sourceProviders.push_back(boost::dynamic_pointer_cast<jsrt::SourceProvider>(provider));
}

// auto Runtime::createEnvironment(const std::string& name) & noexcept -> Environment&
// {
//     auto ctx = std::make_unique<Environment>(*this, name);
//     return *_envs.emplace_back(std::move(ctx));
// }

}

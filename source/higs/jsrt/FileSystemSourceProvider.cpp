//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#include "FileSystemSourceProvider.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <folly/executors/GlobalExecutor.h>
#include <folly/futures/Future.h>

namespace fs = std::filesystem;

namespace higs {

// TODO: handle tsconf

auto readRequiredFile(const std::string& resolvedPath) -> folly::Future<std::string>
{
    std::ifstream reader { resolvedPath, std::ios::in };
    std::stringstream buffer;
    buffer << reader.rdbuf();
    return buffer.str();
}

auto FileSystemSourceProvider::resolve(const jsrt::Runtime& host, const jsrt::ResolveRequest& request) const -> std::string
{
    fs::path requiredPath { request.path };
    fs::path requesterPath { request.requesterPath };

    return requesterPath.parent_path() / requiredPath;
}

auto FileSystemSourceProvider::getSourceFor(
    const jsrt::Runtime& host,
    const std::string& resolvedPath,
    const jsrt::ResolveRequest& originalRequest
) const -> std::future<std::string>
{
    auto promise = std::promise<std::string>{};
    auto stdFuture = promise.get_future();
    const auto executor = folly::getGlobalIOExecutor();
    folly::makeSemiFuture()
        .via(executor)
        .then([&](auto&&) { return readRequiredFile(resolvedPath); });
        // .then([&](auto&& source) { promise.set_value(std::move(source)); });

    return stdFuture;
}

}

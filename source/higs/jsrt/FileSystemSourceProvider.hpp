//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <boost/filesystem.hpp>
#include <higs/jsrt/RefCounted.hpp>
#include <jsrt/jsrt.hpp>

namespace higs {

class FileSystemSourceProvider: public jsrt::SourceProvider, public RefCounted<FileSystemSourceProvider> {
protected:
    FileSystemSourceProvider() noexcept: _baseDir(boost::filesystem::current_path()) {}
    FileSystemSourceProvider(const std::string& baseDir) noexcept: _baseDir(baseDir) {}
    FileSystemSourceProvider(const boost::filesystem::path& baseDir) noexcept: _baseDir(baseDir) {}
public:
    ~FileSystemSourceProvider() noexcept override = default;

    auto handles(const jsrt::Runtime& host, const jsrt::ResolveRequest& request) const noexcept -> bool override
    {
        return true;
    }

    auto resolve(const jsrt::Runtime& host, const jsrt::ResolveRequest& request) const -> std::string override;
    auto getSourceFor(
        const jsrt::Runtime& host,
        const std::string& resolvedPath,
        const jsrt::ResolveRequest& originalRequest
    ) const -> std::future<std::string> override;
private:
    boost::filesystem::path _baseDir;

    friend class higs::RefCounted<FileSystemSourceProvider>;
};

}
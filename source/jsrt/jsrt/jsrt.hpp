//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <future>
#include <memory>
#include <jsi/jsi.h>

//
// TODO:
//  - expose synchronization mechanisms
//
namespace jsrt {

namespace js = facebook::jsi;

class ExecutionPlatform;
class Runtime;
class Environment;

/**
 * Abstract class for marking derivative as reference counted.
 *
 * Extracted as an abstract class, for simplified C API.
 *
 * Reference counting is assumed to be thread safe.
 */
class RefCounted {
  public:
    /**
     * Atomically increases number of references of this object.
     */
    void retain() const noexcept;

    /**
     * Atomically decreases number of references of this object.
     */
    void release() const noexcept;

    /**
     * Number of currently active references to this object.
     *
     * @return
     */
    auto refCount() const noexcept -> std::size_t;
};

class ExecutionPlatform {
  public:
    virtual ~ExecutionPlatform() noexcept = default;
};

/**
 * Information about resolve/import request context.
 */
struct ResolveRequest {
    /**
     * Path imported by user.
     *
     * Contains value passed to `require()` as first argument.
     */
    std::string path;
    std::string requesterPath;
};

/**
 * Interface for handling script import resolving.
 */
class SourceProvider : public RefCounted {
  public:
    virtual ~SourceProvider() noexcept = default;

    /**
     * Determines whenever this source provider can handle specified resolve request.
     *
     * If `false` is returned, this means that this SourceProvider should be skipped
     * when resolved this request.
     *
     * @param runtime Instance of `jsr::Runtime` dispatching this request
     * @param request Resolve request
     *
     * @return True if can handle resolve, false otherwise.
     */
    virtual auto handles(const Runtime& runtime, const ResolveRequest& request) const noexcept -> bool;

    /**
     * Resolves resolve request into loadable path.
     *
     * Called only when this `SourceProvider` handles this requests (`handles` returned true for this request).
     *
     * Returned path does not have to be an existing path, it must resolve into a value
     * that is understandable by this provider.
     *
     * The path will be visible to user as a result of `require.resolve()`.
     *
     * Result of this function are cached, so any successive calls with the same input
     * will not be made.
     *
     * @param runtime Instance of `jsr::Runtime` dispatching this request
     * @param request Request to resolve.
     * @return
     */
    virtual auto resolve(const Runtime& runtime, const ResolveRequest& request) const -> std::string = 0;

    /**
     * Returns source code for resolved path.
     *
     * `resolvedPath` argument is guaranteed to be the result of `resolve` from this
     * source provider.
     *
     * Result of this function are cached, so any successive calls with the same input
     * will not be made.
     *
     * @param runtime Instance of `jsr::Runtime` dispatching this request
     * @param resolvedPath Path to load
     * @param originalRequest Original resolve request
     * @return
     */
    virtual auto getSourceFor(
        const Runtime& runtime,
        const std::string& resolvedPath,
        const ResolveRequest& originalRequest
    ) const -> std::future<std::string>
        = 0;
};

/**
 * Specialized interface of SourceProvider that can return precompiled JS bytecode.
 */
class BytecodeAwareSourceProvider : public SourceProvider {
  public:
    /**
     * Returns source code for resolved path.
     *
     * `resolvedPath` argument is guaranteed to be the result of `resolve` from this
     * source provider.
     *
     * Result of this function are cached, so any successive calls with the same input
     * will not be made.
     *
     * @param resolvedPath Path to load
     * @param originalRequest Original resolve request
     * @return
     */
    virtual auto getPrecompiledSourceFor(const std::string& resolvedPath, const ResolveRequest& originalRequest) const
        -> std::future<std::vector<std::byte>>
        = 0;
};

/**
 * Runtime is the main entry point in JSR API.
 *
 * `Runtime` represents an aggregate JavaScript Virtual Machine, which consists of multiple `Environment`s.
 * Each Environment is a separate, independent and isolated JS environment.
 * Each Environment can, but does not have to, run on a separate thread.
 *
 * By default, `Runtime` contains a single default Environment, which cannot be removed. Additional
 * `Environment`s can be created using `Runtime::createEnvironment`.
 */
class Runtime : public RefCounted {
  protected:
    virtual ~Runtime() noexcept = default;

  public:
    [[nodiscard]]
    virtual auto sourceProviders() noexcept -> std::vector<SourceProvider*>
        = 0;

    [[nodiscard]]
    virtual auto executionPlatform() & noexcept -> ExecutionPlatform& = 0;

    // TODO: support multiple contexts
    // [[nodiscard]]
    // virtual auto createEnvironment(const std::string& name) & noexcept -> Context& = 0;

    /**
     * Gets main JS environment.
     *
     * @return
     */
    // TODO: hide for concurrency security?
    [[nodiscard]]
    virtual auto mainEnvironment() & noexcept -> Environment& = 0;

    /**
     * Gets main JS environment.
     *
     * @return
     */
    [[nodiscard]]
    virtual auto mainEnvironment() const& noexcept -> const Environment& = 0;
};

template<typename TFunc>
class RunnableTarget {
  public:
    virtual void runNowBlocking(TFunc func) = 0;
    virtual void runLater(TFunc func) = 0;
    virtual void runWithPriority(TFunc func, int32_t priority) = 0;
    virtual void runInBackground(TFunc func) = 0;
    virtual void runAfter(TFunc function, std::chrono::milliseconds delay) = 0;
};

/**
 * Progresses environments.
 */
class Agent
    : public RunnableTarget<std::function<void()>>
    , public RefCounted {
  public:
    using Func = std::function<void()>;

    virtual bool isRunning() const noexcept = 0;
    virtual void stop() = 0;
};

/**
 * Alias for a function that can be scheduled in a context.
 */
using ScheduledJSFunction = std::function<void(Environment&)>;

/**
 * Represents a single JavaScript execution environment.
 *
 * Each Environment has its own JavaScript runtime and EventLoop.
 */
class Environment
    : public RunnableTarget<std::function<void(Environment&)>>
    , public RefCounted {
  public:
    /**
     * Gets name of current environment.
     *
     * @return Environment's name
     */
    [[nodiscard]]
    virtual auto name() const noexcept -> const std::string& = 0;

    /**
     * Gets agent running this environment.
     *
     * Multiple environments can be run by one agent.
     *
     * This is not thread safe, caller must perform synchronization manually.
     *
     * @return This environment's agent
     */
    [[nodiscard]]
    // TODO: revise return type (e.g. Synchronized<>)
    virtual auto agent() noexcept -> Agent& = 0;

    // --- JSI interop
    /**
     * Gets JSI Runtime associated with this context.
     *
     * @return JSI Runtime
     */
    [[nodiscard]]
    virtual auto jsVirtualMachine() noexcept -> js::Runtime& = 0;

    /**
     * Provides implicit conversion into `js::Runtime`.
     *
     * This is convenient when working with `js::Value`s, so that this code is possible:
     *
     * @code{.cpp}
     * auto myFunc(jsr::Environment& env) {
     *   // clean version
     *   js::Array::createFromElements(env, {1, 2, 3});
     *   js::Object { env };
     *
     *   // instead of
     *   js::Array::createFromElements(env.jsVirtualMachine(), {1, 2, 3});
     *   js::Object { env.jsVirtualMachine() };
     * }
     * @endcode
     */
    operator js::Runtime&() & noexcept
    {
        return jsVirtualMachine();
    }

    /**
     * Gets global object local for this context.
     *
     * @return Context's global object
     */
    [[nodiscard]]
    virtual auto globalObject() & noexcept -> js::Object
    {
        return jsVirtualMachine().global();
    }

    virtual auto evaluateScript(const std::string& script) & -> js::Value
    {
        return this->evaluateScript(script, "<unknown>");
    }

    virtual auto evaluateScript(const std::string& script, const std::string& name) & -> js::Value
    {
        auto scriptBuffer = std::make_shared<js::StringBuffer>(script);
        return jsVirtualMachine().evaluateJavaScript(scriptBuffer, name);
    }
};

};

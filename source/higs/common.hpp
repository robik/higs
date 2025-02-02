//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once
#include <folly/FBString.h>
#include <folly/FBVector.h>
#include <folly/Expected.h>
#include <folly/Synchronized.h>
#include <hermes/hermes.h>
#include <jsrt/jsrt.hpp>

namespace higs {

/**
 * Alias for `folly::Expected`.
 *
 * We have to do this, since `std::expected` is available starting from C++23 (we target C++20).
 *
 * It should be used as a `std::optional` with error value, that is as return type for
 * functions that a failure (error state) is not an exception, and should not trigger exception throwing,
 * causing costly stack unwinding (exceptions are free only when not thrown).
 *
 * Function that return `Expected`, should start with `try` prefix, and may optionally have
 * a throwing variant without the prefix:
 *
 * @code{.cpp}
 * struct MyError {};
 *
 * auto tryLoad() -> Expected<string, MyError> {}
 * auto load() -> string {
 *   return tryLoad.value();
 * }
 * @endcode
 *
 * As a guide, when you create your own error type (e.g. extending `std::runtime_error`) that
 * is returned from multiple functions, you should create an alias for this return type:
 *
 * @code{.cpp}
 * struct LoadError {}; // <name>Error
 *
 * template <typename T>
 * using LoadResult = Expected<T, LoadError>; // <name>Result
 *
 * auto tryLoad(const std::string& a) -> LoadResult<string> {}
 * auto tryLoad(const String& a) -> LoadResult<string> {}
 * @endcode
 *
 * @see {makeExpected}
 * @see {makeUnexpected}
 */
template <typename T, typename E>
using Expected = folly::Expected<T, E>;

using folly::makeUnexpected;
using folly::makeExpected;

// Folly aliases
template <typename T>
using Vector = folly::fbvector<T>;
using String = folly::fbstring;

template <typename T>
using Synchronized = folly::Synchronized<T>;

using HermesRuntime = facebook::hermes::HermesRuntime;

// using higs::RefPtr;
}

// JSI alias
namespace jsi = facebook::jsi;

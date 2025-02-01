//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once
#include <memory>
#include <string>
#include <hermes/hermes.h>
#include "EventLoop.hpp"

namespace higs {

class Host;

/**
 * Represents a JS Context, which is a single execution environment.
 */
class Context final {
public:
  Context(Host& host, const std::string& name);

  // Non copyable
  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;

  // Non-Moveable
  Context(Context&&) = delete;
  Context& operator=(Context&&) = delete;

private:
  Host& host_;
  std::string name_;
  std::unique_ptr<facebook::hermes::HermesRuntime> mainRuntime_;
  EventLoop eventLoop_;
};

} // higs

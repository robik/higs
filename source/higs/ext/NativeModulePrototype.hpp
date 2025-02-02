//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <string>
#include <jsrt/jsrt.hpp>

namespace higs {
class Environment;
}

namespace higs::ext {

class NativeModulePrototype {
  public:
    NativeModulePrototype(const std::string& name) noexcept;

    void build(Environment& context) -> jsrt::js::Object;

  private:
    std::string _name;
};

}

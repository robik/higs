//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <type_traits>
#include <optional>

namespace higs {

template <template <typename...> typename Template, typename Target>
concept Specializes = requires(Target value) {
    []<typename X>(Template<X>&){}(value);
};

}
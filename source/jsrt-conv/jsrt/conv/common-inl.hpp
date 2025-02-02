//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <jsrt/conv.hpp>

namespace jsrt::conv {

inline auto prettyJSTypeName(const js::Value& value) noexcept -> const std::string&
{
    if (value.isNull()) {
        return "null";
    }

    if (value.isUndefined()) {
        return "undefined";
    }

    if (value.isBool()) {
        return "boolean";
    }

    if (value.isNumber()) {
        return "number";
    }

    if (value.isString()) {
        return "string";
    }

    if (value.isSymbol()) {
        return "Symbol";
    }

    if (value.isBigInt()) {
        return "BigInt";
    }

    return "Object";
}

}

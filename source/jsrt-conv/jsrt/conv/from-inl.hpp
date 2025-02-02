//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <type_traits>

#include <fmt/format.h>
#include <jsrt/jsrt.hpp>
#include <jsrt/conv.hpp>

namespace jsrt::conv {

//
// ------------------------------------------------------------------------------------------------- Core types
//
inline bool convertFromJS(bool& into, const js::Value& from, Environment& environment)
{
    if (from.isBool()) {
        into = from.asBool();
        return true;
    }

    return false;
}

#define FROM_NUMBER_AS(type)                       \
    if (from.isNumber()) {                         \
        into = static_cast<type>(from.asNumber()); \
        return true;                               \
    }                                              \
    return false;

inline bool convertFromJS(std::byte& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(std::byte);
}

inline bool convertFromJS(uint8_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(int8_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(uint16_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(int16_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(uint32_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(int32_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(uint64_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(int64_t& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(float& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}

inline bool convertFromJS(double& into, const js::Value& from, Environment& environment)
{
    FROM_NUMBER_AS(uint8_t);
}
#undef FROM_NUMBER_AS


inline bool convertFromJS(std::string& into, const js::Value& from, Environment& environment)
{
    if (from.isString()) {
        into = from.asString(environment).utf8(environment);
        return true;
    }

    return false;
}

inline bool convertFromJS(std::u16string& into, const js::Value& from, Environment& environment)
{
    if (from.isString()) {
        into = from.asString(environment).utf16(environment);
        return true;
    }

    return false;
}

inline bool convertFromJS(js::Value& into, const js::Value& from, Environment& environment)
{
    if (&into != &from) {
        into = js::Value { environment, from };
    }
    return true;
}

//
// ------------------------------------------------------------------------------------------------- Functions
//
template<typename... Args>
inline bool convertFromJS(std::function<void(Args&&...)>& into, const js::Value& from, Environment& environment)
{
    if (!from.isObject()) {
        return false;
    }

    auto fromObj = from.asObject(environment);
    if (!fromObj.isFunction(environment)) {
        return false;
    }
    auto fromFunc = fromObj.asFunction(environment);

    // how to capture `from`?
    into = [&environment, &fromFunc](Args... args) {
        auto jsArgs = std::apply([&environment](auto&& arg) { return toJS(arg, environment); }, args...);
        fromFunc.call(environment, jsArgs);
    };

    return true;
}

template<typename R, typename... Args>
inline bool convertFromJS(std::function<R(Args&&...)>& into, const js::Value& from, Environment& environment)
{
    if (!from.isObject()) {
        return false;
    }

    auto fromObj = from.asObject(environment);
    if (!fromObj.isFunction(environment)) {
        return false;
    }
    auto fromFunc = fromObj.asFunction(environment);

    // how to capture `from`?
    into = [&environment, &fromFunc](Args... args) {
        auto jsArgs = std::apply([&environment](auto&& arg) { return toJS(arg, environment); }, args...);
        auto res = fromFunc.call(environment, jsArgs);
        return fromJS<R>(res, environment);
    };

    return true;
}

//
// ------------------------------------------------------------------------------------------------- Helpers
//
template<typename T>
auto tryFromJS(const js::Value& from, Environment& environment) -> ConversionResult<T>
{
    T result;
    auto success = convertFromJS(result, from, environment);

    return success ? makeExpected<ConversionError>(result) : makeUnexpected(ConversionError::forInvalidType<T>(from));
}

template<typename T>
auto fromJS(const js::Value& from, Environment& environment) -> T
{
    return tryFromJS<T>(from, environment).value();
}

//
// ------------------------------------------------------------------------------------------------- Impl
//
template<typename T>
inline bool convertFromJS(std::optional<T>& into, const js::Value& from, Environment& environment)
{
    if (from.isUndefined() || from.isNull()) {
        into = std::nullopt;
        return true;
    }

    T value{};
    auto success = convertFromJS(value, from, environment);
    if (success) {
        into = std::make_optional(value);
    }
    else {
        into = std::nullopt;
    }

    return true;
}

template<typename T>
inline bool convertFromJS(std::vector<T>& into, const js::Value& from, Environment& environment)
{
    if (!from.isObject()) {
        return false;
    }

    auto obj = from.asObject(environment);
    if (!obj.isArray(environment)) {
        return false;
    }

    auto arr = obj.asArray(environment);
    auto size = arr.size(environment);
    into.resize(size);

    bool success = true;
    for (auto i = 0; i < size; ++i) {
        success &= convertFromJS(into[i], arr.getValueAtIndex(environment, i), environment);
    }

    return success;
}

template<typename T, size_t Size>
inline bool convertFromJS(std::array<T, Size>& into, const js::Value& from, Environment& environment)
{
    if (!from.isObject()) {
        return false;
    }

    auto obj = from.asObject(environment);
    if (!obj.isArray(environment)) {
        return false;
    }

    auto arr = obj.asArray(environment);
    auto arrSize = arr.size(environment);

    if (arrSize < Size) {
        return false;
    }

    bool success = true;
    for (auto i = 0; i < Size; ++i) {
        success &= convertFromJS(into[i], arr.getValueAtIndex(environment, i), environment);
    }

    return success;
}

}

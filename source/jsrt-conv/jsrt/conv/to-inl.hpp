//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <span>
#include <type_traits>

#include <jsrt/conv.hpp>

namespace jsrt::conv {

/**
 * Converts specified value to js::Value.
 *
 * This specialization handles case where `js::Value` defines constructor
 * for passed value.
 *
 * @param value Value to convert to JS value
 * @return Wrapped JS Value
 */
inline auto toJS(ImplictlyConvertableToJSValue auto value, Environment&) noexcept -> js::Value
{
    if constexpr (std::is_constructible_v<js::Value, decltype(value)>) {
        return { value };
    } else {
        return { (double)value };
    }
}

//
// ------------------------------------------------------------------------------------------------- Strings
//
inline auto toJS(const std::string& value, Environment& environment) -> js::Value
{
    return { js::String::createFromUtf8(environment, value) };
}

inline auto toJS(const std::u16string& value, Environment& environment) -> js::Value
{
    return { js::String::createFromUtf16(environment, value) };
}


//
// ------------------------------------------------------------------------------------------------- Functions
//
template<typename... Args>
auto toJS(std::function<void (Args&&...)> func, Environment& environment, const std::string& name) -> js::Value
{
    return {
        js::Function::createFromHostFunction(
            environment,
            js::PropNameID::forAscii(environment, name),
            sizeof...(Args),
            [&environment, &func](auto&, auto& self, auto args, auto argCount) -> js::Value {
                if constexpr (sizeof...(Args) == 0) {
                    func();
                    return js::Value::undefined();
                } else {
                    auto i = 0;
                    auto mappedArgs = std::make_tuple((fromJS<Args>(args[i++], environment), ...));
                    std::invoke(func, mappedArgs);
                    return js::Value::undefined();
                }
            }
        )
    };
}

template<typename R, typename... Args>
auto toJS(std::function<R (Args&&...)> func, Environment& environment, const std::string& name) -> js::Value
{
    return {
        js::Function::createFromHostFunction(
            environment,
            js::PropNameID::forAscii(environment, name),
            sizeof...(Args),
            [&environment, &func](auto&, auto& self, auto args, auto argCount) -> js::Value {
                if constexpr (sizeof...(Args) == 0) {
                    auto res = func();
                    return toJS(res, environment);
                } else {
                    auto i = 0;
                    auto mappedArgs = std::make_tuple((fromJS<Args>(args[i++], environment), ...));
                    auto res = std::invoke(func, mappedArgs);
                    return toJS(res, environment);
                }
            }
        )
    };
}

};

//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <string>

#include <boost/type_index.hpp>
#include <folly/Expected.h>
#include <fmt/format.h>
#include <jsrt/jsrt.hpp>

namespace jsrt {

namespace conv {
/**
 * Returns user-friendly name for type of `value`.
 *
 * @param value Value to describe
 * @return
 */
inline auto prettyJSTypeName(const js::Value& value) noexcept -> const std::string&;

/**
 * Thrown when conversion failed.
 *
 * This error is thrown by throwing family of conversion functions,
 * non-throwing family is prefixed with `try`.
 */
class ConversionError final : public std::runtime_error {
public:
    explicit ConversionError(const std::string& msg) : std::runtime_error(msg) {}

    /**
     * Helper function to create conversion error for non-matching types.
     *
     * Accepts type parameter `T`, which is the destination type requested.
     * Name of the type is determined at compile-time using CTTI.
     *
     * @tparam T Destination type attempted
     * @param value Value to convert
     * @return Error object
     */
    template<typename T>
    static auto forInvalidType(const js::Value& value) -> ConversionError
    {
        auto typeName = boost::typeindex::type_id<T>().pretty_name();
        return forInvalidType(value, typeName);
    }

    /**
     * Helper function to create conversion error for non-matching types.
     *
     * Accepts parameter `to`, which is the name of destination type requested.
     *
     * @param value Value to convert
     * @param to Name of destination type requested.
     * @return Error object
     */
    static auto forInvalidType(const js::Value& value, std::string to) -> ConversionError
    {
        return ConversionError {
            fmt::format("Could not convert value of type {} to {}", prettyJSTypeName(value), to)
        };
    }
};

template<typename T>
using ConversionResult = folly::Expected<T, ConversionError>;

//
// ------------------------------------------------------------------------------------------------- Helpers
//
/**
 * Converts `js::Value` `value` into native value of type `T`, without throwing.
 *
 * This function is a no-throwing variant of JS to C conversion. For a throwing variant see `fromJS`
 * Returns instance of `ConversionResult<T>`, which can contain converted result, or an ConversionError.
 *
 * @tparam T Type to convert to.
 * @param from Value to convert from
 * @param environment Environment to perform conversion in
 * @return Conversion result
 */
template<typename T>
auto tryFromJS(const js::Value& from, Environment& environment) -> ConversionResult<T>;

/**
 * Converts `js::Value` `value` into native value of type `T`, throwing on error.
 *
 * This function is a throwing variant of JS to C conversion. For a no-throwing variant see `tryFromJS`
 * Returns converted value of type `T`, throwing `ConversionError` on any failure..
 *
 * @tparam T Type to convert to.
 * @param from Value to convert from
 * @param environment Environment to perform conversion in
 * @return Converted value
 */
template<typename T>
auto fromJS(const js::Value& from, Environment& environment) -> T;

//
// ---------------------------------------------------------------------------------------------- CONVERSION FROM JS
//
#pragma region Basic conversion from JS

//
// ------------------------------------------------------------------------------------------------- Core types
//
// ----- BASIC TYPES
inline bool convertFromJS(bool& into, const js::Value& from, Environment& env);
inline bool convertFromJS(std::byte& into, const js::Value& from, Environment& env);

inline bool convertFromJS(uint8_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(int8_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(uint16_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(int16_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(uint32_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(int32_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(uint64_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(int64_t& into, const js::Value& from, Environment& env);
inline bool convertFromJS(float& into, const js::Value& from, Environment& env);
inline bool convertFromJS(double& into, const js::Value& from, Environment& env);

inline bool convertFromJS(std::string& into, const js::Value& from, Environment& env);
inline bool convertFromJS(std::u16string& into, const js::Value& from, Environment& env);

inline bool convertFromJS(js::Value& into, const js::Value& from, Environment& env);

template<typename T>
concept HasCoreJSFromConversion = requires(T& into, js::Value from, Environment& ctx) {
    { convertFromJS(into, from, ctx) } -> std::same_as<bool>;
};

// ----- COMPOSABLE TYPES
template<typename T>
bool convertFromJS(std::optional<T>& into, const js::Value& from, Environment& env);

template<typename T>
bool convertFromJS(std::vector<T>& into, const js::Value& from, Environment& env);

template<typename T, size_t Size>
bool convertFromJS(std::array<T, Size>& into, const js::Value& from, Environment& env);

template<typename... Args>
bool convertFromJS(std::function<void(Args&&...)>& into, const js::Value& from, Environment& env);

// ----- FUNCTIONS
template<typename R, typename... Args>
bool convertFromJS(std::function<R(Args&&...)>& into, const js::Value& from, Environment& env);

#pragma endregion

//
// ---------------------------------------------------------------------------------------------- CONVERSION TO JS
//
#pragma region Basic conversion to JS

/**
 * Concept that requires that `js::Value` can be constructed from T.
 */
template<typename T>
concept ImplictlyConvertableToJSValue = std::is_constructible_v<js::Value, T> || std::is_integral_v<T>;

template<typename T>
concept StringConvertableToJSString = std::is_same_v<T, std::string> || std::is_same_v<T, std::u16string>;

/**
 * Converts specified value to js::Value.
 *
 * This specialization handles trivil case where `js::Value` has a defined constructor
 * for type of passed value.
 *
 * @param value Value to convert to JS value
 * @param environment Environment to perform conversion in
 * @return JS Value
 */
inline auto toJS(ImplictlyConvertableToJSValue auto value, Environment& environment) noexcept -> js::Value;

/**
 * Converts specified `std::string` to `js::Value`.
 *
 * Creates new JS `String` object, which performs memory allocation.
 *
 * @param value String to convert to JS value
 * @param environment Environment to perform conversion in
 * @return JS Value
 */
inline auto toJS(const std::string& value, Environment& environment) -> js::Value;

/**
 * Converts specified `std::u16string` to `js::Value`.
 *
 * Creates new JS `String` object, which performs memory allocation.
 *
 * @param value String to convert to JS value
 * @param environment Environment to perform conversion in
 * @return JS Value
 */
inline auto toJS(const std::u16string& value, Environment& environment) -> js::Value;

/**
 * Converts specified function without a return value to a JS function (`js::Object`).
 *
 * @param func Function wrap into JS Function
 * @param environment Environment to perform conversion in
 * @param name Optional name of the function
 * @return JS Value
 */
template<typename... Args>
auto toJS(std::function<void (Args&&...)> func, Environment& environment, const std::string& name = "function") -> js::Value;

/**
 * Converts specified function with a return value to a JS function (`js::Object`).
 *
 * @param func Function wrap into JS Function
 * @param environment Environment to perform conversion in
 * @param name Optional name of the function
 * @return JS Value
 */
template<typename R, typename... Args>
auto toJS(std::function<R (Args&&...)> func, Environment& environment, const std::string& name = "function") -> js::Value;

#pragma endregion

}

using conv::ConversionError;
using conv::ConversionResult;
using conv::tryFromJS;
using conv::fromJS;
using conv::toJS;

};

#include <jsrt/conv/to-inl.hpp>
#include <jsrt/conv/from-inl.hpp>

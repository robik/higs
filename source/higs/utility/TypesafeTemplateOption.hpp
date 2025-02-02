//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

namespace higs {

/**
 * Helper type to create Typesafe constexpr option flags.
 *
 * Allows to create an explicit types wrappers for booleans.
 *
 * @code{.cpp}
 * using FlagA = higs::TypesafeTemplateOption<struct FlagATag>;
 *
 * consteval FlagA A = true;
 * // or
 * consteval FlagA A = { .enabled = true };
 * @endcode
 *
 * Each instantiation of this template has exactly the same shape, but due to `Tag` template parameter
 * other instances are not implicitly cases into another.
 *
 * The purpose of this type is to pass those flags to template as customisation options.
 * This is possible since C++20, where template parameters can be composite types.
 *
 * For example, consider a simplified implementation of `SharedPtr` class.
 * It provides reference counting functionality for object of type T, so you might write it as:
 *
 * @code{.cpp}
 * template <typename T>
 * class SharedPtr {
 * // ...
 * }
 * @endcode
 *
 * However, you want to introduce two variants of this type:
 *
 *  - Fast reference counting without shigsng with other threads
 *  - Thread safe variant using `std::atomic`
 *
 * Instead of creating two different templates, or using old C++11 template mumbo jumbo, you can simply write:
 *
 * @code{.cpp}
 * using ThreadSafetyOption = higs::TypesafeTemplateOption<struct ThreadSafetyOptionFlag>;
 *
 * template <typename T, ThreadSafetyOption ThreadSafety = false>
 * class SharedPtr {
 * public:
 *   constexpr IsThreadSafe = ThreadSafety.enabled;
 *
 * private:
 *   using CounterType = std::conditional_t<ThreadSafety.enabled, std::atomic<unsigned>, unsigned>;
 * }
 * @endcode
 *
 * @tparam Tag Tag type to differentiate from other flags
 */
template<typename Tag>
struct TypesafeTemplateOption {
    bool isEnabled;

    constexpr explicit TypesafeTemplateOption(bool enabled) noexcept : isEnabled(enabled) {}
    constexpr TypesafeTemplateOption(std::true_type) noexcept : isEnabled(true) {}
    constexpr TypesafeTemplateOption(std::false_type) noexcept : isEnabled(false) {}

    constexpr static std::true_type enable;
    constexpr static std::false_type disable;
};

}

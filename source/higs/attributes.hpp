//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#ifdef _MSC_VER
    #define HIGS_ALWAYS_INLINE __forceinline
    #define HIGS_ALWAYS_INLINE
#elif defined(__clang__)
    #define HIGS_ALWAYS_INLINE [[gnu::always_inline]] inline
    #define HIGS_TRIVIAL_ABI [[clang::trivial_abi]]
#elif defined (__GNUC__)
    #define HIGS_ALWAYS_INLINE [[gnu::always_inline]] inline
    #define HIGS_TRIVIAL_ABI
#endif
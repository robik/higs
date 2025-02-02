//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#define HIGS_MAKE_NON_COPYABLE(ClassName)                  \
    ClassName(const ClassName&) = delete;                  \
    auto operator=(const ClassName&)->ClassName& = delete;

//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#define HIGS_MAKE_NON_MOVEABLE(ClassName)             \
    ClassName(ClassName&&) = delete;                  \
    auto operator=(ClassName&&)->ClassName& = delete;

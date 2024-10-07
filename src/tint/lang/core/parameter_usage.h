// Copyright 2021 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

////////////////////////////////////////////////////////////////////////////////
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/core/parameter_usage.h.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_TINT_LANG_CORE_PARAMETER_USAGE_H_
#define SRC_TINT_LANG_CORE_PARAMETER_USAGE_H_

#include <cstdint>
#include <string>

#include "src/tint/utils/traits/traits.h"

namespace tint::core {

/// ParameterUsage is extra metadata for identifying a parameter based on its
/// overload position
enum class ParameterUsage : uint8_t {
    kArrayIndex,
    kBase,
    kBias,
    kBits,
    kCompareValue,
    kComponent,
    kCoords,
    kDdx,
    kDdy,
    kDelta,
    kDepth,
    kDepthRef,
    kE,
    kElements,
    kExp,
    kHeight,
    kId,
    kInputAttachment,
    kInsert,
    kLevel,
    kLocation,
    kMask,
    kNumLevels,
    kOffset,
    kOriginalValue,
    kRefz,
    kResult,
    kSampleIndex,
    kSampler,
    kSamples,
    kSourceLaneIndex,
    kTexture,
    kValue,
    kW,
    kWidth,
    kX,
    kXy,
    kXyz,
    kY,
    kYz,
    kZ,
    kZw,
    kZyw,
    kNone,
};

/// @param value the enum value
/// @returns the string for the given enum value
std::string_view ToString(ParameterUsage value);

/// @param out the stream to write to
/// @param value the ParameterUsage
/// @returns @p out so calls can be chained
template <typename STREAM, typename = traits::EnableIfIsOStream<STREAM>>
auto& operator<<(STREAM& out, ParameterUsage value) {
    return out << ToString(value);
}

}  // namespace tint::core

#endif  // SRC_TINT_LANG_CORE_PARAMETER_USAGE_H_

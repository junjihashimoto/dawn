#ifndef SRC_TINT_LANG_MSL_BUILTIN_FN_H_
#define SRC_TINT_LANG_MSL_BUILTIN_FN_H_

#include <cstdint>
#include <string>

#include "src/tint/lang/core/ir/call.h"
#include "src/tint/utils/rtti/traits.h"

// \cond DO_NOT_DOCUMENT
namespace tint::msl {

/// Enumerator of all builtin functions
enum class BuiltinFn : uint8_t {
    kAtomicCompareExchangeWeakExplicit,
    kAtomicExchangeExplicit,
    kAtomicFetchAddExplicit,
    kAtomicFetchAndExplicit,
    kAtomicFetchMaxExplicit,
    kAtomicFetchMinExplicit,
    kAtomicFetchOrExplicit,
    kAtomicFetchSubExplicit,
    kAtomicFetchXorExplicit,
    kAtomicLoadExplicit,
    kAtomicStoreExplicit,
    kFence,
    kGather,
    kGatherCompare,
    kGetWidth,
    kGetHeight,
    kGetDepth,
    kGetArraySize,
    kGetNumMipLevels,
    kGetNumSamples,
    kRead,
    kSample,
    kSampleCompare,
    kWrite,
    kDistance,
    kDot,
    kFmod,
    kFrexp,
    kLength,
    kModf,
    kSign,
    kThreadgroupBarrier,
    kSimdBallot,
    kQuadShuffleXor,
    kConvert,
    kAsm,
    kNone,
};

/// @returns the name of the builtin function type.
const char* str(BuiltinFn i);

/// Emits the name of the builtin function type.
template <typename STREAM, typename = traits::EnableIfIsOStream<STREAM>>
auto& operator<<(STREAM& o, BuiltinFn i) {
    return o << str(i);
}

/// @returns access restrictions for a function
tint::core::ir::Instruction::Accesses GetSideEffects(BuiltinFn fn);

}  // namespace tint::msl
// \endcond

#endif  // SRC_TINT_LANG_MSL_BUILTIN_FN_H_

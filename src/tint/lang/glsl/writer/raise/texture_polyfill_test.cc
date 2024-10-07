// Copyright 2024 The Dawn & Tint Authors
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

#include "src/tint/lang/glsl/writer/raise/texture_polyfill.h"

#include <vector>

#include "gtest/gtest.h"
#include "src/tint/lang/core/fluent_types.h"
#include "src/tint/lang/core/ir/transform/helper_test.h"
#include "src/tint/lang/core/number.h"
#include "src/tint/lang/core/type/depth_multisampled_texture.h"
#include "src/tint/lang/core/type/depth_texture.h"
#include "src/tint/lang/core/type/multisampled_texture.h"
#include "src/tint/lang/core/type/sampled_texture.h"
#include "src/tint/lang/core/type/storage_texture.h"

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

namespace tint::glsl::writer::raise {
namespace {

using GlslWriter_TexturePolyfillTest = core::ir::transform::TransformTest;

TEST_F(GlslWriter_TexturePolyfillTest, TextureDimensions_1d) {
    auto* var =
        b.Var("v", handle,
              ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k1d, ty.f32()),
              core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.u32());
    b.Append(func->Block(), [&] {
        auto* result = b.Call<u32>(core::BuiltinFn::kTextureDimensions, b.Load(var));
        b.Return(func, result);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_1d<f32>, read_write> = var @binding_point(0, 0)
}

%foo = func():u32 {
  $B2: {
    %3:texture_1d<f32> = load %v
    %4:u32 = textureDimensions %3
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<f32>, read_write> = var
}

%foo = func():u32 {
  $B2: {
    %3:texture_2d<f32> = load %v
    %4:vec2<i32> = glsl.textureSize %3, 0i
    %5:vec2<u32> = bitcast %4
    %6:u32 = swizzle %5, x
    ret %6
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureDimensions_2d_WithoutLod) {
    auto* var =
        b.Var("v", handle,
              ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()),
              core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.vec2<u32>());
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, b.Load(var));
        b.Return(func, result);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<f32>, read_write> = var @binding_point(0, 0)
}

%foo = func():vec2<u32> {
  $B2: {
    %3:texture_2d<f32> = load %v
    %4:vec2<u32> = textureDimensions %3
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<f32>, read_write> = var
}

%foo = func():vec2<u32> {
  $B2: {
    %3:texture_2d<f32> = load %v
    %4:vec2<i32> = glsl.textureSize %3, 0i
    %5:vec2<u32> = bitcast %4
    ret %5
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureDimensions_2d_WithU32Lod) {
    auto* var =
        b.Var("v", handle,
              ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()),
              core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.vec2<u32>());
    b.Append(func->Block(), [&] {
        auto* result = b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, b.Load(var), 3_u);
        b.Return(func, result);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<f32>, read_write> = var @binding_point(0, 0)
}

%foo = func():vec2<u32> {
  $B2: {
    %3:texture_2d<f32> = load %v
    %4:vec2<u32> = textureDimensions %3, 3u
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<f32>, read_write> = var
}

%foo = func():vec2<u32> {
  $B2: {
    %3:texture_2d<f32> = load %v
    %4:i32 = bitcast 3u
    %5:vec2<i32> = glsl.textureSize %3, %4
    %6:vec2<u32> = bitcast %5
    ret %6
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureDimensions_2dArray) {
    auto* type =
        ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2dArray, ty.f32());
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d_array<f32>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_2d_array<f32> = load %v
    %4:vec2<u32> = textureDimensions %3
    %x:vec2<u32> = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d_array<f32>, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_2d_array<f32> = load %v
    %4:vec3<i32> = glsl.textureSize %3, 0i
    %5:vec2<i32> = swizzle %4, xy
    %6:vec2<u32> = bitcast %5
    %x:vec2<u32> = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureDimensions_Storage2D) {
    auto* type = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2d, core::TexelFormat::kRg32Float, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(core::TexelFormat::kRg32Float, ty));
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_storage_2d<rg32float, read>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d<rg32float, read> = load %v
    %4:vec2<u32> = textureDimensions %3
    %x:vec2<u32> = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_storage_2d<rg32float, read>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d<rg32float, read> = load %v
    %4:vec2<i32> = glsl.imageSize %3
    %5:vec2<u32> = bitcast %4
    %x:vec2<u32> = let %5
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureDimensions_DepthMultisampled) {
    auto* type = ty.Get<core::type::DepthMultisampledTexture>(core::type::TextureDimension::k2d);
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_depth_multisampled_2d, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_depth_multisampled_2d = load %v
    %4:vec2<u32> = textureDimensions %3
    %x:vec2<u32> = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_depth_multisampled_2d, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_depth_multisampled_2d = load %v
    %4:vec2<i32> = glsl.textureSize %3
    %5:vec2<u32> = bitcast %4
    %x:vec2<u32> = let %5
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureNumLayers_2DArray) {
    auto* var =
        b.Var("v", handle,
              ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2dArray, ty.f32()),
              core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call(ty.u32(), core::BuiltinFn::kTextureNumLayers, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d_array<f32>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_2d_array<f32> = load %v
    %4:u32 = textureNumLayers %3
    %x:u32 = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d_array<f32>, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_2d_array<f32> = load %v
    %4:vec3<i32> = glsl.textureSize %3, 0i
    %5:i32 = swizzle %4, z
    %6:u32 = bitcast %5
    %x:u32 = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureNumLayers_Depth2DArray) {
    auto* var =
        b.Var("v", handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2dArray),
              core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call(ty.u32(), core::BuiltinFn::kTextureNumLayers, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_depth_2d_array, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_depth_2d_array = load %v
    %4:u32 = textureNumLayers %3
    %x:u32 = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_depth_2d_array, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_depth_2d_array = load %v
    %4:vec3<i32> = glsl.textureSize %3, 0i
    %5:i32 = swizzle %4, z
    %6:u32 = bitcast %5
    %x:u32 = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureNumLayers_CubeArray) {
    auto* var = b.Var(
        "v", handle,
        ty.Get<core::type::SampledTexture>(core::type::TextureDimension::kCubeArray, ty.f32()),
        core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call(ty.u32(), core::BuiltinFn::kTextureNumLayers, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_cube_array<f32>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_cube_array<f32> = load %v
    %4:u32 = textureNumLayers %3
    %x:u32 = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_cube_array<f32>, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_cube_array<f32> = load %v
    %4:vec3<i32> = glsl.textureSize %3, 0i
    %5:i32 = swizzle %4, z
    %6:u32 = bitcast %5
    %x:u32 = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureNumLayers_DepthCubeArray) {
    auto* var = b.Var("v", handle,
                      ty.Get<core::type::DepthTexture>(core::type::TextureDimension::kCubeArray),
                      core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call(ty.u32(), core::BuiltinFn::kTextureNumLayers, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_depth_cube_array, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_depth_cube_array = load %v
    %4:u32 = textureNumLayers %3
    %x:u32 = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_depth_cube_array, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_depth_cube_array = load %v
    %4:vec3<i32> = glsl.textureSize %3, 0i
    %5:i32 = swizzle %4, z
    %6:u32 = bitcast %5
    %x:u32 = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureNumLayers_Storage2DArray) {
    auto* storage_ty = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2dArray, core::TexelFormat::kRg32Float, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(core::TexelFormat::kRg32Float, ty));

    auto* var = b.Var("v", handle, storage_ty, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        b.Let("x", b.Call(ty.u32(), core::BuiltinFn::kTextureNumLayers, b.Load(var)));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_storage_2d_array<rg32float, read>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d_array<rg32float, read> = load %v
    %4:u32 = textureNumLayers %3
    %x:u32 = let %4
    ret
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_storage_2d_array<rg32float, read>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d_array<rg32float, read> = load %v
    %4:vec3<i32> = glsl.imageSize %3
    %5:i32 = swizzle %4, z
    %6:u32 = bitcast %5
    %x:u32 = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureLoad_1DF32) {
    auto* type = ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k1d, ty.f32());
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Zero<i32>();
        auto* level = b.Zero<u32>();
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, b.Load(var), coords, level));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_1d<f32>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_1d<f32> = load %v
    %4:vec4<f32> = textureLoad %3, 0i, 0u
    %x:vec4<f32> = let %4
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<f32>, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<i32> = construct 0i, 0i
    %4:texture_2d<f32> = load %v
    %5:vec2<i32> = convert %3
    %6:i32 = convert 0u
    %7:vec4<f32> = glsl.texelFetch %4, %5, %6
    %x:vec4<f32> = let %7
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureLoad_2DLevelI32) {
    auto* type = ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.i32());
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Zero<vec2<i32>>();
        auto* level = b.Zero<i32>();
        b.Let("x", b.Call<vec4<i32>>(core::BuiltinFn::kTextureLoad, b.Load(var), coords, level));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<i32>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_2d<i32> = load %v
    %4:vec4<i32> = textureLoad %3, vec2<i32>(0i), 0i
    %x:vec4<i32> = let %4
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_2d<i32>, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_2d<i32> = load %v
    %4:vec2<i32> = convert vec2<i32>(0i)
    %5:i32 = convert 0i
    %6:vec4<i32> = glsl.texelFetch %3, %4, %5
    %x:vec4<i32> = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureLoad_3DLevelU32) {
    auto* type = ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k3d, ty.f32());
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Zero<vec3<i32>>();
        auto* level = b.Zero<u32>();
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, b.Load(var), coords, level));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_3d<f32>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_3d<f32> = load %v
    %4:vec4<f32> = textureLoad %3, vec3<i32>(0i), 0u
    %x:vec4<f32> = let %4
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_3d<f32>, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_3d<f32> = load %v
    %4:vec3<i32> = convert vec3<i32>(0i)
    %5:i32 = convert 0u
    %6:vec4<f32> = glsl.texelFetch %3, %4, %5
    %x:vec4<f32> = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureLoad_Multisampled2DI32) {
    auto* type =
        ty.Get<core::type::MultisampledTexture>(core::type::TextureDimension::k2d, ty.i32());
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Zero<vec2<i32>>();
        auto* sample_idx = b.Zero<u32>();
        b.Let("x",
              b.Call<vec4<i32>>(core::BuiltinFn::kTextureLoad, b.Load(var), coords, sample_idx));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_multisampled_2d<i32>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_multisampled_2d<i32> = load %v
    %4:vec4<i32> = textureLoad %3, vec2<i32>(0i), 0u
    %x:vec4<i32> = let %4
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_multisampled_2d<i32>, read_write> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_multisampled_2d<i32> = load %v
    %4:vec2<i32> = convert vec2<i32>(0i)
    %5:i32 = convert 0u
    %6:vec4<i32> = glsl.texelFetch %3, %4, %5
    %x:vec4<i32> = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureLoad_Storage2D) {
    auto* type = ty.Get<core::type::StorageTexture>(
        core::type::TextureDimension::k2d, core::TexelFormat::kRg32Float, core::Access::kRead,
        core::type::StorageTexture::SubtypeFor(core::TexelFormat::kRg32Float, ty));
    auto* var = b.Var("v", handle, type, core::Access::kReadWrite);
    var->SetBindingPoint(0, 0);
    b.ir.root_block->Append(var);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Zero<vec2<i32>>();
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureLoad, b.Load(var), coords));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %v:ptr<handle, texture_storage_2d<rg32float, read>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d<rg32float, read> = load %v
    %4:vec4<f32> = textureLoad %3, vec2<i32>(0i)
    %x:vec4<f32> = let %4
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %v:ptr<handle, texture_storage_2d<rg32float, read>, read_write> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d<rg32float, read> = load %v
    %4:vec2<i32> = convert vec2<i32>(0i)
    %5:vec4<f32> = glsl.imageLoad %3, %4
    %x:vec4<f32> = let %5
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureStore1D) {
    auto* t = b.Var(ty.ptr(
        handle, ty.Get<core::type::StorageTexture>(
                    core::type::TextureDimension::k1d, core::TexelFormat::kR32Float,
                    core::Access::kReadWrite,
                    core::type::StorageTexture::SubtypeFor(core::TexelFormat::kR32Float, ty))));
    t->SetBindingPoint(0, 0);
    b.ir.root_block->Append(t);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Value(1_i);
        auto* value = b.Composite(ty.vec4<f32>(), .5_f, 0_f, 0_f, 1_f);
        b.Call(ty.void_(), core::BuiltinFn::kTextureStore, b.Load(t), coords, value);
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_storage_1d<r32float, read_write>, read> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_1d<r32float, read_write> = load %1
    %4:void = textureStore %3, 1i, vec4<f32>(0.5f, 0.0f, 0.0f, 1.0f)
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %1:ptr<handle, texture_storage_2d<r32float, read_write>, read> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d<r32float, read_write> = load %1
    %4:vec2<i32> = construct 1i, 0i
    %5:void = glsl.imageStore %3, %4, vec4<f32>(0.5f, 0.0f, 0.0f, 1.0f)
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureStore3D) {
    auto* t = b.Var(ty.ptr(
        handle, ty.Get<core::type::StorageTexture>(
                    core::type::TextureDimension::k3d, core::TexelFormat::kR32Float,
                    core::Access::kReadWrite,
                    core::type::StorageTexture::SubtypeFor(core::TexelFormat::kR32Float, ty))));
    t->SetBindingPoint(0, 0);
    b.ir.root_block->Append(t);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Composite(ty.vec3<i32>(), 1_i, 2_i, 3_i);
        auto* value = b.Composite(ty.vec4<f32>(), .5_f, 0_f, 0_f, 1_f);
        b.Call(ty.void_(), core::BuiltinFn::kTextureStore, b.Load(t), coords, value);
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_storage_3d<r32float, read_write>, read> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_3d<r32float, read_write> = load %1
    %4:void = textureStore %3, vec3<i32>(1i, 2i, 3i), vec4<f32>(0.5f, 0.0f, 0.0f, 1.0f)
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %1:ptr<handle, texture_storage_3d<r32float, read_write>, read> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_3d<r32float, read_write> = load %1
    %4:void = glsl.imageStore %3, vec3<i32>(1i, 2i, 3i), vec4<f32>(0.5f, 0.0f, 0.0f, 1.0f)
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureStoreArray) {
    auto* t = b.Var(ty.ptr(
        handle, ty.Get<core::type::StorageTexture>(
                    core::type::TextureDimension::k2dArray, core::TexelFormat::kRgba32Float,
                    core::Access::kReadWrite,
                    core::type::StorageTexture::SubtypeFor(core::TexelFormat::kR32Float, ty))));
    t->SetBindingPoint(0, 0);
    b.ir.root_block->Append(t);

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Composite(ty.vec2<i32>(), 1_i, 2_i);
        auto* value = b.Composite(ty.vec4<f32>(), .5_f, .4_f, .3_f, 1_f);
        b.Call(ty.void_(), core::BuiltinFn::kTextureStore, b.Load(t), coords, 3_u, value);
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_storage_2d_array<rgba32float, read_write>, read> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d_array<rgba32float, read_write> = load %1
    %4:void = textureStore %3, vec2<i32>(1i, 2i), 3u, vec4<f32>(0.5f, 0.40000000596046447754f, 0.30000001192092895508f, 1.0f)
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %1:ptr<handle, texture_storage_2d_array<rgba32float, read_write>, read> = var @binding_point(0, 0)
}

%foo = @fragment func():void {
  $B2: {
    %3:texture_storage_2d_array<rgba32float, read_write> = load %1
    %4:i32 = convert 3u
    %5:vec3<i32> = construct vec2<i32>(1i, 2i), %4
    %6:void = glsl.imageStore %3, %5, vec4<f32>(0.5f, 0.40000000596046447754f, 0.30000001192092895508f, 1.0f)
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, CombineSamplers_GlobalTextureNoSampler) {
    auto* t = b.Var(ty.ptr(
        handle, ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d, ty.f32()),
        read_write));
    t->SetBindingPoint(0, 0);
    b.ir.root_block->Append(t);

    auto* func = b.Function("foo", ty.vec2<u32>());
    b.Append(func->Block(), [&] {
        auto* tex = b.Load(t);
        auto* result = b.Call<vec2<u32>>(core::BuiltinFn::kTextureDimensions, tex);
        b.Return(func, result);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_2d<f32>, read_write> = var @binding_point(0, 0)
}

%foo = func():vec2<u32> {
  $B2: {
    %3:texture_2d<f32> = load %1
    %4:vec2<u32> = textureDimensions %3
    ret %4
  }
}
)";
    EXPECT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %my_tex:ptr<handle, texture_2d<f32>, read_write> = var
}

%foo = func():vec2<u32> {
  $B2: {
    %3:texture_2d<f32> = load %my_tex
    %4:vec2<i32> = glsl.textureSize %3, 0i
    %5:vec2<u32> = bitcast %4
    ret %5
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

// TODO(dsinclair): Add compare
TEST_F(GlslWriter_TexturePolyfillTest, DISABLED_TextureGatherCompare_Depth2d) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(
            ty.ptr(handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d)));
        tex->SetBindingPoint(0, 0);

        sampler = b.Var(ty.ptr(
            handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kComparisonSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* depth_ref = b.Value(3_f);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x",
              b.Call<vec4<f32>>(core::BuiltinFn::kTextureGatherCompare, t, s, coords, depth_ref));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_depth_2d = load %1
    %6:sampler_comparison = load %2
    %7:vec4<f32> = textureGatherCompare %5, %6, %4, 3.0f
    %x:vec4<f32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_depth_2d = load %1
    %6:sampler_comparison = load %2
    %7:vec4<f32> = %5.GatherCmp %6, %4, 3.0f
    %x:vec4<f32> = let %7
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

// TODO(dsinclair): Add compare
TEST_F(GlslWriter_TexturePolyfillTest, DISABLED_TextureGatherCompare_Depth2dOffset) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(
            ty.ptr(handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d)));
        tex->SetBindingPoint(0, 0);

        sampler = b.Var(ty.ptr(
            handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kComparisonSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* depth_ref = b.Value(3_f);
        auto* offset = b.Construct(ty.vec2<i32>(), b.Value(4_i), b.Value(5_i));

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureGatherCompare, t, s, coords,
                                     depth_ref, offset));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:vec2<i32> = construct 4i, 5i
    %6:texture_depth_2d = load %1
    %7:sampler_comparison = load %2
    %8:vec4<f32> = textureGatherCompare %6, %7, %4, 3.0f, %5
    %x:vec4<f32> = let %8
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:vec2<i32> = construct 4i, 5i
    %6:texture_depth_2d = load %1
    %7:sampler_comparison = load %2
    %8:vec4<f32> = %6.GatherCmp %7, %4, 3.0f, %5
    %x:vec4<f32> = let %8
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

// TODO(dsinclair): Add compare
TEST_F(GlslWriter_TexturePolyfillTest, DISABLED_TextureGatherCompare_DepthCubeArray) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(
            handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::kCubeArray)));
        tex->SetBindingPoint(0, 0);

        sampler = b.Var(ty.ptr(
            handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kComparisonSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec3<f32>(), b.Value(1_f), b.Value(2_f), b.Value(2.5_f));
        auto* array_idx = b.Value(6_u);
        auto* depth_ref = b.Value(3_f);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureGatherCompare, t, s, coords,
                                     array_idx, depth_ref));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_cube_array, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec3<f32> = construct 1.0f, 2.0f, 2.5f
    %5:texture_depth_cube_array = load %1
    %6:sampler_comparison = load %2
    %7:vec4<f32> = textureGatherCompare %5, %6, %4, 6u, 3.0f
    %x:vec4<f32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_cube_array, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec3<f32> = construct 1.0f, 2.0f, 2.5f
    %5:texture_depth_cube_array = load %1
    %6:sampler_comparison = load %2
    %7:f32 = convert 6u
    %8:vec4<f32> = construct %4, %7
    %9:vec4<f32> = %5.GatherCmp %6, %8, 3.0f
    %x:vec4<f32> = let %9
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

// TODO(dsinclair): Add compare
TEST_F(GlslWriter_TexturePolyfillTest, DISABLED_TextureGatherCompare_Depth2dArrayOffset) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(
            handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2dArray)));
        tex->SetBindingPoint(0, 0);

        sampler = b.Var(ty.ptr(
            handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kComparisonSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* array_idx = b.Value(6_i);
        auto* depth_ref = b.Value(3_f);
        auto* offset = b.Construct(ty.vec2<i32>(), b.Value(4_i), b.Value(5_i));

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureGatherCompare, t, s, coords,
                                     array_idx, depth_ref, offset));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d_array, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:vec2<i32> = construct 4i, 5i
    %6:texture_depth_2d_array = load %1
    %7:sampler_comparison = load %2
    %8:vec4<f32> = textureGatherCompare %6, %7, %4, 6i, 3.0f, %5
    %x:vec4<f32> = let %8
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d_array, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler_comparison, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:vec2<i32> = construct 4i, 5i
    %6:texture_depth_2d_array = load %1
    %7:sampler_comparison = load %2
    %8:f32 = convert 6i
    %9:vec3<f32> = construct %4, %8
    %10:vec4<f32> = %6.GatherCmp %7, %9, 3.0f, %5
    %x:vec4<f32> = let %10
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_Alpha) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(handle, ty.Get<core::type::SampledTexture>(
                                       core::type::TextureDimension::k2d, ty.i32())));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<i32>>(core::BuiltinFn::kTextureGather, 3_u, t, s, coords));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_2d<i32>, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_2d<i32> = load %1
    %6:sampler = load %2
    %7:vec4<i32> = textureGather 3u, %5, %6, %4
    %x:vec4<i32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_2d<i32>, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_2d<i32> = load %t_s
    %5:i32 = convert 3u
    %6:vec4<i32> = glsl.textureGather %4, %3, %5
    %x:vec4<i32> = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}
TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_RedOffset) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(handle, ty.Get<core::type::SampledTexture>(
                                       core::type::TextureDimension::k2d, ty.i32())));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* offset = b.Composite<vec2<i32>>(1_i, 3_i);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<i32>>(core::BuiltinFn::kTextureGather, 0_u, t, s, coords, offset));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_2d<i32>, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_2d<i32> = load %1
    %6:sampler = load %2
    %7:vec4<i32> = textureGather 0u, %5, %6, %4, vec2<i32>(1i, 3i)
    %x:vec4<i32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_2d<i32>, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_2d<i32> = load %t_s
    %5:i32 = convert 0u
    %6:vec4<i32> = glsl.textureGatherOffset %4, %3, vec2<i32>(1i, 3i), %5
    %x:vec4<i32> = let %6
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}
TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_GreenArray) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(handle, ty.Get<core::type::SampledTexture>(
                                       core::type::TextureDimension::k2dArray, ty.i32())));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* array_idx = b.Value(1_u);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x",
              b.Call<vec4<i32>>(core::BuiltinFn::kTextureGather, 1_u, t, s, coords, array_idx));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_2d_array<i32>, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_2d_array<i32> = load %1
    %6:sampler = load %2
    %7:vec4<i32> = textureGather 1u, %5, %6, %4, 1u
    %x:vec4<i32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_2d_array<i32>, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_2d_array<i32> = load %t_s
    %5:f32 = convert 1u
    %6:vec3<f32> = construct %3, %5
    %7:i32 = convert 1u
    %8:vec4<i32> = glsl.textureGather %4, %6, %7
    %x:vec4<i32> = let %8
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_BlueArrayOffset) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(handle, ty.Get<core::type::SampledTexture>(
                                       core::type::TextureDimension::k2dArray, ty.i32())));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* array_idx = b.Value(1_i);
        auto* offset = b.Composite<vec2<i32>>(1_i, 2_i);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<i32>>(core::BuiltinFn::kTextureGather, 2_u, t, s, coords, array_idx,
                                     offset));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_2d_array<i32>, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_2d_array<i32> = load %1
    %6:sampler = load %2
    %7:vec4<i32> = textureGather 2u, %5, %6, %4, 1i, vec2<i32>(1i, 2i)
    %x:vec4<i32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_2d_array<i32>, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_2d_array<i32> = load %t_s
    %5:f32 = convert 1i
    %6:vec3<f32> = construct %3, %5
    %7:i32 = convert 2u
    %8:vec4<i32> = glsl.textureGatherOffset %4, %6, vec2<i32>(1i, 2i), %7
    %x:vec4<i32> = let %8
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_Depth) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(
            ty.ptr(handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d)));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, t, s, coords));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_depth_2d = load %1
    %6:sampler = load %2
    %7:vec4<f32> = textureGather %5, %6, %4
    %x:vec4<f32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_depth_2d, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_depth_2d = load %t_s
    %5:vec4<f32> = glsl.textureGather %4, %3, 0.0f
    %x:vec4<f32> = let %5
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}
TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_DepthOffset) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(
            ty.ptr(handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2d)));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* offset = b.Composite<vec2<i32>>(3_i, 4_i);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, t, s, coords, offset));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_depth_2d = load %1
    %6:sampler = load %2
    %7:vec4<f32> = textureGather %5, %6, %4, vec2<i32>(3i, 4i)
    %x:vec4<f32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_depth_2d, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_depth_2d = load %t_s
    %5:vec4<f32> = glsl.textureGatherOffset %4, %3, 0.0f, vec2<i32>(3i, 4i)
    %x:vec4<f32> = let %5
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_DepthArray) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(
            handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2dArray)));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* array_idx = b.Value(4_i);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x", b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, t, s, coords, array_idx));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d_array, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_depth_2d_array = load %1
    %6:sampler = load %2
    %7:vec4<f32> = textureGather %5, %6, %4, 4i
    %x:vec4<f32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_depth_2d_array, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_depth_2d_array = load %t_s
    %5:f32 = convert 4i
    %6:vec3<f32> = construct %3, %5
    %7:vec4<f32> = glsl.textureGather %4, %6, 0.0f
    %x:vec4<f32> = let %7
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

TEST_F(GlslWriter_TexturePolyfillTest, TextureGather_DepthArrayOffset) {
    core::ir::Var* tex = nullptr;
    core::ir::Var* sampler = nullptr;
    b.Append(b.ir.root_block, [&] {
        tex = b.Var(ty.ptr(
            handle, ty.Get<core::type::DepthTexture>(core::type::TextureDimension::k2dArray)));
        tex->SetBindingPoint(0, 0);

        sampler =
            b.Var(ty.ptr(handle, ty.Get<core::type::Sampler>(core::type::SamplerKind::kSampler)));
        sampler->SetBindingPoint(0, 1);
    });

    auto* func = b.Function("foo", ty.void_(), core::ir::Function::PipelineStage::kFragment);
    b.Append(func->Block(), [&] {
        auto* coords = b.Construct(ty.vec2<f32>(), b.Value(1_f), b.Value(2_f));
        auto* array_idx = b.Value(4_u);
        auto* offset = b.Composite<vec2<i32>>(4_i, 5_i);

        auto* t = b.Load(tex);
        auto* s = b.Load(sampler);
        b.Let("x",
              b.Call<vec4<f32>>(core::BuiltinFn::kTextureGather, t, s, coords, array_idx, offset));
        b.Return(func);
    });

    auto* src = R"(
$B1: {  # root
  %1:ptr<handle, texture_depth_2d_array, read> = var @binding_point(0, 0)
  %2:ptr<handle, sampler, read> = var @binding_point(0, 1)
}

%foo = @fragment func():void {
  $B2: {
    %4:vec2<f32> = construct 1.0f, 2.0f
    %5:texture_depth_2d_array = load %1
    %6:sampler = load %2
    %7:vec4<f32> = textureGather %5, %6, %4, 4u, vec2<i32>(4i, 5i)
    %x:vec4<f32> = let %7
    ret
  }
}
)";
    ASSERT_EQ(src, str());

    auto* expect = R"(
$B1: {  # root
  %t_s:ptr<handle, texture_depth_2d_array, read> = var
}

%foo = @fragment func():void {
  $B2: {
    %3:vec2<f32> = construct 1.0f, 2.0f
    %4:texture_depth_2d_array = load %t_s
    %5:f32 = convert 4u
    %6:vec3<f32> = construct %3, %5
    %7:vec4<f32> = glsl.textureGatherOffset %4, %6, 0.0f, vec2<i32>(4i, 5i)
    %x:vec4<f32> = let %7
    ret
  }
}
)";

    capabilities = core::ir::Capabilities{core::ir::Capability::kAllowHandleVarsWithoutBindings};

    TexturePolyfillConfig cfg;
    cfg.placeholder_sampler_bind_point = {2, 2};

    binding::CombinedTextureSamplerPair pair;
    pair.texture = {0, 0};
    pair.sampler = {2, 2};
    cfg.sampler_texture_to_name[pair] = "my_tex";

    Run(TexturePolyfill, cfg);
    EXPECT_EQ(expect, str());
}

}  // namespace
}  // namespace tint::glsl::writer::raise

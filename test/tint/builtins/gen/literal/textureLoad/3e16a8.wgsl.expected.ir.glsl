#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer tint_symbol_1_1_ssbo {
  vec4 tint_symbol;
} v;
layout(binding = 0, r8) uniform highp image2D arg_0;
vec4 textureLoad_3e16a8() {
  vec4 res = imageLoad(arg_0, ivec2(ivec2(1)));
  return res;
}
void main() {
  v.tint_symbol = textureLoad_3e16a8();
}
#version 460

layout(binding = 0, std430)
buffer tint_symbol_1_1_ssbo {
  vec4 tint_symbol;
} v;
layout(binding = 0, r8) uniform highp image2D arg_0;
vec4 textureLoad_3e16a8() {
  vec4 res = imageLoad(arg_0, ivec2(ivec2(1)));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.tint_symbol = textureLoad_3e16a8();
}

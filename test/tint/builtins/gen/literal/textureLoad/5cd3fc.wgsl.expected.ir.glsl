#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer tint_symbol_1_1_ssbo {
  ivec4 tint_symbol;
} v;
layout(binding = 0, r32i) uniform highp iimage2D arg_0;
ivec4 textureLoad_5cd3fc() {
  ivec4 res = imageLoad(arg_0, ivec2(ivec2(1)));
  return res;
}
void main() {
  v.tint_symbol = textureLoad_5cd3fc();
}
#version 310 es

layout(binding = 0, std430)
buffer tint_symbol_1_1_ssbo {
  ivec4 tint_symbol;
} v;
layout(binding = 0, r32i) uniform highp iimage2D arg_0;
ivec4 textureLoad_5cd3fc() {
  ivec4 res = imageLoad(arg_0, ivec2(ivec2(1)));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.tint_symbol = textureLoad_5cd3fc();
}

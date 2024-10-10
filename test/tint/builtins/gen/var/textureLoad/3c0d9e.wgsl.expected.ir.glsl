#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec4 inner;
} v;
layout(binding = 0, rgba8ui) uniform highp readonly uimage2D arg_0;
uvec4 textureLoad_3c0d9e() {
  ivec2 arg_1 = ivec2(1);
  uvec4 res = imageLoad(arg_0, ivec2(arg_1));
  return res;
}
void main() {
  v.inner = textureLoad_3c0d9e();
}
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec4 inner;
} v;
layout(binding = 0, rgba8ui) uniform highp readonly uimage2D arg_0;
uvec4 textureLoad_3c0d9e() {
  ivec2 arg_1 = ivec2(1);
  uvec4 res = imageLoad(arg_0, ivec2(arg_1));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_3c0d9e();
}
#version 310 es


struct VertexOutput {
  vec4 pos;
  uvec4 prevent_dce;
};

layout(binding = 0, rgba8ui) uniform highp readonly uimage2D arg_0;
layout(location = 0) flat out uvec4 vertex_main_loc0_Output;
uvec4 textureLoad_3c0d9e() {
  ivec2 arg_1 = ivec2(1);
  uvec4 res = imageLoad(arg_0, ivec2(arg_1));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), uvec4(0u));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureLoad_3c0d9e();
  return tint_symbol;
}
void main() {
  VertexOutput v = vertex_main_inner();
  gl_Position = v.pos;
  gl_Position[1u] = -(gl_Position.y);
  gl_Position[2u] = ((2.0f * gl_Position.z) - gl_Position.w);
  vertex_main_loc0_Output = v.prevent_dce;
  gl_PointSize = 1.0f;
}

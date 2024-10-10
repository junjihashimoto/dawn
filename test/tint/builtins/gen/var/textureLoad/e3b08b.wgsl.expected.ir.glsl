#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec4 inner;
} v;
layout(binding = 0, r32f) uniform highp readonly image3D arg_0;
vec4 textureLoad_e3b08b() {
  uvec3 arg_1 = uvec3(1u);
  vec4 res = imageLoad(arg_0, ivec3(arg_1));
  return res;
}
void main() {
  v.inner = textureLoad_e3b08b();
}
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec4 inner;
} v;
layout(binding = 0, r32f) uniform highp readonly image3D arg_0;
vec4 textureLoad_e3b08b() {
  uvec3 arg_1 = uvec3(1u);
  vec4 res = imageLoad(arg_0, ivec3(arg_1));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_e3b08b();
}
#version 310 es


struct VertexOutput {
  vec4 pos;
  vec4 prevent_dce;
};

layout(binding = 0, r32f) uniform highp readonly image3D arg_0;
layout(location = 0) flat out vec4 vertex_main_loc0_Output;
vec4 textureLoad_e3b08b() {
  uvec3 arg_1 = uvec3(1u);
  vec4 res = imageLoad(arg_0, ivec3(arg_1));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), vec4(0.0f));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureLoad_e3b08b();
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

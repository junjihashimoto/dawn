#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  float inner;
} v;
uniform highp sampler2DArray arg_0;
float textureLoad_cb57c2() {
  ivec2 v_1 = ivec2(uvec2(1u));
  ivec3 v_2 = ivec3(v_1, int(1u));
  float res = texelFetch(arg_0, v_2, int(1)).x;
  return res;
}
void main() {
  v.inner = textureLoad_cb57c2();
}
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  float inner;
} v;
uniform highp sampler2DArray arg_0;
float textureLoad_cb57c2() {
  ivec2 v_1 = ivec2(uvec2(1u));
  ivec3 v_2 = ivec3(v_1, int(1u));
  float res = texelFetch(arg_0, v_2, int(1)).x;
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_cb57c2();
}
#version 310 es


struct VertexOutput {
  vec4 pos;
  float prevent_dce;
};

uniform highp sampler2DArray arg_0;
layout(location = 0) flat out float vertex_main_loc0_Output;
float textureLoad_cb57c2() {
  ivec2 v = ivec2(uvec2(1u));
  ivec3 v_1 = ivec3(v, int(1u));
  float res = texelFetch(arg_0, v_1, int(1)).x;
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), 0.0f);
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureLoad_cb57c2();
  return tint_symbol;
}
void main() {
  VertexOutput v_2 = vertex_main_inner();
  gl_Position = v_2.pos;
  gl_Position[1u] = -(gl_Position.y);
  gl_Position[2u] = ((2.0f * gl_Position.z) - gl_Position.w);
  vertex_main_loc0_Output = v_2.prevent_dce;
  gl_PointSize = 1.0f;
}

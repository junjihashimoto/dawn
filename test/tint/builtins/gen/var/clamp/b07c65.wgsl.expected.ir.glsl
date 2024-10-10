#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  int inner;
} v;
int clamp_b07c65() {
  int arg_0 = 1;
  int arg_1 = 1;
  int arg_2 = 1;
  int v_1 = arg_2;
  int res = min(max(arg_0, arg_1), v_1);
  return res;
}
void main() {
  v.inner = clamp_b07c65();
}
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  int inner;
} v;
int clamp_b07c65() {
  int arg_0 = 1;
  int arg_1 = 1;
  int arg_2 = 1;
  int v_1 = arg_2;
  int res = min(max(arg_0, arg_1), v_1);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = clamp_b07c65();
}
#version 310 es


struct VertexOutput {
  vec4 pos;
  int prevent_dce;
};

layout(location = 0) flat out int vertex_main_loc0_Output;
int clamp_b07c65() {
  int arg_0 = 1;
  int arg_1 = 1;
  int arg_2 = 1;
  int v = arg_2;
  int res = min(max(arg_0, arg_1), v);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), 0);
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = clamp_b07c65();
  return tint_symbol;
}
void main() {
  VertexOutput v_1 = vertex_main_inner();
  gl_Position = v_1.pos;
  gl_Position[1u] = -(gl_Position.y);
  gl_Position[2u] = ((2.0f * gl_Position.z) - gl_Position.w);
  vertex_main_loc0_Output = v_1.prevent_dce;
  gl_PointSize = 1.0f;
}

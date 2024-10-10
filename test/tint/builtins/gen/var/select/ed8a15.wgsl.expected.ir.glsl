#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  int inner;
} v;
int select_ed8a15() {
  int arg_0 = 1;
  int arg_1 = 1;
  bool arg_2 = true;
  int res = mix(arg_0, arg_1, arg_2);
  return res;
}
void main() {
  v.inner = select_ed8a15();
}
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  int inner;
} v;
int select_ed8a15() {
  int arg_0 = 1;
  int arg_1 = 1;
  bool arg_2 = true;
  int res = mix(arg_0, arg_1, arg_2);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = select_ed8a15();
}
#version 310 es


struct VertexOutput {
  vec4 pos;
  int prevent_dce;
};

layout(location = 0) flat out int vertex_main_loc0_Output;
int select_ed8a15() {
  int arg_0 = 1;
  int arg_1 = 1;
  bool arg_2 = true;
  int res = mix(arg_0, arg_1, arg_2);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), 0);
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = select_ed8a15();
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

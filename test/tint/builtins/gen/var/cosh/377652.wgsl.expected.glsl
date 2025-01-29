//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  vec3 inner;
} v;
vec3 cosh_377652() {
  vec3 arg_0 = vec3(0.0f);
  vec3 res = cosh(arg_0);
  return res;
}
void main() {
  v.inner = cosh_377652();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec3 inner;
} v;
vec3 cosh_377652() {
  vec3 arg_0 = vec3(0.0f);
  vec3 res = cosh(arg_0);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = cosh_377652();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  vec3 prevent_dce;
};

layout(location = 0) flat out vec3 tint_interstage_location0;
vec3 cosh_377652() {
  vec3 arg_0 = vec3(0.0f);
  vec3 res = cosh(arg_0);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f), vec3(0.0f));
  v.pos = vec4(0.0f);
  v.prevent_dce = cosh_377652();
  return v;
}
void main() {
  VertexOutput v_1 = vertex_main_inner();
  gl_Position = vec4(v_1.pos.x, -(v_1.pos.y), ((2.0f * v_1.pos.z) - v_1.pos.w), v_1.pos.w);
  tint_interstage_location0 = v_1.prevent_dce;
  gl_PointSize = 1.0f;
}

//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  ivec4 inner;
} v;
layout(binding = 0, rgba8i) uniform highp readonly iimage2D arg_0;
ivec4 textureLoad_f35ac7() {
  uint arg_1 = 1u;
  uint v_1 = arg_1;
  ivec4 res = imageLoad(arg_0, ivec2(uvec2(min(v_1, (uvec2(imageSize(arg_0)).x - 1u)), 0u)));
  return res;
}
void main() {
  v.inner = textureLoad_f35ac7();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  ivec4 inner;
} v;
layout(binding = 0, rgba8i) uniform highp readonly iimage2D arg_0;
ivec4 textureLoad_f35ac7() {
  uint arg_1 = 1u;
  uint v_1 = arg_1;
  ivec4 res = imageLoad(arg_0, ivec2(uvec2(min(v_1, (uvec2(imageSize(arg_0)).x - 1u)), 0u)));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_f35ac7();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  ivec4 prevent_dce;
};

layout(binding = 0, rgba8i) uniform highp readonly iimage2D arg_0;
layout(location = 0) flat out ivec4 tint_interstage_location0;
ivec4 textureLoad_f35ac7() {
  uint arg_1 = 1u;
  uint v = arg_1;
  ivec4 res = imageLoad(arg_0, ivec2(uvec2(min(v, (uvec2(imageSize(arg_0)).x - 1u)), 0u)));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_1 = VertexOutput(vec4(0.0f), ivec4(0));
  v_1.pos = vec4(0.0f);
  v_1.prevent_dce = textureLoad_f35ac7();
  return v_1;
}
void main() {
  VertexOutput v_2 = vertex_main_inner();
  gl_Position = vec4(v_2.pos.x, -(v_2.pos.y), ((2.0f * v_2.pos.z) - v_2.pos.w), v_2.pos.w);
  tint_interstage_location0 = v_2.prevent_dce;
  gl_PointSize = 1.0f;
}

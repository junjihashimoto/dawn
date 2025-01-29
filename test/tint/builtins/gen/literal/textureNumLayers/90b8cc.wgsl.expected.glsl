//
// fragment_main
//
#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  uint inner;
} v;
layout(binding = 0, rg32f) uniform highp readonly image2DArray arg_0;
uint textureNumLayers_90b8cc() {
  uint res = uint(imageSize(arg_0).z);
  return res;
}
void main() {
  v.inner = textureNumLayers_90b8cc();
}
//
// compute_main
//
#version 460

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uint inner;
} v;
layout(binding = 0, rg32f) uniform highp readonly image2DArray arg_0;
uint textureNumLayers_90b8cc() {
  uint res = uint(imageSize(arg_0).z);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureNumLayers_90b8cc();
}
//
// vertex_main
//
#version 460


struct VertexOutput {
  vec4 pos;
  uint prevent_dce;
};

layout(binding = 0, rg32f) uniform highp readonly image2DArray arg_0;
layout(location = 0) flat out uint tint_interstage_location0;
uint textureNumLayers_90b8cc() {
  uint res = uint(imageSize(arg_0).z);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f), 0u);
  v.pos = vec4(0.0f);
  v.prevent_dce = textureNumLayers_90b8cc();
  return v;
}
void main() {
  VertexOutput v_1 = vertex_main_inner();
  gl_Position = vec4(v_1.pos.x, -(v_1.pos.y), ((2.0f * v_1.pos.z) - v_1.pos.w), v_1.pos.w);
  tint_interstage_location0 = v_1.prevent_dce;
  gl_PointSize = 1.0f;
}

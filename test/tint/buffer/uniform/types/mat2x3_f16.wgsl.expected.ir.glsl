#version 310 es
#extension GL_AMD_gpu_shader_half_float: require

layout(binding = 0, std140)
uniform u_block_std140_1_ubo {
  f16vec3 inner_col0;
  f16vec3 inner_col1;
} v;
layout(binding = 1, std430)
buffer s_block_1_ssbo {
  f16mat2x3 inner;
} v_1;
void tint_store_and_preserve_padding(inout f16mat2x3 target, f16mat2x3 value_param) {
  target[0u] = value_param[0u];
  target[1u] = value_param[1u];
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  f16mat2x3 x = f16mat2x3(v.inner_col0, v.inner_col1);
  tint_store_and_preserve_padding(v_1.inner, x);
}

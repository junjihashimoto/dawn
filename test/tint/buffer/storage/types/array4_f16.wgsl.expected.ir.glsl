#version 310 es
#extension GL_AMD_gpu_shader_half_float: require

layout(binding = 0, std430)
buffer tint_symbol_block_1_ssbo {
  float16_t inner[4];
} v;
layout(binding = 1, std430)
buffer tint_symbol_1_block_1_ssbo {
  float16_t inner[4];
} v_1;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v_1.inner = v.inner;
}

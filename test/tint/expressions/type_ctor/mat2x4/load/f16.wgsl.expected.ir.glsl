#version 310 es
#extension GL_AMD_gpu_shader_half_float: require

layout(binding = 0, std430)
buffer tint_symbol_block_1_ssbo {
  f16mat2x4 inner;
} v;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  f16mat2x4 m = f16mat2x4(f16vec4(0.0hf), f16vec4(0.0hf));
  v.inner = f16mat2x4(m);
}

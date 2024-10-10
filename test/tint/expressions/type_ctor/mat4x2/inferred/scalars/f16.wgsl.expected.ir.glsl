#version 310 es
#extension GL_AMD_gpu_shader_half_float: require

f16mat4x2 m = f16mat4x2(f16vec2(0.0hf, 1.0hf), f16vec2(2.0hf, 3.0hf), f16vec2(4.0hf, 5.0hf), f16vec2(6.0hf, 7.0hf));
layout(binding = 0, std430)
buffer tint_symbol_block_1_ssbo {
  f16mat4x2 inner;
} v;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = m;
}

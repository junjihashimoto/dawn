#version 310 es

layout(binding = 0, std430)
buffer tint_symbol_block_1_ssbo {
  mat3x4 inner;
} v;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  mat3x4 m = mat3x4(vec4(0.0f), vec4(0.0f), vec4(0.0f));
  v.inner = mat3x4(m);
}

#version 310 es
#extension GL_AMD_gpu_shader_half_float: require

f16mat3 m = f16mat3(f16vec3(0.0hf, 1.0hf, 2.0hf), f16vec3(3.0hf, 4.0hf, 5.0hf), f16vec3(6.0hf, 7.0hf, 8.0hf));
layout(binding = 0, std430)
buffer tint_symbol_block_1_ssbo {
  f16mat3 inner;
} v;
void tint_store_and_preserve_padding(inout f16mat3 target, f16mat3 value_param) {
  target[0u] = value_param[0u];
  target[1u] = value_param[1u];
  target[2u] = value_param[2u];
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  tint_store_and_preserve_padding(v.inner, m);
}

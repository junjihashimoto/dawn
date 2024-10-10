#version 310 es


struct S {
  uvec3 v;
  uint tint_pad;
};

layout(binding = 0, std430)
buffer U_block_1_ssbo {
  S inner;
} v_1;
void f() {
  v_1.inner.v = uvec3(1u, 2u, 3u);
  v_1.inner.v[0u] = 1u;
  v_1.inner.v[1u] = 2u;
  v_1.inner.v[2u] = 3u;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
}

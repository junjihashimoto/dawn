#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec2 inner;
} v;
vec2 fwidth_b83ebb() {
  vec2 res = fwidth(vec2(1.0f));
  return res;
}
void main() {
  v.inner = fwidth_b83ebb();
}

#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uint inner;
} v;
layout(binding = 1, std430)
buffer SB_RW_1_ssbo {
  uint arg_0[];
} sb_rw;
uint arrayLength_eb510f() {
  uint res = uint(sb_rw.arg_0.length());
  return res;
}
void main() {
  v.inner = arrayLength_eb510f();
}
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uint inner;
} v;
layout(binding = 1, std430)
buffer SB_RW_1_ssbo {
  uint arg_0[];
} sb_rw;
uint arrayLength_eb510f() {
  uint res = uint(sb_rw.arg_0.length());
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = arrayLength_eb510f();
}

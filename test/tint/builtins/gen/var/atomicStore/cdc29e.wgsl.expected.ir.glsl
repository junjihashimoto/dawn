#version 310 es
precision highp float;
precision highp int;


struct SB_RW {
  uint arg_0;
};

layout(binding = 0, std430)
buffer sb_rw_block_1_ssbo {
  SB_RW inner;
} v;
void atomicStore_cdc29e() {
  uint arg_1 = 1u;
  atomicExchange(v.inner.arg_0, arg_1);
}
void main() {
  atomicStore_cdc29e();
}
#version 310 es


struct SB_RW {
  uint arg_0;
};

layout(binding = 0, std430)
buffer sb_rw_block_1_ssbo {
  SB_RW inner;
} v;
void atomicStore_cdc29e() {
  uint arg_1 = 1u;
  atomicExchange(v.inner.arg_0, arg_1);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  atomicStore_cdc29e();
}

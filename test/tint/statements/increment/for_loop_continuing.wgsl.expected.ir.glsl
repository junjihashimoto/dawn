#version 310 es

layout(binding = 0, std430)
buffer i_block_1_ssbo {
  uint inner;
} v;
void tint_symbol() {
  {
    while(true) {
      if ((v.inner < 10u)) {
      } else {
        break;
      }
      {
        v.inner = (v.inner + 1u);
      }
      continue;
    }
  }
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
}

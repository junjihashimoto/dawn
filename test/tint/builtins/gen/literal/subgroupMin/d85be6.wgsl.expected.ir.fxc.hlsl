SKIP: INVALID


RWByteAddressBuffer prevent_dce : register(u0);
vector<float16_t, 2> subgroupMin_d85be6() {
  vector<float16_t, 2> res = WaveActiveMin((float16_t(1.0h)).xx);
  return res;
}

void fragment_main() {
  prevent_dce.Store<vector<float16_t, 2> >(0u, subgroupMin_d85be6());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store<vector<float16_t, 2> >(0u, subgroupMin_d85be6());
}

FXC validation failure:
<scrubbed_path>(3,8-16): error X3000: syntax error: unexpected token 'float16_t'


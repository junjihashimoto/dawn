SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
vector<float16_t, 3> subgroupShuffleDown_63fdb0() {
  vector<float16_t, 3> arg_0 = (float16_t(1.0h)).xxx;
  uint arg_1 = 1u;
  vector<float16_t, 3> v = arg_0;
  uint v_1 = arg_1;
  vector<float16_t, 3> res = WaveReadLaneAt(v, (WaveGetLaneIndex() + v_1));
  return res;
}

void fragment_main() {
  prevent_dce.Store<vector<float16_t, 3> >(0u, subgroupShuffleDown_63fdb0());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store<vector<float16_t, 3> >(0u, subgroupShuffleDown_63fdb0());
}

FXC validation failure:
<scrubbed_path>(3,8-16): error X3000: syntax error: unexpected token 'float16_t'


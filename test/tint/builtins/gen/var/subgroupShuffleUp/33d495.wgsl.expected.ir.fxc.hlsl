SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
float4 subgroupShuffleUp_33d495() {
  float4 arg_0 = (1.0f).xxxx;
  uint arg_1 = 1u;
  float4 v = arg_0;
  uint v_1 = arg_1;
  float4 res = WaveReadLaneAt(v, (WaveGetLaneIndex() - v_1));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(subgroupShuffleUp_33d495()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(subgroupShuffleUp_33d495()));
}

FXC validation failure:
<scrubbed_path>(8,35-52): error X3004: undeclared identifier 'WaveGetLaneIndex'


SKIP: FAILED

RWByteAddressBuffer prevent_dce : register(u0);

uint2 subgroupMax_b8fb0e() {
  uint2 arg_0 = (1u).xx;
  uint2 res = WaveActiveMax(arg_0);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, asuint(subgroupMax_b8fb0e()));
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, asuint(subgroupMax_b8fb0e()));
  return;
}
FXC validation failure:
<scrubbed_path>(5,15-34): error X3004: undeclared identifier 'WaveActiveMax'


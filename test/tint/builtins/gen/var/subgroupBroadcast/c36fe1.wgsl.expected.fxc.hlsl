SKIP: FAILED

RWByteAddressBuffer prevent_dce : register(u0);

uint subgroupBroadcast_c36fe1() {
  uint arg_0 = 1u;
  uint res = WaveReadLaneAt(arg_0, 1u);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(subgroupBroadcast_c36fe1()));
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(subgroupBroadcast_c36fe1()));
  return;
}
FXC validation failure:
<scrubbed_path>(5,14-38): error X3004: undeclared identifier 'WaveReadLaneAt'


SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
uint quadSwapX_8203ad() {
  uint arg_0 = 1u;
  uint res = QuadReadAcrossX(arg_0);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, quadSwapX_8203ad());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, quadSwapX_8203ad());
}

FXC validation failure:
<scrubbed_path>(5,14-35): error X3004: undeclared identifier 'QuadReadAcrossX'


//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
RWTexture2DArray<uint4> arg_0 : register(u0, space1);
uint4 textureLoad_d0e351() {
  uint4 res = arg_0.Load(int4(int2((1u).xx), int(1), int(0)));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, textureLoad_d0e351());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
RWTexture2DArray<uint4> arg_0 : register(u0, space1);
uint4 textureLoad_d0e351() {
  uint4 res = arg_0.Load(int4(int2((1u).xx), int(1), int(0)));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, textureLoad_d0e351());
}


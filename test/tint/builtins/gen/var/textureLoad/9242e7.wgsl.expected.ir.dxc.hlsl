//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
RWTexture2D<uint4> arg_0 : register(u0, space1);
uint4 textureLoad_9242e7() {
  uint2 arg_1 = (1u).xx;
  uint4 res = uint4(arg_0.Load(int3(int2(arg_1), int(0))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, textureLoad_9242e7());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
RWTexture2D<uint4> arg_0 : register(u0, space1);
uint4 textureLoad_9242e7() {
  uint2 arg_1 = (1u).xx;
  uint4 res = uint4(arg_0.Load(int3(int2(arg_1), int(0))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, textureLoad_9242e7());
}


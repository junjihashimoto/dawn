
RWByteAddressBuffer prevent_dce : register(u0);
RWTexture2D<uint4> arg_0 : register(u0, space1);
uint4 textureLoad_02ef1f() {
  int2 arg_1 = (1).xx;
  RWTexture2D<uint4> v = arg_0;
  uint4 res = uint4(v.Load(int3(int2(arg_1), 0)));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, textureLoad_02ef1f());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, textureLoad_02ef1f());
}


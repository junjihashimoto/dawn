
RWByteAddressBuffer prevent_dce : register(u0);
RWTexture3D<uint4> arg_0 : register(u0, space1);
uint4 textureLoad_4c1a1e() {
  RWTexture3D<uint4> v = arg_0;
  uint4 res = uint4(v.Load(int4(int3((1).xxx), 0)));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, textureLoad_4c1a1e());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, textureLoad_4c1a1e());
}


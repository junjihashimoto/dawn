
RWByteAddressBuffer prevent_dce : register(u0);
RWTexture2D<int4> arg_0 : register(u0, space1);
int4 textureLoad_5cd3fc() {
  int2 arg_1 = (1).xx;
  RWTexture2D<int4> v = arg_0;
  int4 res = int4(v.Load(int3(int2(arg_1), 0)));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_5cd3fc()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_5cd3fc()));
}


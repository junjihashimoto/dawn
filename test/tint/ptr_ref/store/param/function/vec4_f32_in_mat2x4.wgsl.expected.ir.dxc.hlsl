
void func(inout float4 pointer) {
  pointer = (0.0f).xxxx;
}

[numthreads(1, 1, 1)]
void main() {
  float2x4 F = float2x4((0.0f).xxxx, (0.0f).xxxx);
  func(F[1u]);
}


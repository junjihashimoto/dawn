SKIP: FAILED

[numthreads(1, 1, 1)]
void unused_entry_point() {
  return;
}

static vector<float16_t, 4> u = (float16_t(1.0h)).xxxx;

void f() {
  float4 v = float4(u);
}
FXC validation failure:
<scrubbed_path>(6,15-23): error X3000: syntax error: unexpected token 'float16_t'


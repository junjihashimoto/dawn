SKIP: FAILED

[numthreads(1, 1, 1)]
void unused_entry_point() {
  return;
}

static int t = 0;

int4 m() {
  t = 1;
  return int4((t).xxxx);
}

void f() {
  int4 tint_symbol = m();
  vector<float16_t, 4> v = vector<float16_t, 4>(tint_symbol);
}
FXC validation failure:
<scrubbed_path>(15,10-18): error X3000: syntax error: unexpected token 'float16_t'


SKIP: FAILED

RWByteAddressBuffer tint_symbol : register(u0);

void tint_symbol_store(uint offset, matrix<float16_t, 4, 3> value) {
  tint_symbol.Store<vector<float16_t, 3> >((offset + 0u), value[0u]);
  tint_symbol.Store<vector<float16_t, 3> >((offset + 8u), value[1u]);
  tint_symbol.Store<vector<float16_t, 3> >((offset + 16u), value[2u]);
  tint_symbol.Store<vector<float16_t, 3> >((offset + 24u), value[3u]);
}

[numthreads(1, 1, 1)]
void f() {
  matrix<float16_t, 4, 3> m = matrix<float16_t, 4, 3>((float16_t(0.0h)).xxx, (float16_t(0.0h)).xxx, (float16_t(0.0h)).xxx, (float16_t(0.0h)).xxx);
  tint_symbol_store(0u, matrix<float16_t, 4, 3>(m));
  return;
}
FXC validation failure:
<scrubbed_path>(3,44-52): error X3000: syntax error: unexpected token 'float16_t'
<scrubbed_path>(4,3-19): error X3018: invalid subscript 'Store'


SKIP: FAILED

[numthreads(1, 1, 1)]
void unused_entry_point() {
  return;
}

void f() {
  float16_t a = float16_t(1.0h);
  float16_t b = float16_t(1.0h);
  float16_t c = float16_t(1.0h);
}
FXC validation failure:
<scrubbed_path>(7,3-11): error X3000: unrecognized identifier 'float16_t'
<scrubbed_path>(7,13): error X3000: unrecognized identifier 'a'


#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer tint_symbol_1_1_ssbo {
  float tint_symbol;
} v;
uniform highp samplerCubeArrayShadow arg_0_arg_1;
float textureSampleCompare_a3ca7e() {
  vec3 arg_2 = vec3(1.0f);
  int arg_3 = 1;
  float arg_4 = 1.0f;
  vec3 v_1 = arg_2;
  float v_2 = arg_4;
  float res = texture(arg_0_arg_1, vec4(v_1, float(arg_3)), v_2);
  return res;
}
void main() {
  v.tint_symbol = textureSampleCompare_a3ca7e();
}

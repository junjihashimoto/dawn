#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  float inner;
} v;
uniform highp samplerCubeShadow arg_0_arg_1;
float textureSample_ea7030() {
  vec3 arg_2 = vec3(1.0f);
  float res = texture(arg_0_arg_1, vec4(arg_2, 0.0f));
  return res;
}
void main() {
  v.inner = textureSample_ea7030();
}

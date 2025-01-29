//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

void dot_c11efe() {
  int res = 3;
}
void main() {
  dot_c11efe();
}
//
// compute_main
//
#version 310 es

void dot_c11efe() {
  int res = 3;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  dot_c11efe();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
};

void dot_c11efe() {
  int res = 3;
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f));
  v.pos = vec4(0.0f);
  dot_c11efe();
  return v;
}
void main() {
  vec4 v_1 = vertex_main_inner().pos;
  gl_Position = vec4(v_1.x, -(v_1.y), ((2.0f * v_1.z) - v_1.w), v_1.w);
  gl_PointSize = 1.0f;
}

//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray arg_0 : register(t0, space1);
float textureLoad_cb57c2() {
  uint2 arg_1 = (1u).xx;
  uint arg_2 = 1u;
  int arg_3 = int(1);
  uint v = arg_2;
  int v_1 = arg_3;
  int2 v_2 = int2(arg_1);
  int v_3 = int(v);
  float res = arg_0.Load(int4(v_2, v_3, int(v_1))).x;
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(textureLoad_cb57c2()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray arg_0 : register(t0, space1);
float textureLoad_cb57c2() {
  uint2 arg_1 = (1u).xx;
  uint arg_2 = 1u;
  int arg_3 = int(1);
  uint v = arg_2;
  int v_1 = arg_3;
  int2 v_2 = int2(arg_1);
  int v_3 = int(v);
  float res = arg_0.Load(int4(v_2, v_3, int(v_1))).x;
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(textureLoad_cb57c2()));
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
  float prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation float VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


Texture2DArray arg_0 : register(t0, space1);
float textureLoad_cb57c2() {
  uint2 arg_1 = (1u).xx;
  uint arg_2 = 1u;
  int arg_3 = int(1);
  uint v = arg_2;
  int v_1 = arg_3;
  int2 v_2 = int2(arg_1);
  int v_3 = int(v);
  float res = arg_0.Load(int4(v_2, v_3, int(v_1))).x;
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_4 = (VertexOutput)0;
  v_4.pos = (0.0f).xxxx;
  v_4.prevent_dce = textureLoad_cb57c2();
  VertexOutput v_5 = v_4;
  return v_5;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_6 = vertex_main_inner();
  vertex_main_outputs v_7 = {v_6.prevent_dce, v_6.pos};
  return v_7;
}


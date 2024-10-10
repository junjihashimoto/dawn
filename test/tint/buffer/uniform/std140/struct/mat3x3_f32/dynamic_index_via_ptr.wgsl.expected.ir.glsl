#version 310 es


struct Inner_std140 {
  vec3 m_col0;
  uint tint_pad;
  vec3 m_col1;
  uint tint_pad_1;
  vec3 m_col2;
  uint tint_pad_2;
  uint tint_pad_3;
  uint tint_pad_4;
  uint tint_pad_5;
  uint tint_pad_6;
};

struct Outer_std140 {
  Inner_std140 a[4];
};

struct Inner {
  mat3 m;
};

struct Outer {
  Inner a[4];
};

layout(binding = 0, std140)
uniform a_block_std140_1_ubo {
  Outer_std140 inner[4];
} v;
int counter = 0;
int i() {
  counter = (counter + 1);
  return counter;
}
Inner tint_convert_Inner(Inner_std140 tint_input) {
  return Inner(mat3(tint_input.m_col0, tint_input.m_col1, tint_input.m_col2));
}
Outer tint_convert_Outer(Outer_std140 tint_input) {
  Inner v_1[4] = Inner[4](Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))));
  {
    uint v_2 = 0u;
    v_2 = 0u;
    while(true) {
      uint v_3 = v_2;
      if ((v_3 >= 4u)) {
        break;
      }
      v_1[v_3] = tint_convert_Inner(tint_input.a[v_3]);
      {
        v_2 = (v_3 + 1u);
      }
      continue;
    }
  }
  return Outer(v_1);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  int v_4 = i();
  int v_5 = i();
  mat3 v_6 = mat3(v.inner[v_4].a[v_5].m_col0, v.inner[v_4].a[v_5].m_col1, v.inner[v_4].a[v_5].m_col2);
  vec3 v_7 = v_6[i()];
  Outer_std140 v_8[4] = v.inner;
  Outer v_9[4] = Outer[4](Outer(Inner[4](Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))))), Outer(Inner[4](Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))))), Outer(Inner[4](Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))))), Outer(Inner[4](Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))))));
  {
    uint v_10 = 0u;
    v_10 = 0u;
    while(true) {
      uint v_11 = v_10;
      if ((v_11 >= 4u)) {
        break;
      }
      v_9[v_11] = tint_convert_Outer(v_8[v_11]);
      {
        v_10 = (v_11 + 1u);
      }
      continue;
    }
  }
  Outer l_a[4] = v_9;
  Outer l_a_i = tint_convert_Outer(v.inner[v_4]);
  Inner_std140 v_12[4] = v.inner[v_4].a;
  Inner v_13[4] = Inner[4](Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))), Inner(mat3(vec3(0.0f), vec3(0.0f), vec3(0.0f))));
  {
    uint v_14 = 0u;
    v_14 = 0u;
    while(true) {
      uint v_15 = v_14;
      if ((v_15 >= 4u)) {
        break;
      }
      v_13[v_15] = tint_convert_Inner(v_12[v_15]);
      {
        v_14 = (v_15 + 1u);
      }
      continue;
    }
  }
  Inner l_a_i_a[4] = v_13;
  Inner l_a_i_a_i = tint_convert_Inner(v.inner[v_4].a[v_5]);
  mat3 l_a_i_a_i_m = v_6;
  vec3 l_a_i_a_i_m_i = v_7;
  float l_a_i_a_i_m_i_i = v_7[i()];
}

#version 450 core

// 入力変数
layout(location=1) in vec2 inTexcoord;

// 出力変数
out vec4 fragColor;

// ユニフォーム変数
layout(binding=0) uniform sampler2D texColor;

// フラグメントシェーダプログラム
void main()
{
  if (texture(texColor, inTexcoord).a < 0.5) {
    discard;
  }
  fragColor = vec4(0,0,0,0.5);
}

#version 450 core

// ���͕ϐ�
layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexcoord;
layout(location=3) in vec3 vNormal;

// �o�͕ϐ�
layout(location=0) out vec4 outColor;
layout(location=1) out vec2 outTexcoord;
out gl_PerVertex {
  vec4 gl_Position;
};

// ���j�t�H�[���ϐ�
layout(location=0) uniform mat4 matMVP;
layout(location=1) uniform mat4 matModel;

// ���s����
struct DirectionalLight {
  vec4 direction;
  vec4 color;
};
layout(location=2) uniform DirectionalLight directionalLight;

// �_����
struct PointLight {
  vec4 position;
  vec4 color;
};
layout(location=4) uniform PointLight pointLight;

// ���_�V�F�[�_�v���O����
void main()
{
  vec3 worldPosition = vec3(matModel * vec4(vPosition, 1));
  mat3 matNormal = transpose(inverse(mat3(matModel)));
  vec3 worldNormal = matNormal * vNormal;

  vec3 totalLightColor = vec3(0);

  // ���s����
  {
    float theta = max(dot(worldNormal, directionalLight.direction.xyz), 0);
    totalLightColor += directionalLight.color.rgb * theta;
  }

  // �_����
  {
    // �t���O�����g���烉�C�g�֌������x�N�g�����v�Z.
    vec3 lightVector = pointLight.position.xyz - worldPosition;

    // �����ɂ�閾�邳�̕ω��ʂ��v�Z.
    float lengthSq = dot(lightVector, lightVector);
    float intensity = 1.0 / (1.0 + lengthSq);

    // �ʂ̌X���ɂ�閾�邳�̕ω��ʂ��v�Z.
    float theta = 1;
    if (lengthSq > 0) {
      vec3 direction = normalize(lightVector);
      theta = max(dot(worldNormal, direction), 0);
    }

    // �ω��ʂ��������킹�Ė��邳�����߁A���C�g�̖��邳�ϐ��ɉ��Z.
    totalLightColor += pointLight.color.rgb * theta * intensity;
  }

  outColor = vec4(vColor.rgb * totalLightColor, vColor.a);

  outTexcoord = vTexcoord;
  gl_Position = matMVP * vec4(vPosition, 1.0);
}

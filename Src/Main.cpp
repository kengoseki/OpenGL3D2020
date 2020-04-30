/**
* @file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#pragma comment(lib, "opengl32.lib")

/// ���W�f�[�^: �n��
const glm::vec3 posGround[] = {
  // �n��
  {-20, 0, 20},
  { 20, 0, 20},
  { 20, 0,-20},
  {-20, 0,-20},
};

/// ���W�f�[�^: ��
const glm::vec3 posTree[] = {
  { 0.00f, 5.0f, 0.00f},
  { 0.00f, 1.5f,-1.00f},
  {-0.87f, 1.5f, 0.50f},
  { 0.87f, 1.5f, 0.50f},
  { 0.00f, 4.0f, 0.00f},
  { 0.00f, 0.0f,-0.36f},
  {-0.31f, 0.0f, 0.18f},
  { 0.31f, 0.0f, 0.18f},
};

/// ���W�f�[�^: ��
const glm::vec3 posHouse[] = {
  { 2.8f, 0.0f, 3.0f},
  { 3.0f, 4.0f, 3.0f},
  { 0.0f, 6.0f, 3.0f},
  {-3.0f, 4.0f, 3.0f},
  {-2.8f, 0.0f, 3.0f},

  {-2.8f, 0.0f,-3.0f},
  {-3.0f, 4.0f,-3.0f},
  { 0.0f, 6.0f,-3.0f},
  { 3.0f, 4.0f,-3.0f},
  { 2.8f, 0.0f,-3.0f},

  { 2.8f, 0.0f, 3.0f},
  { 3.0f, 4.0f, 3.0f},

  { 0.0f, 6.0f, 3.0f},
  { 0.0f, 6.0f,-3.0f},

  { 0.0f, 6.0f,-3.0f},
  { 0.0f, 6.0f, 3.0f},
};

/// ���W�f�[�^: ������
const glm::vec3 posCube[] = {
  { 1,-1, 1},
  {-1,-1, 1},
  {-1, 1, 1},
  {-1,-1, 1},
  { 1,-1, 1},
  { 1, 1, 1},
  { 1,-1, 1},
  { 1,-1,-1},
  { 1, 1,-1},
  { 1,-1,-1},
  {-1,-1,-1},
  {-1, 1,-1},
  {-1,-1,-1},
  { 1,-1,-1},
};

/// �F�f�[�^: �n��
const glm::vec4 colGround[] = {
  {0.8f, 0.7f, 0.5f, 1.0f},
  {0.8f, 0.7f, 0.5f, 1.0f},
  {0.8f, 0.7f, 0.5f, 1.0f},
  {0.8f, 0.7f, 0.5f, 1.0f},
};

/// �F�f�[�^: ��
const glm::vec4 colTree[] = {
  {0.5f, 0.8f, 0.3f, 1.0f},
  {0.1f, 0.3f, 0.0f, 1.0f},
  {0.1f, 0.3f, 0.0f, 1.0f},
  {0.1f, 0.3f, 0.0f, 1.0f},
  {0.2f, 0.1f, 0.0f, 1.0f},
  {0.5f, 0.3f, 0.2f, 1.0f},
  {0.5f, 0.3f, 0.2f, 1.0f},
  {0.5f, 0.3f, 0.2f, 1.0f},
};

/// �F�f�[�^: ��
const glm::vec4 colHouse[] = {
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
};

/// �F�f�[�^: ������
const glm::vec4 colCube[] = {
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
  {1,1,1,1},
};

/// �e�N�X�`�����W�f�[�^: �n��
const glm::vec2 tcGround[] = {
  {-4.0f,-4.0f },
  { 4.0f,-4.0f },
  { 4.0f, 4.0f },
  {-4.0f, 4.0f },
};

/// �e�N�X�`�����W�f�[�^: ��
const glm::vec2 tcTree[] = {
  { 0.0f, 1.0f },
  { 0.0f, 0.0f },
  { 0.5f, 0.0f },
  { 1.0f, 0.0f },
  { 0.0f, 1.0f },
  { 0.0f, 0.0f },
  { 0.5f, 0.0f },
  { 1.0f, 0.0f },
};

/// �e�N�X�`�����W�f�[�^: ��
const glm::vec2 tcHouse[] = {
  { 1.000f, 0.00f},
  { 1.000f, 0.69f},
  { 0.875f, 1.00f},
  { 0.750f, 0.69f},
  { 0.750f, 0.00f},
  { 0.500f, 0.00f},
  { 0.500f, 0.69f},
  { 0.375f, 1.00f},
  { 0.250f, 0.69f},
  { 0.250f, 0.00f},
  { 0.000f, 0.00f},
  { 0.000f, 0.69f},
  { 0.750f, 1.00f},
  { 0.500f, 1.00f},
  { 0.250f, 1.00f},
  { 0.000f, 1.00f},
};

/// �e�N�X�`�����W�f�[�^: ������
const glm::vec2 tcCube[] = {
  { 0.0f / 4.0f, 1.0f / 3.0f},
  { 1.0f / 4.0f, 1.0f / 3.0f},
  { 2.0f / 4.0f, 1.0f / 3.0f},
  { 2.0f / 4.0f, 0.0f / 3.0f},
  { 3.0f / 4.0f, 0.0f / 3.0f},
  { 3.0f / 4.0f, 1.0f / 3.0f},
  { 4.0f / 4.0f, 1.0f / 3.0f},
  { 4.0f / 4.0f, 2.0f / 3.0f},
  { 3.0f / 4.0f, 2.0f / 3.0f},
  { 3.0f / 4.0f, 3.0f / 3.0f},
  { 2.0f / 4.0f, 3.0f / 3.0f},
  { 2.0f / 4.0f, 2.0f / 3.0f},
  { 1.0f / 4.0f, 2.0f / 3.0f},
  { 0.0f / 4.0f, 2.0f / 3.0f},
};

/// �C���f�b�N�X�f�[�^: �n��
const GLushort indexGround[] = {
  0, 1, 2, 2, 3, 0,
};

/// �C���f�b�N�X�f�[�^: ��
const GLushort indexTree[] = {
  0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3,
  4, 5, 6, 4, 6, 7, 4, 7, 5,
};

/// �C���f�b�N�X�f�[�^: ��
const GLushort indexHouse[] = {
  0, 1, 3, 3, 4, 0, 1, 2, 3,
  4, 3, 6, 6, 5, 4,
  3, 12, 13, 13, 6, 3,
  5, 6, 8, 8, 9, 5, 6, 7, 8,
  9, 8, 11, 11, 10, 9,
  8, 14, 15, 15, 11, 8,
};

/// �C���f�b�N�X�f�[�^: ������
//       10-09
//        |  |
// 13-12-11-08-07
//  |  |  |  |  |
// 00-01-02-05-06
//        |  |
//       03-04
const GLushort indexCube[] = {
  0, 1, 12, 12, 13, 0,
  1, 2, 11, 11, 12, 1,
  2, 5, 8, 8, 11, 2,
  3, 4, 5, 5, 2, 3,
  5, 6, 7, 7, 8, 5,
  11, 8, 9, 9, 10, 11,
};

// �摜�f�[�^.
const int imageWidth = 8; // �摜�̕�.
const int imageHeight = 8; // �摜�̍���.
const GLuint B = 0xff'10'10'10; // ��.
const GLuint G = 0xff'a0'a0'a0; // ��.
const GLuint W = 0xff'ff'ff'ff; // ��.
const GLuint imageGround[imageWidth * imageHeight] = {
  W, W, B, W, W, W, W, W,
  W, B, W, W, W, W, W, W,
  W, W, B, W, W, W, W, W,
  B, B, B, B, B, B, B, B,
  W, W, W, W, W, W, B, W,
  W, W, W, W, W, B, W, W,
  W, W, W, W, W, W, B, W,
  B, B, B, B, B, B, B, B,
};
const GLuint imageTree[] = {
  G, B, G, B, G, B, G, B,
  W, G, W, G, W, G, W, G,
  W, W, W, W, W, W, W, W,
  G, G, G, G, G, G, G, G,
  G, W, G, W, G, W, G, W,
  W, W, W, W, W, W, W, W,
  G, G, G, G, G, G, G, G,
  W, G, W, G, W, G, W, G,
};

/// ���_�V�F�[�_�[.
static const GLchar* const vsCode =
  "#version 450 \n"
  "layout(location=0) in vec3 vPosition; \n"
  "layout(location=1) in vec4 vColor; \n"
  "layout(location=2) in vec2 vTexcoord; \n"
  "layout(location=0) out vec4 outColor; \n"
  "layout(location=1) out vec2 outTexcoord; \n"
  "out gl_PerVertex { \n"
  "  vec4 gl_Position; \n"
  "}; \n"
  "layout(location=0) uniform mat4 matMVP; \n"
  "void main() { \n"
  "  outColor = vColor; \n"
  "  outTexcoord = vTexcoord; \n"
  "  gl_Position = matMVP * vec4(vPosition, 1.0); \n"
  "}";

/// �t���O�����g�V�F�[�_�[.
static const GLchar* const fsCode =
  "#version 450 \n"
  "layout(location=0) in vec4 inColor; \n"
  "layout(location=1) in vec2 inTexcoord; \n"
  "layout(binding=0) uniform sampler2D texColor; \n"
  "out vec4 fragColor; \n"
  "void main() { \n"
//  "  fragColor = inColor * texture(texColor, gl_FragCoord.xy * 0.01); \n"
  "  fragColor = inColor * texture(texColor, inTexcoord); \n"
  "}";

/**
* OpenGL����̃��b�Z�[�W����������.
*
* @param source    ���b�Z�[�W�̔��M��(OpenGL�AWindows�A�V�F�[�_�[�Ȃ�).
* @param type      ���b�Z�[�W�̎��(�G���[�A�x���Ȃ�).
* @param id        ���b�Z�[�W����ʂɎ��ʂ���l.
* @param severity  ���b�Z�[�W�̏d�v�x(���A���A��A�Œ�).
* @param length    ���b�Z�[�W�̕�����. �����Ȃ烁�b�Z�[�W��0�I�[����Ă���.
* @param message   ���b�Z�[�W�{��.
* @param userParam �R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^.
*
* �ڍׂ�(https://www.khronos.org/opengl/wiki/Debug_Output)���Q��.
*/
void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  if (length < 0) {
    std::cerr << "[MSG] " << message << "\n";
  } else {
    const std::string s(message, message + length);
    std::cerr << "[MSG] " << s << "\n";
  }
}

/**
* ���؂�`�悷��.
*/
void DrawLineOfTrees(const Primitive& prim, GLint vp, GLint locMatMVP, const glm::mat4& matVP, const glm::vec3& start, const glm::vec3& direction)
{
  glm::vec3 offset = start;
  for (float i = 0; i < 19; ++i) {
    const glm::vec3 offset = start + direction * i;
    const glm::mat4 matModelT = glm::translate(glm::mat4(1), offset);
    const glm::mat4 matModelR = glm::rotate(glm::mat4(1), glm::radians(i * 90), glm::vec3(0, 1, 0));
    const glm::mat4 matModel = matModelT * matModelR;
    const glm::mat4 matMVP = matVP * matModel;
    glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
    prim.Draw();
  }
}

/**
* GLFW����̃G���[�񍐂���������.
*
* @param error �G���[�ԍ�.
* @param desc  �G���[�̓��e.
*/
void ErrorCallback(int error, const char* desc)
{
  std::cerr << "ERROR: " << desc << "\n";
}

/**
* �G���g���[�|�C���g.
*/
int main()
{
  glfwSetErrorCallback(ErrorCallback);

  // GLFW�̏�����.
  if (glfwInit() != GLFW_TRUE) {
    return 1;
  }

  // �`��E�B���h�E�̍쐬.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  GLFWwindow* window =
    glfwCreateWindow(1280, 720, "OpenGL3DActionGame", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  // OpenGL�֐��̃A�h���X���擾����.
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cerr << "ERROR: GLAD�̏������Ɏ��s���܂���." << std::endl;
    glfwTerminate();
    return 1;
  }

  glDebugMessageCallback(DebugCallback, nullptr);

  // OpenGL�̏����R���\�[���E�B���h�E�֏o�͂���.
  {
#define Output(name) {\
  GLint n = 0;\
  glGetIntegerv(name, &n);\
  std::cout << #name " = " << n << "\n";\
} (void)0
    const GLubyte* renderer = glGetString(GL_RENDERER);
    std::cout << "Renderer: " << renderer << "\n";
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << "\n";
    Output(GL_MAX_VERTEX_ATTRIB_BINDINGS); // ���Ȃ��Ƃ�16.
    Output(GL_MAX_VERTEX_ATTRIBS); // ���Ȃ��Ƃ�16.
    Output(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET); // ���Ȃ��Ƃ�2047
    Output(GL_MAX_VERTEX_ATTRIB_STRIDE); // ���Ȃ��Ƃ�2048
    Output(GL_MAX_UNIFORM_LOCATIONS); // ���Ȃ��Ƃ�1024
    Output(GL_MAX_UNIFORM_BLOCK_SIZE); // ���Ȃ��Ƃ�16k
    Output(GL_MAX_VARYING_COMPONENTS); // ���Ȃ��Ƃ�60
    Output(GL_MAX_VARYING_VECTORS); // ���Ȃ��Ƃ�15
    Output(GL_MAX_VARYING_FLOATS); // ���Ȃ��Ƃ�32
    Output(GL_MAX_VERTEX_ATTRIBS); // ���Ȃ��Ƃ�16
    Output(GL_MAX_TEXTURE_SIZE); // ���Ȃ��Ƃ�1024
    Output(GL_MAX_TEXTURE_IMAGE_UNITS); // ���Ȃ��Ƃ�16
    Output(GL_MAX_TEXTURE_BUFFER_SIZE); // ���Ȃ��Ƃ�64k
#undef Output
  }

  PrimitiveBuffer primitiveBuffer;
  if (!primitiveBuffer.Allocate(20'000, 80'000)) {
    return 1;
  }
  primitiveBuffer.Add(std::size(posGround), posGround, colGround, tcGround, std::size(indexGround), indexGround);
  primitiveBuffer.Add(std::size(posTree), posTree, colTree, tcTree, std::size(indexTree), indexTree);
  primitiveBuffer.Add(std::size(posHouse), posHouse, colHouse, tcHouse, std::size(indexHouse), indexHouse);
  primitiveBuffer.Add(std::size(posCube), posCube, colCube, tcCube, std::size(indexCube), indexCube);

  // �p�C�v���C���E�I�u�W�F�N�g���쐬����.
  const GLuint vp = GLContext::CreateProgram(GL_VERTEX_SHADER, vsCode);
  const GLuint fp = GLContext::CreateProgram(GL_FRAGMENT_SHADER, fsCode);
  const GLuint pipeline = GLContext::CreatePipeline(vp, fp);
  if (!pipeline) {
    return 1;
  }

  // uniform�ϐ��̈ʒu.
  const GLint locMatMVP = 0;

  // �T���v���E�I�u�W�F�N�g���쐬����.
  const GLuint sampler = GLContext::CreateSampler();
  if (!sampler) {
    return 1;
  }
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  //const GLuint texGround = GLContext::CreateImage2D(imageWidth, imageHeight, imageGround);
  const GLuint texGround = GLContext::CreateImage2D("Res/Ground.tga");
  const GLuint texHouse = GLContext::CreateImage2D("Res/House.tga");
  const GLuint texCube = GLContext::CreateImage2D("Res/Rock.tga");
  if (!texGround || !texHouse) {
    return 1;
  }
  const GLuint texTree = GLContext::CreateImage2D(imageWidth, imageHeight, imageTree, GL_RGBA, GL_UNSIGNED_BYTE);

  // ���C�����[�v.
  while (!glfwWindowShouldClose(window)) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ���_����]������.
    const float degree = static_cast<float>(std::fmod(glfwGetTime() * 30.0, 360.0));
    const glm::mat4 matViewRot = glm::rotate(glm::mat4(1), glm::radians(degree), glm::vec3(0, 1, 0));
    const glm::vec3 viewPosition = matViewRot * glm::vec4(20, 30, 20, 1);

    // ���W�ϊ��s����쐬���ăV�F�[�_�[�ɓ]������.
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
    const glm::mat4 matProj =
      glm::perspective(glm::radians(45.0f),  aspectRatio, 0.1f, 500.0f);
    const glm::mat4 matView =
      glm::lookAt(viewPosition, glm::vec3(0), glm::vec3(0, 1, 0));
    //const glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(5, 0, 1));
    //const glm::mat4 matMVP = matProj * matView * matModel;
    //glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);

    primitiveBuffer.BindVertexArray();
    glBindProgramPipeline(pipeline);
    glBindSampler(0, sampler);

    //primTree.Draw();

    // �؂�`��.
#if 1
    glBindTextureUnit(0, texTree);
    for (float j = 0; j < 4; ++j) {
      const glm::mat4 matRot = glm::rotate(glm::mat4(1), glm::radians(90.0f) * j, glm::vec3(0, 1, 0));
      DrawLineOfTrees(primitiveBuffer.Get(1), vp, locMatMVP, matProj * matView, matRot * glm::vec4(-19, 0, 19, 1), matRot * glm::vec4(2, 0, 0, 1));
    }
#else
    for (float i = 0; i < 19; ++i) {
      const glm::mat4 matModelT =
        glm::translate(glm::mat4(1), glm::vec3(-19 + i * 2, 0, 19));
      const glm::mat4 matModelR =
        glm::rotate(glm::mat4(1), glm::radians(i * 90), glm::vec3(0, 1, 0));
      const glm::mat4 matModel = matModelT * matModelR;
      const glm::mat4 matMVP = matVP * matModel;
      glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
      primitiveBuffer.Get(1).Draw();
    }
#endif

    // �n�ʂ�`��.
    {
      const glm::mat4 matModel = glm::mat4(1);
      const glm::mat4 matMVP = matProj * matView * matModel;
      glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
      glBindTextureUnit(0, texGround);
      primitiveBuffer.Get(0).Draw();
    }

    // �Ƃ�`��.
    {
      const glm::mat4 matModel = glm::mat4(1);
      const glm::mat4 matMVP = matProj * matView * matModel;
      glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
      glBindTextureUnit(0, texHouse);
      primitiveBuffer.Get(2).Draw();
    }

    // �����̂�`��.
    {
      const glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(10, 1, 0));
      const glm::mat4 matMVP = matProj * matView * matModel;
      glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
      glBindTextureUnit(0, texCube);
      primitiveBuffer.Get(3).Draw();
    }

    glBindTextureUnit(0, 0);
    glBindSampler(0, 0);
    glBindProgramPipeline(0);
    primitiveBuffer.UnbindVertexArray();

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // ��n��.
  glDeleteTextures(1, &texTree);
  glDeleteTextures(1, &texHouse);
  glDeleteTextures(1, &texGround);
  glDeleteTextures(1, &texCube);
  glDeleteSamplers(1, &sampler);
  glDeleteProgramPipelines(1, &pipeline);
  glDeleteProgram(fp);
  glDeleteProgram(vp);

  // GLFW�̏I��.
  glfwTerminate();

  return 0;
}
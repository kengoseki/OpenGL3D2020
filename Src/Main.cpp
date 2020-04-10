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

/// ���W�f�[�^.
const Position positions[] = {
  // �n��
  {-20, 0, 20},
  { 20, 0, 20},
  { 20, 0,-20},
  {-20, 0,-20},

  // ��
  { 0.00f, 5.0f, 0.00f},
  { 0.00f, 1.5f,-1.00f},
  {-0.87f, 1.5f, 0.50f},
  { 0.87f, 1.5f, 0.50f},
  { 0.00f, 4.0f, 0.00f},
  { 0.00f, 0.0f,-0.36f},
  {-0.31f, 0.0f, 0.18f},
  { 0.31f, 0.0f, 0.18f},

  {-0.3f, -0.3f, 0.5f},
  { 0.2f, -0.3f, 0.5f},
  { 0.2f,  0.5f, 0.5f},
  {-0.3f,  0.5f, 0.5f},

  {-0.2f, -0.5f, 0.1f},
  { 0.3f, -0.5f, 0.1f},
  { 0.3f,  0.3f, 0.1f},
  { 0.3f,  0.3f, 0.1f},
  {-0.2f,  0.3f, 0.1f},
  {-0.2f, -0.5f, 0.1f},

  {-0.33f, 0.5f, 0.5f},
  { 0.33f, 0.5f, 0.5f},
  { 0.0f,  -0.5f, 0.5f},

  {-0.33f - 0.5f, -0.5f, 0.5f},
  { 0.33f - 0.5f, -0.5f, 0.5f},
  { 0.0f - 0.5f,  0.5f, 0.5f},

  {-0.33f + 0.5f, -0.5f, 0.5f},
  { 0.33f + 0.5f, -0.5f, 0.5f},
  { 0.0f + 0.5f,  0.5f, 0.5f},
};

/// �F�f�[�^.
const Color colors[] = {
  // �n��
  {0.3f, 0.2f, 0.1f},
  {0.3f, 0.2f, 0.1f},
  {0.3f, 0.2f, 0.1f},
  {0.3f, 0.2f, 0.1f},

  // ��
  {0.5f, 0.8f, 0.3f, 1.0f},
  {0.1f, 0.3f, 0.0f, 1.0f},
  {0.1f, 0.3f, 0.0f, 1.0f},
  {0.1f, 0.3f, 0.0f, 1.0f},
  {0.2f, 0.1f, 0.0f, 1.0f},
  {0.5f, 0.3f, 0.2f, 1.0f},
  {0.5f, 0.3f, 0.2f, 1.0f},
  {0.5f, 0.3f, 0.2f, 1.0f},

  {0.0f, 1.0f, 0.0f, 1.0f},
  {0.0f, 0.0f, 1.0f, 1.0f},
  {1.0f, 0.0f, 0.0f, 1.0f},
  {0.0f, 0.0f, 1.0f, 1.0f},

  {1.0f, 0.0f, 0.0f, 1.0f},
  {1.0f, 1.0f, 0.0f, 1.0f},
  {1.0f, 0.0f, 0.0f, 1.0f},
  {0.0f, 0.0f, 1.0f, 1.0f},
  {0.0f, 1.0f, 1.0f, 1.0f},
  {0.0f, 0.0f, 1.0f, 1.0f},

  {0.0f, 1.0f, 1.0f, 1.0f},
  {1.0f, 1.0f, 0.0f, 1.0f},
  {1.0f, 0.0f, 1.0f, 1.0f},

  {0.0f, 0.0f, 1.0f, 1.0f},
  {0.0f, 1.0f, 0.0f, 1.0f},
  {1.0f, 0.0f, 0.0f, 1.0f},

  {0.5f, 0.5f, 0.5f, 1.0f},
  {0.0f, 0.0f, 0.0f, 1.0f},
  {1.0f, 1.0f, 1.0f, 1.0f},
};

/// �C���f�b�N�X�f�[�^.
const GLushort indices[] = {
  // �n��
  0, 1, 2, 2, 3, 0,

  // ��
  0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3,
  4, 5, 6, 4, 6, 7, 4, 7, 5,

  0, 1, 2, 2, 3, 0,
  4, 5, 6, 7, 8, 9,
};

/// �`��f�[�^���X�g.
const Primitive primGround(GL_TRIANGLES, 6, 0, 0); // �n��
const Primitive primTree(GL_TRIANGLES, 21, 6 * sizeof(GLushort), 4); // ��

/// ���_�V�F�[�_�[.
static const GLchar* const vsCode =
  "#version 450 \n"
  "layout(location=0) in vec3 vPosition; \n"
  "layout(location=1) in vec4 vColor; \n"
  "layout(location=0) out vec4 outColor; \n"
  "uniform mat4 matMVP; \n"
  "out gl_PerVertex { \n"
  "  vec4 gl_Position; \n"
  "}; \n"
  "void main() { \n"
  "  outColor = vColor; \n"
  "  gl_Position = matMVP * vec4(vPosition, 1.0); \n"
  "}";

/// �t���O�����g�V�F�[�_�[.
static const GLchar* const fsCode =
  "#version 450 \n"
  "layout(location=0) in vec4 inColor; \n"
  "out vec4 fragColor; \n"
  "void main() { \n"
  "  fragColor = inColor; \n"
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

  // VAO���쐬����.
  const GLuint positionBuffer = GLContext::CreateBuffer(sizeof(positions), positions);
  const GLuint colorBuffer = GLContext::CreateBuffer(sizeof(colors), colors);
  const GLuint ibo = GLContext::CreateBuffer(sizeof(indices), indices);
  const GLuint vao = GLContext::CreateVertexArray(positionBuffer, colorBuffer, ibo);
  if (!vao) {
    return 1;
  }

  // �p�C�v���C���E�I�u�W�F�N�g���쐬����.
  const GLuint vp = GLContext::CreateProgram(GL_VERTEX_SHADER, vsCode);
  const GLuint fp = GLContext::CreateProgram(GL_FRAGMENT_SHADER, fsCode);
  const GLuint pipeline = GLContext::CreatePipeline(vp, fp);
  if (!pipeline) {
    return 1;
  }

  // uniform�ϐ��̈ʒu���擾����.
  const GLint locMatMVP = glGetUniformLocation(vp, "matMVP");
  if (locMatMVP < 0) {
    std::cerr << "[�G���[]" << __func__ << ":uniform�ϐ�matMVP��������܂���.\n";
    return 1;
  }

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

    glBindProgramPipeline(pipeline);
    glBindVertexArray(vao);
    //primTree.Draw();

    // �؂�`��.
    for (float j = 0; j < 4; ++j) {
      const glm::mat4 matRot = glm::rotate(glm::mat4(1), glm::radians(90.0f) * j, glm::vec3(0, 1, 0));
      for (float i = 0; i < 19; ++i) {
        const glm::vec3 offset = matRot * glm::vec4(-19 + i * 2, 0, 19, 1);
        const glm::mat4 matModelT = glm::translate(glm::mat4(1), offset);
        const glm::mat4 matModelR = glm::rotate(glm::mat4(1), glm::radians(i * 90), glm::vec3(0, 1, 0));
        const glm::mat4 matModel = matModelT * matModelR;
        const glm::mat4 matMVP = matProj * matView * matModel;
        glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
        primTree.Draw();
      }
    }

    // �n�ʂ�`��.
    {
      const glm::mat4 matModel = glm::mat4(1);
      const glm::mat4 matMVP = matProj * matView * matModel;
      glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
      primGround.Draw();
    }

#if 0
    const float treeCount = 10; // �؂̖{��.
    const float radius = 8; // �؂�A����~�̔��a.
    for (float i = 0; i < treeCount; ++i) {
      const float x = std::cos(glm::pi<float>() * 2 / treeCount * i) * radius;
      const float z = std::sin(glm::pi<float>() * 2 / treeCount * i) * radius;
      const glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
      const glm::mat4 matMVP = matProj * matView * matModel;
      glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
      primTree.Draw();
    }
#endif 

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // ��n��.
  glDeleteProgramPipelines(1, &pipeline);
  glDeleteProgram(fp);
  glDeleteProgram(vp);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteBuffers(1, &positionBuffer);

  // GLFW�̏I��.
  glfwTerminate();

  return 0;
}
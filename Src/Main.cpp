/**
* @file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#pragma comment(lib, "opengl32.lib")

/// ���W�f�[�^.
const Position positions[] = {
  {-0.3f, -0.3f, 0.5f},
  { 0.2f, -0.3f, 0.5f},
  { 0.2f,  0.5f, 0.5f},
  {-0.3f,  0.5f, 0.5f},

  {-0.2f, -0.5f, 0.5f},
  { 0.3f, -0.5f, 0.5f},
  { 0.3f,  0.3f, 0.5f},
  { 0.3f,  0.3f, 0.5f},
  {-0.2f,  0.3f, 0.5f},
  {-0.2f, -0.5f, 0.5f},

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
  0, 1, 2, 2, 3, 0,
  4, 5, 6, 7, 8, 9,
};

/// ���_�V�F�[�_�[.
static const GLchar* const vsCode =
  "#version 450 \n"
  "layout(location=0) in vec3 vPosition; \n"
  "layout(location=1) in vec4 vColor; \n"
  "layout(location=0) out vec4 outColor; \n"
  "out gl_PerVertex { \n"
  "  vec4 gl_Position; \n"
  "}; \n"
  "void main() { \n"
  "  outColor = vColor; \n"
  "  gl_Position = vec4(vPosition, 1.0); \n"
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

  // ���C�����[�v.
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindProgramPipeline(pipeline);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_SHORT, 0);

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
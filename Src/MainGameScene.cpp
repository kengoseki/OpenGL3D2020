/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "Global.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/**
* ���؂�`�悷��.
*/
void DrawLineOfTrees(const Mesh::Primitive& prim, Shader::Pipeline& pipeline, const glm::mat4& matVP, const glm::vec3& start, const glm::vec3& direction)
{
  glm::vec3 offset = start;
  for (float i = 0; i < 19; ++i) {
    const glm::vec3 offset = start + direction * i;
    const glm::mat4 matModelT = glm::translate(glm::mat4(1), offset);
    const glm::mat4 matModelR = glm::rotate(glm::mat4(1), glm::radians(i * 30), glm::vec3(0, 1, 0));
    const glm::mat4 matModel = matModelT * matModelR;
    const glm::mat4 matMVP = matVP * matModel;
    pipeline.SetMVP(matMVP);
    pipeline.SetModelMatrix(matModel);
    prim.Draw();
  }
}

/**
* ���C���Q�[����ʂ�����������.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool MainGameScene::Initialize()
{
  texGround = std::make_shared<Texture::Image2D>("Res/Ground.tga");
  texTree   = std::make_shared<Texture::Image2D>("Res/Tree.tga");
  texHouse  = std::make_shared<Texture::Image2D>("Res/House.tga");
  texCube   = std::make_shared<Texture::Image2D>("Res/Rock.tga");
  if (!texGround ||!texTree || !texHouse || !texCube) {
    return false;
  }

  // �_������ݒ肷��
  pointLight = Shader::PointLight{
    glm::vec4(8, 10,-8, 0),
    glm::vec4(0.4f, 0.7f, 1.0f, 0) * 200.0f
  };

  std::cout << "[���] MainGameScene���J�n.\n";
  return true;
}

/**
* ���C���Q�[����ʂ̃L�[���͂���������.
*/
void MainGameScene::ProcessInput()
{
}

/**
* ���C���Q�[����ʂ��X�V����.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
* @param deltaTime �O��̍X�V����̌o�ߎ���.
*/
void MainGameScene::Update(GLFWwindow* window, float deltaTime)
{
  // �_�������ړ�������.
  const float speed = 10.0f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    pointLight.position.x -= speed;
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    pointLight.position.x += speed;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    pointLight.position.z -= speed;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    pointLight.position.z += speed;
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    pointLight.position.y -= speed;
  } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    pointLight.position.y += speed;
  }
}

/**
* ���C���Q�[����ʂ�`�悷��.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
*/
void MainGameScene::Render(GLFWwindow* window) const
{
  Global& global = Global::Get();
  std::shared_ptr<Shader::Pipeline> pipeline = global.pipeline;
  Mesh::PrimitiveBuffer& primitiveBuffer = global.primitiveBuffer;
  Texture::Sampler& sampler = global.sampler;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  //glEnable(GL_FRAMEBUFFER_SRGB);
  glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ���s������ݒ肷��
  const Shader::DirectionalLight directionalLight{
    glm::normalize(glm::vec4(3,-2, 2, 0)),
    glm::vec4(1, 0.9f, 0.8f, 1)
  };
  pipeline->SetLight(directionalLight);

  pipeline->SetLight(pointLight);

  const glm::vec3 viewPosition(20, 30, 30);

  // ���W�ϊ��s����쐬.
  int w, h;
  glfwGetWindowSize(window, &w, &h);
  const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
  const glm::mat4 matProj =
    glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f);
  const glm::mat4 matView =
    glm::lookAt(viewPosition, glm::vec3(0), glm::vec3(0, 1, 0));

  primitiveBuffer.BindVertexArray();
  pipeline->Bind();
  sampler.Bind(0);

  //primTree.Draw();

  // �؂�`��.
  texTree->Bind(0);
  for (float j = 0; j < 4; ++j) {
    const glm::mat4 matRot = glm::rotate(glm::mat4(1), glm::radians(90.0f) * j, glm::vec3(0, 1, 0));
    DrawLineOfTrees(primitiveBuffer.Get(1), *pipeline, matProj * matView, matRot * glm::vec4(-19, 0, 19, 1), matRot * glm::vec4(2, 0, 0, 1));
  }

  // �n�ʂ�`��.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texGround->Bind(0);
    primitiveBuffer.Get(0).Draw();
  }

  // �Ƃ�`��.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texHouse->Bind(0);
    primitiveBuffer.Get(2).Draw();
  }

  // �����̂�`��.
  {
    const glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(10, 1, 0));
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texCube->Bind(0);
    primitiveBuffer.Get(3).Draw();
  }

  // �_�����̈ʒu��`��.
  {
    // Y����].
    const float degree = static_cast<float>(std::fmod(glfwGetTime() * 180.0, 360.0));
    const glm::mat4 matModelR =
      glm::rotate(glm::mat4(1), glm::radians(degree), glm::vec3(0, 1, 0));
    // �g��k��.
    const glm::mat4 matModelS =
      glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.25f, 0.5f));
    // ���s�ړ�.
    const glm::mat4 matModelT =
      glm::translate(glm::mat4(1), glm::vec3(pointLight.position) + glm::vec3(0, -1.25f, 0));
    // �g��k���E��]�E���s�ړ�������.
    const glm::mat4 matModel = matModelT * matModelR * matModelS;
    pipeline->SetMVP(matProj * matView * matModel);
    pipeline->SetModelMatrix(matModel);
    texTree->Bind(0);
    primitiveBuffer.Get(1).Draw();
  }

  Texture::UnbindAllTextures();
  Texture::UnbindAllSamplers();
  Shader::UnbindPipeline();
  primitiveBuffer.UnbindVertexArray();
}

/**
* ���C���Q�[����ʂ��I������.
*/
void MainGameScene::Finalize()
{
  std::cout << "[���] MainGameScene���I��.\n";
}

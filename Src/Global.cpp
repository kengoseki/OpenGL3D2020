/**
* @file Global.cpp
*/
#include "Global.h"
#include <iostream>

/**
*
*/
Global& Global::Get()
{
  static Global singleton;
  return singleton;
}

/**
* �f�X�g���N�^.
*/
Global::~Global()
{
  std::cout << "[���] �O���[�o���f�[�^��j��.\n";
}

/**
* �O���[�o���f�[�^������������.
*
* @param window GLFW�E�B���h�E�ւ̃|�C���^.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool Global::Initialize(GLFWwindow* window)
{
  // �v���~�e�B�u�o�b�t�@�Ƀ��f���f�[�^��ǂݍ���.
  if (!primitiveBuffer.Allocate(20'000, 80'000)) {
    return false;
  }
  primitiveBuffer.AddFromObjFile("Res/Ground.obj");
  primitiveBuffer.AddFromObjFile("Res/Tree.obj");
  primitiveBuffer.AddFromObjFile("Res/House.obj");
  primitiveBuffer.AddFromObjFile("Res/Cube.obj");
  primitiveBuffer.AddFromObjFile("Res/Plane.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_0.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_1.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_2.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_3.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_4.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_5.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_idle_0.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_0.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_1.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_2.obj");

  // �p�C�v���C���E�I�u�W�F�N�g���쐬����.
  pipeline = std::make_shared<Shader::Pipeline>("Res/FragmentLighting.vert", "Res/FragmentLighting.frag");
  if (!pipeline || !*pipeline) {
    return false;
  }
  pipelineSimple = std::make_shared<Shader::Pipeline>("Res/Simple.vert", "Res/Simple.frag");
  if (!pipelineSimple || !*pipelineSimple) {
    return false;
  }

  // �T���v���E�I�u�W�F�N�g���쐬����.
  sampler.SetWrapMode(GL_REPEAT);
  sampler.SetFilter(GL_NEAREST);

  this->window = window;

  random.seed(std::random_device{}());

  std::cout << "[���] �O���[�o���f�[�^��������.\n";

  return true;
}

/**
* �v���~�e�B�u��`�悷��.
*
* @param id �v���~�e�B�u��ID.
*/
void Global::Draw(Global::PrimNo id) const
{
  primitiveBuffer.Get(static_cast<size_t>(id)).Draw();
}

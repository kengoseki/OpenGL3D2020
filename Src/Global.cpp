/**
* @file Global.cpp
*/
#include "Global.h"
#include <iostream>

/**
*
*/
GameData& GameData::Get()
{
  static GameData singleton;
  return singleton;
}

/**
* �f�X�g���N�^.
*/
GameData::~GameData()
{
  std::cout << "[���] �Q�[���f�[�^��j��.\n";
}

/**
* �O���[�o���f�[�^������������.
*
* @param window GLFW�E�B���h�E�ւ̃|�C���^.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool GameData::Initialize(GLFWwindow* window)
{
  std::cout << "[���] �Q�[���f�[�^�̏��������J�n.\n";

  // �v���~�e�B�u�o�b�t�@�Ƀ��f���f�[�^��ǂݍ���.
  if (!primitiveBuffer.Allocate(20'000, 80'000)) {
    return false;
  }
  primitiveBuffer.AddFromObjFile("Res/Ground.obj");
  primitiveBuffer.AddFromObjFile("Res/Tree.obj");
  primitiveBuffer.AddFromObjFile("Res/House.obj");
  primitiveBuffer.AddFromObjFile("Res/Cube.obj");
  primitiveBuffer.AddFromObjFile("Res/Plane.obj");
  primitiveBuffer.AddFromObjFile("Res/Bullet.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_0.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_1.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_2.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_3.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_4.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_walk_5.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_down_0.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_down_1.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_down_2.obj");
  primitiveBuffer.AddFromObjFile("Res/zombie_male_down_3.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_idle_0.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_idle_1.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_idle_2.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_idle_3.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_idle_4.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_0.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_1.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_2.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_3.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_4.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_5.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_6.obj");
  primitiveBuffer.AddFromObjFile("Res/player_female/player_female_run_7.obj");

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

  std::cout << "[���] �Q�[���f�[�^�̏�����������.\n";

  return true;
}

/**
* �v���~�e�B�u��`�悷��.
*
* @param id �v���~�e�B�u��ID.
*/
void GameData::Draw(GameData::PrimNo id) const
{
  primitiveBuffer.Get(static_cast<size_t>(id)).Draw();
}

/**
* @file GameData.cpp
*/
#include "GameData.h"
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

  primitiveBuffer.AddFromObjFile("Res/player_male_idle_0.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_idle_1.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_idle_2.obj");

  primitiveBuffer.AddFromObjFile("Res/player_male_run_0.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_1.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_2.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_3.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_4.obj");
  primitiveBuffer.AddFromObjFile("Res/player_male_run_5.obj");

  // �p�C�v���C���E�I�u�W�F�N�g���쐬����.
  pipeline = std::make_shared<Shader::Pipeline>("Res/FragmentLighting.vert", "Res/FragmentLighting.frag");
  if (!pipeline || !*pipeline) {
    return false;
  }
  pipelineSimple = std::make_shared<Shader::Pipeline>("Res/Simple.vert", "Res/Simple.frag");
  if (!pipelineSimple || !*pipelineSimple) {
    return false;
  }
  pipelineShadow = std::make_shared<Shader::Pipeline>("Res/Shadow.vert", "Res/Shadow.frag");
  if (!pipelineShadow || !*pipelineShadow) {
    return false;
  }

  // �T���v���E�I�u�W�F�N�g���쐬����.
  sampler.SetWrapMode(GL_REPEAT);
  sampler.SetFilter(GL_NEAREST);

  this->window = window;

  random.seed(std::random_device{}());

  /* �A�j���[�V�����f�[�^���쐬 */

  anmZombieMaleWalk = std::make_shared<Animation>();
  anmZombieMaleWalk->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_walk_0));
  anmZombieMaleWalk->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_walk_1));
  anmZombieMaleWalk->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_walk_2));
  anmZombieMaleWalk->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_walk_3));
  anmZombieMaleWalk->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_walk_4));
  anmZombieMaleWalk->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_walk_5));
  anmZombieMaleWalk->interval = 0.2f;

  anmZombieMaleDown = std::make_shared<Animation>();
  anmZombieMaleDown->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_down_0));
  anmZombieMaleDown->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_down_1));
  anmZombieMaleDown->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_down_2));
  anmZombieMaleDown->list.push_back(&primitiveBuffer.Get(PrimNo::zombie_male_down_3));
  anmZombieMaleDown->interval = 0.125f;
  anmZombieMaleDown->isLoop = false;

  anmPlayerIdle = std::make_shared<Animation>();
  anmPlayerIdle->list.push_back(&primitiveBuffer.Get(PrimNo::player_idle_0));
  anmPlayerIdle->list.push_back(&primitiveBuffer.Get(PrimNo::player_idle_1));
  anmPlayerIdle->list.push_back(&primitiveBuffer.Get(PrimNo::player_idle_2));
  anmPlayerIdle->list.push_back(&primitiveBuffer.Get(PrimNo::player_idle_1));
  anmPlayerIdle->interval = 0.2f;

  anmPlayerRun = std::make_shared<Animation>();
  anmPlayerRun->list.push_back(&primitiveBuffer.Get(PrimNo::player_run_0));
  anmPlayerRun->list.push_back(&primitiveBuffer.Get(PrimNo::player_run_1));
  anmPlayerRun->list.push_back(&primitiveBuffer.Get(PrimNo::player_run_2));
  anmPlayerRun->list.push_back(&primitiveBuffer.Get(PrimNo::player_run_3));
  anmPlayerRun->list.push_back(&primitiveBuffer.Get(PrimNo::player_run_4));
  anmPlayerRun->list.push_back(&primitiveBuffer.Get(PrimNo::player_run_5));
  anmPlayerRun->interval = 0.125f;

  std::cout << "[���] �Q�[���f�[�^�̏�����������.\n";
  return true;
}

/**
* �Q�[���f�[�^�̍X�V.
*/
void GameData::Update()
{
  // [�L�[���̍X�V]
  {
    // �Q�[���f�[�^�̃L�[�ԍ���GLFW�̃L�[�ԍ��̑Ή��\�����.
    // ����L�[�𑝂₵����ς������Ƃ��͂��̑Ή��\��ύX����.
    const struct {
      Key keyGamedata;  // �Q�[���f�[�^�̃L�[.
      uint32_t keyGlfw; // GLFW�̃L�[.
    } keyMap[] = {
      { Key::enter, GLFW_KEY_ENTER },
      { Key::left,  GLFW_KEY_A },
      { Key::right, GLFW_KEY_D },
      { Key::up,    GLFW_KEY_W },
      { Key::down,  GLFW_KEY_S },
      { Key::shot,  GLFW_KEY_J },
    };

    // ���݉�����Ă���L�[���擾.
    uint32_t newKey = 0; // ���݉�����Ă���L�[.
    for (const auto& e : keyMap) {
      if (glfwGetKey(window, e.keyGlfw) == GLFW_PRESS) {
        newKey |= e.keyGamedata;
      }
    }

    // �O���Update�ŉ�����Ă��炸(~keyPressed)�A
    // �����݉�����Ă���(newKey)�L�[���Ō�̃t���[���ŉ����ꂽ�L�[�ɐݒ�.
    keyPressedInLastFrame = ~keyPressed & newKey;

    // ������Ă���L�[���X�V.
    keyPressed = newKey;
  }
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

/**
* @file GameData.h
*/
#ifndef GAMEDATA_H_INCLUDED
#define GAMEDATA_H_INCLUDED
#include "glad/glad.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Actor.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <random>

/**
* �Q�[���S�̂Ŏg���f�[�^.
*/
class GameData
{
public:
  static GameData& Get();
  bool Initialize(GLFWwindow*);
  void Update();

  /**
   �L�[�̎��.

   ������Ă���L�[�̔���̓r�b�g���Z�q���g����:
     const uint32_t keyPressed = GameData::Get().keyPressed;
     if (keyPressed & GameData::Key::enter) {
       // Enter�L�[��������Ă���ꍇ�̏���.
     }
   �̂悤�ɂ���.
   �����ꂽ�u�Ԃ�m�肽���ꍇ��keyPressedInLastFrame���g��.
  */
  enum Key {
    enter = 0b0000'0000'0000'0001, // Enter�L�[
    left  = 0b0000'0000'0000'0010, // ���L�[(��)
    right = 0b0000'0000'0000'0100, // ���L�[(�E)
    up    = 0b0000'0000'0000'1000, // ���L�[(��)
    down  = 0b0000'0000'0001'0000, // ���L�[(��)
    shot  = 0b0000'0000'0010'0000, // �e���˃L�[
  };
  uint32_t keyPressed = 0; // �����Ă���L�[.
  uint32_t keyPressedInLastFrame = 0; // �Ō�̃t���[���ŉ����ꂽ�L�[.

  // �v���~�e�B�u�ԍ�.
  // �v���~�e�B�u�̓ǂݍ��ݏ��ƈ�v�����邱��.
  enum PrimNo {
    ground,
    tree,
    house,
    cube,
    plane,
    bullet,
    zombie_male_walk_0,
    zombie_male_walk_1,
    zombie_male_walk_2,
    zombie_male_walk_3,
    zombie_male_walk_4,
    zombie_male_walk_5,
    zombie_male_down_0,
    zombie_male_down_1,
    zombie_male_down_2,
    zombie_male_down_3,
    player_idle_0,
    player_idle_1,
    player_idle_2,
    player_run_0,
    player_run_1,
    player_run_2,
    player_run_3,
    player_run_4,
    player_run_5,
  };
  void Draw(PrimNo) const;

  std::shared_ptr<Shader::Pipeline> pipeline = nullptr;
  std::shared_ptr<Shader::Pipeline> pipelineSimple;
  std::shared_ptr<Shader::Pipeline> pipelineShadow;
  Mesh::PrimitiveBuffer primitiveBuffer;
  Texture::Sampler sampler;
  GLFWwindow* window = nullptr;

  std::mt19937 random;

  // �A�j���[�V�����f�[�^.
  std::shared_ptr<Animation> anmZombieMaleWalk;
  std::shared_ptr<Animation> anmZombieMaleDown;
  std::shared_ptr<Animation> anmPlayerIdle;
  std::shared_ptr<Animation> anmPlayerRun;

  // �|�����]���r�̐�.
  size_t killCount = 0;

private:
  GameData() = default;
  ~GameData();
  GameData(const GameData&) = delete;
  GameData& operator=(const GameData&) = delete;
};


#endif // GAMEDATA_H_INCLUDED

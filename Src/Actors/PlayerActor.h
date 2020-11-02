/**
* @file PlayerActor.h
*/
#ifndef PLAYERACTOR_H_INCLUDED
#define PLAYERACTOR_H_INCLUDED
#include "../Actor.h"

class MainGameScene;

/**
* �v���C���[�A�N�^�[.
*/
class PlayerActor : public Actor
{
public:
  PlayerActor(const glm::vec3& pos, float rotY, MainGameScene* pScene);
  virtual ~PlayerActor() = default;

  void ProcessInput();
  virtual void OnUpdate(float deltaTime) override;

private:
  MainGameScene* pMainGameScene = nullptr;

  std::shared_ptr<Texture::Image2D> texBullet;
  std::shared_ptr<Texture::Image2D> texGrenade;
  std::shared_ptr<Texture::Image2D> texWoodenBarrior;
  ActorPtr builderActor;

  // �A�˗p�ϐ�.
  float shotTimer = 0; // ���̒e�𔭎˂���܂ł̎c�莞��(�b).
  const float shotInterval = 0.1f; // �e�̔��ˊԊu(�b).
  int leftOfRounds = 0; // �e�̎c��A�ˉ�.
  const int maxRounds = 3; // 1��̃{�^�����͂Ŕ��˂����e��.
};

#endif // PLAYERACTOR_H_INCLUDED

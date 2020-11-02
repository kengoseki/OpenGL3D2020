/**
* @file GrenadeActor.cpp
*/
#include "GrenadeActor.h"
#include "../MainGameScene.h"
#include "../GameData.h"

/**
* �R���X�g���N�^.
*
* @param pos  �A�N�^�[��z�u������W.
* @param vel  �A�N�^�[�̈ړ����x.
* @param rotY �A�N�^�[�̌���.
*/
GrenadeActor::GrenadeActor(const glm::vec3& pos, const glm::vec3& vel, float rotY, MainGameScene* pScene) :
  Actor("grenade",
    &GameData::Get().primitiveBuffer.Get(GameData::PrimNo::m67_grenade),
    std::make_shared<Texture::Image2D>("Res/m67_grenade.tga"),
    pos),
  timer(3),
  pMainGameScene(pScene)
{
  rotation.y = rotY;
  velocity = vel;
  scale = glm::vec3(1.5f);
  gravityScale = 1;

  // �Փˌ`���ݒ�.
  SetCylinderCollision(0.15f, -0.15f, 0.15f);

  OnHit = [](Actor& a, Actor& b) {
    // �Փː悪�e�ۂ܂��̓v���C���[�̏ꍇ�͉������Ȃ�.
    if (b.name == "bullet" || b.name == "player") {
      return;
    }
    a.velocity.x = a.velocity.z = 0;
    static_cast<GrenadeActor&>(a).hasRotation = false;
  };
}

/**
* ��֒e�̏�Ԃ��X�V����.
*/
void GrenadeActor::OnUpdate(float deltaTime)
{
  timer -= deltaTime;
  if (timer <= 0) {
    isDead = true;

    ActorPtr actor = std::make_shared<Actor>("blast", nullptr, nullptr, position);
    actor->SetCylinderCollision(3, -1, 3);
    actor->collision.blockOtherActors = false;
    actor->lifetime = 0.00001f;
    actor->OnHit = [](Actor& a, Actor& b) {
      if (b.collision.shape != Collision::Shape::box) {
        // ������ԕ������v�Z.
        glm::vec3 direction = b.position - a.position;
        direction.y += 2;
        direction = glm::normalize(direction);
        if (direction.y <= 0.5f) {
          direction.y = 0.5f;
          direction = glm::normalize(direction);
        }
        const float speed = 5;
        b.velocity += direction * speed;
      }
    };
    pMainGameScene->AddActor(actor);
    return;
  }

  if (hasRotation) {
    rotation.z += glm::radians(360.0f) * deltaTime;
  }
}


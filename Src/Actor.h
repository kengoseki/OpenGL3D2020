/**
* @file Actor.h
*/
#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include "glad/glad.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

/**
* �A�j���[�V�����f�[�^.
*/
struct Animation
{
  std::vector<const Mesh::Primitive*> list; // �A�j���[�V�����Ɏg���v���~�e�B�u�̃��X�g.
  float interval = 0.3f; // �v���~�e�B�u��؂�ւ���Ԋu(�b).
  bool isLoop = true; // ���[�v�t���O.
};

/**
* �Փ˔���.
*/
struct Collision
{
  // �Փˌ`��̎��.
  enum Shape {
    none,     // �Փ˔���Ȃ�.
    cylinder, // �~��.
    box,      // ������.
  };
  Shape shape = Shape::none;
  bool isBlock = true; // �ʂ蔲�����Ȃ��Ȃ�true�A��������Ȃ�false.

  // �~���̃p�����[�^.
  float top;    // �~���̏�[.
  float bottom; // �~���̉��[.
  float radius; // �~���̔��a.

  // �����̂̃p�����[�^.
  glm::vec3 boxMin;
  glm::vec3 boxMax;
};

/**
* �A�N�^�[.
*/
class Actor
{
public:
  Actor() = default;
  ~Actor() = default;
  Actor(std::string actorName, const Mesh::Primitive* prim,
    std::shared_ptr<Texture::Image2D> tex, const glm::vec3& pos);

  void Update(float deltTIme);
  void Draw(const Shader::Pipeline& pipeline, const glm::mat4& matVP, const glm::mat4& matShadow) const;
  void SetCylinderCollision(float top, float bottom, float radius);
  void SetBoxCollision(const glm::vec3& min, const glm::vec3& max);
  void SetAnimation(
    std::shared_ptr<Animation> animation);

  std::string name; // �A�N�^�[�̖��O.

  // �A�N�^�[�̓�����.
  enum State {
    idle, // �������Ă��Ȃ�(�ҋ@��).
    run,  // �����Ă���.
    search, // ���G���Ă���.
    attack, // �U�����Ă���.
    damage, // �_���[�W���󂯂Ă���.
    down, // �|��Ă���.
  };
  State state = State::idle; // ���݂̓�����.


  const Mesh::Primitive* primitive = nullptr;
  std::shared_ptr<Texture::Image2D> texture;

  glm::vec3 position = glm::vec3(0); // �A�N�^�[�̕\���ʒu.
  glm::vec3 rotation = glm::vec3(0); // �A�N�^�[�̌���.
  glm::vec3 scale = glm::vec3(1); // �A�N�^�[�̑傫��.
  glm::vec3 velocity = glm::vec3(0); // �A�N�^�[�̈ړ����x.

  // �A�j���[�V�����p�f�[�^.
  std::shared_ptr<Animation> animation; // �A�j���[�V�����f�[�^.
  size_t animationNo = 0; // �\������v���~�e�B�u�̔ԍ�.
  float animationTimer = 0; // �v���~�e�B�u�؂�ւ��^�C�}�[(�b).

  // �Փ˔���p�̕ϐ�.
  Collision collision;
  void (*OnHit)(Actor&, Actor&) = [](Actor&, Actor&) {};

  bool isDead = false;
  bool hasShadow = true;
};

using ActorPtr = std::shared_ptr<Actor>; // �A�N�^�[�|�C���^�^.
using ActorList = std::vector<ActorPtr>; // �A�N�^�[�z��^.

void UpdateActorList(ActorList& actorList, float deltaTime);
void RenderActorList(const ActorList& actorList,
  const glm::mat4& matVP, const glm::mat4& matShadow);

bool DetectCollision(Actor&, Actor&);

#endif // ACTOR_H_INCLUDED

/**
* @file Actor.h
*/
#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include "glad/glad.h"
#include "Global.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

/**
* �Փ˔���.
*/
struct Collision 
{
  // �~���̃p�����[�^.
  float top;    // �~���̏�[.
  float bottom; // �~���̉��[.
  float radius; // �~���̔��a.
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

  std::string name; // �A�N�^�[�̖��O.

  const Mesh::Primitive* primitive = nullptr;
  std::shared_ptr<Texture::Image2D> texture;

  glm::vec3 position = glm::vec3(0); // �A�N�^�[�̕\���ʒu.
  glm::vec3 rotation = glm::vec3(0); // �A�N�^�[�̌���.
  glm::vec3 scale = glm::vec3(1); // �A�N�^�[�̑傫��.
  glm::vec3 velocity = glm::vec3(0); // �A�N�^�[�̈ړ����x.

  // �A�j���[�V�����p�f�[�^.
  std::vector<const Mesh::Primitive*> animation; // �A�j���[�V�����Ɏg���v���~�e�B�u�̃��X�g.
  size_t animationNo = 0; // �\������v���~�e�B�u�̔ԍ�.
  float animationTimer = 0; // �v���~�e�B�u�؂�ւ��^�C�}�[(�b).
  float animationInterval = 0.3f; // �v���~�e�B�u��؂�ւ���Ԋu(�b).

  // �Փ˔���p�̕ϐ�.
  Collision collision;

  bool hasShadow = true;
};

// �A�N�^�[�̔z��.
using ActorPtr = std::shared_ptr<Actor>;
using ActorList = std::vector<ActorPtr>;

void UpdateActorList(ActorList& actorList, float deltaTime);
void RenderActorList(const ActorList& actorList,
  const glm::mat4& matVP, const glm::mat4& matShadow);

bool DetectCollision(const Actor&, const Actor&);
void HandleCollisions(ActorList&);

#endif // ACTOR_H_INCLUDED

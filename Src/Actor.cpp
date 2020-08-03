/**
* @file Actor.cpp
*/
#include "Actor.h"
#include <glm/gtc/matrix_transform.hpp>

/**
* �R���X�g���N�^.
*
* @param actorName �쐬����A�N�^�[�ɕt���閼�O.
* @param pid       �A�N�^�[�̊O����\���v���~�e�B�u��ID.
* @param tex       �v���~�e�B�u�ɓ\��t����e�N�X�`��.
* @param pos       �A�N�^�[�̍��W.
*/
Actor::Actor(std::string actorName, const Mesh::Primitive* prim,
  std::shared_ptr<Texture::Image2D> tex, const glm::vec3& pos) :
  name(name), primitive(prim), texture(tex), position(pos)
{
}

/**
* �A�N�^�[�̏�Ԃ��X�V����.
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b).
*/
void Actor::Update(float deltaTime)
{
  position += velocity * deltaTime;

  // �A�j���[�V�������X�V.
  if (!animation.empty()) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
      animationTimer -= animationInterval;
      ++animationNo;
      if (animationNo >= animation.size()) {
        animationNo = 0;
      }
      primitive = animation[animationNo];
    }
  }
}

/**
* �A�N�^�[��`�悷��.
*
* @param pipeline �s��̐ݒ��ƂȂ�p�C�v���C���I�u�W�F�N�g.
* @param matVP    �`��Ɏg�p����r���[�v���W�F�N�V�����s��.
*/
void Actor::Draw(const Shader::Pipeline& pipeline, const glm::mat4& matVP, const glm::mat4& matShadow) const
{
  // �v���~�e�B�u���ݒ肳��Ă��Ȃ��Ƃ��͉��������I��.
  if (!primitive) {
    return;
  }

  // ���s�ړ�������s������.
  const glm::mat4 matTranslate = glm::translate(glm::mat4(1), position);
  // X����]������s������.
  const glm::mat4 matRotateX = glm::rotate(
    glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
  // Y����]������s������.
  const glm::mat4 matRotateY = glm::rotate(
    glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
  // Z����]������s������.
  const glm::mat4 matRotateZ = glm::rotate(
    glm::mat4(1), rotation.z, glm::vec3(0, 0, 1));
  // �傫����ς���s������.
  const glm::mat4 matScale = glm::scale(glm::mat4(1), scale);

  // ���s�ړ��E��]�E�傫���ύX�̍s����|���Z���āA�ЂƂ̃��f���s��ɂ܂Ƃ߂�.
  const glm::mat4 matModel =
    matTranslate * matRotateY * matRotateZ * matRotateX * matScale;

  // GPU�������ɍs���]��.
  pipeline.SetModelMatrix(matModel);
  pipeline.SetMVP(matVP * matModel);

  // �e�N�X�`���C���[�W�X���b�g0�ԂɃe�N�X�`�������蓖�Ă�.
  texture->Bind(0);

  // �v���~�e�B�u��`��.
  primitive->Draw();

    // �e��`��.
  if (hasShadow) {
    // ���s�ړ��E��]�E�傫���ύX�̍s����|���Z���āA�ЂƂ̃��f���s��ɂ܂Ƃ߂�.
    const glm::mat4 matModelShadow =
      matTranslate * matShadow * matRotateY * matRotateZ * matRotateX * matScale;

    // GPU�������ɉe�s���]��.
    pipeline.SetModelMatrix(matModelShadow);
    pipeline.SetMVP(matVP * matModelShadow);

    // �v���~�e�B�u��`��.
    primitive->Draw();
  }
}

/**
* �A�N�^�[���X�g���X�V����.
*
* @param actorList  �X�V����A�N�^�[���X�g.
* @param deltaTime  �O��̍X�V����̌o�ߎ���(�b).
*/
void UpdateActorList(ActorList& actorList, float deltaTime)
{
  for (size_t i = 0; i < actorList.size(); ++i) {
    actorList[i]->Update(deltaTime);
  }
}

/**
* �A�N�^�[���X�g��`�悷��.
*
* @param actorList �`�悷��A�N�^�[���X�g.
* @param matVP     �`��Ɏg�p����r���[�v���W�F�N�V�����s��.
*/
void RenderActorList(const ActorList& actorList,
  const glm::mat4& matVP, const glm::mat4& matShadow)
{
  Global& global = Global::Get();
  for (size_t i = 0; i < actorList.size(); ++i) {
    actorList[i]->Draw(*global.pipeline, matVP, matShadow);
  }
}

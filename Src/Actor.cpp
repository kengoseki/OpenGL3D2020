/**
* @file Actor.cpp
*/
#include "Actor.h"
#include "GameData.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

/**
* �R���X�g���N�^.
*
* @param name �쐬����A�N�^�[�ɕt���閼�O.
* @param pid       �A�N�^�[�̊O����\���v���~�e�B�u��ID.
* @param tex       �v���~�e�B�u�ɓ\��t����e�N�X�`��.
* @param pos       �A�N�^�[�̍��W.
*/
Actor::Actor(std::string name, const Mesh::Primitive* prim,
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
  // ���W���X�V.
  position += velocity * deltaTime;

  // �A�j���[�V�����f�[�^������΃A�j���[�V��������.
  if (animation && !animation->list.empty()) {
    // ���[�v�t���O��true�A�܂��̓��[�v�t���O��false
    // ���A�j���[�V�����ԍ����v���~�e�B�u���X�g�̃f�[�^���𒴂��Ă��Ȃ��ꍇ�A
    // �A�j���[�V�������X�V����.
    if (animation->isLoop || animationNo < animation->list.size() - 1) {
      animationTimer += deltaTime;
      // �A�j���[�V�����^�C�}�[���C���^�[�o�����Ԃ𒴂��Ă�����A
      // �^�C�}�[�����炵�āA�A�j���[�V�����ԍ���E�߂�.
      if (animationTimer >= animation->interval) {
        animationTimer -= animation->interval;
        ++animationNo;
        // �A�j���[�V�����ԍ����v���~�e�B�u���X�g�̃f�[�^���𒴂����ꍇ�A
        // �A�j���[�V�����ԍ���0�ɖ߂�.
        if (animationNo >= animation->list.size()) {
          animationNo = 0;
        }
      }
    }
    primitive = animation->list[animationNo];
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
    glm::mat4 matTransY = glm::mat4(1);
    matTransY[3][1] = position.y;
    glm::mat4 matTransXZ = glm::mat4(1);
    matTransXZ[3][0] = position.x;
    matTransXZ[3][2] = position.z;
    const glm::mat4 matModelShadow =
      matTransXZ * matShadow * matTransY * matRotateY * matRotateZ * matRotateX * matScale;

    // GPU�������ɉe�s���]��.
    pipeline.SetModelMatrix(matModelShadow);
    pipeline.SetMVP(matVP * matModelShadow);

    // �v���~�e�B�u��`��.
    primitive->Draw();
  }
}

/**
* �����~���̏Փ˔����ݒ肷��.
*
* @param top    �~���̏�[�̍��W.
* @param bottom �~���̉��[�̍��W.
* @param radius �~���̔��a.
*/
void Actor::SetCylinderCollision(float top, float bottom, float radius)
{
  collision.shape = Collision::Shape::cylinder;
  collision.top = top;
  collision.bottom = bottom;
  collision.radius = radius;
}

/**
* �����̂̏Փ˔����ݒ肷��.
*
* @param min �����̂̍ŏ����W.
* @param max �����̂̍ő���W.
*/
void Actor::SetBoxCollision(const glm::vec3& min, const glm::vec3& max)
{
  collision.shape = Collision::Shape::box;
  collision.boxMin = min;
  collision.boxMax = max;
}

/**
* �A�j���[�V������ݒ肷��.
*
* @param animation �A�j���[�V�����f�[�^.
*/
void Actor::SetAnimation(
  std::shared_ptr<Animation> animation)
{
  // ���ɓ����A�j���[�V�������ݒ肳��Ă���ꍇ�͉������Ȃ�.
  if (this->animation == animation) {
    return;
  }

  this->animation = animation;
  animationNo = 0;
  animationTimer = 0;
  if (animation && !animation->list.empty()) {
    primitive = animation->list[0];
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
  // ��ԍX�V.
  for (size_t i = 0; i < actorList.size(); ++i) {
    actorList[i]->Update(deltaTime);
  }

  // �Փ˔���.
  for (size_t ia = 0; ia < actorList.size(); ++ia) {
    Actor& a = *actorList[ia]; // �A�N�^�[A
    // �A�N�^�[A�����S���Ă���ꍇ�͏Փ˂��Ȃ�.
    if (a.dead) {
      continue;
    }
    // �v�Z�ς݋y�ю������g�������A�c��̃A�N�^�[�Ƃ̊ԂŏՓ˔�������s.
    for (size_t ib = ia + 1; ib < actorList.size(); ++ib) {
      Actor& b = *actorList[ib]; // �A�N�^�[B
      // �A�N�^�[B�����S���Ă���ꍇ�͏Փ˂��Ȃ�.
      if (b.dead) {
        continue;
      }
      // �Փ˔���.
      if (DetectCollision(a, b)) {
        // �Փ˂��Ă�����A�o����OnHit�֐������s����.
        a.OnHit(a, b);
        b.OnHit(b, a);
      }
    } // �����ʂ̐��ɒ���.
  }

  // dead��Ԃ̃A�N�^�[���폜.
  const auto isDead = [](ActorPtr p) { return p->dead; };
  const ActorList::iterator i = std::remove_if(actorList.begin(), actorList.end(), isDead);
  actorList.erase(i, actorList.end());
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
  GameData& global = GameData::Get();
  for (size_t i = 0; i < actorList.size(); ++i) {
    actorList[i]->Draw(*global.pipeline, matVP, matShadow);
  }
}

/**
* �~���Ɖ~���̏Փ˂���������.
*
* @param a       �Փˌ`�󂪉~���̃A�N�^�[A.
* @param b       �Փˌ`�󂪉~���̃A�N�^�[B.
* @param isBlock �ђʂ����Ȃ��ꍇtrue. �ђʂ���ꍇfalse
*
* @retval true  �Փ˂��Ă���.
* @retval false �Փ˂��Ă��Ȃ�.
*/
bool CollideCylinderAndCylinder(Actor& a, Actor& b, bool isBlock)
{
  // �~��A�̉��[���~��B�̏�[�̏�ɂ���Ȃ�Փ˂��Ă��Ȃ�.
  const float bottomA = a.position.y + a.collision.bottom;
  const float topB = b.position.y + b.collision.top;
  if (bottomA >= topB) {
    return false;
  }
  // �~��A�̏�[���~��B�̉��[�̉��ɂ���Ȃ�Փ˂��Ă��Ȃ�.
  const float topA = a.position.y + a.collision.top;
  const float bottomB = b.position.y + b.collision.bottom;
  if (topA <= bottomB) {
    return false;
  }
  // �A�N�^�[A��B�̏Փ˔���(�~��)�̒��S�Ԃ̋�����2��(d2)���v�Z.
  const float dx = a.position.x - b.position.x;
  const float dz = a.position.z - b.position.z;
  const float d2 = dx * dx + dz * dz;
  // �Փ˂��Ȃ�����r���v�Z.
  const float r = a.collision.radius + b.collision.radius;
  // d2���Փ˂��Ȃ�����r��2��ȏ�Ȃ�Փ˂��Ă��Ȃ�.
  if (d2 >= r * r) {
    return false;
  }

  // �u���b�N�w�肪�Ȃ���ΏՓ˂����Ƃ�����񂾂���Ԃ�.
  if (!isBlock) {
    return true;
  }

  // �u���b�N�w�肪����̂ő���������Ԃ�.

  // Y�������̏d�Ȃ��Ă��镔���̒������v�Z.
  const float overlapY = std::min(topA, topB) - std::max(bottomA, bottomB);

  // �Z���ق��̉~���̍����̔������v�Z.
  const float halfY = std::min(topA - bottomA, topB - bottomB) * 0.5f;

  // �d�Ȃ��Ă��钷�����Z�����̉~���̍����̔��������Ȃ��ɉ����Ԃ�.
  // �����ȏ�Ȃ牡�ɉ����Ԃ�.
  if (overlapY < halfY || d2 <= 0) {
    // ���[�������ʒu�ɂ���A�N�^�[����Ɉړ�.
    if (bottomA > bottomB) {
      a.position.y += topB - bottomA; // A����Ɉړ�.
    } else {
      b.position.y += topA - bottomB; // B����Ɉړ�.
    }
  } else {
    // ���S�Ԃ̋���d���v�Z.
    const float d = std::sqrt(d2);
    // �����Ԃ�����s���v�Z.
    const float s = r - d;
    // �~���̒��S���Ԃ̕����x�N�g��n���v�Z.
    const glm::vec3 n(dx / d, 0, dz / d);
    // �A�N�^�[A��B���ϓ��ɉ����Ԃ�.
    a.position += n * s * 0.5f;
    b.position -= n * s * 0.5f;
  }
  return true;
}

/**
* �~���ƒ����̂̏Փ˂���������.
*
* @param a �Փˌ`�󂪉~���̃A�N�^�[.
* @param b �Փˌ`�󂪒����̂̃A�N�^�[.
* @param isBlock �ђʂ����Ȃ��ꍇtrue. �ђʂ���ꍇfalse
*
* @retval true  �Փ˂��Ă���.
* @retval false �Փ˂��Ă��Ȃ�.
*/
bool CollideCylinderAndBox(Actor& a, Actor& b, bool isBlock)
{
  // �~���̉��[�������̂̏�[�̏�ɂ���Ȃ�Փ˂��Ă��Ȃ�.
  const float bottomA = a.position.y + a.collision.bottom;
  const float topB = b.position.y + b.collision.boxMax.y;
  if (bottomA >= topB) {
    return false;
  }
  // �~���̏�[�������̂̉��[�̉��ɂ���Ȃ�Փ˂��Ă��Ȃ�.
  const float topA = a.position.y + a.collision.top;
  const float bottomB = b.position.y + b.collision.boxMin.y;
  if (topA <= bottomB) {
    return false;
  }

  // �~���̒��S���ƒ����̂̍ł��߂�X����̈ʒupx���v�Z.
  float px = a.position.x;
  if (px < b.position.x + b.collision.boxMin.x) {
    px = b.position.x + b.collision.boxMin.x;
  } else if (px > b.position.x + b.collision.boxMax.x) {
    px = b.position.x + b.collision.boxMax.x;
  }

  // �~���̒��S���ƒ����̂̍ł��߂�Z����̈ʒupz���v�Z.
  float pz = a.position.z;
  if (pz < b.position.z + b.collision.boxMin.z) {
    pz = b.position.z + b.collision.boxMin.z;
  } else if (pz > b.position.z + b.collision.boxMax.z) {
    pz = b.position.z + b.collision.boxMax.z;
  }
  // �~���̒��S������ł��߂��_�܂ł̋�����2��(d2)���v�Z.
  const float dx = a.position.x - px;
  const float dz = a.position.z - pz;
  const float d2 = dx * dx + dz * dz;
  // d2���~���̔��a��2��ȏ�Ȃ�Փ˂��Ă��Ȃ�.
  if (d2 >= a.collision.radius * a.collision.radius) {
    return false;
  }

  // �u���b�N�w�肪�Ȃ���ΏՓ˂����Ƃ�����񂾂���Ԃ�.
  if (!isBlock) {
    return true;
  }

  // �u���b�N�w�肪����̂ŉ~���������Ԃ�.

  // Y�������̏d�Ȃ��Ă��镔���̒������v�Z.
  const float overlapY = std::min(topA, topB) - std::max(bottomA, bottomB);

  // �~���ƒ����̂̂����A�Z���ق��̍����̔������v�Z.
  const float halfY = std::min(topA - bottomA, topB - bottomB) * 0.5f;

  // �d�Ȃ��Ă��钷�����~���̔��������Ȃ��܂��͉��ɉ����Ԃ�.
  // �����ȏ�Ȃ牡�ɉ����Ԃ�.
  if (overlapY < halfY) {
    // �~���̉��[�������̂̉��[��荂���ʒu�ɂ���Ȃ�~������Ɉړ�.
    // �����łȂ���Ή��Ɉړ�.
    if (bottomA > bottomB) {
      a.position.y += topB - bottomA;
    } else {
      a.position.y -= topA - bottomB; // �~��vs�~���ƈႤ�̂Œ���.
    }
  } else if (d2 > 0) {
    // ���S���ƍŋߐړ_�̋���d���v�Z.
    const float d = std::sqrt(d2);
    // �����Ԃ�����s���v�Z.
    const float s = a.collision.radius - d;
    // �ŋߐړ_���璆�S���ւ̕����x�N�g��n���v�Z.
    const glm::vec3 n(dx / d, 0, dz / d);
    // �A�N�^�[A�������Ԃ�.
    a.position += n * s;
  } else {
    // �����̂̒��S���W(cx, cz)���v�Z.
    const float cx = b.position.x +
      (b.collision.boxMin.x + b.collision.boxMax.x) * 0.5f;
    const float cz = b.position.z +
      (b.collision.boxMin.z + b.collision.boxMax.z) * 0.5f;

    // �����̂�X�y��Z�������̒����̔���(hx, hz)���v�Z.
    const float hx = b.collision.boxMax.x - cx;
    const float hz = b.collision.boxMax.z - cz;

    // �����̂̒��S���W����~���̒��S���ւ̋���(ox, oz)���v�Z.
    const float ox = a.position.x - cx;
    const float oz = a.position.z - cz;

    // �����o���������v�Z.
    const float px = a.collision.radius + hx - std::abs(ox);
    const float pz = a.collision.radius + hz - std::abs(oz);

    // �����o���������Z���ق��ɉ����o��.
    if (px < pz) {
      // �~���̒��S����-X���ɂ���Ȃ�-X�����̈ړ������̂ق����Z��.
      if (ox < 0) {
        a.position.x -= px;
      } else {
        a.position.x += px;
      }
    } else {
      // �~���̒��S����-Z���ɂ���Ȃ�-Z�����̈ړ������̂ق����Z��.
      if (oz < 0) {
        a.position.z -= pz;
      } else {
        a.position.z += pz;
      }
    }
  }

  return true;
}

/**
* �A�N�^�[�̏Փ˂���������.
*
* @param a �Փ˂���������A�N�^�[A.
* @param b �Փ˂���������A�N�^�[B.
*
* @retval true  �Փ˂��Ă���.
* @retval false �Փ˂��Ă��Ȃ�.
*/
bool DetectCollision(Actor& a, Actor& b)
{
  // �A�N�^�[A�ƃA�N�^�[B�̗������ʂ蔲���֎~�Ȃ牟���Ԃ�
  const bool isBlock = a.collision.isBlock && b.collision.isBlock;

  // �Փˌ`�󂲂Ƃɏ����𕪂���.
  switch (a.collision.shape) {
  // �A�N�^�[A���~���̏ꍇ.
  case Collision::Shape::cylinder:
    switch (b.collision.shape) {
    case Collision::Shape::cylinder:
      return CollideCylinderAndCylinder(a, b, isBlock);
    case Collision::Shape::box:
      return CollideCylinderAndBox(a, b, isBlock);
    default:
      return false;
    }

  // �A�N�^�[A�������̂̏ꍇ.
  case Collision::Shape::box:
    switch (b.collision.shape) {
    case Collision::Shape::cylinder:
      return CollideCylinderAndBox(b, a, isBlock);
    case Collision::Shape::box:
      return false;
    default:
      return false;
    }

  // �A�N�^�[A������ȊO�̏ꍇ.
  default:
    return false;
  }
}


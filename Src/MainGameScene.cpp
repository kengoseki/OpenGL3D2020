/**
* @file MainGameScene.cpp
*
* ����21���I�B�u�i�`�X2.0�v�̉Ȋw�����u�Q�����g�E�x���R�t�v(47)�́A�l�Ԃ��]���r�������p�u�f�X�E�X�g���N�`�������O�v�����p���B
* �i�`�X�����Ɛ��E�����̖�]�̎�n�߂ɁA�Ƃ��郈�[���b�p�̕�翂ȕГc�ɂ��]���r�܂݂�ɂ��悤�Ƃ��Ă����B
*
* SAS�\�����ŃT�o�C�o���E�C���X�g���N�^�[�́u�W�����E�X�~�X�v(31)�́A���܂��܉ċG�x�ɂŖK�ꂽ���ł��̊�݂Ɋ������܂�Ă��܂��B
* ���낤���ē������񂾒��̋���ŁA�q�t�́u�A���\�j�[�E�E�F�X�g�v��������ׂ�������������ꂽ�̂������B
* �u���Ȃ��͐_�����킳�ꂽ��m�Ȃ̂ł��B���́u�j�����ꂽAK-47�v�Ɓu���Ȃ��֒e�v�������Ȃ����B�v
* �u�����Đ_�̉h���������āA�ׂȎ��҂ǂ��Ɉ����������炷�̂ł��B�v
* �W�����͋����Ȃ�����u��烂̎s�������̂��R�l�̖��߁B�v�Ƃ����㊯�u�G�h�K�[�E���b�Z���v(35)�̋������v���o���A�^���ɏ]�����Ƃɂ���B
* �i�`�X2.0�̖�]�����������߁A�r��ʂĂ����ƃ]���r�̑��H���[���X���삯������W�����̉ߍ��Ȑ킢���n�܂����I
*
* �u�����͂���˂��B�z��͂Ƃ����Ɏ���ł�񂾁B�v
*
* [����̓��e���]
*
* ������:
* - �z���[�����u�������d��.
* - �z���[���̓]���r�̑�Q�Ƃ������_�ŏ\��.
* - �h��Ȕ����A����A�U���A�����Ĕ�юU�錌���Ԃ�.
*
* �v���O�����̋����_:
* - �T�E���h.
* - �}�E�X�ɂ��Ə��Ǝˌ�. �������ƏՓː}�`�Ƃ̏Փ˔��肪�K�v.
* - ���̏Փ˔���.
* - Y����]���������̂̏Փ˔���.
* - �t�F�[�h�C���E�A�E�g.
* - ���[�t�B���O�ɂ��⊮�A�j���[�V����
* - �I�t�X�N���[���T�[�t�F�X�ɂ���ʌ���.
* - �m�[�}���}�b�v.
* - �O�p�|���S���E�l�p�|���S���ƒ����̏Փ˔���.
* - ���f���f�[�^�̌y�ʉ�.
* - ��p�̃��f���t�H�[�}�b�g�̊J��.
*
* �v���C���[�̋@�\:
* - ��֒e�𓊂���. �]����(�ǉ��̃A�j���[�V�������K�v).
* - �������ւ̍U����i.
* - �o���P�[�h�쐬.
* - �O�]�A�U��قǂ��A�W�����v�A������у_�E���A�_�E������̋N���オ��.
* - �ߐڍU��.
*
* �]���r�̋@�\:
* - �]���r���f���̃o���G�[�V�����𑝂₷.
* - �����g����or�����ɂ�陳���ړ�����эU��.
* - ������у_�E��.
* - �_�E������̋N���オ��.
* - �������U����̃_�E��.
* - �㔼�g���U����̃_�E��.
* - ����.
* - ����_�E��.
* - ���݂��U��.
* - ��������܂��̃|�[�Y.
* - �{�X�]���r.
*/
#include "MainGameScene.h"
#include "GameData.h"
#include "SceneManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>

/**
* ���؂�`�悷��.
*/
void MainGameScene::AddLineOfTrees(const glm::vec3& start, const glm::vec3& direction)
{
  GameData& global = GameData::Get();

  glm::vec3 offset = start;
  for (float i = 0; i < 19; ++i) {
    std::shared_ptr<Actor> actor = std::make_shared<Actor>("tree",
      &global.primitiveBuffer.Get(GameData::PrimNo::tree),
      texTree, start + direction * i);
    actor->rotation.y = glm::radians(i * 30);
    actor->SetBoxCollision(glm::vec3(-1, 0, -1), glm::vec3(1, 6, 1));
    actors.push_back(actor);
  }
}

/**
* ���C���Q�[����ʂ�����������.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool MainGameScene::Initialize()
{
  texGround = std::make_shared<Texture::Image2D>("Res/Ground.tga");
  texTree   = std::make_shared<Texture::Image2D>("Res/Tree.tga");
  texHouse  = std::make_shared<Texture::Image2D>("Res/House.tga");
  texCube   = std::make_shared<Texture::Image2D>("Res/Rock.tga");
  if (!texGround ||!texTree || !texHouse || !texCube) {
    return false;
  }

  texZombie = std::make_shared<Texture::Image2D>("Res/zombie_male.tga");
  texPlayer = std::make_shared<Texture::Image2D>("Res/player_male.tga");
  texBullet = std::make_shared<Texture::Image2D>("Res/Bullet.tga");
  texGameClear = std::make_shared<Texture::Image2D>("Res/Survived.tga");
  texGameOver = std::make_shared<Texture::Image2D>("Res/GameOver.tga");
  texBlack = std::make_shared<Texture::Image2D>("Res/Black.tga");
  texPointer = std::make_shared<Texture::Image2D>("Res/Pointer.tga");
  texWoodenBarrior = std::make_shared<Texture::Image2D>("Res/wooden_barrier.tga");

  GameData& global = GameData::Get();

  std::random_device rd;
  std::mt19937 random(rd());

  // �؂�\��.
  for (float j = 0; j < 4; ++j) {
    const glm::mat4 matRot = glm::rotate(glm::mat4(1), glm::radians(90.0f) * j, glm::vec3(0, 1, 0));
    AddLineOfTrees(matRot * glm::vec4(-19, 0, 19, 1), matRot * glm::vec4(2, 0, 0, 1));
  }

  // �Ƃ�\��.
  {
    std::shared_ptr<Actor> actor = std::make_shared<Actor>(
      "house", &global.primitiveBuffer.Get(GameData::PrimNo::house), texHouse, glm::vec3(0));
    actor->SetBoxCollision(glm::vec3(-3, 0, -3), glm::vec3(3, 5, 3));
    actors.push_back(actor);
  }

  // �����̂�\��.
  {
    std::shared_ptr<Actor> actor = std::make_shared<Actor>(
      "cube", &global.primitiveBuffer.Get(GameData::PrimNo::cube), texCube, glm::vec3(10, 1, 0));
    actor->SetBoxCollision(glm::vec3(-1), glm::vec3(1));
    actors.push_back(actor);
  }

  // �v���C���[��\��.
  {
    playerActor = std::make_shared<Actor>("player", &global.primitiveBuffer.Get(GameData::PrimNo::player_idle_0),
      texPlayer, glm::vec3(10, 0, 10));
    playerActor->health = 10;
    playerActor->SetAnimation(GameData::Get().anmPlayerIdle);
    playerActor->SetCylinderCollision(1.7f, 0, 0.5f);
    playerActor->OnHit = [](Actor& a, Actor& b) {
      if (b.name == "zombie_attack") {
        // ����ł����牽�����Ȃ�.
        if (a.state == Actor::State::dead) {
          return;
        }
        // ���G�^�C�}�[�ғ����͏Փ˂��Ȃ�.
        if (a.timer > 0) {
          return;
        }
        // �ϋv�͂����炷.
        a.health -= 1;
        b.collision.shape = Collision::Shape::none;
        // �ϋv�͂�0���傫����΃_���[�W�A�j���[�V�������Đ����A���G�^�C�}�[��ݒ�.
        // 0�ȉ��Ȃ玀�S.
        if (a.health > 0) {
          a.SetAnimation(GameData::Get().anmPlayerDamage);
          a.state = Actor::State::damage;
          a.timer = 2;
        } else {
          a.velocity = glm::vec3(0);
          a.timer = 3;
          a.SetAnimation(GameData::Get().anmPlayerDown);
          a.state = Actor::State::dead;
        }
      }
    };
    actors.push_back(playerActor);
  }

  // �}�E�X�J�[�\���ʒu�������A�N�^�[.
  cursorActor = std::make_shared<Actor>("cursor", &global.primitiveBuffer.Get(GameData::PrimNo::cube), texCube, glm::vec3(0));
  cursorActor->rotation = glm::vec3(0.75f, 0.75f, 0.75f);
  cursorActor->scale = glm::vec3(0.25f);
  actors.push_back(cursorActor);

  // �]���r��\��.
  const Mesh::Primitive* pPrimitive = &global.primitiveBuffer.Get(GameData::PrimNo::zombie_male_walk_0);
  for (size_t i = 0; i < appearanceEnemyCount; ++i) {
    glm::vec3 pos(0);
    pos.x = std::uniform_real_distribution<float>(-18, 18)(global.random);
    pos.z = std::uniform_real_distribution<float>(-18, 18)(global.random);
    std::shared_ptr<Actor> actor = std::make_shared<Actor>("zombie", pPrimitive, texZombie, pos);
    actor->health = 5;
    actor->rotation.y =
      std::uniform_real_distribution<float>(0, glm::radians(360.0f))(global.random);
    // �A�j���[�V������ݒ�.
    actor->SetAnimation(GameData::Get().anmZombieMaleWalk);
    actor->state = Actor::State::run;
    actor->SetCylinderCollision(1.7f, 0, 0.5f);

    // �Փˏ�����ݒ�.
    actor->OnHit = [](Actor& a, Actor& b) {
      if (b.name == "bullet") {
        // �ϋv�l�����炷.
        a.health -= 2;
        // �ϋv�l��0���傫����΃_���[�W�A�j���[�V�������Đ�����.
        // �ϋv�l��0�ȉ��ɂȂ����玀�S.
        if (a.health > 0) {
          // �m�b�N�o�b�N��ݒ肷��.
          if (glm::dot(b.velocity, b.velocity)) {
            a.velocity += glm::normalize(b.velocity) * 2.0f;
          }
          // �����A�j���͍Đ��ł��Ȃ��̂�nullptr���w�肵�ăA�j�����폜����.
          a.SetAnimation(nullptr);
          // �_���[�W�A�j���[�V�������Đ�.
          a.SetAnimation(GameData::Get().anmZombieMaleDamage);
          // �_���[�W��Ԃɐݒ�.
          a.state = Actor::State::damage;
        } else {
          // ���S�A�j���[�V������ݒ�.
          a.SetAnimation(GameData::Get().anmZombieMaleDown);
          // �Փ˔���𖳂���.
          a.collision.shape = Collision::Shape::none;
          // ���S��Ԃɐݒ�.
          a.state = Actor::State::dead;
          // �|�����]���r�̐���1�̑��₷.
          ++GameData::Get().killCount;
        }
      }
    };
    actors.push_back(actor);
  }

  // �_������ݒ肷��
  pointLight = Shader::PointLight{
    glm::vec4(8, 10,-8, 0),
    glm::vec4(0.4f, 0.7f, 1.0f, 0) * 200.0f
  };

  // �Q�[���f�[�^�̏����ݒ�.
  GameData& gamedata = GameData::Get();
  gamedata.killCount = 0;

  // �}�E�X�J�[�\�����\���ɂ���.
  glfwSetInputMode(gamedata.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  std::cout << "[���] MainGameScene���J�n.\n";
  return true;
}

/**
* ���C���Q�[����ʂ̃L�[���͂���������.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
*/
void MainGameScene::ProcessInput(GLFWwindow* window)
{
  // �N���A���Ă���?
  GameData& gamedata = GameData::Get();
  if (isGameClear) {
    // Enter�L�[�������ꂽ��^�C�g����ʂɈړ�.
    if (gamedata.keyPressedInLastFrame & GameData::Key::enter) {
      SceneManager::Get().ChangeScene(TITLE_SCENE_NAME);
    }
    return;
  }

  // �Q�[���I�[�o�[?
  if (isGameOver) {
    // Enter�L�[�������ꂽ��^�C�g����ʂɈړ�.
    if (gamedata.keyPressedInLastFrame & GameData::Key::enter) {
      SceneManager::Get().ChangeScene(TITLE_SCENE_NAME);
    }
    return;
  }

  // �v���C���[������ł�����
  if (playerActor->state == Actor::State::dead) {
    // �A�j���[�V�������I�����Ă�����Q�[���I�[�o�[�ɂ���.
    if (playerActor->animationNo >= playerActor->animation->list.size() - 1) {
      isGameOver = true;
    }
    return;
  }

  // �}�E�X�|�C���^��n�ʂɕ\��.
  {
    const glm::vec2 cursor = GameData::Get().cursorPosition;

    Segment seg;
    const glm::vec2 screenPosition((cursor.x / 1280) * 2, (cursor.y / 720) * 2);
    const glm::mat4 matInverseVP = glm::inverse(matProj * matView);

    const glm::vec4 start = matInverseVP * glm::vec4(screenPosition, -1, 1);
    seg.start = glm::vec3(start) / start.w;

    const glm::vec4 end = matInverseVP * glm::vec4(screenPosition, 1, 1);
    seg.end = glm::vec3(end) / end.w;

    const Plane plane{ glm::vec3(0, 1, 0), glm::vec3(0, 1, 0) };
    Intersect(seg, plane, &cursorActor->position);

    const glm::vec3 direction(cursorActor->position - playerActor->position);
    playerActor->rotation.y = std::atan2(-direction.z, direction.x);
  }

  // �v���C���[�A�N�^�[���ړ�������.
  glm::vec3 direction = glm::vec3(0);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    direction.x -= 1;
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    direction.x += 1;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    direction.z -= 1;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    direction.z += 1;
  }

  GameData& global = GameData::Get();

  std::shared_ptr<Animation> nextAnime;
  if (glm::length(direction) > 0) {
    //playerActor->rotation.y = std::atan2(-direction.z, direction.x);
    const float speed = 4.0f;
    direction = glm::normalize(direction);
    playerActor->velocity = direction * speed;

    const glm::vec3 front(std::cos(playerActor->rotation.y), 0, -std::sin(playerActor->rotation.y));
    const float cf = glm::dot(front, direction);
    if (cf > std::cos(glm::radians(45.0f))) {
      nextAnime = GameData::Get().anmPlayerRunFront;
    } else if (cf < std::cos(glm::radians(135.0f))) {
      nextAnime = GameData::Get().anmPlayerRunBack;
    } else {
      const glm::vec3 right(std::cos(playerActor->rotation.y-glm::radians(90.0f)), 0, -std::sin(playerActor->rotation.y-glm::radians(90.0f)));
      const float cr = glm::dot(right, direction);
      if (cr > std::cos(glm::radians(90.0f))) {
        nextAnime = GameData::Get().anmPlayerRunRight;
      } else {
        nextAnime = GameData::Get().anmPlayerRunLeft;
      }
    }
  } else {
    playerActor->velocity = glm::vec3(0);
    nextAnime = GameData::Get().anmPlayerIdle;
  }

  // �_���[�W�A�j���Đ����̓_���[�W�A�j�����I���܂ő҂�.
  if (playerActor->animation == global.anmPlayerDamage) {
    if (playerActor->animationNo >= playerActor->animation->list.size() - 1) {
      playerActor->SetAnimation(nextAnime);
    }
  } else {
    playerActor->SetAnimation(nextAnime);
  }

  // ���˃L�[��������Ă�����O�_�˂��N��.
  if (GameData::Get().keyPressedInLastFrame & GameData::Key::shot) {
    leftOfRounds = maxRounds;
    shotTimer = 0;
  }
  // ���ː����c���Ă��Ĕ��˃^�C�}�[��0�ȉ��Ȃ�1������.
  if (leftOfRounds > 0 && shotTimer <= 0) {
    --leftOfRounds;
    shotTimer = shotInterval;

    // �v���C���[��Y����]���琳�ʕ������v�Z.
    const float fx = std::cos(playerActor->rotation.y);
    const float fz = -std::sin(playerActor->rotation.y); // Z���̌����͐��w�Ƌt.
    const glm::vec3 front = glm::vec3(fx, 0, fz);

    // �v���C���[��Y����]����E�������v�Z.
    const float rx = std::cos(playerActor->rotation.y - glm::radians(90.0f));
    const float rz = -std::sin(playerActor->rotation.y - glm::radians(90.0f)); // ����
    const glm::vec3 right = glm::vec3(rx, 0, rz);

    // �e�ۂ̔��ˈʒu(�e��)���v�Z. 3D���f���𒲂ׂ��Ƃ���A�e����
    // �v���C���[�̍��W(����)����O��0.6m�A�E��0.2m�A���0.9m�̈ʒu�ɂ���.
    const glm::vec3 position =
      playerActor->position + front * 0.6f + right * 0.2f + glm::vec3(0, 0.9f, 0);

    // �e�ۃA�N�^�[���e���̈ʒu�ɍ쐬.
    std::shared_ptr<Actor> bullet = std::make_shared<Actor>("bullet",
      &global.primitiveBuffer.Get(GameData::PrimNo::bullet), texBullet, position);

    // ����(��])�̓v���C���[�A�N�^�[���p��.
    bullet->rotation = playerActor->rotation;

    // front�����ցu���b20m�v�̑��x�ňړ�����悤�ɐݒ�.
    bullet->velocity = front * 20.0f;

    // �Փˌ`���ݒ�.
    bullet->SetCylinderCollision(0.1f, -0.1f, 0.125f);
    bullet->collision.isBlock = false;

    // �Փˏ�����ݒ�.
    bullet->OnHit = [](Actor& a, Actor& b) {
      // �Փː悪�e�ۂ܂��̓v���C���[�̏ꍇ�͉������Ȃ�.
      if (b.name == "bullet" || b.name == "player") {
        return;
      }
      // �e�ۂ�����.
      a.isDead = true;
    };

    // �A�N�^�[�����X�g�ɒǉ�.
    actors.push_back(bullet);
  }

  // �E�N���b�N�Ńo���P�[�h��z�u.
  if (!builderActor) {
    if (GameData::Get().keyPressed & GameData::Key::build) {
      builderActor = std::make_shared<Actor>("WoodenBarrior",
        &global.primitiveBuffer.Get(GameData::PrimNo::wooden_barrior), texBlack, cursorActor->position);
      builderActor->texture = texWoodenBarrior;
      // �Փˌ`���ݒ�.
      builderActor->SetBoxCollision(glm::vec3(-1, 0, -0.1f), glm::vec3(1, 2, 0.1f));
      builderActor->collision.isBlock = true;
    }
  }

  if (builderActor) {
    builderActor->position = cursorActor->position;
    builderActor->position.y = 0;

    const double scroll = global.curScroll - global.prevScroll;
    if (scroll <= -1) {
      builderActor->rotation.y -= glm::radians(90.0f);
    } else if (scroll >= 1) {
      builderActor->rotation.y += glm::radians(90.0f);
    }
    builderActor->rotation.y = std::fmod(builderActor->rotation.y + glm::radians(360.0f), glm::radians(360.0f));
    if (std::abs(builderActor->rotation.y - glm::radians(90.0f)) < glm::radians(5.0f)) {
      builderActor->SetBoxCollision(glm::vec3(-0.25f, -1, -1), glm::vec3(0.25f, 1, 1));
    } else if (std::abs(builderActor->rotation.y - glm::radians(270.0f)) < glm::radians(5.0f)) {
      builderActor->SetBoxCollision(glm::vec3(-0.25f, -1, -1), glm::vec3(0.25f, 1, 1));
    } else {
      builderActor->SetBoxCollision(glm::vec3(-1, -1, -0.25f), glm::vec3(1, 1, 0.25f));
    }

    glm::vec4 color = glm::vec4(0.2f, 0.2f, 1, 0.5f);
    for (size_t i = 0; i < actors.size(); ++i) {
      if (DetectCollision(*builderActor, *actors[i], false)) {
        color = glm::vec4(1, 0.2f, 0.2f, 0.5f);
        break;
      }
    }
    builderActor->baseColor = color;

    if (!(GameData::Get().keyPressed & GameData::Key::build)) {
      if (builderActor->baseColor.r < 1) {
        builderActor->baseColor = glm::vec4(1);
        actors.push_back(builderActor);
      }
      builderActor.reset();
    }
  }
}

/**
* ���C���Q�[����ʂ��X�V����.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
* @param deltaTime �O��̍X�V����̌o�ߎ���.
*/
void MainGameScene::Update(GLFWwindow* window, float deltaTime)
{
  if (leftOfRounds > 0) {
    shotTimer -= deltaTime;
  }

  float dt = deltaTime;
  deltaTime = 1.0f / 60.0f;
  for (; dt > 0; dt -= 1.0f / 60.0f) {
    if (dt < deltaTime) {
      deltaTime = dt;
    }
    // �A�N�^�[�̍s��������.
    ActorList newActors; // �V�K�A�N�^�[�̔z��.
    newActors.reserve(100);
    for (auto& e : actors) {
      // �]���r�A�N�^�[�̏ꍇ.
      if (e->name == "zombie") {
        // �]���r�̍s��.
        // 1. +X�����ɒ��i.
        // 2. ���݌����Ă�������ɒ��i.
        // 3. �v���C���[�̕���������.
        // 4. �������v���C���[�̕���������.

        // �U�����ȊO�Ȃ�U���͈͂��폜����.
        if (e->attackActor && e->state != Actor::State::attack) {
          e->attackActor->isDead = true;
          e->attackActor = nullptr;
        }

        // �_���[�W��Ԃ̏ꍇ.
        if (e->state == Actor::State::damage) {
          // �A�j�����I��������ړ���Ԃɂ���.
          if (e->animationNo >= e->animation->list.size() - 1) {
            e->velocity = glm::vec3(0);
            e->SetAnimation(GameData::Get().anmZombieMaleWalk);
            e->state = Actor::State::run;
          }
        }
        // �U�����Ȃ�U���I����҂�.
        else if (e->state == Actor::State::attack) {
          // �A�j���[�V�����ԍ����A�j�������ȏゾ������A�U���A�j���I���Ƃ݂Ȃ�.
          if (e->animationNo >= e->animation->list.size() - 1) {
            e->SetAnimation(GameData::Get().anmZombieMaleWalk);
            e->state = Actor::State::run;
          }
          // �A�j���ԍ���4�ȏォ�U���͈͂����݂���΍U���͈͂��폜����.
          else if (e->animationNo >= 4 && e->attackActor) {
            e->attackActor->isDead = true;
            e->attackActor.reset();
          }
          // �A�j���ԍ���3�ȏォ�U���͈͂����݂��Ȃ���΍U���͈͂��쐬����.
          else if (e->animationNo >= 3 && !e->attackActor) {
            // �]���r�̐��ʕ������v�Z.
            const glm::vec3 front(std::cos(e->rotation.y), 0, -std::sin(e->rotation.y));
            // �U������̔����ʒu���v�Z.
            const glm::vec3 pos = e->position + glm::vec3(0, 0.9f, 0) + front;
            // �U������A�N�^�[���쐬.
            e->attackActor = std::make_shared<Actor>("zombie_attack", nullptr, nullptr, pos);
            // �U�������ݒ�.
            e->attackActor->SetCylinderCollision(0.2f, -0.2f, 0.1f);
            e->attackActor->collision.isBlock = false;
            newActors.push_back(e->attackActor);
          }
        }

        // ����ł��Ȃ���Ε���.
        else if (e->state == Actor::State::run) {
          // �v���C���[�̂���������v�Z.
          glm::vec3 toPlayer = playerActor->position - e->position;
          // �]���r�̐��ʕ������v�Z.
          glm::vec3 front(std::cos(e->rotation.y), 0, -std::sin(e->rotation.y));
          // ���E�ǂ���ɉ�]���邩�����߂邽�߂ɊO�ς��v�Z.
          const glm::vec3 c = glm::cross(front, toPlayer);
          // �����x�N�g����y���W���v���X���Ȃ�����𑝂₵�A�}�C�i�X���Ȃ猸�炷.
          constexpr float speed = glm::radians(60.0f);
          if (c.y >= 0) {
            e->rotation.y += speed * deltaTime;
          } else {
            e->rotation.y -= speed * deltaTime;
          }
          // 360�x�𒴂�����0�x�ɖ߂�.
          constexpr float r360 = glm::radians(360.0f);
          e->rotation.y = fmod(e->rotation.y + r360, r360);
          // �������ω������̂ŁA���ʕ����̃x�N�g�����v�Z���Ȃ���.
          front.x = std::cos(e->rotation.y);
          front.z = -std::sin(e->rotation.y);
          // ���ʕ�����1m/s�̑��x�ňړ�����悤�ɐݒ�.
          e->velocity = front;

          // �v���C���[��������������3m�ȓ�������60�x�ȓ��ɂ�����U��.
          if (playerActor->state != Actor::State::dead) {
            const float distanceSq = glm::dot(toPlayer, toPlayer);
            if (distanceSq <= 3 * 3) {
              const float distance = std::sqrt(distanceSq);
              const float angle = std::acos(glm::dot(front, toPlayer * (1.0f / distance)));
              if (angle <= glm::radians(30.0f)) {
                e->SetAnimation(GameData::Get().anmZombieMaleAttack);
                e->state = Actor::State::attack;
              }
            }
          }
        } else {
          e->velocity = glm::vec3(0);
        }
      }
    }

    // �V�K�A�N�^�[�����݂���Ȃ�A������A�N�^�[���X�g�ɒǉ�����.
    if (!newActors.empty()) {
      actors.insert(actors.end(), newActors.begin(), newActors.end());
      newActors.clear();
    }

    // �A�N�^�[���X�g�Ɋ܂܂��A�N�^�[�̏�Ԃ��X�V����.
    UpdateActorList(actors, deltaTime);

    // �Փ˔���.
    for (size_t ia = 0; ia < actors.size(); ++ia) {
      Actor& a = *actors[ia]; // �A�N�^�[A
      // �A�N�^�[A�����S���Ă���ꍇ�͏Փ˂��Ȃ�.
      if (a.isDead) {
        continue;
      }
      // �v�Z�ς݋y�ю������g�������A�c��̃A�N�^�[�Ƃ̊ԂŏՓ˔�������s.
      for (size_t ib = ia + 1; ib < actors.size(); ++ib) {
        Actor& b = *actors[ib]; // �A�N�^�[B
        // �A�N�^�[B�����S���Ă���ꍇ�͏Փ˂��Ȃ�.
        if (b.isDead) {
          continue;
        }
        // �Փ˔���.
        if (DetectCollision(a, b, true)) {
          // �Փ˂��Ă�����A�o����OnHit�֐������s����.
          a.OnHit(a, b);
          b.OnHit(b, a);
        }
      } // �����ʂ̐��ɒ���.
    }
  }

  // �܂��N���A���Ă��Ȃ�?
  if (!isGameClear) {
    // �N���A����(�u�|�����G�̐��v���u�o������G�̐��v�ȏ�)�𖞂����Ă���?
    if (GameData::Get().killCount >= appearanceEnemyCount) {
      // �Q�[���N���A�t���O��true�ɂ���.
      isGameClear = true;

      // �v���C���[�A�N�^�[��ҋ@��Ԃɂ���.
      playerActor->velocity = glm::vec3(0);
      playerActor->SetAnimation(GameData::Get().anmPlayerIdle);

      std::cout << "[���] �Q�[���N���A������B��\n";
    }
  }

  // �J�������v���C���[�A�N�^�[�̂ȂȂߏ�ɔz�u.
  const glm::vec3 viewPosition = playerActor->position + glm::vec3(0, 7, 7);
  // �v���C���[�A�N�^�[�̑�������ʂ̒����ɉf��悤�ɂ���.
  const glm::vec3 viewTarget = playerActor->position;

  // ���W�ϊ��s����쐬.
  int w, h;
  glfwGetWindowSize(window, &w, &h);
  const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
  matProj =
    glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f);
  matView =
    glm::lookAt(viewPosition, viewTarget, glm::vec3(0, 1, 0));
}

/**
* ���C���Q�[����ʂ�`�悷��.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
*/
void MainGameScene::Render(GLFWwindow* window) const
{
  int fbw, fbh;
  glfwGetFramebufferSize(window, &fbw, &fbh);
  if (fbw <= 0 || fbh <= 0) {
    return;
  }

  GameData& global = GameData::Get();
  std::shared_ptr<Shader::Pipeline> pipeline = global.pipeline;
  Mesh::PrimitiveBuffer& primitiveBuffer = global.primitiveBuffer;
  Texture::Sampler& sampler = global.sampler;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  //glEnable(GL_FRAMEBUFFER_SRGB);
  glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // ������ݒ肷��.
  pipeline->SetAmbientLight(glm::vec3(0.1f, 0.125f, 0.15f));

  // ���s������ݒ肷��
  const Shader::DirectionalLight directionalLight{
    glm::normalize(glm::vec4(3,-2,-2, 0)),
    glm::vec4(1, 0.9f, 0.8f, 1)
  };
  pipeline->SetLight(directionalLight);

  pipeline->SetLight(pointLight);

  primitiveBuffer.BindVertexArray();
  pipeline->Bind();
  sampler.Bind(0);

  // �n�ʂ�`��.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    pipeline->SetObjectColor(glm::vec4(1));
    texGround->Bind(0);
    primitiveBuffer.Get(GameData::PrimNo::ground).Draw();
  }

  // �A�N�^�[���X�g��`��.
  const glm::mat4 matVP = matProj * matView;
  for (size_t i = 0; i < actors.size(); ++i) {
    actors[i]->Draw(*pipeline, matVP, Actor::DrawType::color);
  }

  // ���m��̌��z����`��.
  if (builderActor) {
    builderActor->Draw(*pipeline, matVP, Actor::DrawType::color);
  }

  // �_�����̈ʒu��`��.
  {
    // Y����].
    const float degree = static_cast<float>(std::fmod(glfwGetTime() * 180.0, 360.0));
    const glm::mat4 matModelR =
      glm::rotate(glm::mat4(1), glm::radians(degree), glm::vec3(0, 1, 0));
    // �g��k��.
    const glm::mat4 matModelS =
      glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.25f, 0.5f));
    // ���s�ړ�.
    const glm::mat4 matModelT =
      glm::translate(glm::mat4(1), glm::vec3(pointLight.position) + glm::vec3(0, -1.25f, 0));
    // �g��k���E��]�E���s�ړ�������.
    const glm::mat4 matModel = matModelT * matModelR * matModelS;
    pipeline->SetMVP(matProj * matView * matModel);
    pipeline->SetModelMatrix(matModel);
    texTree->Bind(0);
    primitiveBuffer.Get(GameData::PrimNo::tree).Draw();
  }

  // �A�N�^�[�̉e��`��.
  {
    // �X�e���V���o�b�t�@��L���ɂ���.
    glEnable(GL_STENCIL_TEST);
    // �u��r�Ɏg���l�v��1�ɂ��āA��ɔ�r����������悤�ɐݒ�.
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    // �X�e���V���[�x�̗����̃e�X�g�ɐ��������ꍇ�Ɂu��r����l�v���������ނ悤�ɐݒ�.
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // �X�e���V���o�b�t�@�̑S�r�b�g�̏������݂�����.
    glStencilMask(0xff);
    // �J���[�o�b�t�@�ւ̏������݂��֎~.
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    // �[�x�o�b�t�@�ւ̏������݂��֎~.
    glDepthMask(GL_FALSE);

    // ����1m�̕��̂����Ƃ��e�̒������v�Z.
    const float scale = 1.0f / -directionalLight.direction.y;
    const float sx = directionalLight.direction.x * scale;
    const float sz = directionalLight.direction.z * scale;

    // �؂���񂱍s��(Y���W��0�ɂ���s��)���쐬.
    const glm::mat4 matShadow(
      1.00f, 0.00f, 0.00f, 0.00f,
      sx, 0.00f, sz, 0.00f,
      0.00f, 0.00f, 1.00f, 0.00f,
      0.00f, 0.01f, 0.00f, 1.00f);

    // �e�p�p�C�v�������o�C���h.
    std::shared_ptr<Shader::Pipeline> pipelineShadow = GameData::Get().pipelineShadow;
    pipelineShadow->Bind();

    // �؂���񂱍s�񁨃r���[�s�񁨃v���W�F�N�V�����s��̏��ԂɊ|����s������.
    const glm::mat4 matShadowVP = matVP * matShadow;

    // �؂���񂱃r���[�v���W�F�N�V�����s����g���đS�ẴA�N�^�[��`�悷��.
    for (const auto& actor : actors) {
      actor->Draw(*pipelineShadow, matShadowVP, Actor::DrawType::shadow);
    }

    // �X�e���V���l��1�̏ꍇ�̂݃e�X�g�ɐ�������悤�ɐݒ�.
    glStencilFunc(GL_EQUAL, 1, 0xff);
    // �X�e���V���o�b�t�@
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // �J���[�o�b�t�@�ւ̕`�����݂�����.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    // �[�x�o�b�t�@�𖳌���.
    glDisable(GL_DEPTH_TEST);

    // ��ʑS�̂ɉe�F��h��.
    texBlack->Bind(0);
    pipelineShadow->SetMVP(glm::scale(glm::mat4(1), glm::vec3(2)));
    primitiveBuffer.Get(GameData::PrimNo::plane).Draw();

    // �X�e���V���o�b�t�@�𖳌���.
    glDisable(GL_STENCIL_TEST);
    // �[�x�o�b�t�@��L����.
    glEnable(GL_DEPTH_TEST);
    // �[�x�o�b�t�@�ւ̕`�����݂�����.
    glDepthMask(GL_TRUE);
  }

  // 2D�\��.
  {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    // ���W�ϊ��s����쐬.
    const float halfW = fbw / 2.0f;
    const float halfH = fbh / 2.0f;
    const glm::mat4 matProj =
      glm::ortho<float>(-halfW, halfW, -halfH, halfH, 1.0f, 500.0f);
    const glm::mat4 matView =
      glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0), glm::vec3(0, 1, 0));
    const glm::mat4 matVP = matProj * matView;

    std::shared_ptr<Shader::Pipeline> pipeline2D = GameData::Get().pipelineSimple;
    pipeline2D->Bind();

    // �}�E�X�J�[�\���ʒu��`��.
    {
      const glm::mat4 matModelS = glm::scale(glm::mat4(1),
        glm::vec3(texPointer->Width(), texPointer->Height(), 1));
      const glm::mat4 matModelT = glm::translate(glm::mat4(1), glm::vec3(GameData::Get().cursorPosition, 0));
      const glm::mat4 matModel = matModelT * matModelS;
      pipeline2D->SetMVP(matVP * matModel);
      texPointer->Bind(0);
      primitiveBuffer.Get(GameData::PrimNo::plane).Draw();
    }

    // �Q�[���N���A�摜��`��.
    if (isGameClear) {
      const glm::mat4 matModelS = glm::scale(glm::mat4(1),
        glm::vec3(texGameClear->Width() * 2.0f, texGameClear->Height() * 2.0f, 1));
      const glm::mat4 matModelT = glm::translate(glm::mat4(1), glm::vec3(0, 100, 0));
      const glm::mat4 matModel = matModelT * matModelS;
      pipeline2D->SetMVP(matVP * matModel);
      texGameClear->Bind(0);
      primitiveBuffer.Get(GameData::PrimNo::plane).Draw();
    }

    // �Q�[���I�[�o�[�摜��`��.
    if (isGameOver) {
      const glm::mat4 matModelS = glm::scale(glm::mat4(1),
        glm::vec3(texGameOver->Width() * 2.0f, texGameOver->Height() * 2.0f, 1));
      const glm::mat4 matModelT = glm::translate(glm::mat4(1), glm::vec3(0, 100, 0));
      const glm::mat4 matModel = matModelT * matModelS;
      pipeline2D->SetMVP(matVP * matModel);
      texGameOver->Bind(0);
      primitiveBuffer.Get(GameData::PrimNo::plane).Draw();
    }
  }

  Texture::UnbindAllTextures();
  Texture::UnbindAllSamplers();
  Shader::UnbindPipeline();
  primitiveBuffer.UnbindVertexArray();
}

/**
* ���C���Q�[����ʂ��I������.
*/
void MainGameScene::Finalize()
{
  // �}�E�X�J�[�\����\������.
  glfwSetInputMode(GameData::Get().window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  std::cout << "[���] MainGameScene���I��.\n";
}

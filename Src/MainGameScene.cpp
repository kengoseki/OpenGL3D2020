/**
* @file MainGameScene.cpp
*
* ����21���I�B�u�i�`�X2.0�v�̉Ȋw�����u�Q�����g�E�x���R�t�v(47)�́A�l�Ԃ��]���r�������p�u�f�X�E�X�g���N�`�������O�v�����p���B
* �i�`�X�����Ɛ��E�����̖�]�̎�n�߂ɁA�Ƃ��郈�[���b�p�̕�翂ȕГc�ɂ��]���r�܂݂�ɂ��悤�Ƃ��Ă����B
*
* SAS�\�����ŃT�o�C�o���E�C���X�g���N�^�[�́u���T�E�G���t�B�[���h�v(26)�́A���܂��܉ċG�x�ɂŖK�ꂽ���ł��̊�݂Ɋ������܂�Ă��܂��B
* ���낤���ē������񂾒��̋���ŁA�q�t�́u�A���\�j�[�E�E�F�X�g�v��������ׂ�������������ꂽ�̂������B
* �u���Ȃ��͐_�����킳�ꂽ��m�Ȃ̂ł��B���́u�j�����ꂽAK-47�v�Ɓu���Ȃ��֒e�v�������Ȃ����B�v
* �u�����Đ_�̉h���������āA�ׂȎ��҂ǂ��Ɉ����������炷�̂ł��B�v
* ���T�͋����Ȃ�����u��烂̎s�������̂��R�l�̖��߁B�v�Ƃ����㊯�u�G�h�K�[�E���b�Z���v(35)�̋������v���o���A�^���ɏ]�����Ƃɂ���B
* �i�`�X2.0�̖�]�����������߁A�r��ʂĂ����ƃ]���r�̑��H���[���X���삯�����郊�T�̉ߍ��Ȑ킢���n�܂����I
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
  texPlayer = std::make_shared<Texture::Image2D>("Res/player_female/player_female.tga");
  texBullet = std::make_shared<Texture::Image2D>("Res/Bullet.tga");
  texGameClear = std::make_shared<Texture::Image2D>("Res/Survived.tga");

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
    playerActor->SetAnimation(GameData::Get().anmPlayerIdle);
    playerActor->SetCylinderCollision(1.7f, 0, 0.5f);
    actors.push_back(playerActor);
  }

  // �]���r��\��.
  const Mesh::Primitive* pPrimitive = &global.primitiveBuffer.Get(GameData::PrimNo::zombie_male_walk_0);
  for (size_t i = 0; i < appearanceEnemyCount; ++i) {
    glm::vec3 pos(0);
    pos.x = std::uniform_real_distribution<float>(-18, 18)(global.random);
    pos.z = std::uniform_real_distribution<float>(-18, 18)(global.random);
    std::shared_ptr<Actor> actor = std::make_shared<Actor>("zombie", pPrimitive, texZombie, pos);
    actor->rotation.y =
      std::uniform_real_distribution<float>(0, glm::radians(360.0f))(global.random);
    // �A�j���[�V������ݒ�.
    actor->SetAnimation(GameData::Get().anmZombieMaleWalk);
    actor->SetCylinderCollision(1.7f, 0, 0.5f);

    // �Փˏ�����ݒ�.
    actor->OnHit = [](Actor& a, Actor& b) {
      if (b.name == "bullet") {
        // ���S�A�j���[�V������ݒ�.
        a.SetAnimation(GameData::Get().anmZombieMaleDown);
        // �Փ˔���𖳂���.
        a.collision.shape = Collision::Shape::none;
        // �|�����]���r�̐���1�̑��₷.
        ++GameData::Get().killCount;
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
    if (gamedata.keyPressedInLastFrame & GameData::Key::enter) {
      SceneManager::Get().ChangeScene(TITLE_SCENE_NAME);
    }
    return;
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

  if (glm::length(direction) > 0) {
    playerActor->rotation.y = std::atan2(-direction.z, direction.x);
    const float speed = 4.0f;
    playerActor->velocity = glm::normalize(direction) * speed;
    playerActor->SetAnimation(GameData::Get().anmPlayerRun);
  } else {
    playerActor->velocity = glm::vec3(0);
    playerActor->SetAnimation(GameData::Get().anmPlayerIdle);
  }

  static bool prevShootKey = false;
  const bool shootKey = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
  const bool shoot = shootKey & !prevShootKey;
  prevShootKey = shootKey;
  if (shoot) {
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
}

/**
* ���C���Q�[����ʂ��X�V����.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
* @param deltaTime �O��̍X�V����̌o�ߎ���.
*/
void MainGameScene::Update(GLFWwindow* window, float deltaTime)
{
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
      if (DetectCollision(a, b)) {
        // �Փ˂��Ă�����A�o����OnHit�֐������s����.
        a.OnHit(a, b);
        b.OnHit(b, a);
      }
    } // �����ʂ̐��ɒ���.
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
}

/**
* ���C���Q�[����ʂ�`�悷��.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
*/
void MainGameScene::Render(GLFWwindow* window) const
{
  GameData& global = GameData::Get();
  std::shared_ptr<Shader::Pipeline> pipeline = global.pipeline;
  Mesh::PrimitiveBuffer& primitiveBuffer = global.primitiveBuffer;
  Texture::Sampler& sampler = global.sampler;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  //glEnable(GL_FRAMEBUFFER_SRGB);
  glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ������ݒ肷��.
  pipeline->SetAmbientLight(glm::vec3(0.1f, 0.125f, 0.15f));

  // ���s������ݒ肷��
  const Shader::DirectionalLight directionalLight{
    glm::normalize(glm::vec4(3,-2,-2, 0)),
    glm::vec4(1, 0.9f, 0.8f, 1)
  };
  pipeline->SetLight(directionalLight);

  pipeline->SetLight(pointLight);

  const glm::vec3 viewPosition = playerActor->position + glm::vec3(0, 7, 7);
  const glm::vec3 viewTarget = playerActor->position + glm::vec3(0, 0, 0);

  // ���W�ϊ��s����쐬.
  int w, h;
  glfwGetWindowSize(window, &w, &h);
  const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
  const glm::mat4 matProj =
    glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f);
  const glm::mat4 matView =
    glm::lookAt(viewPosition, viewTarget, glm::vec3(0, 1, 0));

  primitiveBuffer.BindVertexArray();
  pipeline->Bind();
  sampler.Bind(0);

  // �n�ʂ�`��.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texGround->Bind(0);
    primitiveBuffer.Get(GameData::PrimNo::ground).Draw();
  }

  // �e�`��p�̍s����쐬.
  // X��Z�𕽍s�����̕����Ɉ����L�΂��AY��0�ɂ���.
  // �n�ʂƖ�������Ƃ�����̂ŏ�����������.
  glm::mat4 matShadow(1);
  matShadow[1][0] = directionalLight.direction.x;
  matShadow[1][1] = 0;
  matShadow[1][2] = directionalLight.direction.z;
  matShadow[3][3] = 1;
  matShadow[3][1] = 0.01f;

  // �A�N�^�[���X�g��`��.
  RenderActorList(actors, matProj * matView, matShadow);

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

  // 2D�\��.
  {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    // ���W�ϊ��s����쐬.
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    const float halfW = w / 2.0f;
    const float halfH = h / 2.0f;
    const glm::mat4 matProj =
      glm::ortho<float>(-halfW, halfW, -halfH, halfH, 1.0f, 500.0f);
    const glm::mat4 matView =
      glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0), glm::vec3(0, 1, 0));
    const glm::mat4 matVP = matProj * matView;

    std::shared_ptr<Shader::Pipeline> pipeline2D = GameData::Get().pipelineSimple;

    pipeline2D->Bind();

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
  std::cout << "[���] MainGameScene���I��.\n";
}

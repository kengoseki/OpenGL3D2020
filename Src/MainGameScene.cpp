/**
* @file MainGameScene.cpp
*
* ����21���I�B�u�i�`�X2.0�v�̉Ȋw�����u�Q�����g�E�x���R�t�v(47)�́A�l�Ԃ��]���r�������p�u�f�X�E�X�g���N�`�������O�v�����p���B
* ���E�����̖�]�̎�n�߂ɁA�Ƃ��郈�[���b�p�̕�翂ȕГc�ɂ��]���r�܂݂�ɂ��悤�Ƃ��Ă����B
*
* SAS�\�����ŃT�o�C�o���E�C���X�g���N�^�[�́u���T�E�G���t�B�[���h�v(26)�́A���܂��܉ċG�x�ɂł��̊�݂Ɋ������܂�Ă��܂��B
* �����āA���낤���ē������񂾒��̋���ŁA�u�A���\�j�[�E�E�F�X�g�v�q�t��������ׂ�������������ꂽ�̂������B
* �u���Ȃ��͐_�����킳�ꂽ��m�Ȃ̂ł��B���́u�j�����ꂽAK-47�v�Ɓu���Ȃ��֒e�v�������Ȃ����B
* �����Đ_�̉h�������܂˂������ɂ��낵�߂��̂ł��B�v
* ���T�͋����Ȃ�����u��烂̎s�������̂��R�l�̖��߁B�v�Ƃ����㊯�u�G�h�K�[�E���b�Z���v(35)�̋����ɏ]�����Ƃɂ���B
* �i�`�X2.0�̖�]�����������߁A�r��ʂĂ����ƃ]���r�̑��H���[���X�𔲂���A���T�̉ߍ��Ȑ킢���n�܂����I
*/
#include "MainGameScene.h"
#include "Global.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/**
* ���؂�`�悷��.
*/
void DrawLineOfTrees(const Mesh::Primitive& prim, Shader::Pipeline& pipeline, const glm::mat4& matVP, const glm::vec3& start, const glm::vec3& direction)
{
  glm::vec3 offset = start;
  for (float i = 0; i < 19; ++i) {
    const glm::vec3 offset = start + direction * i;
    const glm::mat4 matModelT = glm::translate(glm::mat4(1), offset);
    const glm::mat4 matModelR = glm::rotate(glm::mat4(1), glm::radians(i * 30), glm::vec3(0, 1, 0));
    const glm::mat4 matModel = matModelT * matModelR;
    const glm::mat4 matMVP = matVP * matModel;
    pipeline.SetMVP(matMVP);
    pipeline.SetModelMatrix(matModel);
    prim.Draw();
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

  Global& global = Global::Get();

  std::random_device rd;
  std::mt19937 random(rd());

  // �v���C���[��\��.
  {
    std::vector<const Mesh::Primitive*> animation;
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_0));
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_1));
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_2));
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_3));
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_4));
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_5));
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_6));
    animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_7));
    playerActor = std::make_shared<Actor>("player", &global.primitiveBuffer.Get(Global::PrimNo::player_idle_0),
      texPlayer, glm::vec3(10, 0, 10));
    playerActor->animation = animation;
    playerActor->animationInterval = 0.1f;
    actors.push_back(playerActor);
  }

  // �]���r��\��.
  const Mesh::Primitive* pPrimitive = &global.primitiveBuffer.Get(Global::PrimNo::zombie_male_walk_0);
  std::vector<const Mesh::Primitive*> animation;
  animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::zombie_male_walk_0));
  animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::zombie_male_walk_1));
  animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::zombie_male_walk_2));
  animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::zombie_male_walk_3));
  animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::zombie_male_walk_4));
  animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::zombie_male_walk_5));
  for (size_t i = 0; i < 100; ++i) {
    glm::vec3 pos(0);
    pos.x = std::uniform_real_distribution<float>(-18, 18)(global.random);
    pos.z = std::uniform_real_distribution<float>(-18, 18)(global.random);
    std::shared_ptr<Actor> actor = std::make_shared<Actor>("zombie", pPrimitive, texZombie, pos);
    actor->rotation.y =
      std::uniform_real_distribution<float>(0, glm::radians(360.0f))(global.random);
    // �A�j���[�V������ݒ�.
    actor->animation = animation;
    actor->animationInterval = 0.2f;
    actors.push_back(actor);
  }

  // �_������ݒ肷��
  pointLight = Shader::PointLight{
    glm::vec4(8, 10,-8, 0),
    glm::vec4(0.4f, 0.7f, 1.0f, 0) * 200.0f
  };

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

  Global& global = Global::Get();

  if (glm::length(direction) > 0) {
    playerActor->rotation.y = std::atan2(-direction.z, direction.x);
    const float speed = 4.0f;
    playerActor->velocity = glm::normalize(direction) * speed;

    if (playerState != ActionId::run) {
      std::vector<const Mesh::Primitive*> animation;
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_0));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_1));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_2));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_3));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_4));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_5));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_6));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_run_7));
      playerActor->animation = animation;
      playerActor->animationInterval = 0.1f;
      playerActor->animationTimer = 0.1f;
      playerState = ActionId::run;
    }
  } else {
    playerActor->velocity = glm::vec3(0);

    if (playerState != ActionId::idle) {
      std::vector<const Mesh::Primitive*> animation;
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_0));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_1));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_2));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_3));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_4));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_3));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_2));
      animation.push_back(&global.primitiveBuffer.Get(Global::PrimNo::player_idle_1));
      playerActor->animation = animation;
      playerActor->animationInterval = 0.2f;
      playerActor->animationTimer = 0.2f;
      playerState = ActionId::idle;
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
  UpdateActorList(actors, deltaTime);
}

/**
* ���C���Q�[����ʂ�`�悷��.
*
* @param window    GLFW�E�B���h�E�ւ̃|�C���^.
*/
void MainGameScene::Render(GLFWwindow* window) const
{
  Global& global = Global::Get();
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

  // �؂�`��.
  texTree->Bind(0);
  for (float j = 0; j < 4; ++j) {
    const glm::mat4 matRot = glm::rotate(glm::mat4(1), glm::radians(90.0f) * j, glm::vec3(0, 1, 0));
    DrawLineOfTrees(primitiveBuffer.Get(1), *pipeline, matProj * matView, matRot * glm::vec4(-19, 0, 19, 1), matRot * glm::vec4(2, 0, 0, 1));
  }

  // �n�ʂ�`��.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texGround->Bind(0);
    primitiveBuffer.Get(0).Draw();
  }

  // �Ƃ�`��.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texHouse->Bind(0);
    primitiveBuffer.Get(2).Draw();
  }

  // �����̂�`��.
  {
    const glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(10, 1, 0));
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texCube->Bind(0);
    primitiveBuffer.Get(3).Draw();
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
    primitiveBuffer.Get(1).Draw();
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

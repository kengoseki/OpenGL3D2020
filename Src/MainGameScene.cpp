/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "Global.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/**
* 並木を描画する.
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
* メインゲーム画面を初期化する.
*
* @retval true  初期化成功.
* @retval false 初期化失敗.
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

  Global& global = Global::Get();

  std::random_device rd;
  std::mt19937 random(rd());

  // プレイヤーを表示.
  playerActor = std::make_shared<Actor>("player", &global.primitiveBuffer.Get(Global::PrimNo::player_stand),
    texPlayer, glm::vec3(10, 0, 10));
  actors.push_back(playerActor);

  // ゾンビを表示.
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
    // アニメーションを設定.
    actor->animation = animation;
    actor->animationInterval = 0.2f;
    actors.push_back(actor);
  }

  // 点光源を設定する
  pointLight = Shader::PointLight{
    glm::vec4(8, 10,-8, 0),
    glm::vec4(0.4f, 0.7f, 1.0f, 0) * 200.0f
  };

  std::cout << "[情報] MainGameSceneを開始.\n";
  return true;
}

/**
* メインゲーム画面のキー入力を処理する.
*
* @param window    GLFWウィンドウへのポインタ.
*/
void MainGameScene::ProcessInput(GLFWwindow* window)
{
  // プレイヤーアクターを移動させる.
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
  if (glm::length(direction) > 0) {
    playerActor->rotation.y = std::atan2(-direction.z, direction.x);
    const float speed = 4.0f;
    playerActor->velocity = glm::normalize(direction) * speed;
  } else {
    playerActor->velocity = glm::vec3(0);
  }
}

/**
* メインゲーム画面を更新する.
*
* @param window    GLFWウィンドウへのポインタ.
* @param deltaTime 前回の更新からの経過時間.
*/
void MainGameScene::Update(GLFWwindow* window, float deltaTime)
{
  UpdateActorList(actors, deltaTime);
}

/**
* メインゲーム画面を描画する.
*
* @param window    GLFWウィンドウへのポインタ.
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

  // 環境光を設定する.
  pipeline->SetAmbientLight(glm::vec3(0.1f, 0.125f, 0.15f));

  // 平行光源を設定する
  const Shader::DirectionalLight directionalLight{
    glm::normalize(glm::vec4(3,-2, 2, 0)),
    glm::vec4(1, 0.9f, 0.8f, 1)
  };
  pipeline->SetLight(directionalLight);

  pipeline->SetLight(pointLight);

  const glm::vec3 viewPosition = playerActor->position + glm::vec3(2, 3, 3);
  const glm::vec3 viewTarget = playerActor->position + glm::vec3(0, 1, 0);

  // 座標変換行列を作成.
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

  // 木を描画.
  texTree->Bind(0);
  for (float j = 0; j < 4; ++j) {
    const glm::mat4 matRot = glm::rotate(glm::mat4(1), glm::radians(90.0f) * j, glm::vec3(0, 1, 0));
    DrawLineOfTrees(primitiveBuffer.Get(1), *pipeline, matProj * matView, matRot * glm::vec4(-19, 0, 19, 1), matRot * glm::vec4(2, 0, 0, 1));
  }

  // 地面を描画.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texGround->Bind(0);
    primitiveBuffer.Get(0).Draw();
  }

  // 家を描画.
  {
    const glm::mat4 matModel = glm::mat4(1);
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texHouse->Bind(0);
    primitiveBuffer.Get(2).Draw();
  }

  // 立方体を描画.
  {
    const glm::mat4 matModel = glm::translate(glm::mat4(1), glm::vec3(10, 1, 0));
    const glm::mat4 matMVP = matProj * matView * matModel;
    pipeline->SetMVP(matMVP);
    pipeline->SetModelMatrix(matModel);
    texCube->Bind(0);
    primitiveBuffer.Get(3).Draw();
  }

  // 影描画用の行列を作成.
  // XとZを平行光源の方向に引き伸ばし、Yを0にする.
  // 地面と密着するとちらつくので少し浮かせる.
  glm::mat4 matShadow(1);
  matShadow[1][0] = directionalLight.direction.x;
  matShadow[1][1] = 0;
  matShadow[1][2] = directionalLight.direction.z;
  matShadow[3][3] = 1;
  matShadow[3][1] = 0.01f;

  // アクターリストを描画.
  RenderActorList(actors, matProj * matView, matShadow);

  // 点光源の位置を描画.
  {
    // Y軸回転.
    const float degree = static_cast<float>(std::fmod(glfwGetTime() * 180.0, 360.0));
    const glm::mat4 matModelR =
      glm::rotate(glm::mat4(1), glm::radians(degree), glm::vec3(0, 1, 0));
    // 拡大縮小.
    const glm::mat4 matModelS =
      glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.25f, 0.5f));
    // 平行移動.
    const glm::mat4 matModelT =
      glm::translate(glm::mat4(1), glm::vec3(pointLight.position) + glm::vec3(0, -1.25f, 0));
    // 拡大縮小・回転・平行移動を合成.
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
* メインゲーム画面を終了する.
*/
void MainGameScene::Finalize()
{
  std::cout << "[情報] MainGameSceneを終了.\n";
}

/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Actor.h"
#include "Actors/PlayerActor.h"
#include <memory>

/**
* ���C���Q�[�����.
*/
class MainGameScene
{
public:
  MainGameScene() = default;
  ~MainGameScene() { Finalize(); }
  MainGameScene(const MainGameScene&) = delete;
  MainGameScene& operator=(const MainGameScene&) = delete;

  bool Initialize();
  void ProcessInput(GLFWwindow*);
  void Update(GLFWwindow*, float deltaTime);
  void Render(GLFWwindow*) const;
  void Finalize();

  void AddActor(ActorPtr p) { newActors.push_back(p); }
  ActorPtr GetPlayerActor() { return playerActor; }
  const glm::mat4& GetViewMatrix() const { return matView; }
  const glm::mat4& GetProjectionMatrix() const { return matProj; }
  const glm::vec3& GetMouseCursor() const { return posMouseCursor; }

private:
  void AddLineOfTrees(const glm::vec3& start, const glm::vec3& direction);

  std::shared_ptr<Texture::Image2D> texGround = nullptr;
  std::shared_ptr<Texture::Image2D> texTree = nullptr;
  std::shared_ptr<Texture::Image2D> texHouse = nullptr;
  std::shared_ptr<Texture::Image2D> texCube = nullptr;
  std::shared_ptr<Texture::Image2D> texZombie;
  std::shared_ptr<Texture::Image2D> texPlayer;
  std::shared_ptr<Texture::Image2D> texGameClear;
  std::shared_ptr<Texture::Image2D> texGameOver;
  std::shared_ptr<Texture::Image2D> texBlack;
  std::shared_ptr<Texture::Image2D> texPointer;

  Shader::PointLight pointLight;

  glm::mat4 matProj = glm::mat4(1); // �v���W�F�N�V�����s��.
  glm::mat4 matView = glm::mat4(1); // �r���[�s��.

  // �}�E�X�J�[�\�����W.
  glm::vec3 posMouseCursor = glm::vec3(0);

  ActorList actors;
  std::shared_ptr<PlayerActor> playerActor;

  ActorList newActors;

  // �o��������G�̐�.
  size_t appearanceEnemyCount = 100;

  // �N���A�����𖞂��������ǂ���.
  bool isGameClear = false;
  bool isGameOver = false;
};

#endif // MAINGAMESCENE_H_INCLUDED

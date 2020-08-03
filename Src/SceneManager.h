/**
* @file SceneManager.h
*/
#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED
#include "glad/glad.h"
#include "TitleScene.h"
#include "MainGameScene.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

// �V�[����؂�ւ���Ƃ��Ɏw�肷��V�[����.
#define TITLE_SCENE_NAME "TitleScene"
#define MAINGAME_SCENE_NAME "MainGameScene"

/**
* �V�[���Ǘ��N���X.
*/
class SceneManager
{
public:
  static SceneManager& Get();

  void Update(GLFWwindow* window, float deltaTime);
  void Render(GLFWwindow* window) const;
  void ChangeScene(const std::string&);
  void Finalize();

private:
  // �N���X�ϐ������Ȃ����邽�߁A�R���X�g���N�^��f�X�g���N�^��private�ɂ���.
  SceneManager() = default;
  ~SceneManager() = default;
  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  std::string currentSceneName; // ���s���̃V�[����.
  std::string nextSceneName;    // ���Ɏ��s����V�[����.

  std::shared_ptr<TitleScene> titleScene;
  std::shared_ptr<MainGameScene> mainGameScene;
};


#endif // SCENEMANAGER_H_INCLUDED

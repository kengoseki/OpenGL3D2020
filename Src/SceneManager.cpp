/**
* @file SceneManager.cpp
*/
#include "SceneManager.h"

/**
* �V�[���}�l�[�W���̃V���O���g���E�C���X�^���X���擾����.
*/
SceneManager& SceneManager::Get()
{
  static SceneManager singleton;
  return singleton;
}

/**
* ���݋N�����Ă���V�[�����X�V����.
*
* @param window    GL�R���e�L�X�g���Ǘ�����GLFW�E�B���h�E.
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b).
*/
void SceneManager::Update(GLFWwindow* window, float deltaTime)
{
  // ���̃V�[�����w�肳��Ă�����A�V�[����؂�ւ���.
  if (!nextSceneName.empty()) {
    // ���s���̃V�[�����I������.
    titleScene = nullptr;
    mainGameScene = nullptr;

    // �w�肳�ꂽ���O�ɑΉ�����V�[�����쐬���ď���������.
    if (nextSceneName == TITLE_SCENE_NAME) {
      titleScene = std::make_shared<TitleScene>();
      titleScene->Initialize();
    } else if (nextSceneName == MAINGAME_SCENE_NAME) {
      mainGameScene = std::make_shared<MainGameScene>();
      mainGameScene->Initialize();
    }

    // ���s���̃V�[������ύX����.
    currentSceneName = nextSceneName;
    nextSceneName.clear();
  }

  // ���s���̃V�[�����X�V����.
  if (titleScene) {
    titleScene->ProcessInput(window);
    titleScene->Update(window, deltaTime);
  }
  if (mainGameScene) {
    mainGameScene->ProcessInput(window);
    mainGameScene->Update(window, deltaTime);
  }
}

/**
* ���s���̃V�[����`�悷��.
*
* @param window    GL�R���e�L�X�g���Ǘ�����GLFW�E�B���h�E.
*/
void SceneManager::Render(GLFWwindow* window) const
{
  if (titleScene) {
    titleScene->Render(window);
  }else if (mainGameScene) {
    mainGameScene->Render(window);
  }
}

/**
* �V�[����؂�ւ���.
*
* @param scenaName ���ɋN������V�[���̖��O.
*/
void SceneManager::ChangeScene(const std::string& sceneName)
{
  // ���s���̃V�[���Ɠ������O���w�肳��Ă����牽�����Ȃ��ŏI��.
  if (sceneName == currentSceneName) {
    return;
  }
  nextSceneName = sceneName;
}

/**
* �V�[���}�l�[�W�����I������.
*/
void SceneManager::Finalize()
{
  titleScene = nullptr;
  mainGameScene = nullptr;
}


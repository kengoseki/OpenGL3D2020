/**
* @file GrenadeActor.h
*/
#ifndef GRENADEACTOR_H_INCLUDED
#define GRENADEACTOR_H_INCLUDED
#include "../Actor.h"

class MainGameScene;

/**
* ��֒e.
*/
class GrenadeActor : public Actor
{
public:
  GrenadeActor(const glm::vec3& pos, const glm::vec3& vel, float rotY, MainGameScene* pScene);
  virtual ~GrenadeActor() = default;

  virtual void OnUpdate(float) override;
  virtual void OnDestroy() override;

private:
  MainGameScene* pMainGameScene = nullptr;
};

#endif // GRENADEACTOR_H_INCLUDED


/**
* @file Global.cpp
*/
#include "Global.h"
#include <iostream>


Global* Global::p;

/**
*
*/
Global& Global::Get()
{
  return *p;
}

/**
* �O���[�o���f�[�^������������.
*
* @param window GLFW�E�B���h�E�ւ̃|�C���^.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool Global::Initialize(GLFWwindow* window)
{
  if (Global::p) {
    return true;
  }
  Global* p = new Global;// = std::make_shared<Derived>();

  // �v���~�e�B�u�o�b�t�@�Ƀ��f���f�[�^��ǂݍ���.
  if (!p->primitiveBuffer.Allocate(20'000, 80'000)) {
    return false;
  }
  p->primitiveBuffer.AddFromObjFile("Res/Ground.obj");
  p->primitiveBuffer.AddFromObjFile("Res/Tree.obj");
  p->primitiveBuffer.AddFromObjFile("Res/House.obj");
  p->primitiveBuffer.AddFromObjFile("Res/Cube.obj");
  p->primitiveBuffer.AddFromObjFile("Res/Plane.obj");

  // �p�C�v���C���E�I�u�W�F�N�g���쐬����.
  p->pipeline = std::make_shared<Shader::Pipeline>("Res/FragmentLighting.vert", "Res/FragmentLighting.frag");
  if (!p->pipeline || !*p->pipeline) {
    return false;
  }

  // �T���v���E�I�u�W�F�N�g���쐬����.
  p->sampler.SetWrapMode(GL_REPEAT);
  p->sampler.SetFilter(GL_NEAREST);

  p->window = window;
  Global::p = p;

  std::cout << "[���] �O���[�o���f�[�^��������.\n";

  return true;
}

/**
* �O���[�o���f�[�^��j������.
*/
void Global::Finalize()
{
  p = nullptr;
  std::cout << "[���] �O���[�o���f�[�^��j��.\n";
}

/**
* �v���~�e�B�u��`�悷��.
*
* @param id �v���~�e�B�u��ID.
*/
void Global::Draw(Global::PrimitiveId id) const
{
  primitiveBuffer.Get(static_cast<size_t>(id)).Draw();
}




/**
* @file Shader.cpp
*/
#include "Shader.h"
#include "GLContext.h"

/**
* �V�F�[�_�Ɋւ���@�\���i�[���閼�O���.
*/
namespace Shader {

/**
* �R���X�g���N�^.
*
* @param vsCode  ���_�V�F�[�_�[�E�v���O�����̃A�h���X.
* @param fsCode  �t���O�����g�V�F�[�_�[�E�v���O�����̃A�h���X.
*/
Pipeline::Pipeline(const char* vsCode, const char* fsCode)
{
  vp = GLContext::CreateProgram(GL_VERTEX_SHADER, vsCode);
  fp = GLContext::CreateProgram(GL_FRAGMENT_SHADER, fsCode);
  id = GLContext::CreatePipeline(vp, fp);
}

/**
* �f�X�g���N�^.
*/
Pipeline::~Pipeline()
{
  glDeleteProgramPipelines(1, &id);
  glDeleteProgram(fp);
  glDeleteProgram(vp);
}

/**
* �v���O�����p�C�v���C�����o�C���h����.
*/
void Pipeline::Bind() const
{
  glBindProgramPipeline(id);
}

/**
* �v���O�����p�C�v���C���̃o�C���h����������.
*/
void Pipeline::Unbind() const
{
  glBindProgramPipeline(0);
}

/**
* �V�F�[�_��MVP�s���ݒ肷��.
*/
void Pipeline::SetMVP(const glm::mat4& matMVP)
{
  const GLint locMatMVP = 0;
  glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
}

/**
* �v���O�����p�C�v���C���̃o�C���h����������.
*/
void UnbindPipeline()
{
  glBindProgramPipeline(0);
}

} // namespace Shader


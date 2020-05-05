/**
* @file Shader.cpp
*/
#include "Shader.h"
#include "GLContext.h"
#include <iostream>

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
Pipeline::Pipeline(const char* vsFilename, const char* fsFilename)
{
  vp = GLContext::CreateProgramFromFile(GL_VERTEX_SHADER, vsFilename);
  fp = GLContext::CreateProgramFromFile(GL_FRAGMENT_SHADER, fsFilename);
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
*
* @retval true  �ݒ萬��.
* @retval false �ݒ莸�s.
*/
bool Pipeline::SetMVP(const glm::mat4& matMVP) const
{
  glGetError(); // �G���[��Ԃ����Z�b�g.

  const GLint locMatMVP = 0;
  glProgramUniformMatrix4fv(vp, locMatMVP, 1, GL_FALSE, &matMVP[0][0]);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "[�G���[]" << __func__ << ":MVP�s��̐ݒ�Ɏ��s.\n";
    return false;
  }
  return true;
}

/**
* �v���O�����p�C�v���C���̃o�C���h����������.
*/
void UnbindPipeline()
{
  glBindProgramPipeline(0);
}

} // namespace Shader


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
* @param matMVP �ݒ肷��MVP�s��.
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
* �V�F�[�_�ɖ@���s���ݒ肷��.
*
* @param matNormal �ݒ肷��@���s��.
*
* @retval true  �ݒ萬��.
* @retval false �ݒ莸�s.
*/
bool Pipeline::SetNormalMatrix(const glm::mat3& matNormal) const
{
  glGetError(); // �G���[��Ԃ����Z�b�g.

  const GLint locMatNormal = 1;
  glProgramUniformMatrix3fv(vp, locMatNormal, 1, GL_FALSE, &matNormal[0][0]);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "[�G���[]" << __func__ << ":�@���s��̐ݒ�Ɏ��s.\n";
    return false;
  }
  return true;
}

/**
* �V�F�[�_�Ƀ��C�g�f�[�^��ݒ肷��.
*
* @param light �ݒ肷�郉�C�g�f�[�^.
*
* @retval true  �ݒ萬��.
* @retval false �ݒ莸�s.
*/
bool Pipeline::SetLight(const DirectionalLight& light) const
{
  glGetError(); // �G���[��Ԃ����Z�b�g.

  const GLint locDirLight = 2;
  glProgramUniform4fv(vp, locDirLight, 1, &light.direction.x);
  glProgramUniform4fv(vp, locDirLight + 1, 1, &light.color.x);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "[�G���[]" << __func__ << ":���s�����̐ݒ�Ɏ��s.\n";
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


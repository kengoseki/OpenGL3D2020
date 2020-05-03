/**
* @file Texture.cpp
*/
#include "Texture.h"
#include "GLContext.h"
#include <iostream>

namespace /* unnamed */ {

/**
* �e�N�X�`���̃o�C���h��Ԃ�ǐՂ��邽�߂̔z��.
*
* �e�N�X�`���C���[�W���j�b�g�Ƀo�C���h���ꂽ�e�N�X�`��ID��ێ�����.
*/
GLuint textureBindingState[16] = {};

} // unnamed namespace

/**
* �S�Ẵe�N�X�`���̃o�C���h����������.
*/
void UnbindAllTextures()
{
  for (GLuint i = 0; i < std::size(textureBindingState); ++i) {
    glBindTextureUnit(i, 0);
    textureBindingState[i] = 0;
  }
}

/**
* �R���X�g���N�^.
*
* @param filename 2D�e�N�X�`���Ƃ��ēǂݍ��ރt�@�C����.
*/
Image2D::Image2D(const char* filename) : id(GLContext::CreateImage2D(filename))
{
  if (id) {
    glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_WIDTH, &width);
    glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_HEIGHT, &height);
  }
}

/**
* �R���X�g���N�^.
*
* @param width   �摜�̕�(�s�N�Z����).
* @param height  �摜�̍���(�s�N�Z����).
* @param data    �摜�f�[�^�ւ̃|�C���^.
* @param pixelFormat  �摜�̃s�N�Z���`��(GL_BGRA�Ȃ�).
* @param type    �摜�f�[�^�̌^.
*/
Image2D::Image2D(GLsizei width, GLsizei height, const void* data, GLenum format, GLenum type) :
  id(GLContext::CreateImage2D(width, height, data, format, type))
{
  if (id) {
    glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_WIDTH, &width);
    glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_HEIGHT, &height);
  }
}

/**
* �f�X�g���N�^.
*/
Image2D::~Image2D()
{
  Unbind();
  glDeleteTextures(1, &id);
}

/**
* �e�N�X�`�����e�N�X�`���C���[�W���j�b�g�Ƀo�C���h����.
*
* @param unit �o�C���h��̃��j�b�g�ԍ�.
*/
void Image2D::Bind(GLuint unit) const
{
  if (unit >= std::size(textureBindingState)) {
    std::cerr << "[�G���[]" << __func__ << ": ���j�b�g�ԍ����傫�����܂�(unit=" << unit << ")\n";
    return;
  }
  glBindTextureUnit(unit, id);
  textureBindingState[unit] = id;
}

/**
* �e�N�X�`���̃o�C���h����������.
*/
void Image2D::Unbind() const
{
  for (GLuint i = 0; i < std::size(textureBindingState); ++i) {
    if (textureBindingState[i] == id) {
      textureBindingState[i] = 0;
      glBindTextureUnit(i, 0);
    }
  }
}

/**
* �e�N�X�`���̕����擾����.
*
* @return �e�N�X�`���̕�(�s�N�Z����).
*/
GLsizei Image2D::Width() const
{
  return width;
}

/**
* �e�N�X�`���̍������擾����.
*
* @return �e�N�X�`���̍���(�s�N�Z����).
*/
GLsizei Image2D::Height() const
{
  return height;
}

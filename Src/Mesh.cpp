/**
* @file Mesh.cpp
*/
#include "Mesh.h"
#include "GLContext.h"
#include <glm/glm.hpp>
#include <iostream>

/**
* �}�`�f�[�^�Ɋւ��閼�O���.
*/
namespace Mesh {

/**
* �f�[�^���o�b�t�@�I�u�W�F�N�g�ɃR�s�[����.
*
* @param id          �R�s�[��ƂȂ�o�b�t�@�I�u�W�F�N�gID.
* @param unitSize    �v�f�̃o�C�g��.
* @param offsetCount �R�s�[��I�t�Z�b�g(�v�f�P��).
* @param count       �R�s�[����v�f��.
* @param data        �R�s�[����f�[�^�̃A�h���X.
*
* @retval true  �R�s�[����.
* @retval false �R�s�[���s.
*/
bool CopyData(GLuint id, size_t unitSize, GLsizei offsetCount, size_t count, const void* data)
{
  const GLsizei size = static_cast<GLsizei>(count * unitSize);
  const GLuint tmp = GLContext::CreateBuffer(size, data);
  if (!tmp) {
    std::cerr << "[�G���[]" << __func__ << ": �R�s�[���o�b�t�@�̍쐬�Ɏ��s(size=" << size << ").\n";
    return false;
  }
  const GLsizei offset = static_cast<GLsizei>(offsetCount * unitSize);
  glCopyNamedBufferSubData(tmp, id, 0, offset, size);
  glDeleteBuffers(1, &tmp);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "[�G���[]" << __func__ << ": �f�[�^�̃R�s�[�Ɏ��s(size=" << size << ", offset=" << offset << ").\n";
  }
  return true;
}

/**
* �v���~�e�B�u��`�悷��.
*/
void Primitive::Draw() const
{
  glDrawElementsBaseVertex(mode, count, GL_UNSIGNED_SHORT, indices, baseVertex);
}

/**
* �f�X�g���N�^.
*/
PrimitiveBuffer::~PrimitiveBuffer()
{
  Free();
}

/**
* �v���~�e�B�u�p�̃��������m�ۂ���.
*
* @param maxVertexCount  �i�[�\�ȍő咸�_��.
* @param maxIndexCount   �i�[�\�ȍő�C���f�b�N�X��.
*
* @retval true  �m�ې���.
* @retval false �m�ێ��s�A�܂��͊��Ɋm�ۍς�.
*/
bool PrimitiveBuffer::Allocate(GLsizei maxVertexCount, GLsizei maxIndexCount)
{
  if (vao) {
    std::cerr << "[�x��]" << __func__ << ": VAO�͍쐬�ς݂ł�.\n";
    return false;
  }
  vboPosition = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
  vboColor = GLContext::CreateBuffer(sizeof(glm::vec4) * maxVertexCount, nullptr);
  vboTexcoord = GLContext::CreateBuffer(sizeof(glm::vec2) * maxVertexCount, nullptr);
  vboNormal = GLContext::CreateBuffer(sizeof(glm::vec3) * maxVertexCount, nullptr);
  ibo = GLContext::CreateBuffer(sizeof(GLushort) * maxIndexCount, nullptr);
  vao = GLContext::CreateVertexArray(vboPosition, vboColor, vboTexcoord, vboNormal, ibo);
  if (!vboPosition || !vboColor || !vboTexcoord || !vboNormal || !ibo || !vao) {
    std::cerr << "[�G���[]" << __func__ << ": VAO�̍쐬�Ɏ��s.\n";
    Free();
    return false;
  }
  primitives.reserve(100);
  this->maxVertexCount = maxVertexCount;
  this->maxIndexCount = maxIndexCount;
  return true;
}

/**
* �v���~�e�B�u�p�̃��������J������.
*/
void PrimitiveBuffer::Free()
{
  primitives.clear();

  glDeleteVertexArrays(1, &vao);
  vao = 0;
  glDeleteBuffers(1, &ibo);
  ibo = 0;
  glDeleteBuffers(1, &vboTexcoord);
  vboTexcoord = 0;
  glDeleteBuffers(1, &vboColor);
  vboColor = 0;
  glDeleteBuffers(1, &vboPosition);
  vboPosition = 0;

  maxVertexCount = 0;
  curVertexCount = 0;
  maxIndexCount = 0;
  curIndexCount = 0;
}

/**
* �v���~�e�B�u��ǉ�����.
*
* @param vertexCount �ǉ����钸�_�f�[�^�̐�.
* @param pPosition   ���W�f�[�^�ւ̃|�C���^.
* @param pColor      �F�f�[�^�ւ̃|�C���^.
* @param pTexcoord   �e�N�X�`�����W�f�[�^�ւ̃|�C���^.
* @param pNormal     �@���f�[�^�ւ̃|�C���^.
* @param indexCount  �ǉ�����C���f�b�N�X�f�[�^�̐�.
* @param pIndex      �C���f�b�N�X�f�[�^�ւ̃|�C���^.
*
* @retval true  �ǉ�����.
* @retval false �ǉ����s.
*/
bool PrimitiveBuffer::Add(size_t vertexCount, const glm::vec3* pPosition,
  const glm::vec4* pColor, const glm::vec2* pTexcoord, const glm::vec3* pNormal, size_t indexCount, const GLushort* pIndex)
{
  if (!vao) {
    std::cerr << "[�G���[]" << __func__ << ": VAO���쐬����Ă��܂���.\n";
    return false;
  } else if (maxVertexCount < curVertexCount) {
    std::cerr << "[�G���[]" << __func__ << ": ���_�J�E���g�Ɉُ킪����܂�(max=" << maxVertexCount << ", cur=" << curVertexCount << ")\n";
    return false;
  } else if (maxIndexCount < curIndexCount) {
    std::cerr << "[�G���[]" << __func__ << ": �C���f�b�N�X�J�E���g�Ɉُ킪����܂�(max=" << maxIndexCount << ", cur=" << curIndexCount << ")\n";
    return false;
  } else if (vertexCount > static_cast<size_t>(maxVertexCount) - curVertexCount) {
    std::cerr << "[�x��]" << __func__ << ": VBO�����t�ł�(max=" << maxVertexCount <<
      ", cur=" << curVertexCount << ", add=" << vertexCount << ")\n";
    return false;
  } else if (indexCount > static_cast<size_t>(maxIndexCount) - curIndexCount) {
    std::cerr << "[�x��]" << __func__ << ": IBO�����t�ł�(max=" << maxIndexCount <<
      ", cur=" << curIndexCount << ", add=" << indexCount << ")\n";
    return false;
  }

  if (!CopyData(vboPosition, sizeof(glm::vec3), curVertexCount, vertexCount, pPosition)) {
    return false;
  }
  if (!CopyData(vboColor, sizeof(glm::vec4), curVertexCount, vertexCount, pColor)) {
    return false;
  }
  if (!CopyData(vboTexcoord, sizeof(glm::vec2), curVertexCount, vertexCount, pTexcoord)) {
    return false;
  }
  if (!CopyData(vboNormal, sizeof(glm::vec3), curVertexCount, vertexCount, pNormal)) {
    return false;
  }
  if (!CopyData(ibo, sizeof(GLushort), curIndexCount, indexCount, pIndex)) {
    return false;
  }

  primitives.push_back(Primitive(GL_TRIANGLES, static_cast<GLsizei>(indexCount),
    sizeof(GLushort) * curIndexCount, curVertexCount));

  curVertexCount += static_cast<GLsizei>(vertexCount);
  curIndexCount += static_cast<GLsizei>(indexCount);

  return true;
}

/**
* �v���~�e�B�u���擾����.
*
* @param n �v���~�e�B�u�̃C���f�b�N�X.
*
* @return n�ɑΉ�����v���~�e�B�u.
*/
const Primitive& PrimitiveBuffer::Get(size_t n) const
{
  if (n < 0 || n > static_cast<int>(primitives.size())) {
    std::cerr << "[�x��]" << __func__ << ":" << n <<
      "�͖����ȃC���f�b�N�X�ł�(�L���͈�0�`" << primitives.size() - 1 << ").\n";
    static const Primitive dummy;
    return dummy;
  }
  return primitives[n];
}

/**
* VAO���o�C���h����.
*/
void PrimitiveBuffer::BindVertexArray() const
{
  glBindVertexArray(vao);
}

/**
* VAO�̃o�C���h����������.
*/
void PrimitiveBuffer::UnbindVertexArray() const
{
  glBindVertexArray(0);
}

} // namespace Mesh


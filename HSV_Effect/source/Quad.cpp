#include "Quad.h"

Quad::Quad(const float *vertexData, GLuint verticesNumber)
  : vertices(vertexData, vertexData + verticesNumber), vao{}, vbo{}
{
  SetupMesh();
}

Quad::~Quad()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void Quad::SetupMesh()
{

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  const GLuint attributesPerVertex = PositionAndTextureAttributes;

  // position coords
  glEnableVertexAttribArray(PositionAttributeIndex);
  glVertexAttribPointer(PositionAttributeIndex,
    PositionComponentsNumber,
    GL_FLOAT,
    GL_FALSE,
    sizeof(float) * attributesPerVertex,
    reinterpret_cast<void *>(0));

  // texture coords
  glEnableVertexAttribArray(TextureCoordAttributeIndex);
  glVertexAttribPointer(TextureCoordAttributeIndex,
    TextureComponentsNumber,
    GL_FLOAT,
    GL_FALSE,
    sizeof(float) * attributesPerVertex,
    reinterpret_cast<void *>(PositionComponentsNumber * sizeof(float)));

  glBindVertexArray(0);
}

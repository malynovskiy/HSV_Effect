#pragma once

#include <glad/glad.h>
#include <vector>

namespace 
{
constexpr GLuint PositionComponentsNumber = 3;
constexpr GLuint TextureComponentsNumber = 2;

constexpr GLuint PositionAttributes = 3;
constexpr GLuint PositionAndTextureAttributes = 5;

constexpr GLuint QuadVerticesAmount = 6;

constexpr GLuint PositionAttributeIndex = 0;
constexpr GLuint TextureCoordAttributeIndex = 1;
}// namespace

class Quad
{
public:
  Quad() = default;
  Quad(const float *vertexData, GLuint verticesNumber);

  ~Quad();

private:
  void SetupMesh();

public:
  GLuint vao;
  GLuint vbo;

  std::vector<float> vertices;
};

constexpr float QuadVertices[QuadVerticesAmount * PositionAndTextureAttributes] = {// clang-format off
    // positions          // texCoords
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f, 
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f
};// clang-format on

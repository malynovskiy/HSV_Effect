#include "GLRenderer.h"
#include "Quad.h"
#include "Texture.h"

#include <stb_image.h>
#include <iostream>

namespace
{
// SHADERS
constexpr auto HSVEffectVertexShaderPath = "shaders/hsv_effect.vert";
constexpr auto HSVEffectFragmentShaderPath = "shaders/hsv_effect.frag";
constexpr auto PresentationVertexShaderPath = "shaders/presentation.vert";
constexpr auto PresentationFragmentShaderPath = "shaders/presentation.frag";

// TEXTURES
constexpr auto ImageTexturePath = "resources/image.jpg";

constexpr float HSVModifierStep = 0.1f;

inline constexpr float hsvClamp(const float modifier)
{
  return std::clamp(modifier, -1.0f, 1.0f);
}

}// namespace

GLRenderer::GLRenderer(u32 width, u32 height) : m_width{ width }, m_height{ height } {}

void GLRenderer::Initialize()
{
  CreateShaders();
  ConfigureShaders();

  CreateMeshes();
  LoadTextures();

  SetupFramebuffer();
}

void GLRenderer::Render()
{
  // Rendering source image with HSV parameters into m_hsvEffectColorBuffer
  //    it should render only first time and when HSV parameters changed.
  if (m_HSVModifiersDirty)
  {
    m_hsvEffectShader.use();
    m_hsvEffectShader.setUniform("hueMod", m_hueModifier);
    m_hsvEffectShader.setUniform("saturateMod", m_saturationModifier);
    m_hsvEffectShader.setUniform("valueMod", m_valueModifier);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hsvEffectFramebuffer);
    glViewport(0, 0, m_sourceTexture.width, m_sourceTexture.height);
    glBindVertexArray(m_quadMesh->vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sourceTexture.descriptor);
    glDrawArrays(GL_TRIANGLES, 0, QuadVerticesAmount);

    m_HSVModifiersDirty = false;
  }

  // Rendering m_hsvEffectColorBuffer content into default framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, m_width, m_height);

  ClearFramebuffer();
  m_presentationShader.use();
  glBindVertexArray(m_quadMesh->vao);
  glBindTexture(GL_TEXTURE_2D, m_hsvEffectColorBuffer);
  glDrawArrays(GL_TRIANGLES, 0, QuadVerticesAmount);
}

void GLRenderer::CreateShaders()
{
  m_hsvEffectShader = ShaderProgram(HSVEffectVertexShaderPath, HSVEffectFragmentShaderPath);
  m_presentationShader = ShaderProgram(HSVEffectVertexShaderPath, HSVEffectFragmentShaderPath);
}

void GLRenderer::ConfigureShaders()
{
  m_hsvEffectShader.use();
  m_hsvEffectShader.setUniform("imageTexture", 0);

  m_presentationShader.use();
  m_presentationShader.setUniform("screenTexture", 0);
}

void GLRenderer::CreateMeshes()
{
  m_quadMesh = std::make_unique<Quad>(QuadVertices, QuadVerticesAmount * PositionAndTextureAttributes);
}

void GLRenderer::LoadTextures()
{
  stbi_set_flip_vertically_on_load(true);
  m_sourceTexture = CreateTextureFromImage(ImageTexturePath);
}

void GLRenderer::SetupFramebuffer()
{
  // Background framebuffer configuration
  glGenFramebuffers(1, &m_hsvEffectFramebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_hsvEffectFramebuffer);

  glGenTextures(1, &m_hsvEffectColorBuffer);
  glBindTexture(GL_TEXTURE_2D, m_hsvEffectColorBuffer);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGB, m_sourceTexture.width, m_sourceTexture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // attach texture to framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hsvEffectColorBuffer, 0);

  W_CHECK(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cerr << "Error, framebuffer is not complete!\n";

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void GLRenderer::ClearFramebuffer() const
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::HSVIncrease(float &modifier)
{
  modifier += HSVModifierStep;
  modifier = hsvClamp(modifier);
  m_HSVModifiersDirty = true;
}

void GLRenderer::HSVDecrease(float &modifier)
{
  modifier -= HSVModifierStep;
  modifier = hsvClamp(modifier);
  m_HSVModifiersDirty = true;
}

void GLRenderer::OnKeyDown(u32 key)
{
  switch (key)
  {
  case '1': {
    HSVDecrease(m_hueModifier);
  }
  break;

  case '2': {
    HSVIncrease(m_hueModifier);
  }
  break;

  case '3': {
    HSVDecrease(m_saturationModifier);
  }
  break;

  case '4': {
    HSVIncrease(m_saturationModifier);
  }
  break;

  case '5': {
    HSVDecrease(m_valueModifier);
  }
  break;

  case '6': {
    HSVIncrease(m_valueModifier);
  }
  }
}

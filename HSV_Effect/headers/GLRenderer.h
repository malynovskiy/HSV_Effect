#pragma once

#include "ShaderProgram.h"
#include "Texture.h"
#include "Quad.h"

class GLRenderer
{
  using u32 = uint32_t;

public:
  GLRenderer(u32 width, u32 height);
  ~GLRenderer() = default;

  void Initialize();
  void Render();

  u32 GetWidth() const { return m_width; }
  u32 GetHeight() const { return m_height; }

  float GetHueModifier() const { return m_hueModifier; }
  float GetSaturateModifier() const { return m_saturationModifier; }
  float GetValueModifier() const { return m_valueModifier; }

private:
  void CreateShaders();
  void ConfigureShaders();

  void CreateMeshes();
  void LoadTextures();

  void SetupFramebuffer();
  
  inline void ClearFramebuffer() const;

  void HSVIncrease(float &modifier);
  void HSVDecrease(float &modifier);

public:
  void OnKeyDown(u32 key);

private:
  ShaderProgram m_hsvEffectShader;
  ShaderProgram m_presentationShader;

  u32 m_hsvEffectFramebuffer;
  u32 m_hsvEffectColorBuffer;

  std::unique_ptr<Quad> m_quadMesh;
  
  Texture m_sourceTexture;

  bool m_HSVModifiersDirty = true;
  float m_hueModifier = 0.0f;
  float m_saturationModifier = 0.0f;
  float m_valueModifier = 0.0f;

  u32 m_width;
  u32 m_height;
};

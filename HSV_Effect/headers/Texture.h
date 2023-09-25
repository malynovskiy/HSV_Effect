#pragma once

#include <Utility.h>

struct Texture
{
  uint32_t descriptor;
  uint32_t width;
  uint32_t height;
};

Texture CreateTextureFromImage(char const *path);

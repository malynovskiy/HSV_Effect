#include "Utility.h"

#include <Windows.h>
#include <glad/glad.h>

#include <fstream>
#include <iostream>

namespace Utility
{
std::string GetOpenGLContextInformation()
{
  std::string contextInfo = "";
  contextInfo += "OpenGL Context: " + std::to_string(GLVersion.major) + "," + std::to_string(GLVersion.minor) + "\n";
  contextInfo += "OpenGL version: " + std::string((const char *)glGetString(GL_VERSION)) + "\n";
  contextInfo += "GLSL Version: " + std::string((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION)) + "\n";
  return contextInfo;
}

std::filesystem::path GetRootPath(std::wstring rootFolderName)
{
  std::filesystem::path rootPath;

  WCHAR *exePath = new WCHAR[MAX_PATH];
  GetModuleFileName(nullptr, exePath, MAX_PATH);
  rootPath = std::filesystem::path(exePath);

  while (rootPath.filename().wstring() != rootFolderName)
    rootPath = rootPath.parent_path();

  return rootPath;
}

std::string ReadContentFromFile(const std::string &filePath)
{
  std::ifstream file(filePath);
  std::string content(std::istreambuf_iterator<char>(file.rdbuf()), std::istreambuf_iterator<char>());
  return content;
}

long long milliseconds_now()
{
  static LARGE_INTEGER s_frequency;
  static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
  if (s_use_qpc)
  {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (1000LL * now.QuadPart) / s_frequency.QuadPart;
  }
  else
  {
    return GetTickCount();
  }
}

double seconds_now()
{
  return milliseconds_now() / 1000.0;
}

}// namespace Utility

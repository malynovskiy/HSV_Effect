#include <glad/glad.h>
#include <windows.h>

#include "Utility.h"
#include "GLRenderer.h"

#include <iostream>
#include <cassert>

using u32 = uint32_t;

#define NO_WINDOW_RESIZE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

constexpr auto AppName = L"HSV-Effect";
constexpr auto AppClassName = L"Win32HSV-Effect";

constexpr u32 WindowWidth = 1920;
constexpr u32 WindowHeight = 1080;

RECT windowRect;
HGLRC hglrc;

namespace
{
LRESULT CALLBACK WndProc(HWND hWnd, u32 message, WPARAM wParam, LPARAM lParam);

void CreateWin32Context(HINSTANCE hInstance);
HWND CreateWin32Window(HINSTANCE hInstance, GLRenderer *renderer);

HGLRC LoadAndBindOpenGLContext(HDC hDC);
void UnbindOpenGLContext(HWND hWnd, HDC hDC, HGLRC hglrc);

inline bool IsAppAlreadyRunning()
{
  HWND hWnd = FindWindow(AppClassName, AppName);
  if (!hWnd)
    return false;

  if (IsIconic(hWnd))
    ShowWindow(hWnd, SW_RESTORE);
  SetForegroundWindow(hWnd);
  return true;
}
}// namespace

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  // If program already running we are switching to existing process
  if (!hPrevInstance && IsAppAlreadyRunning())
    return 0;

  CreateWin32Context(hInstance);

  std::unique_ptr<GLRenderer> glRenderer = std::make_unique<GLRenderer>(WindowWidth, WindowHeight);

  HWND hWnd{};
  W_CHECK(hWnd = CreateWin32Window(hInstance, glRenderer.get()));

  HDC hDC = GetDC(hWnd);

  Utility::Scope_guard const unbindOpenGLContextGuard = [&hWnd, &hDC] { UnbindOpenGLContext(hWnd, hDC, hglrc); };
  W_CHECK(hglrc = LoadAndBindOpenGLContext(hDC));

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  glRenderer->Initialize();

  MSG msg = { 0 };

  while (true)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    glRenderer->Render();
    SwapBuffers(hDC);

    //std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return static_cast<int>(msg.wParam);
}

namespace
{
LRESULT CALLBACK WndProc(HWND hWnd, u32 message, WPARAM wParam, LPARAM lParam)
{
  GLRenderer *renderer = reinterpret_cast<GLRenderer *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

  switch (message)
  {
  case WM_CREATE: {
    LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
  }
  break;
  case WM_KEYDOWN:
    if (renderer)
    {
      renderer->OnKeyDown(static_cast<UINT8>(wParam));
      std::wstring title = std::wstring(AppName) + L":\thue modifier - " + std::to_wstring(renderer->GetHueModifier())
                           + L"\tsaturate modifier - " + std::to_wstring(renderer->GetSaturateModifier())
                           + L"\tvalue modifier - " + std::to_wstring(renderer->GetValueModifier());
      SetWindowText(hWnd, title.c_str());
    }
    break;
  case WM_SIZE: {
  }
  break;

  case WM_DESTROY: {
    PostQuitMessage(0);
  }
  break;

  case WM_CLOSE: {
    DestroyWindow(hWnd);
  }
  break;

  case WM_ACTIVATEAPP:
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

void CreateWin32Context(HINSTANCE hInstance)
{
  WNDCLASSEX wndClass;
  ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
  wndClass.cbSize = sizeof(WNDCLASSEX);
  wndClass.style = CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WndProc;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = hInstance;
  wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
  wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
  wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
  wndClass.lpszMenuName = 0;
  wndClass.lpszClassName = AppClassName;

  W_CHECK(RegisterClassEx(&wndClass));
}

HWND CreateWin32Window(HINSTANCE hInstance, GLRenderer *renderer)
{
  SetRect(&windowRect,
    (GetSystemMetrics(SM_CXSCREEN) / 2) - (WindowWidth / 2),
    (GetSystemMetrics(SM_CYSCREEN) / 2) - (WindowHeight / 2),
    (GetSystemMetrics(SM_CXSCREEN) / 2) + (WindowWidth / 2),
    (GetSystemMetrics(SM_CYSCREEN) / 2) + (WindowHeight / 2));
  AdjustWindowRectEx(&windowRect, NO_WINDOW_RESIZE, FALSE, 0);

  HWND hWnd = CreateWindowEx(0,
    AppClassName,
    AppName,
    NO_WINDOW_RESIZE,
    windowRect.left,
    windowRect.top,
    windowRect.right - windowRect.left,
    windowRect.bottom - windowRect.top,
    nullptr,
    nullptr,
    hInstance,
    renderer);

  if (!hWnd)
  {
    std::cerr << "Failed to create Win32 window!\n";
    return nullptr;
  }
  return hWnd;
}

HGLRC LoadAndBindOpenGLContext(HDC hDC)
{
  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;
  pfd.iLayerType = PFD_MAIN_PLANE;

  int pixelFormat = ChoosePixelFormat(hDC, &pfd);
  W_CHECK(SetPixelFormat(hDC, pixelFormat, &pfd));

  HGLRC context = wglCreateContext(hDC);
  W_CHECK(wglMakeCurrent(hDC, context));

  if (!gladLoadGL())
  {
    std::cerr << "Failed to not load GLAD OpenGL context\n";
    return nullptr;
  }

  const std::string glInfo = Utility::GetOpenGLContextInformation();
  OutputDebugStringA(glInfo.c_str());

  return context;
}

void UnbindOpenGLContext(HWND hWnd, HDC hDC, HGLRC hglrc)
{
  wglMakeCurrent(nullptr, nullptr);
  wglDeleteContext(hglrc);
  ReleaseDC(hWnd, hDC);
}
}// namespace

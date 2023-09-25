@echo off

pushd .

where vcpkg >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo vcpkg is not installed.
    exit /b 1
)
echo vcpkg is installed.

vcpkg install stb:x64-windows
vcpkg install glm:x64-windows
vcpkg install glad:x64-windows

vcpkg integrate install

popd

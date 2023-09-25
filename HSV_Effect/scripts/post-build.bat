@echo off

pushd .

set SolutionDir=%1
set Target=%2

echo Copying shaders to bin folder
mkdir %SolutionDir%bin\%Target%\shaders
robocopy %SolutionDir%HSV_Effect\shaders %SolutionDir%bin\%Target%\shaders /NFL /NDL /NJH /NJS /nc /ns /np
echo Copying resources to bin folder
mkdir %SolutionDir%bin\%Target%\resources
robocopy  %SolutionDir%HSV_Effect\resources %SolutionDir%bin\%Target%\resources /E /NFL /NDL /NJH /NJS /nc /ns /np

popd

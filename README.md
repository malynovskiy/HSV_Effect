# HSV_Effect
Real-time renderer with HSV effect 

# Requirements: 
  - Visual Studio 2022
    - 143 toolset installed
  - GLM  
  - GLAD
  - STB

I'm using vcpkg package manager to pull all 3td-parties. You could use install.bat script to install all of them and integrate them into Visual Studio.

# Description of test assignment:

I've implemented the WinApi + OpenGL version. 

ImGui wasn't used so image input is static (it loads an image from resources/"image.jpg").

HSV adjusting is also primal, I've bound keyboard keys for HSV modifiers (I think using ImGui for sliders was allowed but I wasn't sure, and asking was too late) 

Key 1: Decrease Hue      Key 3: Decrease Saturation      Key 5: Decrease Value

Key 2: Increase Hue      Key 4: Increase Saturation      Key 6: Increase Value

All HSV adjustments status is shown in the title

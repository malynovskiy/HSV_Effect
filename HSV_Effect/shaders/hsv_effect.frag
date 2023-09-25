#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D imageTexture;

uniform float hueMod;
uniform float saturateMod;
uniform float valueMod;

vec3 hue2rgb(float hue)
{
  hue = fract(hue);
  float r = abs(hue * 6 - 3) - 1;
  float g = 2 - abs(hue * 6 - 2);
  float b = 2 - abs(hue * 6 - 4);
  
  vec3 rgb = vec3(r,g,b); 
  rgb = clamp(rgb, 0.0f, 1.0f); 
  return rgb;
}

vec3 hsv2rgb(vec3 hsv)
{
  vec3 rgb = hue2rgb(hsv.x); 
  rgb = mix(vec3(1.0f), rgb, hsv.y);
  rgb = rgb * hsv.z;
  return rgb;
}

vec3 rgb2hsv(vec3 rgb)
{
  float maxComponent = max(rgb.r, max(rgb.g, rgb.b));
  float minComponent = min(rgb.r, min(rgb.g, rgb.b));

  float diff = maxComponent - minComponent;

  float hue = 0.0f;

  if(maxComponent == rgb.r)
  {
    hue = 0.0f + (rgb.g - rgb.b) / diff;
  }
  else if(maxComponent == rgb.g) 
  {
    hue = 2.0f + (rgb.b - rgb.r) / diff;
  } 
  else if(maxComponent == rgb.b)
  {
    hue = 4 + (rgb.r - rgb.g) / diff;
  }
  hue = fract(hue / 6);
  if (hue < 0)
  {
    hue += 1.0f;
  }

  float saturation = maxComponent == 0 ? 0 : diff / maxComponent;
  float value = maxComponent;
  return vec3(hue, saturation, value);
}

void main()
{
  vec3 rgbColor = texture(imageTexture, TexCoords).rgb;

  vec3 hsv = rgb2hsv(rgbColor);
  
//  hsv.x = mod(hsv.x + hueMod, 1.0);
  hsv.x = fract(hsv.x + hueMod);
  hsv.y = clamp(hsv.y + saturateMod, 0.0, 1.0);
  hsv.z = clamp(hsv.z + valueMod, 0.0, 1.0);

  FragColor = vec4(hsv2rgb(hsv), 1.0f);
}
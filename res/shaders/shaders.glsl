#shader vertexShader
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;

out vec3 theColor;

void main()
{
  gl_Position = vec4(position, 1.0);
  theColor = vertexColor;
}

#shader fragmentShader
#version 430
#define PI 3.141592653

uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;     
out vec4 fragColor;

//rotation function
mat2 rotate(float angle)
{
  float c = cos(angle);
  float s = sin(angle);

  return mat2(c, -s, s, c);
}

//noise function
float noise(vec2 p)
{
  return sin(p.x) * sin(p.y);
}

// fractal noise
float fbm (vec2 p)
{
  float value = 0.0;
  float amplitude = 0.5;
  float frequency = 1.0;
  
  for (int i = 0; i < 6; i++)
    {
      value += amplitude * noise(p * frequency);
      amplitude *= 0.5;
      frequency *= 2.0;
      p = rotate(0.5) * p;
    }

  return value;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
  //normalize
  vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.xy;

  vec2 mouse = iMouse.xy / iResolution.xy;
  mouse = (mouse - 0.5) * 2.0;

  float t = iTime * 0.8;
  
  float angle = atan(uv.y, uv.x);
  float rad = length(uv);

  angle += rad * 3.0 + t;
  angle += fbm(uv * 2.0 + t * 0.5) * 0.5;

  float mouseInfluence = 1.0 / (length(uv - mouse * 0.8) + 0.1);
  angle += mouseInfluence * 0.3;

  vec2 spiralUV = vec2(cos(angle), sin(angle)) * rad;
  
  float pattern1 = sin(spiralUV.x * 8.0 + t * 2.0) * cos(spiralUV.y * 6.0 + t * 1.5);
  
  float rings = sin(rad * 20.0 - t * 4.0) * 0.5 + 0.5;
  rings *= smoothstep(0.0, 0.1, rad) * smoothstep(1.2, 0.8, rad);

  vec2 noiseUV = uv * 3.0;
  noiseUV = rotate(t * 0.3) * noiseUV;
  
  float trubulence = fbm(noiseUV + t);

  float intensity = pattern1 * 0.6 + rings * 0.8 + trubulence * 0.4;
  intensity = smoothstep(-0.2, 1.2, intensity);

  vec3 color1 = vec3(0.1, 0.0, 0.4);
  vec3 color2 = vec3(0.8, 0.2, 0.8);
  vec3 color3 = vec3(0.2, 0.6, 1.0);
  vec3 color4 = vec3(1.0, 0.8, 0.2);

  vec3 finalColor = mix(color1, color2, intensity);
  finalColor = mix(finalColor, color3, rings);
  finalColor = mix(finalColor, color4, smoothstep(0.7, 1.0, intensity));

  float shimmer = sin(length(uv - mouse) * 20 + t * 6.0) * 0.1 + 0.9;
  finalColor *= shimmer;

  float vignette = 1.0 - smoothstep(0.5, 1.2, rad);
  finalColor *= vignette;

  //glow
  finalColor += vec3(0.1, 0.05, 0.2) * intensity * (1.0 - rad);
  
  fragColor = vec4(finalColor,1.0);
}

void main()
{
   mainImage(fragColor, gl_FragCoord.xy);
}

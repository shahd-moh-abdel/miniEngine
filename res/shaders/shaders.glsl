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

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
  //normalize
  vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.xy;

  float t = iTime * 0.8;
  
  float angle = atan(uv.y, uv.x);
  float rad = length(uv);

  float spiral = angle + rad * 3.0 + t;
  float spiralPattern = sin(spiral * 4.0) * 0.5 + 0.5;

  float rings = sin(rad * 15.0 - t * 3.0) * 0.5 + 0.5;

  rings *= smoothstep(0.0, 0.1, rad) * smoothstep(1.0, 0.6, rad);
  float combined = spiralPattern * 0.7 + rings * 0.6;
  
  //float pulse = sin(t * 2.0) * 0.2 + 0.8;
  //pattern *= pulse;

  vec3 color1 = vec3(0.1, 0.0, 0.4);
  vec3 color2 = vec3(0.8, 0.2, 0.8);
  vec3 color3 = vec3(0.2, 0.6, 1.0);
  
  vec3 color = mix(color1, color2, spiralPattern);
  color = mix(color, color3, rings * 0.8);
  fragColor = vec4(color,1.0);
}

void main()
{
   mainImage(fragColor, gl_FragCoord.xy);
}

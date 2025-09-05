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

uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;     
out vec4 fragColor;


void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;

    uv -= .5;
    uv.x *= iResolution.x / iResolution.y;

    float circleRad = 0.2;
    float d = length(uv);
    
    vec3 circleColor = vec3(abs(sin(iTime)), 0.0, abs(cos(iTime)));
    vec3 c = vec3(1.);
    
    if (d < circleRad) c = circleColor; else c = vec3(1.);
    
    fragColor = vec4(c,1.0);
}

void main()
{
   mainImage(fragColor, gl_FragCoord.xy);
}

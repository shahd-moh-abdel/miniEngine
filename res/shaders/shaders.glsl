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

float plot(vec2 uv, float pct)
{
  return smoothstep(pct-0.02, pct, uv.y) -
    smoothstep(pct, pct+0.02, uv.y);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;
    float mp = iMouse.x / iResolution.x;

    //float y = pow(uv.x, 5.0);
    //float y = log(uv.x * PI);
    
    //uv -= .5;
    //uv.x *= iResolution.x / iResolution.y;

    float circleRad = 0.2;
    float d = length(uv);
    
    vec3 circleColor = vec3(abs(sin(iTime)), abs(sin(mp)), abs(cos(iTime)));
    vec3 c = vec3(1.);
    
    if (d < circleRad) c = circleColor; else c = vec3(y, 0.0, 0.0);

    // plot a line
    float pct = plot(uv, y);
    c = (1.0-pct)*c+pct*vec3(0.0,1.0,0.0);
    
    fragColor = vec4(c,1.0);
}

void main()
{
   mainImage(fragColor, gl_FragCoord.xy);
}

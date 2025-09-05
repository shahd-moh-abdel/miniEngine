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
    float d = length(uv);
    float col = d;

    if ( d < .3 ) col = 1.; else col = 0.;
    
    fragColor = vec4(vec3(col),1.0);
}

void main()
{
   mainImage(fragColor, gl_FragCoord.xy);
}

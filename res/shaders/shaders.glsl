#shader vertexShader
#version 430

in layout(location = 0) vec2 position;

void main()
{
  gl_Position = vec4(position, 0.0, 1.0);
}

#shader fragmentShader
#version 430

out vec4 a_color;

void main()
{
  a_color = vec4(0.0, 1.0, 0.0, 1.0); //RGBA
}

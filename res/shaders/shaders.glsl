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

out vec4 a_color;
in vec3 theColor;
void main()
{
  a_color = vec4(theColor, 1.0); //RGBA
}

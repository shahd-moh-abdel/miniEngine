#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

using namespace std;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

void processInput(GLFWwindow * window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

struct shaderProgramSource
{
  string vertexShader;
  string fragmentShader;
};

static shaderProgramSource parseShaders(const string& filePath)
{
  ifstream stream(filePath);

  enum class ShaderType
    {
      NONE = 0, VERTEX = 0, FRAGMENT = 1
    };

    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
      {
	if (line.find("#shader") != string::npos)
	  {
	    if (line.find("vertex") != string::npos)
	      type = ShaderType::VERTEX;
	    else if (line.find("fragment") != string::npos)
	      type = ShaderType::FRAGMENT;
	  }
	else
	  ss[(int)type] << line << '\n';
      }

    return {ss[0].str(), ss[1].str()};
    
}

//Compile Shaders from a file
static unsigned int compileShader(unsigned int type, const string& source)
{
  GLuint id = glCreateShader(type);
  const char* src = source.c_str();

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  //error handling 
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
    {
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char* message = (char*)alloca(length * sizeof(char));
      glGetShaderInfoLog(id, length, &length, message);
      cout << "ERROR: error in "<< type << " Shader: " << message << endl;
      glDeleteShader(id);
      return 0;
    }

  return id;
}

//Creating Shaders 
static unsigned int createShader(const string& vertexShader, const string& fragmentShader)
{
  GLuint program = glCreateProgram();

  GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);

  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

GLFWwindow* windowSetUp() {
  glfwInit();
  
  GLFWwindow* window = glfwCreateWindow(900, 600, "Mini Engine", NULL, NULL);
  if (!window)
    {
      cout << "Error creating the window " << endl;
      return nullptr;
    }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      cout << "Glad error" << endl;
      return nullptr;
    }

  glViewport(0, 0, 900, 600);

  return window;
}


float DELTA = 0.1f;
int triangleNum = 0;
const uint NUM_VERT_PER_TRI = 3;
const uint NUM_FLOAT_PER_VERT = 6;
const uint TRIANGLE_BYTE_SIZE = sizeof(float) * NUM_FLOAT_PER_VERT * NUM_VERT_PER_TRI;
const uint MAX_TRIANGLES = 20;

void createVertexBuffer()
{
  GLuint myBufferId;
  glGenBuffers(1, &myBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, myBufferId);
  glBufferData(GL_ARRAY_BUFFER, MAX_TRIANGLES * TRIANGLE_BYTE_SIZE, NULL, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));
}

void drawAnotherTriangle()
{
  if (triangleNum == MAX_TRIANGLES) return;
  const float THIS_TRIANGL_X = -1 + triangleNum * DELTA;
  GLfloat tri[] = {
    THIS_TRIANGL_X , 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    
    THIS_TRIANGL_X + DELTA, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    THIS_TRIANGL_X , 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
  };

  glBufferSubData(GL_ARRAY_BUFFER, triangleNum * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, tri);

  triangleNum++;  
}

int main()
{
  GLFWwindow* window = windowSetUp();
  glEnable(GL_DEPTH_TEST);
  
  createVertexBuffer();

  shaderProgramSource source = parseShaders("res/shaders/shaders.glsl");
  unsigned int shader = createShader(source.vertexShader, source.fragmentShader);
  glUseProgram(shader);
  
  while (!glfwWindowShouldClose(window))
    {
      processInput(window);
      glClear(GL_DEPTH_BUFFER_BIT);
      
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      drawAnotherTriangle();
      glDrawArrays(GL_TRIANGLES, 0, triangleNum * NUM_VERT_PER_TRI);
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  
  return 0;
}

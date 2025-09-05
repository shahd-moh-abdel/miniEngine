#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../include/glm/glm.hpp"
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

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  return window;
}


struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
};

void createFullScreen()
{
  GLfloat vertices[] = {
    -1.0f, -1.0f,
    +1.0f, -1.0f,
    -1.0f, +1.0f,
    +1.0f, +1.0f
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}

int main()
{
  GLFWwindow* window = windowSetUp();
  glEnable(GL_DEPTH_TEST);
  
  createFullScreen();

  shaderProgramSource source = parseShaders("res/shaders/shaders.glsl");
  unsigned int shader = createShader(source.vertexShader, source.fragmentShader);
  glUseProgram(shader);

  int iResLoc = glGetUniformLocation(shader, "iResolution");
  int iTimeLoc = glGetUniformLocation(shader, "iTime");
  int iMouseLoc = glGetUniformLocation(shader, "iMouse");

  double startTime = glfwGetTime();
  
  while (!glfwWindowShouldClose(window))
    {
      processInput(window);
      glClear(GL_DEPTH_BUFFER_BIT);
      
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      float currentTime = glfwGetTime() - startTime;
      if (iTimeLoc != -1)
	glUniform1f(iTimeLoc, currentTime);

      if (iResLoc != -1)
	glUniform3f(iResLoc, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

      //get mouse pos
      if (iMouseLoc != -1)
	{
	  double mouseX, mouseY;
	  glfwGetCursorPos(window, &mouseX, &mouseY);
	  mouseY =  SCREEN_HEIGHT - mouseY;
	  glUniform4f(iMouseLoc, mouseX, mouseY, 0.0f, 0.0f);
	}
      
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  
  return 0;
}

#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

using namespace std;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

void processInput(GLFWwindow * window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

GLFWwindow* windowSetUp() {
  glfwInit();
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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


int main()
{
  GLFWwindow* window = windowSetUp();

  GLfloat verts[] =
    {
      +0.0f, +0.0f,
      +1.0f, +1.0f,
      -1.0f, +1.0f,

      +0.0f, +0.0f,
      -1.0f, -1.0f,
      +1.0f, -1.0f,
    };

  GLuint myBufferId;
  glGenBuffers(1, &myBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, myBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); 
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  
  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glClearColor(0.4f, 0.3f, 0.8f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glDrawArrays(GL_TRIANGLES, 0,  6);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  
  return 0;
}


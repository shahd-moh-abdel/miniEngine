#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <ostream>>
using namespace std;

void processInput(GLFWwindow * window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(900, 600, "Mini Engine", NULL, NULL);
  if (!window)
    {
      cout << "Error creating the window " << endl;
      return -1;
    }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      cout << "Glad error" << endl;
      return -1;
    }

  glViewport(0, 0, 900, 600);

  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glClearColor(0.4f, 0.3f, 0.8f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  
  return 0;
}


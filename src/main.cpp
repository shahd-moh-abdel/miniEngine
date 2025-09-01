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

void createVertexBuffer()
{  
  GLfloat verts[] =
    {
      // #1 triangle 0, 1, 2
      // #2 triangle 0, 3, 4
      +0.0f, +0.0f, // 0
      +1.0f, +0.0f, +1.0f, // color 
      +1.0f, +1.0f, // 1
      +1.0f, +0.0f, +0.0f, // color
      -1.0f, +1.0f, // 2 
      +1.0f, +0.0f, +0.0f, // color
      -1.0f, -1.0f, // 3
      +1.0f, +0.0f, +0.0f, // color
      +1.0f, -1.0f, // 4
      +1.0f, +0.0f, +0.0f, // color
    };

  GLuint myBufferId;
  glGenBuffers(1, &myBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, myBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));
}

void createIndexBuffer()
{
  GLushort indices[] = {
    0, 1, 2,
    0, 3, 4
  };
  GLuint indexBufferID;
  glGenBuffers(1, &indexBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  
}

int main()
{
  GLFWwindow* window = windowSetUp();

  createVertexBuffer();
  createIndexBuffer();
  
  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glClearColor(0.4f, 0.3f, 0.8f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      //glDrawArrays(GL_TRIANGLES, 0,  6);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  
  return 0;
}


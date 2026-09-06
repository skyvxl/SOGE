// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// clang-format on

#include <cstdlib>
#include <iostream>

#include "Shader.h"

static void framebufferSizeCallback(GLFWwindow*, int width, int height) {
  glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

GLFWwindow* initWindow(int width, int height) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
      glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);

  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glViewport(0, 0, width, height);

  return window;
}

int main() {
  GLFWwindow* window = initWindow(1280, 720);

  Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

  float vertices[] = {
      // position         // color         // tex coord
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
  };

  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  // ---------------------------------------------------------
  // VAO + VBO + EBO
  // ---------------------------------------------------------

  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // vertex data
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(0));
  glEnableVertexAttribArray(0);

  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture coordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // ---------------------------------------------------------
  // TEXTURE
  // ---------------------------------------------------------

  unsigned int texture1;
  unsigned int texture2;

  glGenTextures(1, &texture1);
  glGenTextures(1, &texture2);

  stbi_set_flip_vertically_on_load(true);

  int width;
  int height;
  int nrChannels;

  // ---------------------------------------------------------
  // TEXTURE 1
  // ---------------------------------------------------------

  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char* data =
      stbi_load("images/container.jpg", &width, &height, &nrChannels, 0);

  if (data != nullptr) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load container.jpg: " << stbi_failure_reason()
              << '\n';
  }

  stbi_image_free(data);

  // ---------------------------------------------------------
  // TEXTURE 2
  // ---------------------------------------------------------

  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("images/awesomeface.png", &width, &height, &nrChannels, 0);

  if (data != nullptr) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load awesomeface.png: " << stbi_failure_reason()
              << '\n';
  }

  stbi_image_free(data);

  // ---------------------------------------------------------
  // GAME LOOP
  // ---------------------------------------------------------

  ourShader.Use();
  ourShader.SetInt("texture1", 0);
  ourShader.SetInt("texture2", 1);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    ourShader.Use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // ---------------------------------------------------------
  // CLEANUP
  // ---------------------------------------------------------

  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
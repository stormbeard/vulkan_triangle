/*
 * Author: cyril0allen@gmail.com
 */

#include <iostream>
#include <stdexcept>
#include <functional>

// This automatically loads the Vulkan header as opposed to doing something
// like "#include <vulkan/vulkan.h>" which only works for off-screen rendering.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//-----------------------------------------------------------------------------

static constexpr int kWidth = 800;

static constexpr int kHeight = 600;

//-----------------------------------------------------------------------------

class HelloTriangle {
 public:
  HelloTriangle();
  ~HelloTriangle() = default;

  // Do the actual work.
  void run();

 private:
  // Initialize GLFW and create a window.
  void initWindow();

  // Initilizes the Vulkan objects.
  void initVulkan();

  // Starts the rendering of frames.
  void mainLoop();

  // Deallocates resources used.
  void cleanup();

 private:
  GLFWwindow *window;
};

//-----------------------------------------------------------------------------

HelloTriangle::HelloTriangle() {
}

//-----------------------------------------------------------------------------

void HelloTriangle::initWindow() {
  glfwInit();

  // GLFW was originally designed to create an OpenGL context, so tell it not
  // to create an OpenGL context.
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  // Disable handling of resized windows.
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(kWidth, kHeight, "Vulkan", nullptr, nullptr);
}

//-----------------------------------------------------------------------------

void HelloTriangle::initVulkan() {

}

//-----------------------------------------------------------------------------

void HelloTriangle::mainLoop() {
  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

//-----------------------------------------------------------------------------

void HelloTriangle::cleanup() {
  glfwDestroyWindow(window);

  glfwTerminate();
}

//-----------------------------------------------------------------------------

void HelloTriangle::run() {
  initWindow();

  initVulkan();

  mainLoop();

  cleanup();
}

//-----------------------------------------------------------------------------

int main() {
  HelloTriangle app;

  try {
    app.run();
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

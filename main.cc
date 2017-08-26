/*
 * Author: cyril0allen@gmail.com
 */

#include <iostream>
#include <stdexcept>
#include <functional>

#include "glog/logging.h"

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

  // Create the VkInstance.
  void createInstance();

  // Starts the rendering of frames.
  void mainLoop();

  // Deallocates resources used.
  void cleanup();

 private:
  // Pointer to the window object we'll create.
  GLFWwindow *window;

  // The connection between this application and the Vulkan library.
  VkInstance instance;
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
  createInstance();
}

//-----------------------------------------------------------------------------
void HelloTriangle::createInstance() {
  // Give some information about this application.
  VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  const char **glfwExtensions;
  unsigned int glfwExtensionCount = 0;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // Tell the Vulkan driver which global extentions and validation layers we
  // want to use. This applies to the entire program and not a specific device.
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  VkResult result = vkCreateInstance(&createInfo,
                                     nullptr /* custom allocator callback */,
                                     &instance);
  CHECK(!result);
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

  app.run();

  return EXIT_SUCCESS;
}

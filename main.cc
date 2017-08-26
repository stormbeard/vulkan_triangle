/*
 * Author: Tony Allen (cyril0allen@gmail.com)
 */

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

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
  ~HelloTriangle();

  // Do the actual work.
  void run();

 private:
  // Initialize GLFW and create a window.
  void initWindow();

  // Initilizes the Vulkan objects.
  void initVulkan();

  // Create the VkInstance.
  void createInstance();

  // Check if provided extensions are all supported.
  void verifyExtensions(const std::vector<VkExtensionProperties> extensions);

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

HelloTriangle::~HelloTriangle() {
  cleanup();
}

//-----------------------------------------------------------------------------

void HelloTriangle::initWindow() {
  LOG(INFO) << "Initializing window";

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
  LOG(INFO) << "Initializing Vulkan";
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

  unsigned int glfwExtensionCount = 0;
  const char **glfwExtensions =
    glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  PCHECK(glfwExtensions) << "Unable to get GLFW extensions";

  // Tell the Vulkan driver which global extentions and validation layers we
  // want to use. This applies to the entire program and not a specific device.
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  LOG(INFO) << "Creating VkInstance";
  VkResult result = vkCreateInstance(&createInfo,
                                     nullptr /* custom allocator callback */,
                                     &instance);
  CHECK(!result) << "Could not create VkInstance";

  // Find out how many extensions there are.
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  // Query the extension details.
  LOG(INFO) << "Querying Vulkan extensions";
  std::vector<VkExtensionProperties> extensions;
  vkEnumerateInstanceExtensionProperties(
    nullptr, &extensionCount, extensions.data());
  if (!extensions.empty()) {
    std::unordered_set<std::string> supported_extension_names;
    for (const auto& extension : extensions) {
      LOG(INFO) << "Available Vulkan extension: " << extension.extensionName;
      supported_extension_names.emplace(extension.extensionName);
    }
  }

  // Verify the GLFW extensions are supported.
  const std::vector<std::string> glfwExtensionVec(
      glfwExtensions, glfwExtensions + glfwExtensionCount);
  for (const auto& ex : glfwExtensionVec) {
    LOG(WARNING) << "Unsupported GLFW extension: " << ex;
  }
}

//-----------------------------------------------------------------------------

void HelloTriangle::verifyExtensions(
  const std::vector<VkExtensionProperties> extensions) {

}

//-----------------------------------------------------------------------------

void HelloTriangle::mainLoop() {
  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

//-----------------------------------------------------------------------------

void HelloTriangle::cleanup() {
  LOG(INFO) << "Cleaning up";

  vkDestroyInstance(instance, nullptr /* allocator callback */);

  glfwDestroyWindow(window);

  glfwTerminate();
}

//-----------------------------------------------------------------------------

void HelloTriangle::run() {
  initWindow();

  initVulkan();

  mainLoop();
}

//-----------------------------------------------------------------------------

int main() {
  HelloTriangle app;

  app.run();

  return EXIT_SUCCESS;
}

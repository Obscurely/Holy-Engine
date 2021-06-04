#ifndef HE_WINDOW_H
#define HE_WINDOW_H
#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

// std
#include <string>

namespace Holy_Engine {
class HEWindow {
private:
  static void framebufferResizeCallback(GLFWwindow *window, int width,
                                        int height);
  void initWindow();

  int width;
  int height;
  bool framebufferResized = false;

  std::string windowName;
  GLFWwindow *window;

public:
  HEWindow(int w, int h, std::string name);
  ~HEWindow();

  HEWindow(const HEWindow &) = delete;
  HEWindow &operator=(const HEWindow &) = delete;

  bool shouldClose() { return glfwWindowShouldClose(window); }
  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  bool wasWindowResized() { return framebufferResized; }
  void resetWindowResizedFlag() { framebufferResized = false; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
};
} // namespace Holy_Engine

#endif
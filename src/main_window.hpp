#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

#pragma once

namespace Holy_Engine {
class MainWindow {
private:
  void initWindow();

  const int width;
  const int height;

  std::string windowName;
  GLFWwindow *window;

public:
  MainWindow(int w, int h, std::string name);
  ~MainWindow();

  MainWindow(const MainWindow &) = delete;
  MainWindow &operator=(const MainWindow &) = delete;

  bool shouldClose() { return glfwWindowShouldClose(window); }
  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
};
} // namespace Holy_Engine

#endif
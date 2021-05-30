#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>

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
};
} // namespace Holy_Engine

#endif
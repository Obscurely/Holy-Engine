#include "he_window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Holy_Engine {
HEWindow::HEWindow(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name} {
  initWindow();
}

HEWindow::~HEWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void HEWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void HEWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }
}

void HEWindow::framebufferResizeCallback(GLFWwindow *window, int width,
                                         int height) {
  auto heWindow =
      reinterpret_cast<HEWindow *>(glfwGetWindowUserPointer(window));
  heWindow->framebufferResized = true;
  heWindow->width = width;
  heWindow->height = height;
}
} // namespace Holy_Engine

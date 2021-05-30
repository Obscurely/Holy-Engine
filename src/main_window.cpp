#include "main_window.hpp"
#include <GLFW/glfw3.h>

namespace holy_engine {
MainWindow::MainWindow(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name} {
  initWindow();
}

MainWindow::~MainWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void MainWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
} // namespace holy_engine

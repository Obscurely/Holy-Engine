#include "first_app.hpp"
#include <GLFW/glfw3.h>

namespace holy_engine {
void FirstApp::run() {
  while (!mainWindow.shouldClose()) {
    glfwPollEvents();
  }
}
} // namespace holy_engine

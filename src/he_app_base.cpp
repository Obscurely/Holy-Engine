#include "he_app_base.hpp"
#include <GLFW/glfw3.h>

namespace Holy_Engine {
void HEAppBase::run() {
  while (!heWindow.shouldClose()) {
    glfwPollEvents();
  }
}
} // namespace Holy_Engine

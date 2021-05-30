#ifndef FIRST_APP_H
#define FIRST_APP_H

#include "main_window.hpp"

#pragma once

namespace Holy_Engine {
class HEAppBase {
private:
  MainWindow mainWindow{WIDTH, HEIGHT, "Holy Engine"};

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();
};
} // namespace Holy_Engine
#endif
#ifndef FIRST_APP_H
#define FIRST_APP_H

#include "main_window.hpp"

#pragma once

namespace holy_engine {
class FirstApp {
private:
  MainWindow mainWindow{WIDTH, HEIGHT, "Holy Engine"};

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();
};
} // namespace holy_engine
#endif
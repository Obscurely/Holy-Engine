#ifndef FIRST_APP_H
#define FIRST_APP_H

#include "he_device.hpp"
#include "he_pipeline.hpp"
#include "he_swap_chain.hpp"
#include "main_window.hpp"
#include <vulkan/vulkan_core.h>

#pragma once

namespace Holy_Engine {
class HEAppBase {
private:
  MainWindow mainWindow{WIDTH, HEIGHT, "Holy Engine"};
  HEDevice heDevice{mainWindow};
  HESwapChain heSwapChain{heDevice, mainWindow.getExtent()};
  VkPipelineLayout pipelineLayout;
  HEPipeline hePipeline{heDevice, "../src/shaders/simple_shader.vert.spv",
                        "../src/shaders/simple_shader.frag.spv",
                        HEPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();
};
} // namespace Holy_Engine
#endif
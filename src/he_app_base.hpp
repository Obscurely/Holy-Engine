#ifndef HE_APP_BASE_H
#define HE_APP_BASE_H

#include "he_device.hpp"
#include "he_pipeline.hpp"
#include "he_swap_chain.hpp"
#include "he_window.hpp"
#include <vulkan/vulkan_core.h>

// std
#include <memory>

#pragma once

namespace Holy_Engine {
class HEAppBase {
private:
  HEWindow heWindow{WIDTH, HEIGHT, "Holy Engine"};
  HEDevice heDevice{heWindow};
  HESwapChain heSwapChain{heDevice, heWindow.getExtent()};
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
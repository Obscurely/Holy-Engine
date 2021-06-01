#ifndef HE_APP_BASE_H
#define HE_APP_BASE_H

#include "he_device.hpp"
#include "he_model.hpp"
#include "he_pipeline.hpp"
#include "he_swap_chain.hpp"
#include "he_window.hpp"
#include <vulkan/vulkan_core.h>

// std
#include <memory>
#include <vector>

#pragma once

namespace Holy_Engine {
class HEAppBase {
private:
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);

  HEWindow heWindow{WIDTH, HEIGHT, "Holy Engine"};
  HEDevice heDevice{heWindow};
  std::unique_ptr<HESwapChain> heSwapChain;
  VkPipelineLayout pipelineLayout;
  std::unique_ptr<HEPipeline> hePipeline;
  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<HEModel> heModel;

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  HEAppBase();
  ~HEAppBase();

  HEAppBase(const HEAppBase &) = delete;
  HEAppBase &operator=(const HEAppBase &) = delete;

  void run();
};
} // namespace Holy_Engine
#endif
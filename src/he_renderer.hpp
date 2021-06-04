#ifndef HE_RENDERER_H
#define HE_RENDERER_H
#pragma once

// files
#include "he_device.hpp"
#include "he_swap_chain.hpp"
#include "he_window.hpp"

// libs
#include <vulkan/vulkan_core.h>

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Holy_Engine {
class HERenderer {
private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  HEWindow &heWindow;
  HEDevice &heDevice;
  std::unique_ptr<HESwapChain> heSwapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex;
  int currentFrameIndex{0};
  bool isFrameStarted{false};

public:
  HERenderer(HEWindow &window, HEDevice &device);
  ~HERenderer();

  HERenderer(const HERenderer &) = delete;
  HERenderer &operator=(const HERenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {
    return heSwapChain->getRenderPass();
  }
  bool isFrameInProgress() const { return isFrameStarted; }
  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(isFrameStarted &&
           "Cannot get frame index when frame not in progress");
    return currentFrameIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
};
} // namespace Holy_Engine
#endif
#ifndef HERENDERSYSTEM_H
#define HERENDERSYSTEM_H
#pragma once

// files
#include "he_device.hpp"
#include "he_game_object.hpp"
#include "he_pipeline.hpp"

// libs
#include <vulkan/vulkan_core.h>

// std
#include <memory>
#include <vector>

namespace Holy_Engine {
class HeRenderSystem {
private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  HEDevice &heDevice;

  std::unique_ptr<HEPipeline> hePipeline;
  VkPipelineLayout pipelineLayout;

public:
  HeRenderSystem(HEDevice &device, VkRenderPass renderPass);
  ;
  ~HeRenderSystem();

  HeRenderSystem(const HeRenderSystem &) = delete;
  HeRenderSystem &operator=(const HeRenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer commandBuffer,
                         std::vector<HEGameObject> &gameObjects);
};
} // namespace Holy_Engine
#endif
#ifndef HE_MODEL_H
#define HE_MODEL_H
#pragma once

// files
#include "he_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

// std
#include <vector>

namespace Holy_Engine {
class HEModel {
public:
  struct Vertex {
    glm::vec2 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };

private:
  void createVertexBuffers(const std::vector<Vertex> &vertices);

  HEDevice &heDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;

public:
  HEModel(HEDevice &device, const std::vector<Vertex> &vertices);
  ~HEModel();
  HEModel(const HEModel &) = delete;
  HEModel &operator=(const HEModel &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);
};
} // namespace Holy_Engine
#endif
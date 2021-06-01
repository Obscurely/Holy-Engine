#ifndef HE_PIPELINE_H
#define HE_PIPELINE_H
#include <vulkan/vulkan_core.h>
#pragma once

#include "he_device.hpp"
#include <string>
#include <vector>

namespace Holy_Engine {
struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class HEPipeline {
private:
  static std::vector<char> readFile(const std::string &filepath);

  void createGraphicsPipeline(const std::string &vertFilepath,
                              const std::string &fragFilepath,
                              const PipelineConfigInfo &configInfo);

  void createShaderModule(const std::vector<char> &code,
                          VkShaderModule *shaderModule);

  HEDevice &heDevice;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;

public:
  HEPipeline(HEDevice &device, const std::string &vertFilepath,
             const std::string &fragFilepath,
             const PipelineConfigInfo &configInfo);
  ~HEPipeline();

  HEPipeline(const HEPipeline &) = delete;
  void operator=(const HEPipeline &) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);
};
} // namespace Holy_Engine

#endif
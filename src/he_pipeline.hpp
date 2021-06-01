#ifndef HE_PIPELINE_H
#define HE_PIPELINE_H
#pragma once

#include "he_device.hpp"
#include <string>
#include <vector>

namespace Holy_Engine {
struct PipelineConfigInfo {};

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

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);
};
} // namespace Holy_Engine

#endif
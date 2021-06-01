#include "he_pipeline.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace Holy_Engine {

HEPipeline::HEPipeline(HEDevice &device, const std::string &vertFilepath,
                       const std::string &fragFilepath,
                       const PipelineConfigInfo &configInfo)
    : heDevice{device} {
  createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

HEPipeline::~HEPipeline() {
  vkDestroyShaderModule(heDevice.device(), vertShaderModule, nullptr);
  vkDestroyShaderModule(heDevice.device(), fragShaderModule, nullptr);
  vkDestroyPipeline(heDevice.device(), graphicsPipeline, nullptr);
}

std::vector<char> HEPipeline::readFile(const std::string &filepath) {
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}

void HEPipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                        const std::string &fragFilepath,
                                        const PipelineConfigInfo &configInfo) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
  std::cout << "Fragment Shader Code Size: " << vertCode.size() << std::endl;
}

void HEPipeline::createShaderModule(const std::vector<char> &code,
                                    VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(heDevice.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module");
  }
}

PipelineConfigInfo HEPipeline::defaultPipelineConfigInfo(uint32_t width,
                                                         uint32_t height) {
  PipelineConfigInfo configInfo{};

  return configInfo;
}

} // namespace Holy_Engine

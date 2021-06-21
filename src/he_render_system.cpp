// files
#include "he_render_system.hpp"
#include "he_device.hpp"
#include "he_game_object.hpp"
#include "he_pipeline.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>

// std
#include <array>
#include <cassert>
#include <memory>
#include <stdexcept>

namespace Holy_Engine {
struct SimplePushConstantData {
  glm::mat4 transform{1.f};
  alignas(16) glm::vec3 color{};
};

HeRenderSystem::HeRenderSystem(HEDevice &device, VkRenderPass renderPass)
    : heDevice{device} {
  createPipelineLayout();
  createPipeline(renderPass);
}

HeRenderSystem::~HeRenderSystem() {
  vkDestroyPipelineLayout(heDevice.device(), pipelineLayout, nullptr);
}

void HeRenderSystem::createPipelineLayout() {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(heDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void HeRenderSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  HEPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  hePipeline = std::make_unique<HEPipeline>(
      heDevice, "../src/shaders/simple_shader.vert.spv",
      "../src/shaders/simple_shader.frag.spv", pipelineConfig);
}

void HeRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer,
                                       std::vector<HEGameObject> &gameObjects) {
  hePipeline->bind(commandBuffer);

  for (auto &obj : gameObjects) {
    obj.transform.rotation.y =
        glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
    obj.transform.rotation.y =
        glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.color = obj.color;
    push.transform = obj.transform.mat4();

    vkCmdPushConstants(commandBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &push);

    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}
} // namespace Holy_Engine

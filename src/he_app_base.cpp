#include "he_app_base.hpp"
#include "he_pipeline.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

// std
#include <array>
#include <cassert>
#include <memory>
#include <stdexcept>

namespace Holy_Engine {
struct SimplePushConstantData {
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

HEAppBase::HEAppBase() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
}

HEAppBase::~HEAppBase() {
  vkDestroyPipelineLayout(heDevice.device(), pipelineLayout, nullptr);
}

void HEAppBase::run() {
  while (!heWindow.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }
}

void HEAppBase::loadModels() {
  std::vector<HEModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  heModel = std::make_unique<HEModel>(heDevice, vertices);
}

void HEAppBase::createPipelineLayout() {
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

void HEAppBase::createPipeline() {
  assert(heSwapChain != nullptr && "Cannot create pipeline before swap chain");
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  HEPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = heSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  hePipeline = std::make_unique<HEPipeline>(
      heDevice, "../src/shaders/simple_shader.vert.spv",
      "../src/shaders/simple_shader.frag.spv", pipelineConfig);
}

void HEAppBase::recreateSwapChain() {
  auto extent = heWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = heWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(heDevice.device());

  if (heSwapChain == nullptr) {
    heSwapChain = std::make_unique<HESwapChain>(heDevice, extent);
  } else {
    heSwapChain =
        std::make_unique<HESwapChain>(heDevice, extent, std::move(heSwapChain));
    if (heSwapChain->imageCount() != commandBuffers.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  createPipeline();
}

void HEAppBase::createCommandBuffers() {
  commandBuffers.resize(heSwapChain->imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = heDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(heDevice.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void HEAppBase::freeCommandBuffers() {
  vkFreeCommandBuffers(heDevice.device(), heDevice.getCommandPool(),
                       static_cast<uint32_t>(commandBuffers.size()),
                       commandBuffers.data());
  commandBuffers.clear();
}

void HEAppBase::recordCommandBuffer(int imageIndex) {
  static int frame = 0;
  frame = (frame + 1) % 1000;

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = heSwapChain->getRenderPass();
  renderPassInfo.framebuffer = heSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = heSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(heSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(heSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, heSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  hePipeline->bind(commandBuffers[imageIndex]);
  heModel->bind(commandBuffers[imageIndex]);

  for (int i = 0; i < 4; i++) {
    SimplePushConstantData push{};
    push.offset = {-0.5f + frame * 0.002f, -0.4f + i * 0.25f};
    push.color = {0.0f, 0.0f, 0.2f + 0.2f * i};

    vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &push);
    heModel->draw(commandBuffers[imageIndex]);
  }

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void HEAppBase::drawFrame() {
  uint32_t imageIndex;
  auto result = heSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  recordCommandBuffer(imageIndex);
  result = heSwapChain->submitCommandBuffers(&commandBuffers[imageIndex],
                                             &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      heWindow.wasWindowResized()) {
    heWindow.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}
} // namespace Holy_Engine

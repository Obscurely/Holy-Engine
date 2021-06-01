#include "he_app_base.hpp"
#include "he_pipeline.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

// std
#include <array>
#include <cassert>
#include <memory>
#include <stdexcept>

namespace Holy_Engine {
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
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout(heDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void HEAppBase::createPipeline() {
  auto pipelineConfig = HEPipeline::defaultPipelineConfigInfo(
      heSwapChain->width(), heSwapChain->height());
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
  heSwapChain = nullptr;
  heSwapChain = std::make_unique<HESwapChain>(heDevice, extent);
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

void HEAppBase::recordCommandBuffer(int imageIndex) {
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
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  hePipeline->bind(commandBuffers[imageIndex]);
  heModel->bind(commandBuffers[imageIndex]);
  heModel->draw(commandBuffers[imageIndex]);

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
  }

  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}
} // namespace Holy_Engine

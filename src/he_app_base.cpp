// files
#include "he_app_base.hpp"
#include "he_pipeline.hpp"
#include "he_render_system.hpp"

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
HEAppBase::HEAppBase() { loadGameObjects(); }

HEAppBase::~HEAppBase() {}

void HEAppBase::run() {
  HeRenderSystem heRenderSystem{heDevice, heRenderer.getSwapChainRenderPass()};

  while (!heWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = heRenderer.beginFrame()) {
      heRenderer.beginSwapChainRenderPass(commandBuffer);
      heRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      heRenderer.endSwapChainRenderPass(commandBuffer);
      heRenderer.endFrame();
    }
  }
}

void HEAppBase::loadGameObjects() {
  std::vector<HEModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  auto heModel = std::make_shared<HEModel>(heDevice, vertices);

  auto triangle = HEGameObject::createGameObject();
  triangle.model = heModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotation = .25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}
} // namespace Holy_Engine

#ifndef HE_GAMEOBJECT_H
#define HE_GAMEOBJECT_H
#include <glm/ext/matrix_transform.hpp>
#pragma once

// files
#include "he_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace Holy_Engine {
struct TransformComponent {
  glm::vec3 translation{}; // (position offset)
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
  glm::mat4 mat4() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{{
                         scale.x * (c1 * c3 + s1 * s2 * s3),
                         scale.x * (c2 * s3),
                         scale.x * (c1 * s2 * s3 - c3 * s1),
                         0.0f,
                     },
                     {
                         scale.y * (c3 * s1 * s2 - c1 * s3),
                         scale.y * (c2 * c3),
                         scale.y * (c1 * c3 * s2 + s1 * s3),
                         0.0f,
                     },
                     {
                         scale.z * (c2 * s1),
                         scale.z * (-s2),
                         scale.z * (c1 * c2),
                         0.0f,
                     },
                     {translation.x, translation.y, translation.z, 1.0f}};
  }
};

class HEGameObject {
public:
  using id_t = unsigned int;

  static HEGameObject createGameObject() {
    static id_t currentId = 0;
    return HEGameObject{currentId++};
  }

  HEGameObject(const HEGameObject &) = delete;
  HEGameObject &operator=(const HEGameObject &) = delete;
  HEGameObject(HEGameObject &&) = default;
  HEGameObject &operator=(HEGameObject &&) = default;

  id_t getId() { return id; }

  std::shared_ptr<HEModel> model{};
  glm::vec3 color{};
  TransformComponent transform{};

private:
  HEGameObject(id_t objId) : id{objId} {}

  id_t id;
};
} // namespace Holy_Engine
#endif
#ifndef HE_GAMEOBJECT_H
#define HE_GAMEOBJECT_H
#pragma once

#include "he_model.hpp"

// std
#include <memory>

namespace Holy_Engine {
struct Transform2dComponent {
  glm::vec2 translation{}; // (position offset)
  glm::vec2 scale{1.f, 1.f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};

    glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
    return rotMatrix * scaleMat;
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
  Transform2dComponent transform2d{};

private:
  HEGameObject(id_t objId) : id{objId} {}

  id_t id;
};
} // namespace Holy_Engine
#endif
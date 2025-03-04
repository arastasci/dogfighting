#pragma once

#include <at/ecs/Component.h>
#include <glm/vec3.hpp>
namespace at
{
    struct Transform : public Component
    {
        Transform(const Transform& other) = default;

        glm::vec3 position;
        glm::vec3 rotation; // Euler angles
        glm::vec3 scale;
        Transform* parent;

        Transform()
            : position(0.0f),
            rotation(0.0f),
            scale(1.0f),
            parent(nullptr)
        {
        }

        Transform(Transform* parent)
            : Transform()
        {

        }
    };

}
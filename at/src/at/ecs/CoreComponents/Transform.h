#pragma once

#include "at/core/Core.h"
#include <at/ecs/Component.h>
#include "at/math/Math.h"
namespace at
{
    struct AT_API Transform : public Component
    {
        Transform(const Transform& other) = default;

        glm::vec3 position;
        glm::vec3 rotation; // Euler angles
        glm::vec3 scale;

        Transform()
            : position(0.0f),
            rotation(0.0f),
            scale(1.0f)
        {
        }

    };

}
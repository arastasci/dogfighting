#pragma once

#include "at/core/Core.h"
#include <at/ecs/Component.h>
#include "at/utils/Math.h"

namespace at
{
    struct AT_API Transform : public Component
    {
        Transform(const Transform& other) = default;



        glm::vec3 position;
        glm::quat rotation; // Euler angles
        glm::vec3 scale;

        Transform(vec3 pos, quat rot, vec3 scale)
            :
            position(pos), rotation(rot), scale(scale)
        {
        }
        Transform(vec3 pos)
            : position(pos),
            rotation(),
            scale(1.0f)
        {
        }

        Transform()
            : position(0.0f),
            rotation(),
            scale(1.0f)
        {
        }

    };

}
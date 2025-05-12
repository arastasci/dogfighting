#pragma once

#include "at/core/Core.h"
#include <at/ecs/Component.h>
#include "at/utils/Math.h"

namespace at
{
    struct AT_API Transform : public Component
    {
        Transform(const Transform& other) = default;

        Transform(Transform* p, vec3 pos, quat rot, vec3 scale)
            :
            position(pos), rotation(rot), scale(scale), parent(p)
        {
        }

        Transform(vec3 pos, quat rot, vec3 scale, Transform* p = nullptr)
            :
            position(pos), rotation(rot), scale(scale), parent(p)
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

        // local transform
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        Transform* parent = nullptr;

        mat4 GetWorldTransform()
        {
            mat4 parentTransform(1.0f);
            if (parent)
            {
                parentTransform = parent->GetWorldTransform();
            }
            glm::mat4 r = glm::toMat4(glm::quat(rotation));

            return parentTransform * glm::translate(glm::mat4(1.0f), position) * r * glm::scale(glm::mat4(1.0f), scale);
        }
    };

}
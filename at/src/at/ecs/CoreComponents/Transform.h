#pragma once

#include "at/core/Core.h"
#include "at/utils/Math.h"


namespace at
{
    using namespace Math;
    struct AT_API Transform 
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

        glm::quat GetWorldRotation() const
        {
            return parent ? parent->GetWorldRotation() * rotation : rotation;
        }

        glm::vec3 Forward() const { return GetWorldRotation() * Vector3::forward; } // Z‑
        glm::vec3 Right()   const { return GetWorldRotation() * Vector3::right; } // X+
        glm::vec3 Up()      const { return GetWorldRotation() * Vector3::up; } // Y+


        mat4 GetWorldTransform() const
        {
            mat4 parentTransform(1.0f);
            if (parent)
            {
                parentTransform = parent->GetWorldTransform();
            }
            glm::mat4 r = glm::toMat4(glm::quat(rotation));

            return parentTransform * glm::translate(glm::mat4(1.0f), position) * r * glm::scale(glm::mat4(1.0f), scale);
        }
        
        // local transform
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        Transform* parent = nullptr;


    };
}

namespace Math
{
    inline btTransform toBt(const at::Transform& t)
    {
        btTransform btT;
        btT.setOrigin(toBt(t.position));
        btT.setBasis(toBt(t.rotation));
        return btT;


    }
}
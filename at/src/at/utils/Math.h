#pragma once

#include "glm/glm.hpp"
#include <LinearMath/btVector3.h>
#include <LinearMath/btMatrix3x3.h>   

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

using vec4 = glm::vec4;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
using mat4 = glm::mat4;
using quat = glm::quat;


inline btVector3 toBt(const vec3& v) noexcept
{
    return { static_cast<btScalar>(v.x),
             static_cast<btScalar>(v.y),
             static_cast<btScalar>(v.z) };
}

inline vec3 toGlm(const btVector3& v) noexcept
{
    return { static_cast<float>(v.x()),
             static_cast<float>(v.y()),
             static_cast<float>(v.z()) };
}


template<typename T>
inline btMatrix3x3 toBt(const glm::mat<3, 3, T, glm::defaultp>& m)
{
    return btMatrix3x3(
        // first row
        static_cast<btScalar>(m[0][0]), static_cast<btScalar>(m[1][0]), static_cast<btScalar>(m[2][0]),
        // second row
        static_cast<btScalar>(m[0][1]), static_cast<btScalar>(m[1][1]), static_cast<btScalar>(m[2][1]),
        // third row
        static_cast<btScalar>(m[0][2]), static_cast<btScalar>(m[1][2]), static_cast<btScalar>(m[2][2])
    );
}

namespace Vector3
{
    inline const vec3 forward  =   vec3(0.f, 0.f, 1.f);
    inline const vec3 up       =   vec3(0.f, 1.f, 0.f);
    inline const vec3 right    =   vec3(1.f, 0.f, 0.f);
    inline const vec3 zero     =   vec3(0.f, 0.f, 0.f);
    inline const vec3 one      =   vec3(1.f, 1.f, 1.f);
}

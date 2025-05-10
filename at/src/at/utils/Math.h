#pragma once

#include "glm/glm.hpp"
#include <LinearMath/btVector3.h>
#include <LinearMath/btMatrix3x3.h>   
#include <LinearMath/btTransform.h>
#include <assimp/matrix4x4.h>   // aiMatrix4x4

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

template<typename T>
inline btTransform toBt(const glm::mat<4, 4, T, glm::defaultp>& m)
{
   
    return btTransform(
        // first row
        toBt(glm::mat3(m)),
        btVector3(static_cast<btScalar>(m[0][3]), static_cast<btScalar>(m[1][3]), static_cast<btScalar>(m[2][3]))
    );
}


/**
 * Convert Assimp's aiMatrix4x4 to a glm::mat4, preserving the same transform.
 *
 * Assimp stores its matrix as:
 *   | a1  a2  a3  a4 |
 *   | b1  b2  b3  b4 |
 *   | c1  c2  c3  c4 |
 *   | d1  d2  d3  d4 |
 *
 * GLM uses column‑major order, so we supply the values column‑by‑column.
 */
inline glm::mat4 toGlm(const aiMatrix4x4& m)
{
    return glm::mat4(
        // Column 0
        m.a1, m.b1, m.c1, m.d1,
        // Column 1
        m.a2, m.b2, m.c2, m.d2,
        // Column 2
        m.a3, m.b3, m.c3, m.d3,
        // Column 3
        m.a4, m.b4, m.c4, m.d4
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

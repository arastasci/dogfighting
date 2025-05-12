#pragma once

#include "glm/glm.hpp"
#include <LinearMath/btVector3.h>
#include <LinearMath/btMatrix3x3.h>   
#include <LinearMath/btTransform.h>
#include <assimp/matrix4x4.h>   // aiMatrix4x4

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/matrix_decompose.hpp>
using vec4 = glm::vec4;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
using mat4 = glm::mat4;
using quat = glm::quat;



// credit to: TheCherno: https://www.youtube.com/TheChernoProject
inline bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale)
{
    // From glm::decompose in matrix_decompose.inl

    using namespace glm;
    using T = float;

    mat4 LocalMatrix(transform);
    vec3 eulerRotation; 
    // Normalize the matrix.
    if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
        return false;

    // First, isolate perspective.  This is the messiest.
    if (
        epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
        // Clear the perspective partition
        LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
        LocalMatrix[3][3] = static_cast<T>(1);
    }

    // Next take care of translation (easy).
    translation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
        for (length_t j = 0; j < 3; ++j)
            Row[i][j] = LocalMatrix[i][j];

    // Compute X scale factor and normalize first row.
    scale.x = length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    scale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    scale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
#if 0
    Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(Row[0], Pdum3) < 0)
    {
        for (length_t i = 0; i < 3; i++)
        {
            scale[i] *= static_cast<T>(-1);
            Row[i] *= static_cast<T>(-1);
        }
    }
#endif

    eulerRotation.y = asin(-Row[0][2]);
    if (cos(eulerRotation.y) != 0) {
        eulerRotation.x = atan2(Row[1][2], Row[2][2]);
        eulerRotation.z = atan2(Row[0][1], Row[0][0]);
    }
    else {
        eulerRotation.x = atan2(-Row[2][0], Row[1][1]);
        eulerRotation.z = 0;
    }

    rotation = quat(eulerRotation);

    return true;
}

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

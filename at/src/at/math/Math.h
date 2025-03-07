#pragma once

#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"


using vec3 = glm::vec3;
using mat4 = glm::mat4;
using quat = glm::quat;

namespace Vector3
{
    inline const vec3 forward  =   vec3(0.f, 0.f, 1.f);
    inline const vec3 up       =   vec3(0.f, 1.f, 0.f);
    inline const vec3 right    =   vec3(1.f, 0.f, 0.f);
    inline const vec3 zero     =   vec3(0.f, 0.f, 0.f);
    inline const vec3 one      =   vec3(1.f, 1.f, 1.f);
}

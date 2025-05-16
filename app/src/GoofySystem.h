#pragma once

#include "at.h"
using namespace at;

class GoofyBehaviour : public BehaviourComponent
{

};

class GoofySystem : public System
{
public:
    GoofySystem() = default;

    void Start() override
    {
        //auto view = GetStartedView<GoofyBehaviour, Rigidbody>();
        //for (auto& [e, _, g, rb] : view.each())
        //{
        //    vec3 forceDirection(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
        //    forceDirection = glm::normalize(forceDirection - vec3(0.5f));
        //    float force = 100 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        //    rb.ApplyForce(forceDirection, force);
        //    // Your model’s current forward axis (+Y)
        //    const glm::vec3 forward(0.0f, 1.0f, 0.0f);

        //    /*  Shortest‑arc quaternion from `forward` to `forceDir`.
        //        GLM 0.9.9 still ships glm::rotation(a,b). If you’re on
        //        a newer GLM where it’s gone, use the manual path below. */
        //    glm::quat q;

        //    float cosTheta = glm::clamp(glm::dot(forward, forceDirection), -1.0f, 1.0f);
        //    if (cosTheta < -0.9999f) {                            // opposite directions
        //        // Choose any axis perpendicular to `forward`
        //        glm::vec3 axis = glm::normalize(glm::cross(forward, glm::vec3(1, 0, 0)));
        //        if (glm::length2(axis) < 1e-6f)                   // `forward` was parallel to X
        //            axis = glm::normalize(glm::cross(forward, glm::vec3(0, 0, 1)));
        //        q = glm::angleAxis(glm::pi<float>(), axis);
        //    }
        //    else {
        //        glm::vec3 axis = glm::cross(forward, forceDirection);
        //        q = glm::quat(1.0f + cosTheta, axis.x, axis.y, axis.z);
        //        q = glm::normalize(q);
        //    }
        //    auto wt = rb.GetRigidbody()->getWorldTransform();
        //    wt.setBasis(toBt(glm::mat3_cast(q)));
        //    rb.GetRigidbody()->setWorldTransform(wt);
        //}
    }

    void Update(float dt) override
    {
        
        if (Input::GetKeyPress(Key::F))
        {
            auto view = GetView<GoofyBehaviour>();

            for (auto& [e] : view.each())
            {
                Entity en = { e, m_Scene };
                en.DestroyEntity();
            }
        }
    }
private:
    

};

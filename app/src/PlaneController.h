#pragma once
#include "at.h"

using namespace at;

#pragma once
#include "at.h"

using namespace at;

struct PlaneController : public Component
{
    float MaxThrust = 10000.0f; 
    float MaxSpeed = 40.0f;    
    float Throttle = 0.0f;     
    float ThrottleRate = 0.15f;    
    float PitchTorque = 600.0f;   
    float RollTorque = 600.0f;   

    float DragCoeff = 0.20f;
    float CdArea = 4.0f;
    float RotDragK = 0.05f;
};

class PlaneControllerSystem : public System
{
    void Start() override
    {
        auto view = GetStartedView<PlaneController, Rigidbody>();
        for (auto [e, _, pc, rb] : view.each())
        {
            rb.SetGravity(vec3(0, 0, 0));
            rb.GetRigidbody()->setDamping(0.04f, 0.50f);

            btVector3 inertia = rb.GetRigidbody()->getLocalInertia();
            m_RotDragConst[e] = pc.RotDragK * inertia.getX();
        }
    }

    void FixedUpdate() override
    {
        const float dt = Constants::FIXED_TIMESTEP;

        auto view = GetView<PlaneController, Transform, Rigidbody>();
        for (auto [e, _, pc, tr, rb] : view.each())
        {
            btRigidBody* body = rb.GetRigidbody().get();
            if (!body) continue;
            if (!body->isActive()) body->activate();

            Logger::GetClientLogger()->info("velocity: {}, percent: {}", body->getLinearVelocity().length(), pc.Throttle);

            if (Input::GetKeyPress(Key::Space))
                pc.Throttle = glm::clamp(pc.Throttle + pc.ThrottleRate * dt, 0.f, 1.f);
            if (Input::GetKeyPress(Key::LeftControl) || Input::GetKeyPress(Key::RightControl))
                pc.Throttle = glm::clamp(pc.Throttle - pc.ThrottleRate * dt, 0.f, 1.f);

            const btVector3 fwd = toBt(glm::normalize(tr.Forward()));
            const btVector3 rgt = toBt(glm::normalize(tr.Right()));

            btVector3 v = body->getLinearVelocity();
            float speedAlongFwd = v.dot(fwd);
            float target = pc.Throttle * pc.MaxSpeed;
            if (speedAlongFwd < target - 0.5f)
                body->applyCentralForce(fwd * (pc.MaxThrust * pc.Throttle));

            float pitch = (Input::GetKeyPress(Key::W) ? -1.f : 0.f) +
                (Input::GetKeyPress(Key::S) ? 1.f : 0.f);
            if (pitch != 0.f)
                body->applyTorque(rgt * (-pitch * pc.PitchTorque));

            float roll = (Input::GetKeyPress(Key::D) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::A) ? -1.f : 0.f);
            if (roll != 0.f)
                body->applyTorque(fwd * (-roll * pc.RollTorque));

            const float rho = 1.225f;
            float v2 = v.length2();
            if (v2 > SIMD_EPSILON)
            {
                btVector3 quadraticDrag = v.normalized() * (-0.5f * rho * pc.CdArea * v2);
                body->applyCentralForce(quadraticDrag);
            }
            body->applyCentralForce(-v * pc.DragCoeff);

            btVector3 torqueDrag = -body->getAngularVelocity() * m_RotDragConst[e];
            body->applyTorque(torqueDrag);
        }
    }

private:
    entt::dense_map<entt::entity, float> m_RotDragConst;
};


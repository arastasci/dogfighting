#pragma once
#include "at.h"

using namespace at;

#pragma once
#include "at.h"

using namespace at;

#pragma once
#include "at.h"

using namespace at;

// ----------------------------------------------------
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

    float WingArea = 6.0f;   // m²
    float Cl = 1.0f;   // lift coeff
};
// ----------------------------------------------------
class PlaneControllerSystem : public System
{
    void Start() override
    {
        auto view = GetStartedView<PlaneController, Rigidbody>();
        for (auto [e, _, pc, rb] : view.each())
        {
            rb.GetRigidbody()->setDamping(0.04f, 0.50f);

            btVector3 inertia = rb.GetRigidbody()->getLocalInertia();
            m_RotDragConst[e] = pc.RotDragK * inertia.getX();
        }
    }

    void FixedUpdate() override
    {
        const float dt = Constants::FIXED_TIMESTEP;
        const float rho0 = 1.225f;      // sea‑level air density
        const float scaleHeight = 10000.0f;    // density fall‑off

        auto view = GetView<PlaneController, Transform, Rigidbody>();
        for (auto [e, _, pc, tr, rb] : view.each())
        {
            btRigidBody* body = rb.GetRigidbody().get();
            if (!body) continue;
            if (!body->isActive()) body->activate();

            if (Input::GetKeyPress(Key::Space))
                pc.Throttle = glm::clamp(pc.Throttle + pc.ThrottleRate * dt, 0.f, 1.f);
            if (Input::GetKeyPress(Key::LeftControl) || Input::GetKeyPress(Key::RightControl))
                pc.Throttle = glm::clamp(pc.Throttle - pc.ThrottleRate * dt, 0.f, 1.f);

            const btVector3 fwd = toBt(glm::normalize(tr.Forward()));
            const btVector3 rgt = toBt(glm::normalize(tr.Right()));

            btVector3 v = body->getLinearVelocity();
            float      speedFwd = v.dot(fwd);
            float      target = pc.Throttle * pc.MaxSpeed;
            if (speedFwd < target - 0.5f)
                body->applyCentralForce(fwd * (pc.MaxThrust * pc.Throttle));

            float pitch = (Input::GetKeyPress(Key::W) ? -1.f : 0.f) +
                (Input::GetKeyPress(Key::S) ? 1.f : 0.f);
            if (pitch) body->applyTorque(rgt * (-pitch * pc.PitchTorque));

            float roll = (Input::GetKeyPress(Key::D) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::A) ? -1.f : 0.f);
            if (roll)  body->applyTorque(fwd * (-roll * pc.RollTorque));

            float v2 = v.length2();
            if (v2 > SIMD_EPSILON)
            {
                // lift: perpendicular to velocity and wing span (right axis)
                btVector3 liftDir = rgt.cross(v);
                if (liftDir.length2() > SIMD_EPSILON)
                {
                    liftDir.normalize();
                    float altitude = body->getWorldTransform().getOrigin().getY();
                    float rho = rho0 * std::exp(-altitude / scaleHeight);
                    float liftMag = 0.5f * rho * pc.Cl * pc.WingArea * v2;
                    body->applyCentralForce(liftDir * liftMag);
                }

                btVector3 quadDrag = v.normalized() * (-0.5f * rho0 * pc.CdArea * v2);
                body->applyCentralForce(quadDrag);
            }
            body->applyCentralForce(-v * pc.DragCoeff);

            btVector3 torqueDrag = -body->getAngularVelocity() * m_RotDragConst[e];
            body->applyTorque(torqueDrag);
        }
    }

private:
    entt::dense_map<entt::entity, float> m_RotDragConst;
};

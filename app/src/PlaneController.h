#pragma once
#include "at.h"

using namespace at;

struct PlaneController : public Component
{
    // ––– physical constants (default values for a small GA trainer) –––
    float Mass = 300.0f;   // kg
    float InertiaScale = 0.25f;    // crude Ix=Iy=Iz ≈ Mass × InertiaScale × span²

    // ––– pilot controls –––
    float ThrustForce = 600.0f;   // N (positive forward)
    float PitchTorque = 150.0f;   // N·m (about right axis)
    float RollTorque = 150.0f;   // N·m (about forward axis)

    // ––– damping & aero –––
    float LinearDamping = 0.04f;    // Bullet linear damping [0‑1]
    float AngularDamping = 0.50f;    // Bullet angular damping [0‑1]
    float CdArea = 4.0f;     // drag coefficient * reference area (m²)
    float RotDragK = 0.05f;    // τ = −k ω  (multiplied by inertia in Start)
};

//-----------------------------------------------------------------------------
//  PlaneControllerSystem
//-----------------------------------------------------------------------------
class PlaneControllerSystem : public System
{
public:
    // Called once when the entity first gains PlaneController (+ required comps)
    void Start(entt::entity e,
        PlaneController& pc,
        Transform& tr,
        Rigidbody& rb) override
    {
        // 1) ensure rigid body exists
        btRigidBody* body = rb.GetRigidbody().get();
        if (!body) return;

        // 2) set mass & inertia (simple uniform box approximation)
        btVector3 inertia(pc.InertiaScale * pc.Mass,
            pc.InertiaScale * pc.Mass,
            pc.InertiaScale * pc.Mass);
        body->setMassProps(pc.Mass, inertia);
        body->updateInertiaTensor();

        // 3) built‑in Bullet damping
        body->setDamping(pc.LinearDamping, pc.AngularDamping);

        // 4) pre‑compute rotational‑drag constant in world units
        m_RotDragConst[e] = pc.RotDragK * inertia.getX(); // assume Ix≈Iy≈Iz
    }

    // Fixed‑timestep physics update
    void FixedUpdate() override
    {
        auto view = GetView<PlaneController, Transform, Rigidbody>();

        for (auto [e, _, pc, tr, rb] : view.each())
        {
            btRigidBody* body = rb.GetRigidbody().get();
            if (!body) continue;

            //-------------------------------------------------------------
            //   WORLD axes
            //-------------------------------------------------------------
            const btVector3 Fwd = toBt(glm::normalize(tr.Forward()));
            const btVector3 Rgt = toBt(glm::normalize(tr.Right()));

            //-------------------------------------------------------------
            //   Thrust & brake
            //-------------------------------------------------------------
            if (Input::GetKeyPress(Key::Space))
                body->applyCentralForce(Fwd * pc.ThrustForce);

            if (Input::GetKeyPress(Key::LeftControl) || Input::GetKeyPress(Key::RightControl))
                body->applyCentralForce(Fwd * -pc.ThrustForce);

            //-------------------------------------------------------------
            //   Pitch (W/S)  → torque about right axis
            //-------------------------------------------------------------
            float pitchInput = (Input::GetKeyPress(Key::W) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::S) ? -1.f : 0.f);

            if (pitchInput != 0.f)
                body->applyTorque(Rgt * (-pitchInput * pc.PitchTorque));

            //-------------------------------------------------------------
            //   Roll (A/D)   → torque about forward axis
            //-------------------------------------------------------------
            float rollInput = (Input::GetKeyPress(Key::D) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::A) ? -1.f : 0.f);

            if (rollInput != 0.f)
                body->applyTorque(Fwd * (-rollInput * pc.RollTorque));

            //-------------------------------------------------------------
            //   Quadratic aerodynamic drag  (‑½ ρ CdA v |v|)
            //-------------------------------------------------------------
            const float rho = 1.225f;                        // kg m⁻³
            btVector3 v = body->getLinearVelocity();
            btScalar v2 = v.length2();
            if (v2 > SIMD_EPSILON)
            {
                btVector3 Fdrag = v.normalized() * (-0.5f * rho * pc.CdArea * v2);
                body->applyCentralForce(Fdrag);
            }

            //-------------------------------------------------------------
            //   Rotational damping  τ = −k ω
            //-------------------------------------------------------------
            btVector3 w = body->getAngularVelocity();
            btVector3 τ = -w * m_RotDragConst[e];
            body->applyTorque(τ);
        }
    }

private:
    // per‑entity rotational‑drag scalar (pre‑multiplied by inertia in Start)
    entt::dense_map<entt::entity, float> m_RotDragConst;
};
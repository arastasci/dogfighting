#pragma once
#include "at.h"

#include "AppMessages.h"

#include "GoofySystem.h"
#include "RocketSystem.h"

using namespace at; 
struct PlaneFlightBehaviour
{
    float MaxThrust = 15000.0f;
    float MaxSpeed = 80.0f;
    float Throttle = 0.0f;
    float ThrottleRate = 0.15f;

    float PitchTorque = 600.0f;
    float RollTorque = 600.0f;

    float DragCoeff = 0.20f;
    float CdArea = 4.0f;
    float RotDragK = 0.05f;

    float WingArea = 6.0f;   
    float Cl = 0.2f;   
};



struct PlayerPlaneController
{
    float pitch;
    float roll;
    float thrust;
    bool shooting;
    bool IsOwned = false;
    void Reset()
    {
        pitch = 0;
        roll = 0;
        thrust = 0;
        shooting = false;
    }
};

namespace Messages
{
    struct PlaneInputMessage : public Message<PlayerInput>
    {
        PlaneInputMessage(entt::entity e_, const PlayerPlaneController& payload) : e(e_), inputPayload(payload) {}
        entt::entity e;
        PlayerPlaneController inputPayload;
    };
}


class PlayerPlaneControllerSystem : public System
{
public:
    void Update(float dt) override
    {
        m_AccTime += dt;
    }

    void FixedUpdate() override
    {

        auto& view = GetView<PlayerPlaneController>();
        for (auto& [e, controller] : view.each())
        {
            if (!controller.IsOwned)
                continue;
            controller.pitch = (Input::GetKeyPress(Key::W) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::S) ? -1.f : 0.f);

            controller.roll = (Input::GetKeyPress(Key::D) ? -1.f : 0.f) +
                (Input::GetKeyPress(Key::A) ? 1.f : 0.f);

            controller.thrust = (Input::GetKeyPress(Key::Space) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::LeftControl) ? -1.f : 0.f);

            if (m_AccTime > 0.3f && Input::GetKeyPress(Key::F))
            {
                controller.shooting = true;
                m_AccTime = 0;
            }
            auto* msg = new Messages::PlaneInputMessage(e, controller);
            Networking::Get().SendToHost(msg, sizeof(*msg));
            controller.Reset();
        }

    }
private:
    float m_AccTime{};
};

struct RocketPrefab : public Prefab
{
    void InitEntity(Entity e) override
    {
        e.AddComponent<MeshRenderer>("rocket", "defaultMaterial");
        e.AddComponent<Rigidbody>(false, true);
        e.AddComponent<RocketBehaviour>();
    }
};

class PlaneFlightSystem : public System
{
public:
    void Start() override
    {
        auto view = GetStartedView<PlaneFlightBehaviour, Rigidbody>();
        for (auto [e, pc, rb] : view.each())
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

        auto view = GetView<PlaneFlightBehaviour, PlayerPlaneController, Transform, Rigidbody>();
        for (auto [e, planeFlight, controller, tr, rb] : view.each())
        {
            btRigidBody* body = rb.GetRigidbody().get();
            if (!body) continue;
            if (!body->isActive()) body->activate();

            btVector3 v = body->getLinearVelocity();

            if (controller.shooting)
            {
                ShootRocket(v, tr);
            }

            if (controller.thrust == 1)
                planeFlight.Throttle = glm::clamp(planeFlight.Throttle + planeFlight.ThrottleRate * dt, 0.f, 1.f);
            else if (controller.thrust == -1)
                planeFlight.Throttle = glm::clamp(planeFlight.Throttle - planeFlight.ThrottleRate * dt, 0.f, 1.f);

            const btVector3 fwd =Math::toBt(glm::normalize(tr.Forward()));
            const btVector3 rgt =Math::toBt(glm::normalize(tr.Right()));

            float      speedFwd = v.dot(fwd);
            float      target = planeFlight.Throttle * planeFlight.MaxSpeed;
            if (speedFwd < target - 0.5f)
                body->applyCentralForce(fwd * (planeFlight.MaxThrust * planeFlight.Throttle));

            /*float pitch = (Input::GetKeyPress(Key::W) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::S) ? -1.f : 0.f);*/
            float pitch = controller.pitch;
            if (pitch) body->applyTorque(rgt * (-pitch * planeFlight.PitchTorque));

            //float roll = (Input::GetKeyPress(Key::D) ? -1.f : 0.f) +
            //    (Input::GetKeyPress(Key::A) ? 1.f : 0.f);
            float roll = controller.roll;
            if (roll)  body->applyTorque(fwd * (-roll * planeFlight.RollTorque));


            //Logger::GetClientLogger()->info("vel: {} | rate {}", v.length(), pc.Throttle);

            float v2 = v.length2();
            if (v2 > SIMD_EPSILON)
            {
                // lift: perpendicular to velocity and wing span (right axis)
                btVector3 liftDir = v.cross(rgt);
                if (liftDir.length2() > SIMD_EPSILON)
                {
                    liftDir.normalize();
                    float altitude = body->getWorldTransform().getOrigin().getY();
                    float rho = rho0 * std::exp(-altitude / scaleHeight);
                    float liftMag = 0.5f * rho * planeFlight.Cl * planeFlight.WingArea * v2;
                    body->applyCentralForce(liftDir * liftMag);
                }

                btVector3 quadDrag = v.normalized() * (-0.5f * rho0 * planeFlight.CdArea * v2);
                body->applyCentralForce(quadDrag);
            }
            body->applyCentralForce(-v * planeFlight.DragCoeff);

            btVector3 torqueDrag = -body->getAngularVelocity() * m_RotDragConst[e];
            body->applyTorque(torqueDrag);

           /* if (Networking::Get().IsHost() && m_Scene->GetRegistry().any_of<DirtyComponent>(e))
            {
                auto& dc = m_Scene->GetRegistry().get<DirtyComponent>(e);
                dc.SetDirty<Transform>();
            }*/
        }
    }

    void Update(float dt) override
    {
        m_AccTime += dt;
    }
    static quat RotateRocket(const Transform& t)
    {
        auto fwd = t.Forward();
        const glm::vec3 forward(0.0f, 1.0f, 0.0f);

        glm::quat q;

        float cosTheta = glm::clamp(glm::dot(forward, fwd), -1.0f, 1.0f);
        if (cosTheta < -0.9999f) {

            glm::vec3 axis = glm::normalize(glm::cross(forward, glm::vec3(1, 0, 0)));
            if (glm::length2(axis) < 1e-6f)
                axis = glm::normalize(glm::cross(forward, glm::vec3(0, 0, 1)));
            q = glm::angleAxis(glm::pi<float>(), axis);
        }
        else {
            glm::vec3 axis = glm::cross(forward, fwd);
            q = glm::quat(1.0f + cosTheta, axis.x, axis.y, axis.z);
            q = glm::normalize(q);
        }

        return q;
    }
    static inline std::unordered_map<HSteamNetConnection, entt::entity> m_ConnToEntityMap;
private:
    
    entt::dense_map<entt::entity, float> m_RotDragConst;

    void ShootRocket(const btVector3& velocity, const Transform& t)
    {
        if (m_AccTime <= 0.3)
            return;

        m_AccTime = 0.0;
        auto& nc = Networking::Get();
        auto* msg = new Messages::RocketFiredMessage();
        nc.SendToHost(msg, sizeof(*msg));

    }

   
    double m_AccTime = 0.0;
};

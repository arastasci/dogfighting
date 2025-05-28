#pragma once
#include "at.h"

#include "AppMessages.h"

#include "GoofySystem.h"
#include "RocketSystem.h"

using namespace at; 
struct PlaneController
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

struct RocketPrefab : public Prefab<RocketPrefab>
{
    void InitEntity(Entity e) override
    {
        e.AddComponent<MeshRenderer>("rocket", "defaultMaterial");
        e.AddComponent<Rigidbody>(false, true);
        e.AddComponent<RocketBehaviour>();
    }
};

class PlaneControllerSystem : public System
{
public:
    void Start() override
    {
        auto view = GetStartedView<PlaneController, Rigidbody>();
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

        auto view = GetView<PlaneController, Transform, Rigidbody>();
        for (auto [e, pc, tr, rb] : view.each())
        {
            btRigidBody* body = rb.GetRigidbody().get();
            if (!body) continue;
            if (!body->isActive()) body->activate();

            btVector3 v = body->getLinearVelocity();

            if (Input::GetMouseButtonPress(GLFW_MOUSE_BUTTON_RIGHT))
            {
                ShootRocket(v, tr);
            }

            if (Input::GetKeyPress(Key::Space))
                pc.Throttle = glm::clamp(pc.Throttle + pc.ThrottleRate * dt, 0.f, 1.f);
            if (Input::GetKeyPress(Key::LeftControl) || Input::GetKeyPress(Key::RightControl))
                pc.Throttle = glm::clamp(pc.Throttle - pc.ThrottleRate * dt, 0.f, 1.f);

            const btVector3 fwd =Math::toBt(glm::normalize(tr.Forward()));
            const btVector3 rgt =Math::toBt(glm::normalize(tr.Right()));

            float      speedFwd = v.dot(fwd);
            float      target = pc.Throttle * pc.MaxSpeed;
            if (speedFwd < target - 0.5f)
                body->applyCentralForce(fwd * (pc.MaxThrust * pc.Throttle));

            float pitch = (Input::GetKeyPress(Key::W) ? 1.f : 0.f) +
                (Input::GetKeyPress(Key::S) ? -1.f : 0.f);
            if (pitch) body->applyTorque(rgt * (-pitch * pc.PitchTorque));

            float roll = (Input::GetKeyPress(Key::D) ? -1.f : 0.f) +
                (Input::GetKeyPress(Key::A) ? 1.f : 0.f);
            if (roll)  body->applyTorque(fwd * (-roll * pc.RollTorque));


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
                    float liftMag = 0.5f * rho * pc.Cl * pc.WingArea * v2;
                    body->applyCentralForce(liftDir * liftMag);
                }

                btVector3 quadDrag = v.normalized() * (-0.5f * rho0 * pc.CdArea * v2);
                body->applyCentralForce(quadDrag);
            }
            body->applyCentralForce(-v * pc.DragCoeff);

            btVector3 torqueDrag = -body->getAngularVelocity() * m_RotDragConst[e];
            body->applyTorque(torqueDrag);

            if (Networking::Get().IsHost() && m_Scene->GetRegistry().any_of<DirtyComponent>(e))
            {
                auto& dc = m_Scene->GetRegistry().get<DirtyComponent>(e);
                dc.SetDirty<Transform>();
            }
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
        nc.SendToHost(new Messages::RocketFiredMessage());

    }

   
    double m_AccTime = 0.0;
};

#pragma once
#include "at.h"
using namespace at;


struct FreeCamera : public BehaviourComponent
{
    float yaw = 0.0f;
    float pitch = 0.0f;
    float moveSpeed = 5.0f;
    float mouseSensitivity = 0.1f;
    glm::vec2 lastMousePos = { 0.0f, 0.0f };
    bool firstFrame = true;
};

class FreeCameraSystem : public at::System
{
public:
    void Update(float dt) override
    {
        using namespace glm;
        auto view = GetView<at::Transform, FreeCamera>();

        auto [mx, my] = Input::GetMousePos();

        for (auto [e, tr, cam] : view.each())
        {
            if (Input::GetMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
            {
                Input::SetCursorMode(Input::CursorMode::Locked);
                cam.firstFrame = false;
            }

            if (Input::GetKeyPress(Key::Escape) &&
                Input::GetCursorMode() == Input::CursorMode::Hidden)
            {
                Input::SetCursorMode(Input::CursorMode::Normal);
                cam.firstFrame = true;
                return;
            }

            const vec2  delta = vec2(mx, my) - cam.lastMousePos;
            cam.lastMousePos = { mx, my };

            cam.yaw -= delta.x * cam.mouseSensitivity;
            cam.pitch -= delta.y * cam.mouseSensitivity;

            cam.pitch = clamp(cam.pitch, -89.0f, 89.0f);

            const float yawRad = radians(cam.yaw);
            const float pitchRad = radians(cam.pitch);

            vec3 forward{
                cos(pitchRad) * sin(yawRad),
                sin(pitchRad),
                cos(pitchRad) * cos(yawRad)
            };
            forward = normalize(forward);

            const vec3 worldUp = vec3{ 0, 1, 0 };
            vec3 right = normalize(cross(forward, worldUp));
            vec3 up = cross(right, forward);

            float v = cam.moveSpeed * dt;
            if (Input::GetKeyPress(Key::Up)) tr.position += forward * v;
            if (Input::GetKeyPress(Key::Down)) tr.position -= forward * v;
            if (Input::GetKeyPress(Key::Left)) tr.position -= right * v;
            if (Input::GetKeyPress(Key::Right)) tr.position += right * v;
            if (Input::GetKeyPress(Key::P))
            {
                m_WillPause = true;
            }
            else if (m_WillPause && Input::GetKeyRelease(Key::P))
            {
                m_WillPause = false;

                auto pW = m_Scene->GetPhysicsWorld();
                pW->SetIsSimulated(!pW->GetIsSimulated());
            }

            tr.rotation = glm::quatLookAt(forward, up);
        }
    }
private:
    bool m_WillPause = false;
};


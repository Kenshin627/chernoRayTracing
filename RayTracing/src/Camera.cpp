#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Walnut/Input/Input.h"
using namespace Walnut;

Camera::Camera(float verticalFov, float nearPlane, float farPlane, const glm::vec3 position, const glm::vec3 forward, const glm::vec3 up): fov(verticalFov), nearPlane(nearPlane), farPlane(farPlane), eye(position), forward(forward), up(up)
{
	viewport_width = 0;
	viewport_height = 0;
}

float Camera::GetRotationSpeed()
{
	return 0.3f;
}

bool Camera::onUpdate(float ts)
{
	glm::vec2 mousePos = Input::GetMousePosition();
	glm::vec2 delta = (mousePos - lastMousePosition) * 0.002f;
	lastMousePosition = mousePos;

	if (!Input::IsMouseButtonDown(MouseButton::Right))
	{
		Input::SetCursorMode(CursorMode::Normal);
		return false;
	}

	Input::SetCursorMode(CursorMode::Locked);

	bool moved = false;
	glm::vec3 rightDirection = glm::cross(forward, up);

	float speed = 5.0f;

	// Movement
	if (Input::IsKeyDown(KeyCode::W))
	{
		eye += forward * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyDown(KeyCode::S))
	{
		eye -= forward * speed * ts;
		moved = true;
	}
	if (Input::IsKeyDown(KeyCode::A))
	{
		eye -= rightDirection * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyDown(KeyCode::D))
	{
		eye += rightDirection * speed * ts;
		moved = true;
	}
	if (Input::IsKeyDown(KeyCode::Q))
	{
		eye -= up * speed * ts;
		moved = true;
	}
	else if (Input::IsKeyDown(KeyCode::E))
	{
		eye += up * speed * ts;
		moved = true;
	}

	// Rotation
	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitchDelta = delta.y * GetRotationSpeed();
		float yawDelta = delta.x * GetRotationSpeed();

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
			glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		forward = glm::rotate(q, forward);

		moved = true;
	}

	if (moved)
	{
		ReCalcView();
		ReCalcRayDirections();
	}
	return moved;
}

void Camera::onResize(uint32_t width, uint32_t height)
{
	if (viewport_width == width && viewport_height == height)
	{
		return;
	}
	viewport_width = width;
	viewport_height = height;
	ReCalcProjection();
	ReCalcRayDirections();
}

void Camera::ReCalcView()
{
	view = glm::lookAt(eye, eye + forward, up);
	invertView = glm::inverse(view);
}

void Camera::ReCalcProjection()
{
	projection = glm::perspectiveFov(glm::radians(fov), (float)viewport_width, (float)viewport_height, nearPlane, farPlane);
	invertProjection = glm::inverse(projection);
}

void Camera::ReCalcRayDirections()
{
	rayDirections.resize(viewport_width * viewport_height);
	for (uint32_t y = 0; y < viewport_height; y++)
	{
		for (uint32_t x = 0; x < viewport_width; x++)
		{
			glm::vec2 coord = { (float)x / (float)viewport_width, (float)y / (float)viewport_height  };
			coord = coord * 2.0f - 1.0f;// -1 -> 1 
			glm::vec4 target =  invertProjection * glm::vec4(coord.x, coord.y, 1, 1);
			glm::vec3 rayDirection =glm::vec3(invertView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));
			rayDirections[x + y * viewport_width] = rayDirection;
		}
	}
}



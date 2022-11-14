#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(float verticalFov, float nearPlane, float farPlane, const glm::vec3 position, const glm::vec3 forward, const glm::vec3 up): fov(verticalFov), nearPlane(nearPlane), farPlane(farPlane), eye(position), forward(forward), up(up)
{
	viewport_width = 0;
	viewport_height = 0;
}

void Camera::onUpdate(float ts)
{

}

void Camera::onResize(uint32_t width, uint32_t height)
{
	if (viewport_width == width || viewport_height == height)
	{
		return;
	}
	viewport_width = width;
	viewport_height = height;
	ReCalcView();
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
			glm::vec4 target =  invertProjection * glm::vec4(coord.x, coord.y, 1.0, 1.0);
			glm::vec3 rayDirection = glm::normalize(glm::vec3(invertView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0.0)));
			rayDirections[x + y * viewport_width] = rayDirection;
		}
	}
}



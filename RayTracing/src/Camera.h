#include<glm/glm.hpp>
#include<vector>

class Camera {
public:
	Camera(float verticalFov, float nearPlane, float farPlane, const glm::vec3 position, const glm::vec3 forward, const glm::vec3 up);
	bool onUpdate(float ts);
	void onResize(uint32_t viewportWidth, uint32_t viewportHeight);

	const glm::mat4& GetProjection() const { return projection; };
	const glm::mat4& getInvertProjection() const { return invertProjection; };
	const glm::mat4& GetView() const { return view; }
	const glm::mat4& GetInvertView() const { return invertView; }
	const glm::vec3& GetPosition() const { return eye; }
	const std::vector<glm::vec3>& GetRayDirections() const { return rayDirections; }

	void ReCalcView();
	void ReCalcProjection();
	void ReCalcRayDirections();
	float GetRotationSpeed();

private:
	float fov;
	float nearPlane;
	float farPlane;
	uint32_t viewport_width;
	uint32_t viewport_height;
	glm::vec3 eye{ 1.0f };
	glm::vec3 up{ 1.0f };
	glm::vec3 forward{ 1.0f };
	std::vector<glm::vec3> rayDirections;
	glm::mat4 projection{ 1.0f };
	glm::mat4 invertProjection{ 1.0f };
	glm::mat4 view{ 1.0f };
	glm::mat4 invertView{ 1.0f };
	glm::vec2 lastMousePosition{0.0f, 0.0f};
};
#include <glm/glm.hpp>

class Ray {
public:
	Ray() {
		origin = glm::vec3(0.0f);
		direction = glm::vec3(0.0f, 0.0f, -1.0f);
	};
	Ray(const glm::vec3& origin, const glm::vec3& direction) :origin(origin), direction(direction) {}
public:
	glm::vec3 at(const float t) {
		return origin + t * direction;
	}
public:
	glm::vec3 origin;
	glm::vec3 direction;
};
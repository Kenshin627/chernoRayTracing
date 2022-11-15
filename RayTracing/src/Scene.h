#include <vector>

#include <glm/glm.hpp>

struct Sphere
{
public:
	Sphere() {}
	Sphere(const glm::vec3& origin, glm::vec3& albedo, float r) :origin(origin), radius(r), albedo(albedo) {}
	glm::vec3 origin{ 0.0f };
	float radius{ 0.5f };
	glm::vec3 albedo{ 1.0f };
};

class Scene
{
public:
	Scene() {};
public:
	std::vector<Sphere> spheres;
};


#include <vector>

#include <glm/glm.hpp>

struct Sphere
{
public:
	Sphere() {}
	Sphere(const glm::vec3& origin, uint32_t materialIndex, float r) :origin(origin), radius(r), materialIndex(materialIndex) {}
	glm::vec3 origin{ 0.0f };
	float radius{ 0.5f };
	int materialIndex{ -1 };
};

struct Material {
	glm::vec3 albedo{ 1.0f };
	float roughness{ 1.0f };
	float metalness{ 0.0f };
};

class Scene
{
public:
	Scene() {};
public:
	std::vector<Sphere> spheres;
	std::vector<Material> materials;
};


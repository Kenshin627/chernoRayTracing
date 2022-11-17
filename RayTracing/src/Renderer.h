#pragma once

#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"


class Renderer
{
public:
	Renderer() = default;

private:
	struct Setting {
		bool accumulate = false;
	};
	struct HitPayload {
		float hitDistance;
		glm::vec3 worldPosition;
		glm::vec3 worldNormal;
		uint32_t objectIndex;
	};
	glm::vec4 perPixel(uint32_t x, uint32_t y);
	Renderer::HitPayload TraceRay(const Ray& ray);
	Renderer::HitPayload HitclosetObj(const Ray& ray, float hitDistance, uint32_t objIndex);
	Renderer::HitPayload Miss(const Ray& ray);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_accumulateData = nullptr;
	const Camera* activeCamera = nullptr;
	const Scene* activeScene = nullptr;
	uint32_t frameIndex = 1;
	Setting setting;

public:
	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
	void resetFrameIndex() { frameIndex = 1; };
	Renderer::Setting& getSetting() { return setting; };
};
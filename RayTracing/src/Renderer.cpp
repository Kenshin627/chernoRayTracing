#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{	
	Ray ray;
	this->scene = &scene;
	activeCamera = &camera;
	ray.origin = camera.GetPosition();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{			
			ray.direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			uint32_t idx = x + m_FinalImage->GetWidth() * y;
			glm::vec4 color  = CastRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[idx] = Utils::ConvertToRGBA(color);			
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::CastRay(const Ray& ray)
{
	glm::vec3 lightDir = { -1.0f, -1.0f, -1.0f };
	lightDir = glm::normalize(lightDir);
	float intensity = 0.0f;
	if (scene->spheres.size() == 0)
	{
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	int closetSphere = -1;
	float hitDistance = FLT_MAX;
	for (size_t i = 0; i < scene->spheres.size(); i++)
	{
		Sphere s = scene->spheres[i];
		glm::vec3 origin = ray.origin - s.origin;
		//(bx^2 + by^2 + bz^2) t^2 + 2(axbx + ayby + axyz)t + (ax^2 + ay^2 + az^2 - r^2) = 0;
		float a = glm::dot(ray.direction, ray.direction);
		float b = 2.0f * glm::dot(ray.direction, origin);
		float c = glm::dot(origin, origin) - s.radius * s.radius;

		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0)
		{
			continue;
		}
		
		float t0 = (-b - glm::sqrt(discriminant)) / 2.0f * a;
		float t1 = (-b + glm::sqrt(discriminant)) / 2.0f * a;
		float t = t0 >= 0 ? t0 : t1;
		if (t < hitDistance)
		{
			closetSphere = i;
			hitDistance = t;
		}
	}

	if (closetSphere < 0)
	{
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	Sphere s = scene->spheres[closetSphere];
	glm::vec3 n = ray.origin - s.origin + hitDistance * ray.direction;
	n = glm::normalize(n);
	intensity = glm::max(0.0f, glm::dot(n, -lightDir));
	return glm::vec4(intensity * s.albedo, 1.0f);
}



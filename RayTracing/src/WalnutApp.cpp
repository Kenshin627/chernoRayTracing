#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>


using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		: activeCamera(45.0f, 0.1f, 100.0f, glm::vec3(0, 0, 6), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0))
	{

		Material mat;
		mat.albedo = glm::vec3(1.0, 0.5, 0.7);
		mat.roughness = 1.0f;
		mat.metalness = 0.0f;

		Material mat2;
		mat2.albedo = glm::vec3(0.2, 0.9, 0.3);
		mat2.roughness = 0.5f;
		mat2.metalness = 0.0f;
		scene.materials.push_back(mat);
		scene.materials.push_back(mat2);

		Sphere s1;
		s1.origin = glm::vec3(-0.1f, 0.5f, 0.0f);
		s1.radius = 1.0f;
		s1.materialIndex = 0;
		Sphere s2;
		s2.origin = glm::vec3(0.0f, -5.2f, 4.5f);
		s2.radius = 5.0f;
		s2.materialIndex = 1;
		scene.spheres.push_back(s1);
		scene.spheres.push_back(s2);
	}

	virtual void OnUpdate(float ts) override
	{
		activeCamera.onUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("Scene");
		for (size_t i = 0; i < scene.spheres.size(); i++)
		{
			Sphere& s = scene.spheres[i];
			ImGui::PushID(i);

			ImGui::DragFloat3("position", glm::value_ptr(s.origin), 0.1f);
			ImGui::DragFloat("radius", &s.radius, 0.01f);
			//ImGui::ColorEdit3("albedo", glm::value_ptr(s.albedo));
			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		activeCamera.onResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(scene, activeCamera);
		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera activeCamera;
	Scene scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}
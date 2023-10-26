#include "SceneEXTRA.h"
#include "Material.h"
#include "Utils.h"

SceneEXTRA::~SceneEXTRA()
{
}

void SceneEXTRA::Initialize()
{
	m_Camera.origin = { 0.f,1.f,-5.f };
	m_Camera.fovAngle = 45.f;

	const auto treeMat = AddMaterial(new dae::Material_Lambert(dae::ColorRGB(0.61f, 0.79f, 0.62f), 10.f));
	const auto groundMat = AddMaterial(new dae::Material_SolidColor(dae::ColorRGB(0.61f, 0.39f, 0.39f)));
	const auto rockMat = AddMaterial(new dae::Material_SolidColor(dae::ColorRGB(0.7f, 0.7f, 0.7f)));

	AddTriangleMesh("Resources/ground.obj", {}, { 0.1f, 0.1f, 0.1f }, 0.f, groundMat);
	AddTriangleMesh("Resources/Rock_21.obj", { 0.6f, 0.f, 0.f }, { 0.001f, 0.001f, 0.001f }, 0.f, rockMat);
	AddTriangleMesh("Resources/Rock_21.obj", { -0.5f, 0.05f, 0.3f }, { 0.0012f, 0.0011f, 0.0009f }, 30.f, rockMat);
	m_pTree = AddTriangleMesh("Resources/DeadSpruce1.obj", { 0.f, 0.7f, 0.f }, { 0.001f, 0.001f, 0.001f }, 0.f, treeMat);

	//Light
	AddPointLight(dae::Vector3{ 5.f, 15.f, 5.f }, 500.f, dae::ColorRGB{ 1.f, 1.f, .7f }); //Backlight
	AddPointLight(dae::Vector3{ 0.f, 15.f, -5.f }, 2500.f, dae::ColorRGB{ 1.f, 1.f, .7f }); //Backlight
}

void SceneEXTRA::Update(dae::Timer* pTimer)
{
	Scene::Update(pTimer);

	m_pTree->RotateY(std::sinf(pTimer->GetTotal() * m_MeshRotationSpeed) * 2 * dae::PI);
	m_pTree->UpdateTransforms();
}

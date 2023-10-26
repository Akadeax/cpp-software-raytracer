#include "SceneW4.h"
#include "Material.h"
#include "Utils.h"
using namespace dae;

SceneW4::~SceneW4()
{
}

void SceneW4::Initialize()
{
	sceneName = "Bunny Scene";
	m_Camera.origin = { 0.f, 3.f, -9.f };
	m_Camera.fovAngle = 45.0f;

	const auto matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, .57f, .57f }, 1.f));
	const auto matLambert_White = AddMaterial(new Material_Lambert(colors::White, 1.f));
	//Plane
	AddPlane({ -5.f, 0.f, 0.f }, { 1.f, 0.f,0.f }, matLambert_GrayBlue);
	AddPlane({ 5.f, 0.f, 0.f }, { -1.f, 0.f,0.f }, matLambert_GrayBlue);
	AddPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f,0.f }, matLambert_GrayBlue);
	AddPlane({ 0.f, 10.f, 0.f }, { 0.f, -1.f,0.f }, matLambert_GrayBlue);
	AddPlane({ 0.f, 0.f, 10.f }, { 0.f, 0.f,-1.f }, matLambert_GrayBlue);

	m_pMesh = AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
	Utils::ParseOBJ("Resources/lowpoly_bunny2.obj", m_pMesh->positions, m_pMesh->normals, m_pMesh->indices);

	m_pMesh->Scale({ 2.f, 2.f, 2.f });
	m_pMesh->UpdateAABB();
	m_pMesh->UpdateTransforms();


	AddPointLight({ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f });		//backlight
	AddPointLight({ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f });		// front light left
	AddPointLight({ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
}

void SceneW4::Update(dae::Timer* pTimer)
{
	Scene::Update(pTimer);
	m_pMesh->RotateY(PI_DIV_2 * pTimer->GetTotal() * m_MeshRotationSpeed);
	m_pMesh->UpdateTransforms();
}
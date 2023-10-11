#include "SceneW3.h"
#include "Material.h"

//void SceneW3::Initialize()
//{
//	sceneName = "Week 3";
//	m_Camera.origin = { 0,3,-9 };
//	m_Camera.fovAngle = 45.f;
//
//	const auto matCT_GrayRoughMetal = AddMaterial(new dae::Material_CookTorrence({ .972f, .960f, .915f }, 1.f, 1.f));
//	const auto matCT_GrayMediumMetal = AddMaterial(new dae::Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .6f));
//	const auto matCT_GraySmoothMetal = AddMaterial(new dae::Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .1f));
//	const auto matCT_GrayRoughPlastic = AddMaterial(new dae::Material_CookTorrence({ .75f, .75f, .75f }, .0f, 1.f));
//	const auto matCT_GrayMediumPlastic = AddMaterial(new dae::Material_CookTorrence({ .75f, .75f, .75f }, .0f, .6f));
//	const auto matCT_GraySmoothPlastic = AddMaterial(new dae::Material_CookTorrence({ .75f, .75f, .75f }, .0f, .1f));
//
//	const auto matLambert_GrayBlue = AddMaterial(new dae::Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
//	const auto matLambert_White = AddMaterial(new dae::Material_Lambert(dae::colors::White, 1.f));
//
//	AddPlane(dae::Vector3{ 0.f, 0.f, 10.f }, dae::Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
//	AddPlane(dae::Vector3{ 0.f, 0.f, 0.f }, dae::Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
//	AddPlane(dae::Vector3{ 0.f, 10.f, 0.f }, dae::Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
//	AddPlane(dae::Vector3{ 5.f, 0.f, 0.f }, dae::Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
//	AddPlane(dae::Vector3{ -5.f, 0.f, 0.f }, dae::Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT
//
//	AddSphere(dae::Vector3{ -1.75f, 1.f, 0.f }, .75f, matCT_GrayRoughMetal);
//	AddSphere(dae::Vector3{ 0.f, 1.f, 0.f }, .75f, matCT_GrayMediumMetal);
//	AddSphere(dae::Vector3{ 1.75f, 1.f, 0.f }, .75f, matCT_GraySmoothMetal);
//	AddSphere(dae::Vector3{ -1.75f, 3.f, 0.f }, .75f, matCT_GrayRoughPlastic);
//	AddSphere(dae::Vector3{ 0.f, 3.f, 0.f }, .75f, matCT_GrayMediumPlastic);
//	AddSphere(dae::Vector3{ 1.75f, 3.f, 0.f }, .75f, matCT_GraySmoothPlastic);
//
//
//
//	AddPointLight(dae::Vector3{ 0.f, 5.f, 5.f }, 50.f, dae::ColorRGB{ 1.f, .61f, .45f }); //Backlight
//	AddPointLight(dae::Vector3{ -2.5f, 5.f, -5.f }, 70.f, dae::ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
//	AddPointLight(dae::Vector3{ 2.5f, 2.5f, -5.f }, 50.f, dae::ColorRGB{ .34f, .47f, .68f });
//}

void SceneW3::Initialize()
{
    m_Camera.origin = { 0.f, 1.f, -5.f };
    m_Camera.fovAngle = 45.f;

    //default: Material id0 >> SolidColor Material (RED)
    constexpr unsigned char matId_Solid_Red = 0;
    const unsigned char matId_Solid_Blue = AddMaterial(new dae::Material_Lambert{ dae::colors::Blue, 1.f });
    const unsigned char matId_Solid_Yellow = AddMaterial(new dae::Material_Lambert{ dae::colors::Yellow, 1.f });
    const unsigned char matId_LambertPhong_Blue = AddMaterial(new dae::Material_LambertPhong{ dae::colors::Blue, 1.f, 1.f, 60.f });

    //Spheres
    AddSphere({ -.75f, 1.f, .0f }, 1.f, matId_Solid_Red);
    AddSphere({ .75f, 1.f, .0f }, 1.f, matId_LambertPhong_Blue);

    //Plane
    AddPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, matId_Solid_Yellow);

    //Light
    AddPointLight({ 0.f, 5.f, 5.f }, 25.f, dae::colors::White);
    AddPointLight({ 0.f, 2.5f, -5.f }, 25.f, dae::colors::White);
}

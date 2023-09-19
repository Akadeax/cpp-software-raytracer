#include "SceneW1.h"
#include "Material.h"

void SceneW1::Initialize()
{
	//default: Material id0 >> SolidColor Material (RED)
	constexpr unsigned char matId_Solid_Red = 0;
	const unsigned char matId_Solid_Blue = AddMaterial(new dae::Material_SolidColor{ dae::colors::Blue });

	const unsigned char matId_Solid_Yellow = AddMaterial(new dae::Material_SolidColor{ dae::colors::Yellow });
	const unsigned char matId_Solid_Green = AddMaterial(new dae::Material_SolidColor{ dae::colors::Green });
	const unsigned char matId_Solid_Magenta = AddMaterial(new dae::Material_SolidColor{ dae::colors::Magenta });

	//Spheres
	AddSphere({ -25.f, 0.f, 100.f }, 50.f, matId_Solid_Red);
	AddSphere({ 25.f, 0.f, 100.f }, 50.f, matId_Solid_Blue);

	//Plane
	AddPlane({ -75.f, 0.f, 0.f }, { 1.f, 0.f,0.f }, matId_Solid_Green);
	AddPlane({ 75.f, 0.f, 0.f }, { -1.f, 0.f,0.f }, matId_Solid_Green);
	AddPlane({ 0.f, -75.f, 0.f }, { 0.f, 1.f,0.f }, matId_Solid_Yellow);
	AddPlane({ 0.f, 75.f, 0.f }, { 0.f, -1.f,0.f }, matId_Solid_Yellow);
	AddPlane({ 0.f, 0.f, 125.f }, { 0.f, 0.f,-1.f }, matId_Solid_Magenta);
}
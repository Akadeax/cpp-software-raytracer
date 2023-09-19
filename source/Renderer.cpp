//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include <iostream>
#include "Renderer.h"
#include "Math.h"
#include "Matrix.h"
#include "Material.h"
#include "Scene.h"
#include "Utils.h"
#include "Vector3.h"

using namespace dae;

Renderer::Renderer(SDL_Window * pWindow) :
	m_pWindow(pWindow),
	m_pBuffer(SDL_GetWindowSurface(pWindow))
{
	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
	m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
}

void Renderer::Render(Scene* pScene) const
{
	Camera& camera = pScene->GetCamera();
	auto& materials = pScene->GetMaterials();
	auto& lights = pScene->GetLights();	

	for (int px{}; px < m_Width; ++px)
	{
		for (int py{}; py < m_Height; ++py)
		{
			ColorRGB finalColor{};

			float aspectRatio{ static_cast<float>(m_Width) / static_cast<float>(m_Height) };
			
			float cameraX{ (2 * (px + 0.5f) / m_Width - 1) * aspectRatio };
			float cameraY{ 1 - (2 * (py + 0.5f) / m_Height)};
			
			Vector3 rayDir{ camera.forward + (camera.right * cameraX) + (camera.up * cameraY) };
			rayDir.Normalize();
			Ray ray{ camera.origin, rayDir };

			HitRecord closestHit{};
			Plane testPlane{ { 0.f, -50.f, 0.f }, {0.f, 1.f, 0.f }, 0 };
			GeometryUtils::HitTest_Plane(testPlane, ray, closestHit);
			//pScene->GetClosestHit(ray, closestHit);

			if (closestHit.didHit)
			{
				const float scaledT{ closestHit.t / 500.f };
				finalColor = { scaledT, scaledT, scaledT };
			}

			//Update Color in Buffer
			finalColor.MaxToOne();
			m_pBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255),
				static_cast<uint8_t>(finalColor.g * 255),
				static_cast<uint8_t>(finalColor.b * 255)
			);
		}
	}

	//@END
	//Update SDL Surface
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}

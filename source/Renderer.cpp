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

	camera.CalculateCameraToWorld();
	float fov{ std::tanf(camera.fovAngle * dae::TO_RADIANS / 2.f) };
	float aspectRatio{ static_cast<float>(m_Width) / static_cast<float>(m_Height) };

	for (int px{}; px < m_Width; ++px)
	{
		for (int py{}; py < m_Height; ++py)
		{
			ColorRGB finalColor{};

			// Scale x value for pixel to be in [-aspectRatio; aspectRatio]
			float cameraX{ (2 * (px + 0.5f) / m_Width - 1) * aspectRatio * fov };
			// Scale y value for pixel to be in [-1; 1]
			float cameraY{ (1 - (2 * (py + 0.5f) / m_Height)) * fov };
			
			Vector3 rayDir{ camera.forward + (camera.right * cameraX) + (camera.up * cameraY) };
			rayDir.Normalize();

			// Shoot ray from camera in pixel direction
			Ray ray{ camera.origin, rayDir };

			HitRecord closestHit{};
			pScene->GetClosestHit(ray, closestHit);

			if (closestHit.didHit)
			{
				for (const Light& light : pScene->GetLights())
				{
					Vector3 dirToLight{ LightUtils::GetDirectionToLight(light, closestHit.origin) };
					float distToLight{ dirToLight.Normalize() };

					float observedArea{ Vector3::Dot(closestHit.normal, dirToLight) };
					if (observedArea <= 0.f) continue;

					
					Ray shadowRay{ closestHit.origin + closestHit.normal * 0.001f, dirToLight, 0.f, distToLight };

					// Skip this lights' calculations if shadowed
					if (m_ShadowsEnabled && pScene->DoesHit(shadowRay))
					{
						continue;
					}

					ColorRGB radiance{ LightUtils::GetRadiance(light, closestHit.origin) };
					ColorRGB shade{ materials[closestHit.materialIndex]->Shade(closestHit, dirToLight, -rayDir) };
					
					switch (m_CurrentLightingMode)
					{
					case LightingMode::ObservedArea:
						finalColor += ColorRGB(observedArea, observedArea, observedArea);
						break;
					case LightingMode::Radiance:
						finalColor += radiance;
						break;
					case LightingMode::BRDF:
						finalColor += shade;
						break;

					case LightingMode::Combined:
						finalColor += radiance * shade * observedArea;
						break;
					}
				}
			}

			// Apply tone mapping
			switch (m_CurrentToneMapping)
			{
			case ToneMapping::MaxToOne:
				finalColor.MaxToOne();
				break;
			case ToneMapping::Hable:
				finalColor.HableToneMap();
				break;
			}

			//Update Color in Buffer
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

void dae::Renderer::CycleLightingMode()
{
	int current{ static_cast<int>(m_CurrentLightingMode) };
	int max{ static_cast<int>(LightingMode::Combined) };

	m_CurrentLightingMode = static_cast<LightingMode>((current + 1) % (max + 1));
}

void dae::Renderer::CycleToneMapping()
{
	int current{ static_cast<int>(m_CurrentToneMapping) };
	int max{ static_cast<int>(ToneMapping::Hable) };

	m_CurrentToneMapping = static_cast<ToneMapping>((current + 1) % (max + 1));
}

#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"
#include <bitset>
#include <iostream>

namespace dae
{
	struct Camera
	{
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle):
			origin{_origin},
			fovAngle{_fovAngle}
		{
		}


		Vector3 origin{};
		float fovAngle{ 90.f };

		Vector3 forward{ Vector3::UnitZ };
		Vector3 up{ Vector3::UnitY };
		Vector3 right{ Vector3::UnitX };

		float totalPitch{ 0.f };
		float totalYaw{ 0.f };

		Matrix cameraToWorld{};

		float cameraRotationSpeed{ 1.f };
		float cameraTranslationSpeed{ 5.f };

		Matrix CalculateCameraToWorld()
		{
			Matrix rotation{ Matrix::CreateRotation(totalPitch, totalYaw, 0.f) };

			forward = rotation.TransformVector(Vector3::UnitZ);
			right = Vector3::Cross(Vector3::UnitY, forward).Normalized();
			up = Vector3::Cross(forward, right).Normalized();

			cameraToWorld = Matrix(right, up, forward, origin);
			return cameraToWorld;
		}


		void Update(Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();

			// Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
			
			// Mouse Input
			int mouseX{}, mouseY{};
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

			bool leftMouse{ (mouseState & SDL_BUTTON(1)) != 0 };
			bool rightMouse{ (mouseState & SDL_BUTTON(3)) != 0 };

			// == Translation ==
			// WASD
			if (pKeyboardState[SDL_SCANCODE_W])
			{
				origin += forward * cameraTranslationSpeed * deltaTime;
			}
			else if (pKeyboardState[SDL_SCANCODE_S])
			{
				origin -= forward * cameraTranslationSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_D])
			{
				origin += right * cameraTranslationSpeed * deltaTime;
			}
			else if (pKeyboardState[SDL_SCANCODE_A])
			{
				origin -= right * cameraTranslationSpeed * deltaTime;
			}

			// LMB + MouseY
			if (leftMouse && !rightMouse && mouseY != 0)
			{
				origin -= forward * mouseY * cameraTranslationSpeed * deltaTime;
			}
			// LMB + RMB + MouseY
			if (leftMouse && rightMouse && mouseY != 0)
			{
				origin += Vector3::UnitY * mouseY * cameraTranslationSpeed * deltaTime;
			}

			// Rotation
			if (leftMouse && !rightMouse && mouseX != 0)
			{
				totalYaw += cameraRotationSpeed * mouseX * deltaTime;
			}
			else if (!leftMouse && rightMouse)
			{
				totalYaw += cameraRotationSpeed * mouseX * deltaTime;
				totalPitch -= cameraRotationSpeed * mouseY * deltaTime;
			}
		}
	};
}

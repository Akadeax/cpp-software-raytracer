#pragma once

#include <cstdint>

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Scene;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render(Scene* pScene) const;
		bool SaveBufferToImage() const;

		void CycleLightingMode();
		void CycleToneMapping();
		void ToggleShadows() { m_ShadowsEnabled = !m_ShadowsEnabled; }

	private:
		SDL_Window* m_pWindow{};

		SDL_Surface* m_pBuffer{};
		uint32_t* m_pBufferPixels{};

		enum class LightingMode
		{
			ObservedArea,
			Radiance,
			BRDF,
			Combined
		};
		enum class ToneMapping
		{
			MaxToOne,
			Hable
		};

		LightingMode m_CurrentLightingMode{ LightingMode::Combined };
		ToneMapping m_CurrentToneMapping{ ToneMapping::MaxToOne };
		bool m_ShadowsEnabled{ true };

		int m_Width{};
		int m_Height{};
	};
}

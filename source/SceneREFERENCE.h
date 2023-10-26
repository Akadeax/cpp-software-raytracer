#pragma once
#include "Scene.h"

class SceneREFERENCE final : public dae::Scene
{
public:
	SceneREFERENCE() = default;
	~SceneREFERENCE() override = default;

	SceneREFERENCE(const SceneREFERENCE&) = delete;
	SceneREFERENCE(SceneREFERENCE&&) noexcept = delete;
	SceneREFERENCE& operator=(const SceneREFERENCE&) = delete;
	SceneREFERENCE& operator=(SceneREFERENCE&&) noexcept = delete;

	void Initialize() override;
	void Update(dae::Timer* pTimer) override;

private:
	std::vector<dae::TriangleMesh*> m_Meshes{ 3 };
};
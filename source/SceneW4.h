#pragma once
#include "Scene.h"

class SceneW4 final : public dae::Scene
{
public:
	SceneW4() = default;
	~SceneW4() override;

	SceneW4(const SceneW4&) = delete;
	SceneW4(SceneW4&&) noexcept = delete;
	SceneW4& operator=(const SceneW4&) = delete;
	SceneW4& operator=(SceneW4&&) noexcept = delete;

	void Initialize() override;
	void Update(dae::Timer* pTimer) override;

private:
	dae::TriangleMesh* m_pMesh{};
	float m_MeshRotationSpeed{ 0.15f };
};

